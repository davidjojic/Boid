#include <SFML/Graphics.hpp>
#include<cmath>
#include <iostream>
#include <vector>
using namespace std;

int width = 1200, hight = 800;
float xang[360], yang[360];

sf::Color some_color(float k) {
    if (k < 0.0) k = 0;
    else if (k > 1.0) k = 1;
    int r = 0, g = 0, b = 0;
    if (k < 0.5) {
        b = 255 * (1.f - 2 * k);
        g = 255 * 2 * k;
    }
    else {
        g = 255 * (2.f - 2 * k);
        r = 255 - (2 * k - 1);
    }
    return sf::Color(r, g, b);
}

class Zid {
private:
    sf::Vector2f poz;
    sf::RectangleShape zi;
public:
    Zid(float poz_x, float poz_y, int Zx, int Zy) {
        poz = { poz_x, poz_y };
        zi.setFillColor(sf::Color::Red);
        zi.setSize(sf::Vector2f(Zx, Zy));
        zi.setPosition(poz);
    }
    void crtaj(sf::RenderWindow& window) {
        window.draw(zi);
    }
    sf::Vector2f getPoz() {
        return poz;
    }
};

vector<Zid> k;

class Boid {
private:
    sf::Vector2f poz;
    sf::ConvexShape boid;
    float rot;
public:
    Boid(float poz_x, float poz_y, float Rot) {
        poz = { poz_x, poz_y };
        rot = Rot;
        boid.setPointCount(3);
        boid.setPoint(0, sf::Vector2f(0, -10));
        boid.setPoint(1, sf::Vector2f(-5, 5));
        boid.setPoint(2, sf::Vector2f(5, 5));
    }

    void crtanje(sf::RenderWindow& window) {
        boid.setPosition(poz);
        boid.setRotation(rot + 90);
        poz = { poz.x + 2 * xang[(int)rot], poz.y + 2 * yang[(int)rot] };

        if (poz.x < 0) poz.x = (float)width;
        if (poz.x > width) poz.x = 0;
        if (poz.y < 0) poz.y = (float)hight;
        if (poz.y > hight) poz.y = 0;

        window.draw(boid);
    }

    void promena(vector<Boid>& v, int j) {
        float ang = 0;

        for (int i = 0; i < v.size(); i++) {
            if (i == j) continue;
            float temx2 = v[i].poz.x;
            float temy2 = v[i].poz.y;
            float temx1 = v[j].poz.x;
            float temy1 = v[j].poz.y;
            float dis = sqrt((temx2 - temx1) * (temx2 - temx1) + (temy2 - temy1) * (temy2 - temy1));

            if (dis > 30 && dis < 75) {
                if (v[i].rot > v[j].rot) ang += (float)75 / dis;
                else ang -= (float)75 / dis;
            }
            else if (dis <= 30) {
                if (v[i].rot > v[j].rot) ang -= (float)75 / dis;
                else ang += (float)75 / dis;
            }
        }

        for (int i = 0; i < k.size(); i++) {
            sf::Vector2f wallPos = k[i].getPoz();
            float temx2 = wallPos.x;
            float temy2 = wallPos.y;
            float temx1 = v[j].poz.x;
            float temy1 = v[j].poz.y;
            float dis = sqrt((temx2 - temx1) * (temx2 - temx1) + (temy2 - temy1) * (temy2 - temy1));

            if (dis < 50) {
                if (v[j].rot > 180) ang += 5 * (float)75 / dis;
                else ang -= 5 * (float)75 / dis;
            }
        }

        rot += ang;
        if (rot > 359) rot = 0;
        if (rot < 0) rot = 359;
    }

    void boja(sf::Color al) {
        boid.setFillColor(al);
    }
};

int main() {
    for (int i = 0; i < 360; i++) {
        float radians = i * 3.14159265358979323846 / 180.0;
        xang[i] = cos(radians);
        yang[i] = sin(radians);
    }

    sf::RenderWindow window(sf::VideoMode(width, hight), "Boid Simulation", sf::Style::Default);
    window.setFramerateLimit(60);

    vector<Boid> v;
    int numOfBoids = 150;

    for (int i = 0; i < numOfBoids; i++) {
        int rnd = rand() % 360;
        int sx = rand() % width;
        int sy = rand() % hight;
        v.push_back({ (float)sx, (float)sy, (float)rnd });
        sf::Color smt = some_color((float)i / numOfBoids);
        v[i].boja(smt);
    }
    sf::sleep(sf::seconds(4));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int i = 0; i < numOfBoids; i++) {
            v[i].crtanje(window);
            v[i].promena(v, i);
        }

        for (int i = 0; i < k.size(); i++) {
            k[i].crtaj(window);
        }

        window.display();
    }
}