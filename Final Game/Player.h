#pragma once
#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <cmath>

class Player
{
public:
	Player(sf::RenderWindow* window);
	~Player();

	sf::Vector2f getPosition();
	float getRotation();
	sf::FloatRect getBounds();
	
	void updatePlayer(float time);
	void updateKills();


private:
	void updateRotation();

public:
	sf::RectangleShape bottomUI;
	int kills = 0;
	int health = 250, maxHealth = 250;

private:
	std::unique_ptr<sf::RectangleShape> playerSquare_;
	sf::RenderWindow* window_;
	sf::CircleShape playerCircle_;
	sf::RectangleShape healthBox, healthBar, missingHealth;
	sf::Text healthText, killText;
	sf::Font font;
	float angle, healthBarSize;
};