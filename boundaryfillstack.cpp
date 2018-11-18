#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

void PolyFill(std::vector<std::pair<int,int> >& mystack, sf::RenderWindow& window,sf::Image& img,sf::Color& desired)
{
    static int callcount = 0;
    callcount++;

//    std::cout << "CALL COUNT " << callcount << " Stack Size : " << mystack.size() <<  '\n';
//    for(int i = 0; i < mystack.size(); i ++ ) std::cout << "{ " << mystack[i].first << ", " << mystack[i].second << ' ';
//    std::cout << std::endl;

    if (mystack.size() == 0) return;
    std::pair<int,int> curPoint = mystack[mystack.size()-1];mystack.pop_back();

//    if (img.getPixel(curPoint.first , curPoint.second) == sf::Color::White || img.getPixel(curPoint.first , curPoint.second)== desired) return;

//sf::Texture texture;
//    sf::Sprite sprite;
//    sf::Event event;
//    while(window.pollEvent(event))
//    {
//        if(event.type == sf::Event::Closed)
//            window.close();
//    }
//        texture.loadFromImage(img);
//        texture.setRepeated(true);
//        sprite.setTexture(texture, true);
//        window.draw(sprite);
//    window.display();
    bool top = false,bottom = false;
    //go right
    for(int i = 0; ; i++)
    {
        if (img.getPixel(curPoint.first + i, curPoint.second) == sf::Color::White)
        {
            break;
        }

        if (!top) if (img.getPixel(curPoint.first + i , curPoint.second - 1) == sf::Color::Black) mystack.push_back({curPoint.first + i , curPoint.second - 1}), top = true;
        if (!bottom) if (img.getPixel(curPoint.first + i , curPoint.second + 1) == sf::Color::Black) mystack.push_back({curPoint.first + i , curPoint.second + 1}), bottom = true;

        if (img.getPixel(curPoint.first + i , curPoint.second - 1) == sf::Color::White) top = false;
        if (img.getPixel(curPoint.first + i , curPoint.second + 1) == sf::Color::White) bottom = false;
    }
    //go left
    for(int i = 0; ; i--)
    {
        if (img.getPixel(curPoint.first + i, curPoint.second) == sf::Color::White) break;

        img.setPixel(curPoint.first + i, curPoint.second,desired);

        if (!top) if (img.getPixel(curPoint.first + i, curPoint.second - 1) == sf::Color::Black) mystack.push_back({curPoint.first + i , curPoint.second - 1}), top = true;
        if (!bottom) if (img.getPixel(curPoint.first + i, curPoint.second + 1) == sf::Color::Black) mystack.push_back({curPoint.first + i , curPoint.second + 1}), bottom = true;

        if (img.getPixel(curPoint.first + i, curPoint.second - 1) == sf::Color::White) top = false;
        if (img.getPixel(curPoint.first + i, curPoint.second + 1) == sf::Color::White) bottom = false;
    }
//    std::cin.get();
    PolyFill(mystack,window,img,desired);
//
//    {
////        std::cout << "CALL " << callcount << "\n";
//        img.setPixel(curPoint.first , curPoint.second,desired);
//        PolyFill(x+1,y,window,img,desired);
//        PolyFill(x,y+1,window,img,desired);
//        PolyFill(x,y-1,window,img,desired);
//        PolyFill(x-1,y,window,img,desired);
//    }
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(600,400),"Poly fill");
    window.setVerticalSyncEnabled(true);
//    window.setFramerateLimit(5);
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
    static sf::Color desired = sf::Color::Red;
    sf::Image img;
    std::vector< std::pair<int,int> > stk;
    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(polygon,5,sf::LinesStrip);
        img = window.capture();
        stk.push_back({30,30});
        PolyFill(stk,window,img,desired);
        texture.loadFromImage(img);
        texture.setRepeated(true);
        sprite.setTexture(texture, true);
        window.draw(sprite);
        window.display();
//        std::cout  << "END";
    }
}
