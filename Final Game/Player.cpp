#include "Player.h"

Player::Player(sf::RenderWindow* window) {
	window_ = window;

	// Set up fonts
	font.loadFromFile("Fonts/pixel_font.ttf");
	killText.setFont(font);
	healthText.setFont(font);

	// Set up player square
	playerSquare_ = std::make_unique<sf::RectangleShape>();
	playerSquare_->setSize(sf::Vector2f(40, 40));
	playerSquare_->setOrigin(20, 20);
	playerSquare_->setPosition(window_->getSize().x / 2, window_->getSize().y / 2);
	playerSquare_->setFillColor(sf::Color::Cyan);
	playerSquare_->setOutlineColor(sf::Color::White);
	playerSquare_->setOutlineThickness(2.5);

	// Set up bottom UI
	bottomUI.setSize(sf::Vector2f(window_->getSize().x, window_->getSize().y / 8));
	bottomUI.setPosition(0, window_->getSize().y - window_->getSize().y / 8);
	bottomUI.setFillColor(sf::Color(80,80,80));
	bottomUI.setOutlineColor(sf::Color(120,120,120));
	bottomUI.setOutlineThickness(5);

	// Set up player health
	healthBox.setSize(sf::Vector2f(window_->getSize().x/4, window_->getSize().y/16));
	healthBox.setOrigin(0, healthBox.getSize().y/2);
	healthBox.setPosition(25, bottomUI.getPosition().y + bottomUI.getSize().y/2);
	healthBox.setFillColor(sf::Color(60, 60, 60));

	healthBar.setSize(sf::Vector2f(healthBox.getSize().x, healthBox.getSize().y / 2));
	healthBar.setPosition(healthBox.getPosition().x, healthBox.getPosition().y);
	healthBar.setFillColor(sf::Color(0, 200, 100));
	healthBarSize = healthBar.getSize().x;

	healthText.setString("Health: " + std::to_string(health) + "/" + std::to_string(maxHealth));
	healthText.setCharacterSize(16);
	healthText.setFillColor(sf::Color::White);
	healthText.setPosition(healthBox.getPosition().x + healthBox.getPosition().x/4, healthBox.getPosition().y + healthBox.getSize().y/16);

	missingHealth.setSize(sf::Vector2f(healthBox.getSize().x, healthBox.getSize().y / 2));
	missingHealth.setPosition(healthBox.getPosition().x, healthBox.getPosition().y);
	missingHealth.setFillColor(sf::Color(255, 90, 90));

	// Set up player collision detection (not hitbox)
	playerCircle_.setRadius(40/sqrt(2));
	playerCircle_.setOrigin(sf::Vector2f(40 / sqrt(2), 40 / sqrt(2)));
}

Player::~Player() {
	std::cout << "Deleted" << std::endl;
}

sf::Vector2f Player::getPosition() {
	return playerSquare_->getPosition();
}

sf::FloatRect Player::getBounds() {
	return playerSquare_->getGlobalBounds();
}

float Player::getRotation() {
	return playerSquare_->getRotation();
}



void Player::updateRotation() {
	sf::Vector2f mousePos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
	angle = atan2((mousePos.y - playerSquare_->getPosition().y), mousePos.x - playerSquare_->getPosition().x) * 180 / 3.1416;

	playerSquare_->setRotation(angle);
}

void Player::updatePlayer(float time) {

	healthText.setString("Health: " + std::to_string(health) + "/" + std::to_string(maxHealth));
	healthBar.setSize(sf::Vector2f(((health / static_cast<float>(maxHealth)) * healthBarSize), healthBox.getSize().y / 2));

	updateRotation();
	float speed = 250;
	sf::Vector2f v;	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) speed = 500;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) v.y -= speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) v.x -= speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) v.y += speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) v.x += speed;

	if (v.x != 0 && v.y != 0) v /= sqrt(2.f);

	if ((playerCircle_.getPosition().x - playerCircle_.getRadius() + v.x * time) < 0
		|| (playerCircle_.getPosition().x + playerCircle_.getRadius() + v.x * time) > window_->getSize().x) {
		v.x = 0;
	}

	if ((playerCircle_.getPosition().y - playerCircle_.getRadius() + v.y * time) < 0
		|| (playerCircle_.getPosition().y + playerCircle_.getRadius() + v.y * time) > bottomUI.getPosition().y) {
		v.y = 0;
	}

	playerSquare_->move(v * time);
	playerCircle_.setPosition(playerSquare_->getPosition());

	// Draw player
	window_->draw(*playerSquare_);

	// Draw bottom UI
	window_->draw(bottomUI);
	
	// Draw Health UI
	window_->draw(healthBox);
	window_->draw(missingHealth);
	window_->draw(healthBar);
	window_->draw(healthText);
	
	updateKills();

}

void Player::updateKills() {
	killText.setString("Kills: " + std::to_string(kills));
	killText.setPosition(healthBox.getPosition().x + healthBox.getPosition().x / 4, healthBox.getPosition().y - window_->getSize().y/36);
	killText.setCharacterSize(16);

	window_->draw(killText);
}