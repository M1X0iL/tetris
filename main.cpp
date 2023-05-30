#include <SFML/Graphics.hpp>
#include <time.h>
#include <sstream>
#include <iostream>

using namespace sf;

const int height = 20;
const int width = 10;

int space[height][width] = {0};
int pixels = 34;

struct Block
        {
    int x, y;
}a[4], b[4], c[4];

int figures[7][4] = {
        1,3,5,7,
        2,4,5,7,
        3,5,4,6,
        3,5,4,7,
        2,3,5,7,
        3,5,7,6,
        2,3,4,5,
};

bool check(){
    for (int i = 0; i < 4; i++)
        if (a[i].x<0 || a[i].x >= width || a[i].y >= height)
            return 0;
        else if (space[a[i].y][a[i].x])
            return 0;

    return 1;
}


bool game_over() {
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= width || a[i].y >= height)
            return 0;
        else if (space[a[i].y][a[i].x])
            return 0;

    return 1;
}

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode((width) * pixels+200, (height) * pixels), "Tetris");

    Texture texture;
    texture.loadFromFile("C:/Users/79297/CLionProjects/untitled6/Paint/tiles.png");
    Sprite tiles(texture);

    Texture bg;
    bg.loadFromFile("C:/Users/79297/CLionProjects/untitled6/Paint/bg.png");
    Sprite background(bg);

    Texture go;
    go.loadFromFile("C:/Users/79297/CLionProjects/untitled6/Paint/gameover1.png");
    Sprite gg(go);

    Font font;
    font.loadFromFile("C:/Users/79297/CLionProjects/untitled6/Rostov.ttf");
    Text text("",font,20);
    text.setFillColor(Color::Red);


    int dx = 0, colorNum = 1, score = 0;
    bool rotate = false;
    float timer=0, delay=0.3, difficulty = 0.001, diff = 0;
    Clock clock;
    bool add = true, add1 = true;


    while (window.isOpen())
    {
        float time=clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type== Event::KeyPressed)
                if (event.key.code == Keyboard::Up)
                    rotate = true;
                else if (event.key.code == Keyboard::Right)
                    dx = 1;
                else if (event.key.code == Keyboard::Left)
                    dx = -1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;


        for (int i = 0; i < 4; i++) {
            b[i]=a[i];
            a[i].x += dx;
        }

        if (!check()){
            for (int i = 0; i < 4; i++)
                a[i]=b[i];
        }

        if (rotate){
            Block p=a[1];
            for (int i = 0; i < 4; i++){
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;

                a[i].x=p.x-x;
                a[i].y=p.y+y;
            }

            if (!check()){
                for (int i = 0; i < 4; i++)
                    a[i]=b[i];
            }
        }

        if (timer>delay){
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
            }
            if (!check()){
                for (int i = 0; i < 4; i++)
                    space[b[i].y][b[i].x]=colorNum;
                colorNum= 1 + rand() % 7;
                int n = rand() % 7;
                difficulty+=0.001;
                if (difficulty>0.2)
                    difficulty=0.2;
                for (int i = 0; i < 4; i++) {
                    a[i] = c[i];
                    c[i].x=figures[n][i]%2;
                    c[i].y=figures[n][i]/2;

                }

            }
            timer = 0;
        }

        if (add) {
            int n = rand() % 7;
            if (c[0].x == 0)
                for (int i = 0; i < 4; i++) {
                    c[i].x = figures[n][i] % 2;
                    c[i].y = figures[n][i] / 2;
                }
            add = false;
        }

        if (add1) {
            int n = rand() % 7;
            if (a[0].x == 0)
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            add1 = false;
        }


        int k = height - 1;
        for (int i=height-1; i>0; i--){
            int count = 0;
            for (int j = 0; j<width; j++){
                if (space[i][j]) {
                    count++;
                }
                space[k][j] = space[i][j];
            }
            if (count<width)
                k--;
        }


        dx = 0;
        rotate = false;
        delay=0.3-difficulty;
        diff = difficulty*1000;

        window.clear(Color::White);
        window.draw(background);

        if (!game_over()) {
            window.clear(Color::White);
            gg.setPosition(170, 340);
            window.draw(gg);
        }
        else {

            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++) {
                    if (space[i][j] == 0)
                        continue;
                    tiles.setTextureRect(IntRect(space[i][j] * pixels, 0, pixels, pixels));
                    tiles.setPosition(j * pixels, i * pixels);
                    window.draw(tiles);
                }

            for (int i = 0; i < 4; i++) {
                tiles.setTextureRect(IntRect(colorNum * pixels, 0, pixels, pixels));
                tiles.setPosition(a[i].x * pixels, a[i].y * pixels);
                window.draw(tiles);
            }
            std::ostringstream delaystring;
            delaystring << diff;
            text.setString("Difficulty:"+ delaystring.str());
            text.setPosition(360,440);
            window.draw(text);

            for (int i = 0; i < 4; i++) {
                tiles.setTextureRect(IntRect(colorNum * pixels, 0, pixels, pixels));
                tiles.setPosition(c[i].x * pixels + 400, c[i].y * pixels + 200);
                window.draw(tiles);
            }
        }
        window.display();
    }

    return 0;
}
