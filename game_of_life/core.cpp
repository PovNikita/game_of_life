#include "core.h"
#include <stdexcept>
#include <string>
#include <thread>
#include <iostream>

Grid::Grid(int rows, int cols) 
    : rows_(rows)
    , cols_(cols)
{
    cells_.resize(rows_);
    for(int i = 0; i < rows_; ++i)
    {
        cells_.at(i).resize(cols_);
    }
}

int Grid::GetRows() const {
    return rows_;
}

int Grid::GetCols() const {
    return cols_;
}

Cell Grid::GetCell(int row, int col) const {
    using namespace std::literals;
    if(!IsRowColValid(row, col))
    {
        throw std::invalid_argument("Invalid row/col argument"s);
    }
    return cells_.at(row).at(col);
}

void Grid::SetCell(int row, int col, Cell::State state) {
    using namespace std::literals;
    if(!IsRowColValid(row, col))
    {
        throw std::invalid_argument("Invalid row/col argument"s);
    }
    cells_.at(row).at(col).SetState(state);
}

int Grid::CountAliveNeighbors(int row, int col) const
{
    using namespace std::literals;
    if(!IsRowColValid(row, col))
    {
        throw std::invalid_argument("Invalid row/col argument"s);
    }
    int nummber_of_allive_neighbors = 0;
    for(auto [row_neighboor, col_neighboor] : GetNeighboursCoordinates(row, col))
    {
        if(cells_.at(row_neighboor).at(col_neighboor).GetState() == Cell::State::Alive)
        {
            ++nummber_of_allive_neighbors;
        }
    }
    return nummber_of_allive_neighbors;
}


bool Grid::IsRowColValid(int row, int col) const
{
    if(row >= rows_ || row < 0)
    {
        return false;
    }
    if(col >= cols_ || col < 0)
    {
        return false;
    }
    return true;
}

void Grid::NextGeneration() {
    std::vector<std::vector<Cell>> cells_temp;
    cells_temp.resize(rows_);
    for(int row_y = 0; row_y < rows_; ++row_y)
    {
        cells_temp.at(row_y).resize(cols_);
        for(int col_x = 0; col_x < cols_; ++col_x)
        {
            int number_of_neighbours = CountAliveNeighbors(row_y, col_x);
            if(cells_.at(row_y).at(col_x).GetState() == Cell::State::Alive)
            {
                if(number_of_neighbours == 3 || number_of_neighbours == 2)
                {
                    cells_temp.at(row_y).at(col_x).SetState(Cell::State::Alive);
                }
            }
            else
            {
                if(number_of_neighbours == 3)
                {
                    cells_temp.at(row_y).at(col_x).SetState(Cell::State::Alive);
                }
            }
        }
    }
    cells_ = std::move(cells_temp);
}

void Grid::Clear() {
    for(int y = 0; y < rows_; ++y)
    {
        for(int x = 0; x < cols_; ++x)
        {
            cells_.at(y).at(x).SetState(Cell::State::Dead);
        }
    }
}

std::vector<std::pair<int, int>> Grid::GetNeighboursCoordinates(int row, int col) const {
    
    std::vector<std::pair<int, int>> neigbours_coordinate;
    neigbours_coordinate.reserve(8);
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0)
                continue;

            int new_row = row + dr;
            int new_col = col + dc;
            new_row = (new_row + rows_) % rows_;
            new_col = (new_col + cols_) % cols_;
            neigbours_coordinate.push_back({new_row, new_col});

        }
    }
    return neigbours_coordinate;
}

Game::Game(int rows, int grid_length, uint32_t delay)
    : grid_(rows, rows)
    , is_start_(false)
    , grid_length_(grid_length)
    , delay_(delay)
    , cell_length_(grid_length_/rows)
{
    if(grid_length % rows != 0)
    {
        grid_length_ = cell_length_ * rows;
    }
    using namespace std::literals;
    window_ = SDL_CreateWindow("Game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          grid_length_ + window_height_offset_with_button, grid_length_, SDL_WINDOW_SHOWN);
    if (!window_) {
        std::string error(SDL_GetError());
        throw std::runtime_error("SDL_CreateWindow Error: "s + error + "\n"s);
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        std::string error(SDL_GetError());
        SDL_DestroyWindow(window_);
        throw std::runtime_error("SDL_CreateRenderer Error: "s + error + "\n"s);
    }
    pause_button_ = std::make_unique<Button>(SDL_Rect{grid_length_ + distance_between_buttons_and_grid, button_offset_from_y0, 
                                                        button_weight, button_height}, 
                                            button_color,
                                            [this]() { this->HandlePauseButton(); },"Start"s, renderer_);

}

void Game::Run()
{
    SDL_Event event;
    bool running = true;
    int rows = grid_.GetRows();
    int cols = grid_.GetCols();
    std::thread next_gen(&Game::HandleGenerations, this);
    is_running_ = true;
    next_gen.detach();
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    HandleMouseClick(event);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer_, 30, 30, 30, 255);
        SDL_RenderClear(renderer_);
        pause_button_->Render();
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                SDL_Rect cellRect = {
                    col * cell_length_,
                    row * cell_length_,
                    cell_length_,
                    cell_length_
                };
                {
                    std::lock_guard<std::mutex> lock(access_to_grid_);
                    if (grid_.GetCell(row, col).GetState() == Cell::State::Alive) {
                        SDL_SetRenderDrawColor(renderer_, 0, 200, 0, 255); // зелёная клетка
                        SDL_RenderFillRect(renderer_, &cellRect);
                    }
                }

                SDL_SetRenderDrawColor(renderer_, 80, 80, 80, 255); // серый цвет для сетки
                SDL_RenderDrawRect(renderer_, &cellRect);
            }
        }
        SDL_RenderPresent(renderer_);
        std::this_thread::sleep_for(std::chrono::milliseconds(std::min(15u, std::max(1u, delay_))));
    }
    {
        std::lock_guard<std::mutex> lock(access_to_is_running_);
        is_running_ = false;
    }
}

void Game::HandleGenerations()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100u));
    while(1)
    {
        {
            std::lock_guard<std::mutex> lock(access_to_is_running_);
            if(is_running_ == false)
            {
                break;
            }
        }
        {
            std::lock_guard<std::mutex> lock(access_to_is_start_);
            if(is_start_) 
            {
                {
                    std::lock_guard<std::mutex> lock(access_to_grid_);
                    grid_.NextGeneration();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(std::max(delay_, 1u)));
    }



}

void Game::HandleMouseClick(const SDL_Event &event)
{
    if(event.button.x > grid_length_)
    {
        pause_button_->HandleEvent(event);
    }
    else
    {
        HandleGridClick(event);
    }

}

void Game::HandleGridClick(const SDL_Event &event)
{
    int mouseX = event.button.x;
    int mouseY = event.button.y;

    int col = mouseX / cell_length_;
    int row = mouseY / cell_length_;
    {
        std::lock_guard<std::mutex> lock(access_to_grid_);
        if (row >= 0 && row < grid_.GetRows() && col >= 0 && col < grid_.GetCols()) {
            ToggleCell(row, col);
        }
    }
}

void Game::HandlePauseButton()
{
    using namespace std::literals;
    {
        std::lock_guard<std::mutex> lock(access_to_is_start_);
        is_start_ = !is_start_;
    }
    if(pause_button_->GetText() == "Start"s)
    {
        pause_button_->SetText("Pause"s);
    }
    else
    {
        pause_button_->SetText("Start"s);
    }
}

void Game::ToggleCell(int row, int col)
{
    Cell::State state = (grid_.GetCell(row, col).GetState() == Cell::State::Alive) ? Cell::State::Dead : Cell::State::Alive;
    grid_.SetCell(row, col, state);
}

Game::~Game()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}