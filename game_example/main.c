#include "raylib.h"
#include <stdlib.h>
#include <math.h> 

#define SQUARE_SIZE 40


        /*|-----------------------------------------------------|
          |        STRUCT's and DESCRIPTION's of GAMEPLAY       |
          |-----------------------------------------------------|*/


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

typedef struct Tank //возможно добавить выстрелы через указатель и еще что-то
{
    Vector2 position; 
    Vector2 speed; 
    Vector2 size; 
    Color color; 
    float cooldown; 

} Tank;


    static const int screenWidth = 1500;
    static const int screenHeight = 1000;
    static const int count = 1000000;
    
    static bool gameOver = false; 
    static Vector2 offset = { 0 };
    static bool allowMove = false; 
    static Tank tank[count] = { 0 };
    



    static void InitGame(void);
    static void UpdateGame(void); 
    static void DrawGame(void); 



        /*|-----------------------------------------------------|
          |             ENTRY POINT                             |
          |-----------------------------------------------------|*/

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
        else if (currentScreen == GAME)
        {
            if(IsKeyPressed(KEY_TAB))
            {
                currentScreen = END;
            }
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        /*|-----------------------------------------------------|
          |all calculation of the pulsation effect through sine |
          |-----------------------------------------------------|*/
        float blinkPeriod = 1.5f; 
        float timeElapsed = GetTime();
        float t = fmod(timeElapsed, blinkPeriod) / blinkPeriod;
        float alphaFactor = (sinf(t * 2 * PI - PI / 2) + 1) / 2;

        int alpha = (int)(alphaFactor * 255);
        Color blinkingColor = (Color){ DARKGREEN.r, DARKGREEN.g, DARKGREEN.b, alpha} ;
        //-------------------------------------------------------------------------------
        Color blinkingGameOver = (Color){ BLACK.r, BLACK.g, BLACK.b, alpha };
        switch (currentScreen)
        {
            case TITLE:
                DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                DrawText("HARDCORE TANKS 2D", 20, 20, 40, DARKGREEN); 
                DrawText("PRESS ENTER or TAP to Jump to GAME INSTRUCTION", 120, 220, 20, blinkingColor);
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
                
                for(int i = 0; i < count; i++)
                { 
                    DrawRectangleV(tank[i].position, tank[i].size, tank[i].color);
                }
                break;


            case END: 
                DrawRectangle(0, 0, screenWidth, screenHeight, RED);
                DrawText("GAME OVER:(", 400, 350, 100, blinkingGameOver);
            break;

            default:
                break;
                
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void InitGame(void)
{
    gameOver = false; 
    allowMove = false; 

    offset.x = screenWidth % SQUARE_SIZE; 
    offset.y = screenHeight % SQUARE_SIZE;

    for(int i = 0; i < count; i++)
    {
        tank[i].position = (Vector2){ offset.x / 2, offset.y / 2 };
        tank[i].speed = (Vector2){ SQUARE_SIZE, 0 };
        tank[i].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
        
        if (i == 0) tank[i].color = DARKBLUE;
        else tank[i].color = BLUE;
    }
     
}
