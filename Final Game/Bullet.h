#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Player.h"

class Player;

class Bullet
{
public:
	Bullet(sf::Vector2f playerPosition, sf::Vector2f mousePosition, sf::RenderWindow* window, std::shared_ptr<Player> player);
	Bullet(sf::Vector2f playerPosition, sf::Vector2f mousePosition, sf::RenderWindow* window, float rotation, std::shared_ptr<Player> player);
	bool outOfWindowBounds();
	bool outOfWindowBoundsX();
	bool outOfWindowBoundsY();
	void updateBullet(float time);
	void updateEnemyBullet(float time);
	sf::FloatRect getBounds();


public:
	static int speedBullet;
	int bulletLife = 1;
	float playerPosX = playerPosition_.x, playerPosY = playerPosition_.y, mousePosX = mousePosition_.x, mousePosY = mousePosition_.y;
	sf::RectangleShape bullet;
	sf::Vector2f v;
	
private:
	std::shared_ptr<Player> player_;
	sf::Vector2f playerPosition_, mousePosition_, enemyPosition_;
	sf::RenderWindow* window_;
	
};

