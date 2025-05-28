#pragma once
#include <vector>
#include <functional>
#include <SDL2/SDL.h>
#include "user_interface.h"
#include "SDL_wrappler.h"
#include <memory>
#include <mutex>

static const SDLInit sdl_controller;
const int button_weight = 70;
const int button_height = 35;
const int grid_width = 600;
const int grid_height = 600;
const int distance_between_buttons_and_grid = 10;
const int button_offset_from_y0 = 10;
const int window_height_offset_with_button = distance_between_buttons_and_grid + button_weight + 10;
const SDL_Color button_color{222, 206, 206, 255};

class Cell {
public:
    enum class State { Dead, Alive };

    Cell() = default;
    explicit Cell(State state) : state_(state) {}

    State GetState() const
    {
        return state_;
    }
    void SetState(State state)
    {
        state_ = state;
    }
    void ToggleState()
    {
        state_ = (state_ == State::Alive) ? State::Dead : State::Alive;
    }

private:
    State state_ = State::Dead;
};

class Grid {
public:
    explicit Grid(int rows, int cols);

    int GetRows() const;
    int GetCols() const;

    Cell GetCell(int row, int col) const;
    void SetCell(int row, int col, Cell::State state);

    int CountAliveNeighbors(int row, int col) const;

    void NextGeneration();
    void Clear();

private:
    std::vector<std::vector<Cell>> cells_;
    int rows_;
    int cols_;
    bool IsRowColValid(int row, int col) const;
    std::vector<std::pair<int, int>> GetNeighboursCoordinates(int row, int col) const;
};

class Game {
public:
    explicit Game(int rows, int cols, uint32_t delay = 0);

    void Run();

    ~Game();

private:
    Grid grid_;
    int grid_width_;
    int grid_height_;
    uint32_t delay_;

    SDL_Window* window_;
    SDL_Renderer* renderer_;
    int cell_width_;
    int cell_height_;

    std::unique_ptr<Button> pause_button_;
    void HandleGenerations();

    void HandleMouseClick(const SDL_Event &event);
    void HandleGridClick(const SDL_Event &event);
    void HandlePauseButton();
    void ToggleCell(int row, int col);
    void HandleWindowSize();

    bool is_start_;
    bool is_running_ = false;
    std::mutex access_to_grid_;
    std::mutex access_to_is_start_;
    std::mutex access_to_is_running_;
};