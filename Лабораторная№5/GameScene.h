#pragma once
#include "TmxLevel.h"
#include "GameView.h"
#include <vector>

// Структура для разрушаемого блока
struct BreakableBlock
{
    TmxObject obj;
    float timeToBreak;
    bool isTriggered;
};

// Структура для врага
struct Enemy
{
    TmxObject obj;
    float speed;
    float direction; // 1.0f (вправо) или -1.0f (влево)
};

struct GameScene
{
    TmxLevel level;
    TmxObject player;

    // Списки объектов
    std::vector<Enemy> enemies;         // Враги теперь имеют свою структуру
    std::vector<TmxObject> coins;
    std::vector<TmxObject> blocks;      // Невидимые стены
    std::vector<TmxObject> blocksB;     // Видимые неразрушаемые блоки
    std::vector<BreakableBlock> bricks; // Разрушаемые блоки
    std::vector<TmxObject> bonusBoxes;
    std::vector<TmxObject> glitchCoins;

    sf::Vector2f startPosition;

    sf::Vector2f playerVelocity;
    bool isGrounded;

    float currentMoveSpeed;
    float currentJumpForce;
    float bonusTimer;
};

GameScene* NewGameScene();
void UpdateGameScene(void* pData, GameView& view, float deltaSec);
void DrawGameScene(void* pData, GameView& view);
void DestroyGameScene(GameScene*& pScene);