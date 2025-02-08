#include "raylib.h"
#include <stdlib.h>
#include <math.h> 

#define SQUARE_SIZE 40
#define MAX_ENEMIES 50
#define ENEMY_SPEED 2.0f

// Новые константы для пуль
#define MAX_BULLETS 100
#define BULLET_SPEED 5.0f
#define BULLET_RADIUS 5

/*|-----------------------------------------------------|
  |        STRUCT's and DESCRIPTION's of GAMEPLAY       |
  |-----------------------------------------------------|*/

typedef enum GameScreen { TITLE, GAMEPLAY, GAME, END } GameScreen;

typedef struct Enemy { 
    Vector2 position; 
    float speed; 
    bool active;
} Enemy;

Enemy enemies[MAX_ENEMIES];

typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    bool active;
} Bullet;

Bullet enemyBullets[MAX_BULLETS];

typedef enum TileType {
    TILE_EMPTY, 
    TILE_WALL, 
    TILE_WATER,
} TileType;

typedef struct Map {
    int width; 
    int height; 
    int tileSize; 
    TileType *tiles;
} Map;

typedef struct Tank {
    Vector2 position; 
    Vector2 speed; 
    Vector2 size; 
    Color color; 
    float cooldown; 
} Tank;

// Здесь всего один танк игрока
static const int count = 1;  

static const int screenWidth = 1500;
static const int screenHeight = 1000;

static bool gameOver = false; 
static Vector2 offset = { 0 };
static bool allowMove = false; 
static Tank tank[count] = { 0 };

// Флаг для однократной инициализации игрока при переходе в состояние GAME
static bool gameInitialized = false;

static void InitGame(void);
static void SpawnEnemies(void); 
static void UpdateEnemies(Vector2 playerPos); 
static void DrawEnemies(void); 
static void DrawDetailedTank(Vector2 position, Vector2 size, Color bodyColor, Color turretColor);

// Новые функции для пуль
static void InitBullets(void);
static void SpawnEnemyBullet(Vector2 enemyPos, Vector2 playerPos);
static void UpdateBullets(void);
static void DrawBullets(void);

/*|-----------------------------------------------------|
  |             ENTRY POINT                             |
  |-----------------------------------------------------|*/

int main(void)
{
    InitWindow(screenWidth, screenHeight, "TANKS");
    GameScreen currentScreen = TITLE;
    SetTargetFPS(60);

    // Инициализируем всех врагов как неактивных
    for (int i = 0; i < MAX_ENEMIES; i++) { 
        enemies[i].active = false; 
    }
    // Инициализируем пули как неактивные
    InitBullets();

    // Таймер для спавна врагов
    float spawnTimer = 0.0f; 
    float spawnInterval = 1.0f; 

    while (!WindowShouldClose())    // Основной игровой цикл
    {
        // Обработка переключения экранов
        if (currentScreen == TITLE)
        {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                currentScreen = GAMEPLAY; 
            }
        }
        else if (currentScreen == GAMEPLAY)
        {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (!gameInitialized) {
                    InitGame();       // Инициализируем игрока
                    gameInitialized = true;
                }
                currentScreen = GAME;
            }
        }
        else if (currentScreen == GAME)
        {
            if (IsKeyPressed(KEY_TAB)) {
                currentScreen = END;
            }
        }
        
        // Если мы в состоянии GAME, обновляем спавн врагов
        if (currentScreen == GAME)
        {
            spawnTimer += GetFrameTime();
            if (spawnTimer >= spawnInterval)
            {
                SpawnEnemies();
                spawnTimer = 0.0f;
            }
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        /* Пульсирующий эффект для текста */
        float blinkPeriod = 1.5f; 
        float timeElapsed = GetTime();
        float t = fmod(timeElapsed, blinkPeriod) / blinkPeriod;
        float alphaFactor = (sinf(t * 2 * PI - PI / 2) + 1) / 2;
        int alpha = (int)(alphaFactor * 255);
        Color blinkingColor = (Color){ DARKGREEN.r, DARKGREEN.g, DARKGREEN.b, alpha};
        Color blinkingGameOver = (Color){ BLACK.r, BLACK.g, BLACK.b, alpha};
        
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
                DrawText("PRESS ENTER or TAP to Jump to GAME", 500, 750, 20, blinkingColor);
                break;
                
            case GAME:
            {
                // Обновляем позицию врагов, используя позицию игрока (танк игрока хранится в tank[0])
                Vector2 playerPos = tank[0].position;
                UpdateEnemies(playerPos);

                // Для каждого врага случайная стрельба:
                for (int i = 0; i < MAX_ENEMIES; i++) {
                    if (enemies[i].active) {
                        // Случайная вероятность выстрела (например, 1% шанс за кадр)
                        if (GetRandomValue(0, 1000) < 10) {
                            SpawnEnemyBullet(enemies[i].position, playerPos);
                        }
                    }
                }

                // Обновляем пули
                UpdateBullets();

                // Отрисовываем танк игрока с детальной отрисовкой
                for (int i = 0; i < count; i++) { 
                    DrawDetailedTank(tank[i].position, tank[i].size, tank[i].color, DARKGRAY);
                }
                
                // Отрисовываем врагов
                DrawEnemies();

                // Отрисовываем пули врагов
                DrawBullets();
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

// Функция инициализации игрока (одного танка)
// Вызывается один раз при переходе в состояние GAME
void InitGame(void)
{
    gameOver = false; 
    allowMove = false; 

    offset.x = screenWidth % SQUARE_SIZE; 
    offset.y = screenHeight % SQUARE_SIZE;

    // Инициализируем танк игрока: располагаем его по центру нижней части экрана
    for (int i = 0; i < count; i++)
    {
        tank[i].position = (Vector2){ screenWidth/2 - SQUARE_SIZE/2, screenHeight - SQUARE_SIZE - 20 };
        tank[i].speed = (Vector2){ SQUARE_SIZE, 0 };
        tank[i].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
        tank[i].color = DARKBLUE;
    }
}

// Функция спавна одного врага (ищет неактивный слот в массиве enemies)
void SpawnEnemies(void) 
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].position.x = GetRandomValue(0, screenWidth); 
            enemies[i].position.y = 0;  // враг появляется сверху
            enemies[i].speed = ENEMY_SPEED;
            enemies[i].active = true;
            break;
        }
    }
}

// Функция обновления движения врагов (движутся в направлении игрока)
void UpdateEnemies(Vector2 playerPos) 
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].active)
        {
            Vector2 direction = { playerPos.x - enemies[i].position.x, playerPos.y - enemies[i].position.y };
            float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
            if (length != 0) {
                direction.x /= length;
                direction.y /= length;
            }
            enemies[i].position.x += direction.x * enemies[i].speed;
            enemies[i].position.y += direction.y * enemies[i].speed;
        }
    }
}

// Функция отрисовки детализированного танка (используется как для игрока, так и для врагов)
void DrawDetailedTank(Vector2 position, Vector2 size, Color bodyColor, Color turretColor) {
    // Рисуем корпус танка
    DrawRectangleV(position, size, bodyColor);
    
    // Вычисляем размеры и позицию башни (60% от размеров корпуса)
    Vector2 turretSize = { size.x * 0.6f, size.y * 0.6f };
    Vector2 turretPos = {
        position.x + (size.x - turretSize.x) / 2,
        position.y + (size.y - turretSize.y) / 2
    };
    
    // Рисуем башню
    DrawRectangleV(turretPos, turretSize, turretColor);
    
    // Рисуем пушку: линия, выходящая из центра башни вверх
    Vector2 turretCenter = { turretPos.x + turretSize.x / 2, turretPos.y + turretSize.y / 2 };
    Vector2 cannonEnd = { turretCenter.x, turretCenter.y - turretSize.y * 0.8f };
    DrawLineEx(turretCenter, cannonEnd, 5, BLACK);
}

// Функция отрисовки врагов с детализированным танком
void DrawEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            DrawDetailedTank(enemies[i].position, (Vector2){40, 40}, RED, GRAY);
        }
    }
}

/*--------------------------*/
/* Функции для работы с пулями */
/*--------------------------*/

// Инициализация всех пуль как неактивных
void InitBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        enemyBullets[i].active = false;
    }
}

// Спавн пули врага, направленной к игроку
void SpawnEnemyBullet(Vector2 enemyPos, Vector2 playerPos) {
    // Ищем неактивную пулю в массиве
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!enemyBullets[i].active) {
            enemyBullets[i].position = enemyPos;
            // Вычисляем направление от врага к игроку
            Vector2 direction = { playerPos.x - enemyPos.x, playerPos.y - enemyPos.y };
            float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
            if (length != 0) {
                direction.x /= length;
                direction.y /= length;
            }
            // Устанавливаем скорость пули
            enemyBullets[i].velocity.x = direction.x * BULLET_SPEED;
            enemyBullets[i].velocity.y = direction.y * BULLET_SPEED;
            enemyBullets[i].active = true;
            break;
        }
    }
}

// Обновление позиции пуль
void UpdateBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (enemyBullets[i].active) {
            enemyBullets[i].position.x += enemyBullets[i].velocity.x;
            enemyBullets[i].position.y += enemyBullets[i].velocity.y;
            // Если пуля вышла за пределы экрана, деактивируем её
            if (enemyBullets[i].position.x < 0 || enemyBullets[i].position.x > screenWidth ||
                enemyBullets[i].position.y < 0 || enemyBullets[i].position.y > screenHeight) {
                enemyBullets[i].active = false;
            }
        }
    }
}

// Отрисовка пуль
void DrawBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (enemyBullets[i].active) {
            DrawCircleV(enemyBullets[i].position, BULLET_RADIUS, ORANGE);
        }
    }
}
