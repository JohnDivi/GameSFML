#include "Enemy.h"

Enemy::Enemy(sf::RenderWindow* window, std::shared_ptr<Player> player, int& enemyCount, float fireRate, int XPos, int YPos) {

	player_ = player;

	this->fireRate = fireRate;

	enemyCount++;
	
	enemyMarker = sf::CircleShape(10, 3);

	enemyMissingHealth.setSize(sf::Vector2f(30, 5));
	enemyMissingHealth.setOrigin(enemyMissingHealth.getLocalBounds().width / 2, enemyMissingHealth.getLocalBounds().height / 2);
	enemyMissingHealth.setFillColor(sf::Color::Red);
	healthBarSize = enemyMissingHealth.getSize().x;

	enemyHealth.setSize(sf::Vector2f(health / static_cast<float>(maxHealth) * healthBarSize, 5));
	enemyHealth.setOrigin(enemyHealth.getLocalBounds().width / 2, enemyHealth.getLocalBounds().height / 2);
	enemyHealth.setFillColor(sf::Color::Green);

	enemyBody.setSize(sf::Vector2f(35, 35));
	enemyBody.setOrigin(17.5, 17.5);
	enemyBody.setPosition(XPos, YPos);
	enemyBody.setOutlineColor(sf::Color::Red);
	enemyBody.setOutlineThickness(1);

	moveAngle = atan2((player_->getPosition().y) - enemyBody.getPosition().y, player_->getPosition().x - enemyBody.getPosition().x);
	v.x = 0;
	v.y = 0;

	window_ = window;
}

void Enemy::updateEnemy(std::vector<Bullet>& bullets, float time) {

	if (v.x == 0 || v.y == 0) {
		v.x = speed * -cos(moveAngle);
		v.y = speed * -sin(moveAngle);
	}

	if (timePassed >= fireRate
		&& enemyBody.getPosition().x > 0
		&& enemyBody.getPosition().x < window_->getSize().x
		&& enemyBody.getPosition().y > 0
		&& enemyBody.getPosition().y < window_->getSize().y - player_->bottomUI.getSize().y) {
		timePassed = 0;
		bullets.push_back(Bullet(enemyBody.getPosition(), player_->getPosition(), window_, player_));
	}
	else {
		timePassed += clock->restart().asSeconds();
	}

	angle = atan2((player_->getPosition().y) - enemyBody.getPosition().y, player_->getPosition().x - enemyBody.getPosition().x) * 180 / 3.1416;
	enemyBody.setRotation(angle);

	if (enemyBody.getPosition().x <= 0
		|| enemyBody.getPosition().x >= window_->getSize().x) {

		moveAngle = atan2((player_->getPosition().y) - enemyBody.getPosition().y, player_->getPosition().x - enemyBody.getPosition().x);
		v.x = (speed * cos(moveAngle));
		v.y = speed * sin(moveAngle);

	}
	if (enemyBody.getPosition().y <= 0
		|| enemyBody.getPosition().y >= window_->getSize().y - player_->bottomUI.getSize().y) {

		moveAngle = atan2((player_->getPosition().y) - enemyBody.getPosition().y, player_->getPosition().x - enemyBody.getPosition().x);
		v.x = speed * cos(moveAngle);
		v.y = (speed * sin(moveAngle));
	}

	// Enemy marker
	if (enemyBody.getPosition().x <= 0) {

		float green = 255 - (255 * (enemyBody.getPosition().x / (window_->getSize().x * 1.5))), red = 255;

		enemyMarker.setOrigin(enemyMarker.getLocalBounds().width / 2, 0);
		enemyMarker.setRotation(270);
		enemyMarker.setPosition(0, enemyBody.getPosition().y);
		enemyMarker.setFillColor(sf::Color(red, green, 0));
	}
	else if (enemyBody.getPosition().x >= window_->getSize().x) {

		float green = 255 - (255 * ((window_->getSize().x - enemyBody.getPosition().x) / (window_->getSize().x * 1.5))), red = 255;

		enemyMarker.setOrigin(enemyMarker.getLocalBounds().width / 2, 0);
		enemyMarker.setRotation(90);
		enemyMarker.setPosition(window_->getSize().x, enemyBody.getPosition().y);
		enemyMarker.setFillColor(sf::Color(red, green, 0));
	}
	else if (enemyBody.getPosition().y <= 0) {
		float green = 255 - (255 * (enemyBody.getPosition().y / (window_->getSize().y * 1.5))), red = 255;

		enemyMarker.setOrigin(enemyMarker.getLocalBounds().width / 2, 0);
		enemyMarker.setRotation(0);
		enemyMarker.setPosition(enemyBody.getPosition().x, 0);
		enemyMarker.setFillColor(sf::Color(red, green, 0));

	}
	else if (enemyBody.getPosition().y >= window_->getSize().y - player_->bottomUI.getSize().y) {

		float green = 255 - (255 * (((window_->getSize().y - player_->bottomUI.getSize().y) - enemyBody.getPosition().y) / (window_->getSize().y * 1.5))), red = 255;

		enemyMarker.setOrigin(enemyMarker.getLocalBounds().width / 2, enemyMarker.getLocalBounds().height / 2 * -1);
		enemyMarker.setRotation(180);
		enemyMarker.setPosition(enemyBody.getPosition().x, window_->getSize().y - player_->bottomUI.getSize().y);
		enemyMarker.setFillColor(sf::Color(red, green, 0));
	}
	else {
		enemyMarker.setFillColor(sf::Color::Transparent);
	}

	enemyBody.move(v.x * time, v.y * time);


	drawHealth();
	window_->draw(enemyMarker);
	window_->draw(enemyBody);

}

void Enemy::drawHealth() {
	enemyMissingHealth.setPosition(enemyBody.getPosition().x, enemyBody.getPosition().y + 30);
	enemyHealth.setPosition(enemyBody.getPosition().x, enemyBody.getPosition().y + 30);
	enemyHealth.setSize(sf::Vector2f(health / static_cast<float>(maxHealth) * healthBarSize, 5));

	window_->draw(enemyMissingHealth);
	window_->draw(enemyHealth);
}

void Enemy::lostHealth(int damage) {
	if (health > 0) {
		health -= damage;
	}
}

bool Enemy::isDead() {
	if (health <= 0) {
		return true;
	}
	return false;
}

sf::FloatRect Enemy::getBounds() {
	return enemyBody.getGlobalBounds();
}

Enemy::~Enemy() {

	killsTillLevelUp--;

	if (killsTillLevelUp == 0) {
		Bullet::speedBullet += 50;
		killsTillLevelUp = 5;
	}
	std::cout << "\nEnemy leveling up in: " << killsTillLevelUp << " more kill(s).\nCurrent enemy bullet speed: " << Bullet::speedBullet << "u/s.\n" <<  std::endl;
}