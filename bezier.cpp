#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>
#include <iostream>

std::vector< std::vector<long long> > binarr;
long long binomial(int n, int k)
{
    while(n >= binarr.size())
    {
        int s = binarr.size();
        std::vector<long long> nextRow;
        nextRow.push_back(1);
        for(int i = 1; i < s; i++)
            nextRow.push_back(binarr[s-1][i-1] + binarr[s-1][i]);
        nextRow.push_back(1);
        binarr.push_back(nextRow);
    }
    return binarr[n][k];
}
void Bezier(int n, sf::Vector2f* points,sf::RenderWindow& app)
{
    static int coutn  = 0;
    float x,y;
    sf::Vertex pixel;
    pixel.color = sf::Color::Black;
    std::vector<sf::Vertex> bez;
//    sf::Uint8* pixarr = new sf::Uint8[500][400][4];
//    for(int i=0;i < 500; i ++)
//    for(int j=0;j < 500; j ++)
//    {
//        pixarr[i][j][0] = sf::Color::White.r;
//        pixarr[i][j][1] = sf::Color::White.g;
//        pixarr[i][j][2] = sf::Color::White.b;
//        pixarr[i][j][3] = sf::Color::White.a;
////    std::fill(pixarr,pixarr + 500*400*4,sf::Color::White.r);
////    std::fill(pixarr,pixarr + 500*400*4,sf::Color::White.g);
////    std::fill(pixarr,pixarr + 500*400*4,sf::Color::White.b);
////    std::fill(pixarr,pixarr + 500*400*4,sf::Color::White.a);
////
//    }
    for(float t = 0.0 ; t <= 1.0 ; t += 0.001)
    {
        x = y = 0;
        float tpow[n+1] = {1};
        for(int i = 1; i <= n; i++)
            tpow[i] = tpow[i-1] * t;

        float mtp = 1;
        for(int i = 0; i <= n; i++)
        {
            x += binomial(n,i) * points[i].x * tpow[n-i] * mtp;
            y += binomial(n,i) * points[i].y * tpow[n-i] * mtp;
            mtp *= (1-t);
        }
//        pixarr[int(x)][int(y)][0] = sf::Color::Black.r;
//        pixarr[int(x)][int(y)][1] = sf::Color::Black.g;
//        pixarr[int(x)][int(y)][2] = sf::Color::Black.b;
//        pixarr[int(x)][int(y)][3] = sf::Color::Black.a;
        bez.push_back(sf::Vertex(sf::Vector2f(x,y),sf::Color::Black));
//        pixel.position = sf::Vector2f(x,y),
//        app.draw(&pixel,1,sf::Points);
    }
    app.draw(&bez[0],bez.size(),sf::Points);
//    sf::Image im;
//    im.create(500u,400u, &pixarr[0][0][0]);
//    sf::Sprite sp;
//    sf::Texture te;
//    te.create(500,400);
//    te.update(&pixarr[0][0][0]);
//    sp.setTexture(te);
//    app.draw(sp);
//    std::cout << "Drew " << coutn++ << "\n";
}

int main()
{
    std::vector<long long> a;
    a.push_back(1);
    binarr.push_back(a);

    sf::RenderWindow window(sf::VideoMode(500, 400), "Bezier Curve",sf::Style::Default);
    window.setVerticalSyncEnabled(true);
//    window.setFramerateLimit(120);
    sf::Vector2f coords[] = {{30,30}, {90,150}, {420,51},{450,300}};
    int degree = sizeof coords / (sizeof coords[0]) - 1;
    int ismovable = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonReleased)
            {
                ismovable = 0;
                window.setView(window.getDefaultView());
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ismovable)
            {
                int x,y;
                for(int i = 0; i <= degree; i++)
                {
//                    if()
                    x = sf::Mouse::getPosition(window).x;
                    y = sf::Mouse::getPosition(window).y;
                    if(abs(x-coords[i].x) < 7 && abs(y-coords[i].y) < 7)
                    {
                        ismovable = i + 1;
                        break;
                    }
                }
//                if (event.type.MouseButtonPressed == sf::Mouse::Left)
//                    ismovable = true;
            }
        }

//        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        if (ismovable)
        {
            coords[ismovable-1] = (sf::Vector2f) sf::Mouse::getPosition(window);
//            sf::View view = window.getDefaultView();
//            view.setCenter(coords[ismovable-1]);
//            view.zoom(1.5f);

//            window.setView(view);
        }


        window.clear(sf::Color::White);
//        window.draw((sf::VertexArray *) coords,degree,sf::Lines);
        Bezier(degree,coords,window);
        sf::CircleShape point(3);
        point.setFillColor(sf::Color::Blue);
//        coords[2] = (sf::Vector2f) sf::Mouse::getPosition(window);

        for(int i = 0; i <= degree; i++)
        {
            point.setPosition(coords[i]);
            point.setOrigin(point.getRadius(),point.getRadius());
            window.draw(point);
        }
//        std::cout << "x : " << sf::Mouse::getPosition().x << "y : " << sf::Mouse::getPosition().y << "\n";
//        std::cout << "Is pressed : " << sf::Mouse::isButtonPressed(sf::Mouse::Left) << "\n";
        window.display();
    }
    return 0;
}
