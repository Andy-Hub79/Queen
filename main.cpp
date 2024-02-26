#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

#define N 8 // Tamaño del tablero
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

int board[N][N] = {0};

bool isSafe(int row, int col) {
    int i, j;
    for (i = 0; i < col; i++)
        if (board[row][i])
            return false;
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j])
            return false;
    for (i = row, j = col; j >= 0 && i < N; i++, j--)
        if (board[i][j])
            return false;
    return true;
}

bool solveNQUtil(int col) {
    if (col >= N)
        return true;
    for (int i = 0; i < N; i++) {
        if (isSafe(i, col)) {
            board[i][col] = 1;
            if (solveNQUtil(col + 1))
                return true;
            board[i][col] = 0; // BACKTRACK
        }
    }
    return false;
}

bool solveNQ() {
    if (!solveNQUtil(0)) {
        printf("Solution does not exist\n");
        return false;
    }
    return true;
}

void drawBoard(SDL_Renderer *renderer, SDL_Texture *queenTexture) {
    int size = WINDOW_WIDTH / N;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect rect, dst;
    dst.w = dst.h = size;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            rect.x = j * size;
            rect.y = i * size;
            rect.w = size;
            rect.h = size;

            SDL_SetRenderDrawColor(renderer, (i + j) % 2 ? 0 : 255, (i + j) % 2 ? 0 : 255, (i + j) % 2 ? 0 : 255, 255);
            SDL_RenderFillRect(renderer, &rect);

            if (board[i][j] == 1) {
                dst.x = j * size;
                dst.y = i * size;
                SDL_RenderCopy(renderer, queenTexture, NULL, &dst); // Renderiza la reina
            }
        }
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG_Init: Failed to init required png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("N Queens Problem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* queenTexture = IMG_LoadTexture(renderer, "C:\\Users\\Andre\\Desktop\\Reinas_Final\\QueenFinal\\queen.png");
    if (!queenTexture) {
    printf("Failed to load texture image: %s\n", IMG_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}
    if (solveNQ()) {
        drawBoard(renderer, queenTexture);
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
        }
    }

    SDL_DestroyTexture(queenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
