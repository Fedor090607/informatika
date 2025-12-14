#include <iostream>
#include "GameView.h"
#include "GameScene.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    try
    {
        // Создаем окно
        GameView* pGameView = NewGameView({ 1500, 1600 });

        // Создаем сцену
        GameScene* pGameScene = NewGameScene();

        if (pGameScene)
        {
            // Запускаем игровой цикл
            EnterGameLoop(*pGameView, UpdateGameScene, DrawGameScene, pGameScene);
        }

        // Чистим память
        DestroyGameScene(pGameScene);
        DestroyGameView(pGameView);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}