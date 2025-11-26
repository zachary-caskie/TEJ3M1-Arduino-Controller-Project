#include "Crosshair.h"

Crosshair::Crosshair(const sf::Texture& tex) : chSprite(tex)
{
	speed = 800;

	chSprite.setScale({ 0.4, 0.4 });
	float mid = chSprite.getLocalBounds().size.x / 2;
	chSprite.setOrigin({mid, mid});
	chSprite.setPosition({ 400,400 });
}

void Crosshair::update(sf::Vector2f direction, float dt)
{
	sf::Vector2f chPos = chSprite.getPosition();
	sf::Vector2f velocity = direction * speed * dt;
	chSprite.setPosition(chPos + velocity);
}

void Crosshair::draw(sf::RenderTarget& window)
{
	window.draw(chSprite);
}