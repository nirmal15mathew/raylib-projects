#include "raylib.h"

#define CELL_COUNT_X 20
#define CELL_COUNT_Y 20

void renderCells(int x, int y, bool cells[x][y], int cell_width, int cell_height);
void drawGrid(int cell_count_x, int cell_count_y, int cell_width, int cell_height);
void clearAllDrawings(int x, int y, bool cells[x][y]);
void copy2dArr(int x, int y, bool a[x][y], bool b[x][y]);
int getLiveNeighbours(int i, int j, bool cells[CELL_COUNT_X][CELL_COUNT_Y]);

typedef struct
{
    bool cells[CELL_COUNT_X][CELL_COUNT_Y];
} arr2d;

arr2d calculateNextCellState(int x, int y, bool current_cells[x][y]);

int main()
{
    const int screenWidth = 600;
    const int screenHeight = 600;

    bool cells[CELL_COUNT_X][CELL_COUNT_Y];
    bool isSimulating = false;

    const int cell_width = screenWidth / CELL_COUNT_X;
    const int cell_height = screenHeight / CELL_COUNT_Y;

    for (int i = 0; i < CELL_COUNT_X; i++)
    {
        for (int j = 0; j < CELL_COUNT_Y; j++)
        {
            cells[i][j] = false;
        }
    }

    InitWindow(screenWidth, screenHeight, "Cellular Automata");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            int cell_posX = GetMouseX() / cell_width;
            int cell_posY = GetMouseY() / cell_height;
            cells[cell_posX][cell_posY] = true;
        }
        if (IsKeyPressed(KEY_R))
        {
            clearAllDrawings(CELL_COUNT_X, CELL_COUNT_Y, cells);
        }
        if (IsKeyPressed(KEY_P))
        {
            isSimulating = !isSimulating;
        }
        if (isSimulating)
        {
            arr2d calc = calculateNextCellState(CELL_COUNT_X, CELL_COUNT_Y, cells);
            copy2dArr(CELL_COUNT_X, CELL_COUNT_Y, cells, calc.cells);
        }
        BeginDrawing();
        ClearBackground(WHITE);
        drawGrid(CELL_COUNT_X, CELL_COUNT_Y, cell_width, cell_height);
        DrawText("Press r to clear all drawings", 10, 10, 15, GRAY);
        DrawText("Press p to play / pause", 10, 25, 15, GRAY);
        renderCells(CELL_COUNT_X, CELL_COUNT_Y, cells, cell_width, cell_height);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void renderCells(int x, int y, bool cells[x][y], int cell_width, int cell_height)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (cells[i][j])
            {
                DrawRectangle(i * cell_width, j * cell_height, cell_width, cell_height, BLACK);
            }
        }
    }
}

void drawGrid(int cell_count_x, int cell_count_y, int cell_width, int cell_height)
{
    const int screenWidth = cell_count_x * cell_width;
    const int screenHeight = cell_count_y * cell_height;
    for (int i = 0; i < cell_count_x; i++)
    {
        DrawLine(i * cell_width, 0, i * cell_width, screenHeight, GRAY);
    }
    for (int j = 0; j < cell_count_y; j++)
    {
        DrawLine(0, j * cell_height, screenWidth, j * cell_height, GRAY);
    }
}

arr2d calculateNextCellState(int x, int y, bool current_cells[x][y])
{
    arr2d buffer_state;
    copy2dArr(x, y, buffer_state.cells, current_cells);
    for (int i = 1; i < x; i++)
    {
        for (int j = 1; j < y; j++)
        {
            int live_neighbours = getLiveNeighbours(i, j, current_cells);

            if (current_cells[i][j])
            {
                if (live_neighbours < 2 || live_neighbours > 3)
                {
                    buffer_state.cells[i][j] = false;
                }
            }
            else
            {
                if (live_neighbours == 3)
                {
                    buffer_state.cells[i][j] = true;
                }
            }
        }
    }

    return buffer_state;
}

void clearAllDrawings(int x, int y, bool cells[x][y])
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            cells[i][j] = false;
        }
    }
}

void copy2dArr(int x, int y, bool a[x][y], bool b[x][y])
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            a[i][j] = b[i][j];
        }
    }
}

int getLiveNeighbours(int i, int j, bool current_cells[CELL_COUNT_X][CELL_COUNT_Y])
{
    int live_neighbours = 0;
    for (int m = -1; m < 2; m++)
    {
        for (int n = -1; n < 2; n++)
        {
            if (!(m==0 && n==0)) {
                live_neighbours += current_cells[i + m][j + n];
            }
        }
    }
    return live_neighbours;
}