#include "Bird.h"
#include "Helpers.h"
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>


Bird::Bird() {

	head.setRadius(60.f * factor);
	head.setFillColor(sf::Color::Black);
	head.setPosition({ 960,540 });
	head.setOrigin({ head.getRadius(), head.getRadius()});

	test1.setFillColor(sf::Color::Black);
	test1.setRadius(30.f * factor);
	test2.setFillColor(sf::Color::Black);
	test2.setRadius(30.f * factor);
	test3.setFillColor(sf::Color::Black);
	test3.setRadius(30.f * factor);
	test4.setFillColor(sf::Color::Black);
	test4.setRadius(30.f * factor);

	wingR1.setPointCount(4);
	wingR2.setPointCount(3);
	wingL1.setPointCount(4);
	wingL2.setPointCount(3);

	//Random bird characteristics
	//random generators
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0.5, 3);
	std::uniform_real_distribution<> dist2(0.7, 1.5);
	std::uniform_real_distribution<> posOrNeg(1, 2);
	//assign random values to floats
	ID = dist(gen);
	float rand2 = dist2(gen);
	//assign characteristics using randoms
	factor = 0.15 * rand2;
	speed = 600.f - 100 * rand2;
	float r = dist(rd);
	float g = dist(rd);
	float b = dist(rd);
	fthClr = sf::Color(18 * r, 22 * g, 32 * b);

	//position and direction calculation
	// 
	//random generators
	std::uniform_real_distribution<float> xTarg(200, 1720);
	std::uniform_real_distribution<float> yTarg(200, 880);
	std::uniform_real_distribution<float> xPos(200, 1720);
	std::uniform_real_distribution<float> yPos(200, 880);
	//spawn position
	std::uniform_int_distribution xy(1, 4);
	int side = xy(rd);
	sf::Vector2f targ;
	if (side <= 2)
	{
		spawnPt.x = xPos(rd);
		if (side == 1)
		{spawnPt.y = 0;
		targ = { xTarg(rd), 1080 };
		}
		else{spawnPt.y = 1080;
		targ = { xTarg(rd), 0 };
		}
	}
	else
	{
		spawnPt.y = yPos(rd);
		if (side == 3)
		{spawnPt.x = 0;
		targ = { 1920, yTarg(rd)};
		}
		else { spawnPt.y = 1920; 
		targ = { 0, yTarg(rd)};	
		}
	}
	head.setPosition(spawnPt);
	//target position
	baseDir = getDir(spawnPt, targ);

	destroy = false;

	if (posOrNeg(rd) == 1)
	{
		inversion = 1;
	}
	else {
		inversion = -1;
	}
}

bool Bird::isTargeted(sf::Vector2f crossHair) const
{
	if (getDist(head.getPosition(), crossHair) < 50.f)
	{
		return true;
	}
	else {
		return false;
	}
}

void Bird::update(float dt) {
	dir = baseDir.rotatedBy(sf::degrees(inversion * 38 * ID * sin(flapClock.getElapsedTime().asSeconds())));
	// Head tracking
	
	sf::Vector2f velocity = dir * speed * dt;
	head.move(velocity);
	sf::Vector2f headPos = head.getPosition();
	//Perpendicular to head direction
	sf::Vector2f perp(-dir.y, dir.x);
	//-------------Flap Logic-----------
	float flapOffset = 30.f * sin(flapClock.getElapsedTime().asSeconds() * 12);

	//set wing tips to offset from main point
	rTip = headPos + -dir * (30.f + flapOffset) * factor + perp * (330.f + flapOffset) * factor;
	lTip = headPos + -dir * (30.f + flapOffset) * factor + -perp * (330.f + flapOffset) * factor;
	//move test points to track wings
	test1.setPosition(rTip);
	test2.setPosition(lTip);

	//-------------Inverse Kinematic Logic------------
	float a = 160.f * factor;//first wing segment length
	float b = 220.f * factor;//second wing segment length 
	float cR = getDist(rTip, headPos);//distance from center to right wingtip
	float cL = getDist(lTip, headPos);//distance from center to left wingtip

	sf::Vector2f dR = getDir(headPos, rTip);//direction vector from center to right wingtip
	sf::Vector2f dL = getDir(headPos, lTip);//direction vector from center to left wingtip

	float denom = 2.f * a * cR;//denominator for law of cosines (2bc)
	float cosAngleA = (cR * cR + a * a - b * b) / denom;// return cos of angle to C joint
	cosAngleA = std::clamp(cosAngleA, -1.f, 1.f);//clamp angle to avoid impossible values
	float angleR = acos(cosAngleA);// angle A in radians
	//same math for for opposite wing
	denom = 2.f * a * cL;
	cosAngleA = (cL * cL + a * a - b * b) / denom;
	cosAngleA = std::clamp(cosAngleA, -1.f, 1.f);
	float angleL = acos(cosAngleA);
	//rotate direction to tip by angle A to get direction to joint
	dR = rotaMtrx(dR, -angleR);
	dL = rotaMtrx(dL, angleL);
	//set joint position using direction and distance
	sf::Vector2f jointR = headPos + dR * a;
	test3.setPosition(jointR);
	sf::Vector2f jointL = headPos + dL * a;
	test4.setPosition(jointL);

	points.clear();
	points =
	{
		rTip,
		jointR + dir * 30.f * factor,
		jointR + -dir * 30.f * factor,
		headPos + -dir * 60.f * factor,
		headPos + dir * 60.f * factor,
		jointL + dir * 30.f * factor,
		jointL + -dir * 30.f * factor,
		lTip,
	};

	for (int i = 0; i < 3; i++)
	{
		wingR2.setPoint(i, points[i]);
		wingL2.setPoint(i, points[points.size() - (i + 1)]);
	}
	for (int i = 0; i < 4; i++)
	{
		wingR1.setPoint(i, points[i + 1]);
		wingL1.setPoint(i, points[i + 3]);
	}

	mesh = 
	{
		&wingR1,
		&wingR2,
		&wingL1,
		&wingL2
	};

	for (auto* i : mesh)
	{
		i->setFillColor(fthClr);
	}

	//death case
	sf::Vector2f center(960, 540);
	if (getDist(center, headPos) > 1300.f)
	{
		destroy = true;
	}

}

void Bird::draw(sf::RenderTarget& window) {
	/*window.draw(head);
	window.draw(test1);
	window.draw(test2);
	window.draw(test3);
	window.draw(test4);*/
	for (auto* i : mesh)
	{
		window.draw(*i);
	}
}