#include "GameScene.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using sf::Keyboard;
using sf::Vector2f;

//Константы физики и геймплея
const float GRAVITY = 1200.f;
const float BASE_SPEED = 200.f;
const float BASE_JUMP_FORCE = -350.f;

//Бонусные значения
const float BOOSTED_SPEED = 400.f;
const float BOOSTED_JUMP_FORCE = -650.f;
const float BONUS_DURATION = 5.0f;

//Параметры объектов
const float BRICK_LIFE_TIME = 1.0f;
const float ENEMY_SPEED = 100.0f;

GameScene* NewGameScene()
{
    GameScene* pLogic = new GameScene;

    //Загрузка карты
    if (!pLogic->level.LoadFromFile("res/platformer.tmx"))
    {
        delete pLogic;
        return nullptr;
    }

    //Инициализация стандартных списков
    pLogic->player = pLogic->level.GetFirstObject("player");
    pLogic->coins = pLogic->level.GetAllObjects("coin");
    pLogic->blocks = pLogic->level.GetAllObjects("block");     // Невидимые стены
    pLogic->blocksB = pLogic->level.GetAllObjects("blockB");   // Видимые стены
    pLogic->bonusBoxes = pLogic->level.GetAllObjects("box");
    pLogic->glitchCoins = pLogic->level.GetAllObjects("glitchCoin");

    //Инициализация врагов (преобразование в структуру Enemy)
    std::vector<TmxObject> rawEnemies = pLogic->level.GetAllObjects("enemy");
    for (const auto& obj : rawEnemies)
    {
        Enemy enemy;
        enemy.obj = obj;
        enemy.speed = ENEMY_SPEED;
        enemy.direction = 1.0f; // Изначально идут вправо
        pLogic->enemies.push_back(enemy);
    }

    //Инициализация разрушаемых блоков (преобразование в структуру BreakableBlock)
    std::vector<TmxObject> rawBricks = pLogic->level.GetAllObjects("brick");
    for (const auto& obj : rawBricks)
    {
        BreakableBlock brick;
        brick.obj = obj;
        brick.timeToBreak = BRICK_LIFE_TIME;
        brick.isTriggered = false;
        pLogic->bricks.push_back(brick);
    }

    //Начальные параметры игрока
    pLogic->startPosition = pLogic->player.sprite.getPosition();
    pLogic->playerVelocity = { 0.f, 0.f };
    pLogic->isGrounded = false;
    pLogic->currentMoveSpeed = BASE_SPEED;
    pLogic->currentJumpForce = BASE_JUMP_FORCE;
    pLogic->bonusTimer = 0.f;

    return pLogic;
}

//Проверка столкновений со статичными объектами (blocks и blocksB)
bool CheckStaticCollision(const sf::FloatRect& rect, const std::vector<TmxObject>& list)
{
    for (const auto& obj : list)
    {
        if (rect.intersects(obj.rect)) return true;
    }
    return false;
}

//Проверка столкновений с разрушаемыми блоками
//Возвращает true при ударе. Если блок еще не активирован, активирует его.
bool CheckBreakableCollision(const sf::FloatRect& rect, std::vector<BreakableBlock>& bricks)
{
    bool collided = false;
    for (auto& brick : bricks)
    {
        if (rect.intersects(brick.obj.rect))
        {
            collided = true;
            if (!brick.isTriggered)
            {
                brick.isTriggered = true;
                //Визуальная индикация удара (красный оттенок)
                brick.obj.sprite.setColor(sf::Color(255, 150, 150));
            }
        }
    }
    return collided;
}

//Проверка коллизий для врагов (стены, видимые стены, кирпичи)
bool CheckEnemyWallCollision(const sf::FloatRect& rect, GameScene* pLogic)
{
    if (CheckStaticCollision(rect, pLogic->blocks)) return true;
    if (CheckStaticCollision(rect, pLogic->blocksB)) return true;

    for (const auto& brick : pLogic->bricks) {
        if (rect.intersects(brick.obj.rect)) return true;
    }

    return false;
}

void UpdateGameScene(void* pData, GameView& view, float deltaSec)
{
    GameScene* pLogic = (GameScene*)pData;

    //Логика Врагов
    for (auto& enemy : pLogic->enemies)
    {
        // Двигаем врага
        Vector2f move = { enemy.speed * enemy.direction * deltaSec, 0.f };
        enemy.obj.MoveBy(move);

        //Если врезался в стену — откат и разворот
        if (CheckEnemyWallCollision(enemy.obj.rect, pLogic))
        {
            enemy.obj.MoveBy(-move);
            enemy.direction *= -1.0f;
        }
    }

    //Обновление разрушаемых блоков
    for (auto it = pLogic->bricks.begin(); it != pLogic->bricks.end();)
    {
        if (it->isTriggered)
        {
            it->timeToBreak -= deltaSec;
            //Эффект исчезновения (прозрачность)
            sf::Color c = it->obj.sprite.getColor();
            c.a = static_cast<sf::Uint8>(255 * (it->timeToBreak / BRICK_LIFE_TIME));
            it->obj.sprite.setColor(c);
        }

        //Если время вышло — удаляем блок
        if (it->timeToBreak <= 0)
        {
            it = pLogic->bricks.erase(it);
        }
        else
        {
            ++it;
        }
    }

    //Таймер бонусов
    if (pLogic->bonusTimer > 0)
    {
        pLogic->bonusTimer -= deltaSec;
        if (pLogic->bonusTimer <= 0)
        {
            //Возврат характеристик к базовым значениям
            pLogic->currentMoveSpeed = BASE_SPEED;
            pLogic->currentJumpForce = BASE_JUMP_FORCE;
        }
    }

    //Физика и Управление игроком

    //Ввод (X)
    pLogic->playerVelocity.x = 0;
    if (Keyboard::isKeyPressed(Keyboard::A)) pLogic->playerVelocity.x = -pLogic->currentMoveSpeed;
    if (Keyboard::isKeyPressed(Keyboard::D)) pLogic->playerVelocity.x = pLogic->currentMoveSpeed;

    //Гравитация (Y)
    pLogic->playerVelocity.y += GRAVITY * deltaSec;

    //Прыжок
    if (Keyboard::isKeyPressed(Keyboard::Space) && pLogic->isGrounded)
    {
        pLogic->playerVelocity.y = pLogic->currentJumpForce;
        pLogic->isGrounded = false;
    }

    //Применение движения (X)
    Vector2f moveX = { pLogic->playerVelocity.x * deltaSec, 0.f };
    pLogic->player.sprite.move(moveX);
    sf::FloatRect playerBounds = pLogic->player.sprite.getGlobalBounds();

    //Проверка столкновений по X (со всеми типами препятствий)
    bool hitStaticX = CheckStaticCollision(playerBounds, pLogic->blocks);
    bool hitBlockBX = CheckStaticCollision(playerBounds, pLogic->blocksB);
    bool hitBrickX = CheckBreakableCollision(playerBounds, pLogic->bricks);

    if (hitStaticX || hitBlockBX || hitBrickX)
    {
        pLogic->player.sprite.move(-moveX);
    }

    //Применение движения (Y)
    Vector2f moveY = { 0.f, pLogic->playerVelocity.y * deltaSec };
    pLogic->player.sprite.move(moveY);
    playerBounds = pLogic->player.sprite.getGlobalBounds();

    pLogic->isGrounded = false;

    //Проверка столкновений по Y
    bool hitStaticY = CheckStaticCollision(playerBounds, pLogic->blocks);
    bool hitBlockBY = CheckStaticCollision(playerBounds, pLogic->blocksB);
    bool hitBrickY = CheckBreakableCollision(playerBounds, pLogic->bricks);

    if (hitStaticY || hitBlockBY || hitBrickY)
    {
        pLogic->player.sprite.move(-moveY);
        //Если падали вниз и ударились — значит, встали на землю
        if (pLogic->playerVelocity.y > 0)
        {
            pLogic->isGrounded = true;
        }
        pLogic->playerVelocity.y = 0;
    }

    //Взаимодействие с предметами
    //Сбор монет
    for (auto it = pLogic->coins.begin(); it != pLogic->coins.end();)
    {
        if (pLogic->player.sprite.getGlobalBounds().intersects(it->sprite.getGlobalBounds()))
            it = pLogic->coins.erase(it);
        else
            ++it;
    }

    //Сбор бонусных коробок
    for (auto it = pLogic->bonusBoxes.begin(); it != pLogic->bonusBoxes.end();)
    {
        if (pLogic->player.sprite.getGlobalBounds().intersects(it->sprite.getGlobalBounds()))
        {
            int randomBonus = rand() % 2;
            if (randomBonus == 0) {
                pLogic->currentMoveSpeed = BOOSTED_SPEED;
                pLogic->currentJumpForce = BASE_JUMP_FORCE;
            }
            else {
                pLogic->currentJumpForce = BOOSTED_JUMP_FORCE;
                pLogic->currentMoveSpeed = BASE_SPEED;
            }
            pLogic->bonusTimer = BONUS_DURATION;
            it = pLogic->bonusBoxes.erase(it);
        }
        else ++it;
    }

    //Монета выхода (закрывает игру)
    for (auto it = pLogic->glitchCoins.begin(); it != pLogic->glitchCoins.end();)
    {
        if (pLogic->player.sprite.getGlobalBounds().intersects(it->sprite.getGlobalBounds()))
        {
            view.window.close();
            return;
        }
        else ++it;
    }

    //Взаимодействие с врагами (смерть)
    for (const auto& enemy : pLogic->enemies)
    {
        if (pLogic->player.sprite.getGlobalBounds().intersects(enemy.obj.sprite.getGlobalBounds()))
        {
            // Возврат на старт и сброс параметров
            pLogic->player.MoveTo(pLogic->startPosition);
            pLogic->playerVelocity = { 0, 0 };
            pLogic->currentMoveSpeed = BASE_SPEED;
            pLogic->currentJumpForce = BASE_JUMP_FORCE;
            pLogic->bonusTimer = 0;
        }
    }

    //Камера следует за игроком
    SetCameraCenter(view, pLogic->player.sprite.getPosition());
}

void DrawGameScene(void* pData, GameView& view)
{
    GameScene* pLogic = (GameScene*)pData;

    //Рисуем карту (фон)
    pLogic->level.Draw(view.window);

    //Рисуем объекты
    for (const auto& block : pLogic->blocksB) view.window.draw(block.sprite);
    for (const auto& brick : pLogic->bricks) view.window.draw(brick.obj.sprite);
    for (const auto& coin : pLogic->coins) view.window.draw(coin.sprite);
    for (const auto& gCoin : pLogic->glitchCoins) view.window.draw(gCoin.sprite);
    for (const auto& box : pLogic->bonusBoxes) view.window.draw(box.sprite);

    //Рисуем врагов
    for (const auto& enemy : pLogic->enemies) view.window.draw(enemy.obj.sprite);

    //Рисуем игрока
    view.window.draw(pLogic->player.sprite);
}

void DestroyGameScene(GameScene*& pScene)
{
    if (pScene)
    {
        delete pScene;
        pScene = nullptr;
    }
}