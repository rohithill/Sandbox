#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
sf::Window ad;
sf::Music mas;
//using nameace std;
bool slowly = false;
void BoundaryFill(float x,float y, sf::RenderWindow& window,sf::Image& img,sf::Color& desired)
{
    static int callcount = 0;
    callcount++;
    if (img.getPixel(int(x) , int(y)) == sf::Color::White || img.getPixel(int(x),int(y))== desired) return;
    else
    {
//        std::cout << "CALL " << callcount << "\n";
        img.setPixel(int(x),int(y),desired);
    if (slowly) {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            window.close();
    }
        texture.loadFromImage(img);
        texture.setRepeated(true);
        sprite.setTexture(texture, true);
        window.draw(sprite);
    window.display();
    }
        if ( img.getPixel(int(x+1),int(y))!= desired)
        BoundaryFill(x+1,y,window,img,desired);
        if ( img.getPixel(int(x),int(y+1))!= desired)
        BoundaryFill(x,y+1,window,img,desired);
        if ( img.getPixel(int(x),int(y-1))!= desired)
        BoundaryFill(x,y-1,window,img,desired);
        if ( img.getPixel(int(x-1),int(y))!= desired)
        BoundaryFill(x-1,y,window,img,desired);
    }
//    std::cout << "AV";
//    std::cout << "ECI";
}
void DrawRectangle();
int main()
{
    slowly = true;
    sf::RenderWindow window(sf::VideoMode(600,400),"Poly fill");
    window.setVerticalSyncEnabled(true);
//    window.setFramerateLimit(50);
    int counta = 0;
    sf::Vertex polygon[] =
    {
        sf::Vertex(sf::Vector2f(10,10)),
        sf::Vertex(sf::Vector2f(30,100)),
        sf::Vertex(sf::Vector2f(70,80)),
        sf::Vertex(sf::Vector2f(100,30)),
        sf::Vertex(sf::Vector2f(10,10)),
//        sf::Vertex(sf::Vector2f(10,10)),
    };
    sf::Texture texture;
    sf::Sprite sprite;

//Then, in PlayState::render()
//    game->window.draw(sprite);
    static sf::Color desired = sf::Color::Red;
    sf::Image img;
    while (window.isOpen())
    {
//        std::cout << counta ++ << "  Desired " <<std::hex<< desired.toInteger() <<  "\n";
//        int sp = 1 << 8;
//        std::cout << "Sp: " << sp << "  Desired " << (desired.toInteger() + sp) <<  "\n";
//        std::cout << "AV";
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
//        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
//        {
//            desired = sf::Color((desired.toInteger() + (1<<20)));
//            std::cout << (1<<4) << "  Updated " << desired.toInteger() <<   "\n";
//        }
        window.clear();
        window.draw(polygon,5,sf::LinesStrip);
        img = window.capture();
        BoundaryFill(30.5,30.5,window,img,desired);
        texture.loadFromImage(img);
        texture.setRepeated(true);
        sprite.setTexture(texture, true);
        window.draw(sprite);
        window.display();
//        std::cout << "END";
    }

}
