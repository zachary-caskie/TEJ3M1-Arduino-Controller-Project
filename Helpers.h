#pragma once
#include <SFML/Graphics.hpp>

sf::Vector2f getDir(sf::Vector2f a, sf::Vector2f b);
float getDist(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f rotaMtrx(sf::Vector2f dir, float angle);
float mapValue(float x, float in_min, float in_max, float out_min, float out_max);