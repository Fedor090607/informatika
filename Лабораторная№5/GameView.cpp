#include "GameView.h"

GameView* NewGameView(const sf::Vector2i& windowSize)
{
    GameView* pView = new GameView;
    pView->windowSize = windowSize;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    pView->window.create(sf::VideoMode(windowSize.x, windowSize.y),
        "Platformer Game",
        sf::Style::Close, settings);
    pView->window.setFramerateLimit(60);

    pView->camera.reset(sf::FloatRect(0.0f, 0.0f, float(windowSize.x), float(windowSize.y)));
    pView->camera.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

    return pView;
}

void EnterGameLoop(GameView& view, OnUpdate onUpdate, OnDraw onDraw, void* pData)
{
    while (view.window.isOpen())
    {
        sf::Event event;
        while (view.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                view.window.close();
                break;
            }
        }

        const sf::Time elapsedTime = view.clock.restart();
        float deltaSec = elapsedTime.asSeconds();
        if (deltaSec > 0.1f) deltaSec = 0.1f;

        if (onUpdate) onUpdate(pData, view, deltaSec);

        // Просто очищаем черным
        view.window.clear();

        if (onDraw) onDraw(pData, view);
        view.window.display();
    }
}

void SetCameraCenter(GameView& view, const sf::Vector2f& center)
{
    view.camera.setCenter(center);
    view.window.setView(view.camera);
}

void DestroyGameView(GameView*& pView)
{
    if (pView) {
        pView->window.close();
        delete pView;
        pView = nullptr;
    }
}