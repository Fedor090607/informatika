#include <SFML/Graphics.hpp>
#include <functional>
#include <cmath>
#include <string>
#include <iostream>

void drawGraph(sf::RenderWindow& window, std::function<float(float)> func, float xMin, float xMax, float scaleX, float scaleY, sf::Color color) {
    sf::VertexArray graph(sf::LinesStrip);
    for (float x = xMin; x <= xMax; x += 0.1f) {
        float y = func(x); 
        float screenX = 400 + x * scaleX;
        float screenY = 300 - y * scaleY;
        graph.append(sf::Vertex(sf::Vector2f(screenX, screenY), color));
    }
    window.draw(graph);
}

int main() {
    setlocale(LC_ALL, "Russian");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Graphic 15 Var");
    sf::CircleShape userPoint(5);
    userPoint.setOrigin(5, 5); 
    userPoint.setFillColor(sf::Color::Red);
    bool userPointExists = false;
 
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: Could not load font arial.ttf" << std::endl;
        return -1;
    }

    sf::Text coordinatesText;
    coordinatesText.setFont(font);
    coordinatesText.setCharacterSize(20);
    coordinatesText.setFillColor(sf::Color::White);
    coordinatesText.setPosition(10, 10);
    coordinatesText.setString("Click on the window to set a point");

    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(50, 300);
    xAxis[0].color = sf::Color::White; 
    xAxis[1].position = sf::Vector2f(750, 300);
    xAxis[1].color = sf::Color::White;

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(400, 50);
    yAxis[0].color = sf::Color::White; 
    yAxis[1].position = sf::Vector2f(400, 550);
    yAxis[1].color = sf::Color::White;

    float scaleX = 30.0f; 
    float scaleY = 5.0f;  

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    float mathX = (mousePos.x - 400) / scaleX; 
                    float mathY = -(mousePos.y - 300) / scaleY; 

                    userPoint.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                    userPointExists = true;
                    std::string zoneString = "Undefined";

                    float y1 = 4.0f*(mathX)+8.0f;  
                    float y2 = 6.0f*(mathX);         

                    if (std::fabs(mathY - y1) < 0.3f || std::fabs(mathY - y2) < 0.3f) {
                        zoneString = "Boundary";
                    }
                    else if (mathY < y1 && mathY < y2) {
                        zoneString = "Zone: 1";
                    }
                    else if (mathY > y1 && mathY > y2) {
                        zoneString = "Zone: 2";
                    }
                    else if (mathY < y1 && mathY > y2) {
                        zoneString = "Zone: 3";
                    }
                    else if (mathY > y1 && mathY < y2) {
                        zoneString = "Zone: 4";
                    }
                    else {
                        zoneString = "Error";
                    }
                    coordinatesText.setString("Coordinates: (" + std::to_string(mathX) + ", " + std::to_string(mathY) + ")\n" + zoneString);
                }
            }
        }

        window.clear(sf::Color(40, 42, 54));
        window.draw(xAxis);
        window.draw(yAxis);
        drawGraph(window, [](float x) { return 4.0f * x + 8.0f; }, -10, 10, scaleX, scaleY, sf::Color::Red);
        drawGraph(window, [](float x) { return 6.0f * x; }, -10, 10, scaleX, scaleY, sf::Color::Blue);
        if (userPointExists) {
            window.draw(userPoint);
            window.draw(coordinatesText);
        }
        window.display();
    }
    return 0;
}
