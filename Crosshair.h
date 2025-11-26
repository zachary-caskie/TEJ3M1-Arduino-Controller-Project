#pragma once
#include <SFML/Graphics.hpp>

class Crosshair
{
public:
	Crosshair(const sf::Texture& tex);
	void update(sf::Vector2f direction, float dt);
	void draw(sf::RenderTarget& window);
	sf::Vector2f getPos() { return chSprite.getPosition(); }
	void setColour(const sf::Color& colour) { chSprite.setColor(colour); }

private:

	sf::Sprite chSprite;

	sf::Vector2f velocity;
	sf::Vector2f chPos;
	float speed;
};