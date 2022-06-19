#include "Bullet.h"

Bullet::Bullet(sf::Vector2f enemyPosition, sf::Vector2f playerPosition, sf::RenderWindow* window, std::shared_ptr<Player> player) {

	window_ = window;

	player_ = player;
	enemyPosition_ = enemyPosition;
	playerPosition_ = playerPosition;

	float angle = atan2((playerPosition_.y - enemyPosition_.y), playerPosition_.x - enemyPosition_.x);

	bullet.setSize(sf::Vector2f(8, 8));
	bullet.setOrigin(sf::Vector2f(4, 4));
	bullet.setRotation(angle * 180 / 3.1416);
	bullet.setPosition(enemyPosition_.x + 18 * cos(angle), enemyPosition_.y + 18 * sin(angle));
	bullet.setFillColor(sf::Color::Magenta);
	
	v.x = speedBullet * cos(bullet.getRotation() * 3.1416 / 180);
	v.y = speedBullet * sin(bullet.getRotation() * 3.1416 / 180);
}

Bullet::Bullet(sf::Vector2f playerPosition, sf::Vector2f mousePosition, sf::RenderWindow* window, float rotation, std::shared_ptr<Player> player) {

	window_ = window;

	player_ = player;
	playerPosition_ = playerPosition;
	mousePosition_ = mousePosition;

	bullet.setSize(sf::Vector2f(8, 8));
	bullet.setOrigin(sf::Vector2f(4, 4));
	bullet.setRotation(rotation);
	bullet.setPosition(playerPosition_.x + 18 * cos(rotation * 3.1416 / 180), playerPosition_.y + 18 * sin(rotation * 3.1416 / 180));
	bullet.setFillColor(sf::Color::Cyan);

}


void Bullet::updateBullet(float time) {
	const float speed = 2000;

	bullet.move((speed*cos(bullet.getRotation() * 3.1416 / 180))*time, (speed*sin(bullet.getRotation() * 3.1416 / 180))*time);

	window_->draw(bullet);
}

void Bullet::updateEnemyBullet(float time) {

	v.x = speedBullet * cos(bullet.getRotation() * 3.1416 / 180);
	v.y = speedBullet * sin(bullet.getRotation() * 3.1416 / 180);

	bullet.move(v.x * time, v.y * time);

	window_->draw(bullet);
}

bool Bullet::outOfWindowBounds() {
	return (bullet.getPosition().x <= 0 || bullet.getPosition().y <= 0 || bullet.getPosition().x >= window_->getSize().x || bullet.getPosition().y >= window_->getSize().y - player_->bottomUI.getSize().y);
}

bool Bullet::outOfWindowBoundsX() {
	return (bullet.getPosition().x <= 0 || bullet.getPosition().x >= window_->getSize().x);
}
bool Bullet::outOfWindowBoundsY() {
	return (bullet.getPosition().y <= 0 || bullet.getPosition().y >= window_->getSize().y - player_->bottomUI.getSize().y);
}

sf::FloatRect Bullet::getBounds() {
	return bullet.getGlobalBounds();
}