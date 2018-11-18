#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> //For std::swap

void drawClippingWindow(sf::Vertex [],sf::RenderWindow&);
bool clipTest(float p, float q, float& u1, float& u2)
{
    float r;
    bool retVal = true;

    if (p < 0.0) {
        r = q / p;
        if (r > u2)
            retVal = false;
        else
            if (r > u1)
                u1 = r;
    }
    else
    if (p > 0) {
        r = q / p;
        if ( r < u1)
            retVal = false;
        else
            if (r < u2)
                u2 = r;
    }
    else if (q < 0)
        retVal = false;
    return retVal;
}
void clipLine(sf::Vector2f& winMin,sf::Vector2f& winMax,sf::Vector2f p1,sf::Vector2f p2,sf::RenderWindow& window)
{
    float u1 = 0.0, u2 = 1.0, dx = p2.x - p1.x, dy;
    if(clipTest(-dx,p1.x - winMin.x, u1, u2))
    if(clipTest(dx, winMax.x - p1.x, u1, u2)) {
        dy = p2.y - p1.y;
        if (clipTest(-dy, p1.y - winMin.y, u1, u2))
        if (clipTest(dy, winMax.y - p1.y, u1, u2)) {
            if (u2 < 1.0) {
                p2.x = p1.x + u2 * dx;
                p2.y = p1.y + u2 * dy;
            }
            if (u1 > 0.0)
            {
                p1.x += u1 * dx;
                p1.y += u1 * dy;
            }
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(p1.x,p1.y)),
                sf::Vertex(sf::Vector2f(p2.x,p2.y)),
            };
            line[0].color = sf::Color::Red;
            line[1].color = sf::Color::Red;
            window.draw(line,2,sf::Lines);
//            std::cout << "DRawn";
        }
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(600,400),"Liang Barsky - Line Clipping");
    window.setVerticalSyncEnabled(true);
//    window.setFramerateLimit(250);
    sf::Vertex winCoord[] =
    {
        sf::Vertex(sf::Vector2f(50,50)),
        sf::Vertex(sf::Vector2f(150,150)),
    };

    sf::Vertex lines[] =
    {
        sf::Vertex(sf::Vector2f(40,70)),
        sf::Vertex(sf::Vector2f(80,20)),
        sf::Vertex(sf::Vector2f(50,50)),
        sf::Vertex(sf::Vector2f(100,50)),
        sf::Vertex(sf::Vector2f(70,50)),
        sf::Vertex(sf::Vector2f(70,200)),
    };
    bool showHiddenLines = false;
    sf::Clock clock;
    double lastTime = 0;
    int c;
    while(window.isOpen())
    {
        double currentTime = clock.restart().asSeconds();
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed: window.close(); break;
                case sf::Event::KeyPressed:
                    switch(event.key.code)
                    {
                        case sf::Keyboard::H:
                            showHiddenLines = !showHiddenLines;
                            break;
                        case sf::Keyboard::Left:
                            winCoord[0].position.x -= 2;
                            winCoord[1].position.x -= 2;
                            break;
                        case sf::Keyboard::Right:
                            winCoord[0].position.x += 2;
                            winCoord[1].position.x += 2;
                            break;
                        case sf::Keyboard::Up:
                            winCoord[0].position.y -= 2;
                            winCoord[1].position.y -= 2;
                            break;
                        case sf::Keyboard::Down:
                            winCoord[0].position.y += 2;
                            winCoord[1].position.y += 2;
                            break;
                        case sf::Keyboard::S:
                            winCoord[1].position.x += 2;
                            winCoord[1].position.y += 2;
                            break;
                        case sf::Keyboard::A:
                            winCoord[1].position.x -= 2;
                            winCoord[1].position.y -= 2;
                            break;
                    }
            }
        }
        window.clear();
        drawClippingWindow(winCoord,window);
        if (showHiddenLines)
            window.draw(lines,6,sf::Lines);
        for(int i = 0; i < 3;i ++)
            clipLine(winCoord[0].position,winCoord[1].position,lines[i*2].position,lines[i*2+1].position,window);

        window.display();
//        std::cin.get();
        lastTime +=  1/currentTime;
        if(c>=1000)
        std::cout << "Fps: " << lastTime/1000 << "\n", c = 0,lastTime = 0;
        c++;
//        std::cout << "Fps: " << 1.f/currentTime << "\n";
    }
}

void drawClippingWindow(sf::Vertex winCoord[],sf::RenderWindow& window)
{
    sf::Color borderColor = sf::Color::Yellow;
    sf::Vertex lines[] =
    {
        winCoord[0],
        sf::Vertex(sf::Vector2f(winCoord[0].position.x,winCoord[1].position.y)),
        winCoord[1],
        sf::Vertex(sf::Vector2f(winCoord[1].position.x,winCoord[0].position.y)),
        winCoord[0],
    };
    lines[0].color = borderColor;
    lines[1].color = borderColor;
    lines[2].color = borderColor;
    lines[3].color = borderColor;
    lines[4].color = borderColor;
    window.draw(lines,5,sf::LinesStrip);
//    std::cout << "DREW";
}
