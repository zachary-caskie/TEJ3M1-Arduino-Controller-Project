#include <SFML/Graphics.hpp>
#include "Bird.h"
#include <iostream>
#include <fstream>
#include "Crosshair.h"
#include "SerialPort.h"
#include "Helpers.h"

int main()
{
    //---------------INITIALIZE--------------------
    sf::Clock dtClock;
    sf::Clock spawnClock;
    std::vector<Bird> birds;
    bool clicked = false;
    float y2 = 0.f;

    SerialPort arduino("COM8", CBR_9600);

    //----Score code----
    int score = 0;

    sf::Font uiFont;

    if (!uiFont.openFromFile("assets/uiFont.ttf")) {
        std::cerr << "Failed to load glitch font\n";
    }
    
    sf::Text scoreText(uiFont);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color(5, 7, 23));
    scoreText.setPosition({ 1700, 100 });

    //texture initialization
    sf::Texture chTex;
    if (!chTex.loadFromFile("assets/Crosshair2.png")) {
        std::cerr << "Failed to load crosshair texture\n";
    }

    std::cout << "Crosshair texture: "
        << chTex.getSize().x << ", "
        << chTex.getSize().y << "\n";

    sf::RenderTexture gameTex({ 320, 180 });
    gameTex.setSmooth(false);

    Crosshair cross(chTex);

    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "arduino game", sf::State::Fullscreen);
    window.setMouseCursorVisible(false);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }
        //-------------UPDATE---------------

        //delta time
        float dt = dtClock.restart().asSeconds();
        //---------Parser------------
        std::string line;
        if (arduino.readLine(line)) {

            std::cout << "RAW: [" << line << "]\n";

            std::string xs, ys, cs, y2s;
            std::stringstream ss(line);

            if (std::getline(ss, xs, '/') &&
                std::getline(ss, ys, '/') &&
                std::getline(ss, cs, '/') &&
                std::getline(ss, y2s))
            {
                float x = std::stof(xs);
                float y = std::stof(ys);
                clicked = (cs == "1");
                y2 = std::stof(y2s);

                //normalize joystick values
                float nx = -((x - 512.0f) / 512.0f);
                float ny = -((y - 512.0f) / 512.0f);

                const float deadzone = 0.05f;

                if (std::abs(nx) < deadzone) nx = 0;
                if (std::abs(ny) < deadzone) ny = 0;

                cross.update({ nx, ny }, dt);
            }
        }


        //-----------------BIRD SPAWNING------------
        static float timeBetweenSpawns = 0.5;
        float spawnTime = spawnClock.getElapsedTime().asSeconds();
        if (spawnTime > timeBetweenSpawns && birds.size() < 50)
        {
            Bird x;
            birds.push_back(x);
            spawnClock.restart();
        }
        //--BIRD UPDATING--
        static bool wasClicked = false;
        static bool fire = false;
        if (wasClicked) { fire = false; }
        if (!wasClicked && clicked)
            {
             if (clicked)
                {
                 wasClicked = true;
                 fire = true;
                }
            }
        if (!clicked)
        {
            wasClicked = false;
        }
        
        //Bird shooting code
        for (auto& i : birds)
        {
            i.update(dt);
            if (i.isTargeted(cross.getPos()) && fire)
            {
                i.kill();
                score++;
            }
        }
        birds.erase(
            std::remove_if(birds.begin(), birds.end(),
                [](const Bird& b) { return b.isDead(); }),
            birds.end()
        );
        scoreText.setString("Score: " + std::to_string(score));//update score
        //Night scaling
        //uses right joystick to shift sky colour from day to night
        static int scaleNum = 50;
        static float mappedY2 = 0;
        mappedY2 = mapValue(y2, 0, 1023, -5, 5);//map joystick to scale velocity
        if (mappedY2 > -0.1 && mappedY2 < 0.1) { mappedY2 = 0; }
        if (scaleNum >= 0 && mappedY2 < 0)//interpolate using joystick velocity
        {
            scaleNum += mappedY2;
            scaleNum = std::clamp(scaleNum, 0, 50);
        }
        if (scaleNum <= 50 && mappedY2 > 0)
        {
            scaleNum += mappedY2;
            scaleNum = std::clamp(scaleNum, 0, 50);
        }

        float ctrlR = mapValue(scaleNum, 0, 50, 7, 180);
        float ctrlR2 = mapValue(scaleNum, 0, 50, 185, 5);
        float ctrlG = mapValue(scaleNum, 0, 50, 3, 212);
        float ctrlG2 = mapValue(scaleNum, 0, 50, 195, 7);
        float ctrlB = mapValue(scaleNum, 0, 50, 20, 212);
        float ctrlB2 = mapValue(scaleNum, 0, 50, 239, 23);

        scoreText.setFillColor(sf::Color(ctrlR2, ctrlG2, ctrlB2));
        cross.setColour(sf::Color(ctrlR2, ctrlG2, ctrlB2));

        //std::cout << mappedY2 << "\n"; // debug right y output
    
        //std::cout << birds.size() << "\n";

        //-------------DRAW---------------
        sf::Color night(7, 3, 20);
        window.clear(sf::Color(ctrlR, ctrlG, ctrlB));
        for (auto& i : birds)
        {
            i.draw(window);
        }
        cross.draw(window);
        window.draw(scoreText);
        window.display();

    }
}