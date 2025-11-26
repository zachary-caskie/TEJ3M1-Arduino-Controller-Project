#include "Helpers.h"

//Return normalized direction between two coordinates
sf::Vector2f getDir(sf::Vector2f a, sf::Vector2f b)
{
	sf::Vector2f dir = (b - a).normalized();

	return dir;
}

float getDist(sf::Vector2f a, sf::Vector2f b)
{
	sf::Vector2f dif = (b - a);
	float mag = sqrt((dif.x * dif.x) + (dif.y * dif.y));

	return mag;
}

sf::Vector2f rotaMtrx(sf::Vector2f dir, float angle)//rotate a given direction vector by an angle in radians
{
	float cosA = std::cos(angle);
	float sinA = std::sin(angle);

	return {
		dir.x * cosA - dir.y * sinA,
		dir.y * cosA + dir.x * sinA
	};
}
float mapValue(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}