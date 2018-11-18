#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> //For std::swap
void drawClippingWindow(sf::Vertex [],sf::RenderWindow&);

#define ROUND(a) ((int) (a + 0.5))
#define LEFT_EDGE 0x1
#define RIGHT_EDGE 0x2
#define BOTTOM_EDGE 0x4
#define TOP_EDGE 0x8

#define INSIDE(a) (!a)
#define REJECT(a,b) (a&b)
#define ACCEPT(a,b) (!(a|b))

unsigned char encode(const sf::Vector2f& point, const sf::Vector2f& winMin, const sf::Vector2f& winMax);

void swapPts(sf::Vector2f& a,sf::Vector2f& b)
{ std::swap(a,b); }
void swapCodes(unsigned char & c1,unsigned char & c2)
{
    std::swap(c1,c2);
}

void clipLine(sf::Vector2f& winMin,sf::Vector2f& winMax,sf::Vector2f p1,sf::Vector2f p2, sf::RenderWindow& window)
{
    unsigned char code1,code2;
    bool done = false,draw = false;
    float m;
    while (!done)
    {
        code1 = encode(p1,winMin,winMax);
        code2 = encode(p2,winMin,winMax);
//        std::cout << "code1 : "  << code1 << " code 2 : " << code2 << "\n";
        if (ACCEPT(code1,code2))
        {
            done = true;
            draw = true;
        }
        else
        {
            if(REJECT(code1,code2))
                done = true;
            else {
                if ( INSIDE(code1))
                    swapPts(p1,p2), swapCodes(code1,code2);
                if(p2.x != p1.x)
                    m = (p2.y - p1.y) / (p2.x - p1.x);
                if(code1 & LEFT_EDGE) {
                    p1.y += (winMin.x - p1.x) * m;
                    p1.x = winMin.x;
                }
                else
                if (code1 & RIGHT_EDGE) {
                    p1.y += (winMax.x - p1.x) * m;
                    p1.x = winMax.x;
                }
                else
                if (code1 & BOTTOM_EDGE) {
                    if (p2.x != p1.x)
                        p1.x += (winMin.y - p1.y) / m;
                    p1.y = winMin.y;

                }
                else
                if(code1 & TOP_EDGE) {
                    if (p2.x != p1.x)
                        p1.x += (winMax.y  - p1.y) / m;
                    p1.y = winMax.y;
                }
            }
        }
    }
    if (draw)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(p1.x,p1.y)),
            sf::Vertex(sf::Vector2f(p2.x,p2.y)),
        };
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        window.draw(line,2,sf::Lines);
//        std::cout << "Clipped";
    }
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(600,400),"Cohen Sutherland");
    window.setVerticalSyncEnabled(true);
//    window.setFramerateLimit(5);
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
    sf::Clock clock;
    double lastTime = 0;
    int c = 1;
    bool showHiddenLines = false;
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
//        std::cout << "LASR RIMW : " << lastTime << "\n";
//        std::cout << "CURR RIMW : " << currentTime << "\n";
//        float fps = 1.f / (currentTime - lastTime);
        lastTime +=  1/currentTime;
        if(c>=1000)
        std::cout << "Fps: " << lastTime/1000 << "\n", c = 0,lastTime = 0;
        c++;
//        std::cin.get();
    }
}

unsigned char encode(const sf::Vector2f& point, const sf::Vector2f& winMin, const sf::Vector2f& winMax)
{
    unsigned char code = 0x00;
    if (point.x < winMin.x) code = code | LEFT_EDGE;
//    std::cout << "Reutr Cdo : " << (int)code << "\n";    if (point.x > winMax.x) code = code | RIGHT_EDGE;
//    std::cout << "Reutr Cdo : " << (int)code << "\n";    if (point.y < winMin.y) code = code | BOTTOM_EDGE;
//    std::cout << "Reutr Cdo : " << (int)code << "\n";    if (point.y > winMax.y) code = code | TOP_EDGE;
//    std::cout << "Reutr Cdo : " << (int)code << "\n";
    return code;
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
