#include "raylib.h"
#include <stdlib.h>
#include <math.h> 

#define SQUARE_SIZE 40;

    static const int screenWidth = 1500;
    static const int screenHeight = 1000;

    static bool gameOver = false; 
    static Vector2 offset = { 0 };
    static bool allowMove = false; 



    static void InitGame(void);
    static void UpdateGame(void); 
    static void DrawGame(void); 




typedef enum GameScreen { TITLE, GAMEPLAY, GAME, END } GameScreen;

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

    InitWindow(screenWidth, screenHeight, "TANKS");
    
    GameScreen currentScreen = TITLE;
    SetTargetFPS(60);

    while (!WindowShouldClose())    // Основной игровой цикл
    {
        if (currentScreen == TITLE)
        {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) 
            { 
                currentScreen = GAMEPLAY; 
            }
        }
        else if (currentScreen == GAMEPLAY)
        {
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            {
                currentScreen = GAME;
            }
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float blinkPeriod = 1.5f; 
        float timeElapsed = GetTime();
        float t = fmod(timeElapsed, blinkPeriod) / blinkPeriod;
        float alphaFactor = (sinf(t * 2 * PI - PI / 2) + 1) / 2;

        int alpha = (int)(alphaFactor * 255);
        Color blinkingColor = (Color){DARKGREEN.r, DARKGREEN.g, DARKGREEN.b, alpha};

        switch (currentScreen)
        {
            case TITLE:
                DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                DrawText("HARDCORE TANKS 2D", 20, 20, 40, DARKGREEN); 
                DrawText("PRESS ENTER or TAP to Jump to GAME INSTRUCTION", 120, 220, 20, DARKGREEN);
                break;
                
            case GAMEPLAY:
                DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                DrawText("GAMEPLAY INSTRUCTION", 20, 20, 40, DARKGREEN); 
                DrawText("MOVE THE TANK ON THE MAP USING THE WASD KEYS,", 30, 250, 50, DARKGREEN);
                DrawText("SHOOT THROUGH THE SPACEBAR,", 300, 350, 50, DARKGREEN);
                DrawText("ENJOY THE GAME:)", 350, 500, 80, DARKGREEN);
                DrawText("PRESS ENTER or TAP to Jump to GAMEPLAY", 500, 750, 20, blinkingColor);
                break;
                

            case GAME: 
                InitGame(); 
                break;
                
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void InitGame(void)
{
    gameOver = 0; 
    allowMove = false; 

    offset.x = screenWidth%SQUARE_SIZE; 
    offset.y = screenHeight%SQUARE_SIZE;

}
