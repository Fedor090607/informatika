#include <SFML/Graphics.hpp>
#include <iostream>
int main()
{
    const int cellSize = 50;
    const int gridSize = 10;

    sf::RenderWindow window(sf::VideoMode(500, 500), "Var 15");
    window.setFramerateLimit(60);

    sf::RectangleShape cells[gridSize][gridSize];

    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            cells[i][j].setSize(sf::Vector2f(cellSize - 2, cellSize - 2));
            cells[i][j].setPosition(i * cellSize, j * cellSize);
            cells[i][j].setOutlineColor(sf::Color::Black);
            cells[i][j].setOutlineThickness(1);

            if (i == 0 || i == gridSize - 1 || i == j) {
                cells[i][j].setFillColor(sf::Color::Green);
            }
            else {
                cells[i][j].setFillColor(sf::Color::White);
            }
        }
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);
        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                window.draw(cells[i][j]);
            }
        }

        window.display();
    }

    return 0;
}

