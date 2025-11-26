#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Bird {
public:
	Bird();
	void update(float dt);
	void draw(sf::RenderTarget& window);
	bool isDead() const { return destroy; }
	void kill() { destroy = true; }
	bool isTargeted(sf::Vector2f crossHair) const;

private:

	float speed;
	float factor;
	float ID;
	float angleRand;

	sf::Vector2f spawnPt;
	sf::Vector2f baseDir;
	sf::Vector2f dir;

	sf::Color fthClr;
	sf::Color bkClr;

	sf::CircleShape head;

	sf::CircleShape test1;
	sf::CircleShape test2;
	sf::CircleShape test3;
	sf::CircleShape test4;
	sf::CircleShape test5;

	std::vector<sf::Vector2f> points;
	std::vector<sf::ConvexShape*> mesh;

	sf::ConvexShape wingR1;
	sf::ConvexShape wingR2;
	sf::ConvexShape wingL1;
	sf::ConvexShape wingL2;

	sf::Clock flapClock;

	sf::Vector2f rTip;
	sf::Vector2f lTip;

	bool destroy;
	bool targeted;
	int inversion;
};