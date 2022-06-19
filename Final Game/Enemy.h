#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"

class Bullet;
class Player;

class Enemy
{
public:
	Enemy(sf::RenderWindow* window, std::shared_ptr<Player> player, int& enemyCount, float fireRate, int XPos, int YPos);
	~Enemy();
	void updateEnemy(std::vector<Bullet>& bullets, float time);
	void lostHealth(int damage);
	void drawHealth();
	bool isDead();
	sf::FloatRect getBounds();
	static int killsTillLevelUp;

private:
	int speed = 175;
	sf::RectangleShape enemyBody, enemyHealth, enemyMissingHealth;
	sf::CircleShape enemyMarker;
	sf::RenderWindow* window_;
	std::shared_ptr<sf::Clock> clock = std::make_shared<sf::Clock>();
	sf::Vector2f v;
	float moveAngle;
	std::shared_ptr<Player> player_;
	int health = 100, maxHealth = 100;
	float angle, fireRate, healthBarSize;
	float timePassed = 0;
};

