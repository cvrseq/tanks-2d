#include "raylib.h"
#include <stdlib.h>

#define MAP_WIDTH   20
#define MAP_HEIGHT  15

typedef enum GameScreen { TITLE, GAMEPLAY, END } GameScreen;

typedef enum TileType //реализовать плитки на мапе
{
    TILE_EMPTY, 
    TILE_WALL, 
    TILE_WATER,
}TileType;


typedef struct Map //реализовать мапу
{
    int width; 
    int height; 
    int tileSize; 
    TileType *tiles;
}Map;

typedef enum TankType // союзники и враги
{
    TANK_PLAYER, 
    TANK_ENEMY 
} TankType;

typedef struct Tank //возможно добавить выстрелы через указатель и еще что-то
{
    Vector2 position; 
    float rotation; 
    float speed; 
    int health;
    Color color; 
    TankType type;
    float cooldown; 

} Tank;


int main(void)
{
    const int screenWidth = 1500;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "TANKS");
    
    GameScreen currentScreen = TITLE;
    SetTargetFPS(60);

    while (!WindowShouldClose())    // Основной игровой цикл
    {
        if (currentScreen == TITLE)
        {
            if (IsKeyPressed(KEY_ENTER)) 
            { 
                currentScreen = GAMEPLAY; 
            }
        }
        else if (currentScreen == GAMEPLAY)
        {
            
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
            case TITLE:
                DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN); 
                DrawText("PRESS ENTER or TAP to Jump to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
                break;
                
            case GAMEPLAY:
                DrawRectangle(0, 0, screenWidth, screenHeight, RED);
                DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON); 
                DrawText("GAME PLAY", 150, 250, 50, MAROON);
                break;
                
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
