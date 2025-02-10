#include "raylib.h"
#include <stdlib.h>
#include <math.h>

// --- Константы для игрового мира ---
#define SQUARE_SIZE 40
#define MAX_ENEMIES 50
#define ENEMY_SPEED 2.0f

// Константы для пуль врагов
#define MAX_BULLETS 100
#define BULLET_SPEED 5.0f
#define BULLET_RADIUS 5

// Константы для пуль игрока
#define MAX_PLAYER_BULLETS 100

// Скорость движения игрока (пикселей в секунду)
#define PLAYER_SPEED 200.0f

// --- Определение игровых состояний ---
typedef enum GameScreen { TITLE, GAMEPLAY, GAME, END } GameScreen;

// --- Структуры для врагов, пуль и танка ---

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
    int bounceCount;  // 0 - ещё не отскакивал, 1 - уже отскочил
} Bullet;
Bullet enemyBullets[MAX_BULLETS];

// Глобальный массив пуль игрока
static Bullet playerBullets[MAX_PLAYER_BULLETS];

typedef struct Tank {
    Vector2 position; 
    Vector2 speed; 
    Vector2 size; 
    Color color; 
    float cooldown; 
} Tank;
// Здесь всего один танк игрока
static const int count = 1;  
static Tank tank[count] = { 0 };

static const int screenWidth = 1500;
static const int screenHeight = 1000;

static bool gameOver = false; 
static Vector2 offset = { 0 };
static bool allowMove = false; 

// Флаг для однократной инициализации игрока при переходе в состояние GAME
static bool gameInitialized = false;

// --- Прототипы игровых функций ---
static void InitGame(void);
static void SpawnEnemies(void); 
static void UpdateEnemies(Vector2 playerPos); 
static void DrawEnemies(void); 
static void DrawDetailedTank(Vector2 position, Vector2 size, Color bodyColor, Color turretColor);

// --- Прототипы функций для пуль врагов ---
static void InitBullets(void);
static void SpawnEnemyBullet(Vector2 enemyPos, Vector2 playerPos);
static void UpdateBullets(void);
static void DrawBullets(void);

// --- Прототипы функций для пуль игрока ---
static void InitPlayerBullets(void); 
static void SpawnPlayerBullet(Vector2 playerPos);
static void UpdatePlayerBullets(void); 
static void DrawPlayerBullets(void);

int main(void)
{
    InitWindow(screenWidth, screenHeight, "TANKS");
    GameScreen currentScreen = TITLE;
    SetTargetFPS(60);

    // Инициализируем врагов как неактивных
    for (int i = 0; i < MAX_ENEMIES; i++) { 
        enemies[i].active = false; 
    }
    // Инициализируем пули врагов и пули игрока
    InitBullets();
    InitPlayerBullets();

    // Таймер для спавна врагов
    float spawnTimer = 0.0f; 
    float spawnInterval = 1.0f; 

    while (!WindowShouldClose())    // Основной игровой цикл
    {
        // Обработка переключения экранов
        if (currentScreen == TITLE)
        {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                currentScreen = GAMEPLAY;
        }
        else if (currentScreen == GAMEPLAY)
        {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            {
                if (!gameInitialized)
                {
                    InitGame();       // Инициализируем танк игрока
                    gameInitialized = true;
                }
                currentScreen = GAME;
            }
        }
        else if (currentScreen == GAME)
        {
            if (IsKeyPressed(KEY_TAB))
                currentScreen = END;
        }
        
        // Если в состоянии GAME
        if (currentScreen == GAME)
        {
            // Обработка передвижения танка игрока с помощью WASD
            if (IsKeyDown(KEY_W)) tank[0].position.y -= PLAYER_SPEED * GetFrameTime();
            if (IsKeyDown(KEY_S)) tank[0].position.y += PLAYER_SPEED * GetFrameTime();
            if (IsKeyDown(KEY_A)) tank[0].position.x -= PLAYER_SPEED * GetFrameTime();
            if (IsKeyDown(KEY_D)) tank[0].position.x += PLAYER_SPEED * GetFrameTime();

            // Выстрел игрока по нажатию SPACE (один выстрел за нажатие)
            if (IsKeyPressed(KEY_SPACE))
            {
                // Пуля появляется из центра верхней части танка
                Vector2 bulletSpawnPos = { tank[0].position.x + SQUARE_SIZE/2, tank[0].position.y };
                SpawnPlayerBullet(bulletSpawnPos);
            }
            UpdatePlayerBullets();

            // Обновляем спавн врагов
            spawnTimer += GetFrameTime();
            if (spawnTimer >= spawnInterval)
            {
                SpawnEnemies();
                spawnTimer = 0.0f;
            }
            // Обновляем позицию врагов, используя позицию игрока
            Vector2 playerPos = tank[0].position;
            UpdateEnemies(playerPos);

            // Враги стреляют с небольшой вероятностью (~1% шанс за кадр)
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active && GetRandomValue(0, 1000) < 10)
                    SpawnEnemyBullet(enemies[i].position, playerPos);
            }
            // Обновляем пули врагов
            UpdateBullets();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        /* Пульсирующий эффект для текста */
        float blinkPeriod = 1.5f;
        float timeElapsed = GetTime();
        float t = fmod(timeElapsed, blinkPeriod) / blinkPeriod;
        float alphaFactor = (sinf(t * 2 * PI - PI/2) + 1) / 2;
        int alpha = (int)(alphaFactor * 255);
        Color blinkingColor = (Color){ DARKGREEN.r, DARKGREEN.g, DARKGREEN.b, alpha };
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
                DrawText("PRESS ENTER or TAP to Jump to GAME", 500, 750, 20, blinkingColor);
                break;

            case GAME:
            {
                // Отрисовываем танк игрока с детализированной отрисовкой (примитивами)
                DrawDetailedTank(tank[0].position, tank[0].size, tank[0].color, DARKGRAY);

                // Отрисовываем врагов с детализированной отрисовкой
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (enemies[i].active)
                        DrawDetailedTank(enemies[i].position, (Vector2){40, 40}, RED, GRAY);
                }

                // Отрисовываем пули врагов
                DrawBullets();
                // Отрисовываем пули игрока
                DrawPlayerBullets();
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

// --- Функция инициализации игрока ---
void InitGame(void)
{
    gameOver = false;
    allowMove = false;
    offset.x = screenWidth % SQUARE_SIZE;
    offset.y = screenHeight % SQUARE_SIZE;
    // Размещаем танк игрока по центру нижней части экрана
    tank[0].position = (Vector2){ screenWidth/2 - SQUARE_SIZE/2, screenHeight - SQUARE_SIZE - 20 };
    tank[0].speed = (Vector2){ PLAYER_SPEED, PLAYER_SPEED };
    tank[0].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
    tank[0].color = DARKBLUE;
}

// --- Функция спавна врагов ---
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

// --- Функция обновления движения врагов (движутся в направлении игрока) ---
void UpdateEnemies(Vector2 playerPos)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].active)
        {
            Vector2 direction = { playerPos.x - enemies[i].position.x, playerPos.y - enemies[i].position.y };
            float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
            if (length != 0)
            {
                direction.x /= length;
                direction.y /= length;
            }
            enemies[i].position.x += direction.x * enemies[i].speed;
            enemies[i].position.y += direction.y * enemies[i].speed;
        }
    }
}

// --- Функция отрисовки детализированного танка (для игрока и врагов) ---
void DrawDetailedTank(Vector2 position, Vector2 size, Color bodyColor, Color turretColor)
{
    // Рисуем корпус танка
    DrawRectangleV(position, size, bodyColor);
    
    // Вычисляем размеры и позицию башни (60% от размеров корпуса)
    Vector2 turretSize = { size.x * 0.6f, size.y * 0.6f };
    Vector2 turretPos = {
        position.x + (size.x - turretSize.x) / 2,
        position.y + (size.y - turretSize.y) / 2
    };
    DrawRectangleV(turretPos, turretSize, turretColor);
    
    // Рисуем пушку: линия, выходящая из центра башни вверх
    Vector2 turretCenter = { turretPos.x + turretSize.x / 2, turretPos.y + turretSize.y / 2 };
    Vector2 cannonEnd = { turretCenter.x, turretCenter.y - turretSize.y * 0.8f };
    DrawLineEx(turretCenter, cannonEnd, 5, BLACK);
}

// --- Функции для пуль врагов ---
void InitBullets(void)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        enemyBullets[i].active = false;
        enemyBullets[i].bounceCount = 0;
    }
}

void SpawnEnemyBullet(Vector2 enemyPos, Vector2 playerPos)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!enemyBullets[i].active)
        {
            enemyBullets[i].position = enemyPos;
            Vector2 direction = { playerPos.x - enemyPos.x, playerPos.y - enemyPos.y };
            float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
            if (length != 0)
            {
                direction.x /= length;
                direction.y /= length;
            }
            enemyBullets[i].velocity.x = direction.x * BULLET_SPEED;
            enemyBullets[i].velocity.y = direction.y * BULLET_SPEED;
            enemyBullets[i].bounceCount = 0;
            enemyBullets[i].active = true;
            break;
        }
    }
}

void UpdateBullets(void)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (enemyBullets[i].active)
        {
            enemyBullets[i].position.x += enemyBullets[i].velocity.x;
            enemyBullets[i].position.y += enemyBullets[i].velocity.y;
            // Если пуля вышла за пределы экрана, деактивируем её
            if (enemyBullets[i].position.x < 0 || enemyBullets[i].position.x > screenWidth ||
                enemyBullets[i].position.y < 0 || enemyBullets[i].position.y > screenHeight)
            {
                enemyBullets[i].active = false;
            }
        }
    }
}

void DrawBullets(void)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (enemyBullets[i].active)
        {
            DrawCircleV(enemyBullets[i].position, BULLET_RADIUS, ORANGE);
        }
    }
}

// --- Функции для пуль игрока ---
void InitPlayerBullets(void)
{
    for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
    {
        playerBullets[i].active = false;
        playerBullets[i].bounceCount = 0;
    }
}

void SpawnPlayerBullet(Vector2 playerPos)
{
    for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
    {
        if (!playerBullets[i].active)
        {
            // Пуля появляется из центра верхней части танка игрока
            playerBullets[i].position = playerPos;
            // Танк игрока стреляет вверх
            playerBullets[i].velocity = (Vector2){ 0, -BULLET_SPEED };
            playerBullets[i].bounceCount = 0;
            playerBullets[i].active = true;
            break;
        }
    }
}

void UpdatePlayerBullets(void)
{
    for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
    {
        if (playerBullets[i].active)
        {
            playerBullets[i].position.x += playerBullets[i].velocity.x;
            playerBullets[i].position.y += playerBullets[i].velocity.y;
            // Если пуля выходит за верхнюю границу, деактивируем её
            if (playerBullets[i].position.y + BULLET_RADIUS < 0)
                playerBullets[i].active = false;
        }
    }
}

void DrawPlayerBullets(void)
{
    for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
    {
        if (playerBullets[i].active)
        {
            DrawCircleV(playerBullets[i].position, BULLET_RADIUS, BLUE);
        }
    }
}
