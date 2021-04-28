#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <unistd.h>


struct point {
    float x = 0;
    float y = 0;
};


float norm(float a, float b) {
    return sqrt(a*a + b*b);
}


void randDots(int amount, std::vector<point>& lst) {
    for (int i = 0; i < amount; i++) {
        lst[i].x = static_cast<int> (rand()) / (static_cast<int> (RAND_MAX/1100));
        lst[i].y = static_cast<int> (rand()) / (static_cast<int> (RAND_MAX/800));    
    }
}


void drawDots(sf::RenderTarget& surface ,int amount, std::vector<point> lst) {
    sf::CircleShape circ;
    circ.setRadius(2.f);
    circ.setFillColor(sf::Color::White);

    for (int i = 0; i < amount; i++) {
        circ.setPosition(sf::Vector2f(lst[i].x, lst[i].y));
        surface.draw(circ);
    }
}

void moveDots(std::vector<point>& lst, float dst) {
    for (int i = 0; i < lst.size(); i++) {

        float angle = static_cast<float> (rand()) / (static_cast<float> (RAND_MAX/(2*M_PI)));
        

        lst[i].x += cos(angle) * dst;
        lst[i].y += sin(angle) * dst;
    }
}


class bolt {
    public:
    std::vector<int> used;
    point head;


    void advance(point& pos, std::vector<int>& beenTo ,std::vector<point>& lst) {
        float dst = 10000;
        point p;
        int dex;
        for (int i = 0; i < lst.size(); i++) {
            bool u = false;
            for (int j = 0; j < beenTo.size(); j++) {
                if (i == beenTo[j]) {
                    u = true;
                }
            }

            int temp = norm(pos.x-lst[i].x, pos.y-lst[i].y);
            if (lst[i].x > pos.x) {
                temp += 5;
            }
            if (temp < dst && u == false) {
                dst = temp;
                p = lst[i];
                dex = i;
            }
        }
        beenTo.push_back(dex);
        pos = p;
    }


    void draw(sf::RenderTarget& surface, std::vector<int> beenTo, std::vector<point> lst) {
        sf::VertexArray line(sf::LinesStrip, 2);
        line[0].color = sf::Color::White;
        line[1].color = sf::Color::White;
        
        if (beenTo.size() > 1) {
            for (int i = 0; i < beenTo.size() - 1; i++) {
                line[0].position = sf::Vector2f(lst[beenTo[i]].x, lst[beenTo[i]].y);
                line[1].position = sf::Vector2f(lst[beenTo[i+1]].x, lst[beenTo[i+1]].y);
                surface.draw(line);
            }
        }
    }
};



int main() {
    sf::RenderWindow window(sf::VideoMode(1100,800), "lightning");

    //std::srand(2);

    int dotNum = 10000;

    std::vector<point> dots;
    dots.resize(dotNum);
    randDots(dotNum, dots);

    bolt light = bolt();
    light.head.x = 1100;
    light.head.y = 400;
    


    while (window.isOpen()) {
        //////////////////////////////////
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        /////////////////////////////////
        randDots(dotNum, dots);
        bolt light = bolt();
        light.head.x = 1100;
        light.head.y = 400;




        while (light.head.x > 10 && (light.used.size() < dotNum/2.f)) {
            light.advance(light.head, light.used ,dots);
            light.draw(window, light.used, dots);
            window.display();
            window.clear();
        }
        
        

        usleep(0.5f * 1000000);


        /////////////////////////////////
        window.display();
        window.clear();
    }
    return 0;
}