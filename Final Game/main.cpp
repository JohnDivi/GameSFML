#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <time.h>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"


void updateCursor(sf::RenderWindow& window, sf::Sprite cursor);
void updateBullets(sf::RenderWindow& window, std::vector<Bullet>& bullets, float time, std::vector<std::shared_ptr<Enemy>>& enemies, std::shared_ptr<Player> player, int& enemyCount);
void updateEnemyBullets(sf::RenderWindow& window, std::vector<Bullet>& enemyBullets, std::shared_ptr<Player> player, float time);
void checkGameOver(std::shared_ptr<Player> player, sf::RenderWindow* window, sf::RectangleShape* darkenScreen, sf::Text* gameOver, sf::Text* gameOverDescription, int& restartGame);
void restart(sf::RenderWindow* window, int& enemyCount, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Bullet>& bullets, std::vector<Bullet>& enemyBullets, sf::Clock* clock);
int Enemy::killsTillLevelUp = 5;
int Bullet::speedBullet = 300;

int main()
{

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Game", sf::Style::Titlebar | sf::Style::Close);
    sf::Event event;
    window.setMouseCursorVisible(false);
    std::shared_ptr<Player> player = std::make_shared<Player>(&window);

    sf::RectangleShape spawnArea;
    spawnArea.setSize(sf::Vector2f(window.getSize().x / 1.5, window.getSize().y / 1.5));
    spawnArea.setOrigin(spawnArea.getSize() / 2.f);
    spawnArea.setPosition(sf::Vector2f(window.getSize().x / 2, ((window.getSize().y) - player->bottomUI.getSize().y) / 2.f));
    
    // Set up pause window
    sf::RectangleShape darkenScreen;
    sf::Text pauseText, pauseTextDescription;
    sf::Font font;
    font.loadFromFile("Fonts/pixel_font.ttf");
    darkenScreen.setSize(sf::Vector2f(window.getSize()));
    darkenScreen.setFillColor(sf::Color(0,0,0,175));

    pauseText.setFont(font);
    pauseTextDescription.setFont(font);

    pauseText.setString("PAUSED");
    pauseTextDescription.setString("Press the \"R\" key to resume.");

    pauseText.setCharacterSize(48);
    pauseTextDescription.setCharacterSize(24);

    pauseText.setOrigin(pauseText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
    pauseText.setPosition(window.getSize().x / 2, window.getSize().y / 3);

    pauseTextDescription.setOrigin(pauseTextDescription.getLocalBounds().width / 2, pauseTextDescription.getLocalBounds().height / 2);
    pauseTextDescription.setPosition(pauseText.getPosition().x, 5 * pauseText.getPosition().y / 4);

    // Set up game over
    sf::Text gameOver, gameOverDescription;
    gameOver.setFont(font);
    gameOverDescription.setFont(font);
    gameOver.setString("GAME OVER");
    gameOverDescription.setString("Press \"Spacebar\" to quit or press \"U\" to restart");
    gameOver.setCharacterSize(64);
    gameOverDescription.setCharacterSize(32);
    gameOver.setOrigin(gameOver.getLocalBounds().width / 2, gameOver.getLocalBounds().height / 2);
    gameOverDescription.setOrigin(gameOverDescription.getLocalBounds().width / 2, gameOverDescription.getLocalBounds().height / 2);
    gameOver.setPosition(window.getSize().x / 2, window.getSize().y / 3);
    gameOverDescription.setPosition(gameOver.getPosition().x, gameOver.getPosition().y + gameOver.getPosition().y / 2.5);
    int restartGame = 0;

    // Enemy vector
    std::vector<std::shared_ptr<Enemy>> enemies;
    
    // Initialize bullets vector capacity
    std::vector<Bullet> bullets, enemyBullets;
    int enemyCount = 0;

    // Set up mouse cursor
    sf::Texture cursorTexture;
    cursorTexture.loadFromFile("Textures/cursor.png");
    sf::Sprite cursor(cursorTexture);
    cursor.setScale(sf::Vector2f(0.75,0.75));
    cursor.setOrigin(cursorTexture.getSize().x / 2, cursorTexture.getSize().y / 2);

    // Set up timers
    sf::Clock clock;
    float timeSinceLastFrame = 0, maxFireRate = 0.25, fireRate = maxFireRate, enemyTimer = 0;
    srand(time(0));

    while (window.isOpen())
    {
        // Restart game
        if (restartGame == 1) {
            restart(&window, enemyCount, enemies, bullets, enemyBullets, &clock);
            player = std::make_shared<Player>(&window);
            restartGame = 0;
            Bullet::speedBullet = 300;
            srand(time(0));
        }

        // Timers
        fireRate += clock.getElapsedTime().asSeconds();
        enemyTimer += clock.getElapsedTime().asSeconds();
        timeSinceLastFrame = clock.restart().asSeconds();

        // Spawn enemy every 1 <= seconds <= 3 randomly
        
        int r = rand() % 3 + 1;

        if (enemyTimer >= r) {

            srand(time(0));
            int xSide = rand() % 2 + 1, ySide = rand() % 2 + 1;
            int XPos = (rand() % window.getSize().x) * 1.5, YPos = (rand() % window.getSize().y) * 1.5;

            while (XPos >= 0
                && XPos <= window.getSize().x
                && YPos >= 0
                && YPos <= window.getSize().y - player->bottomUI.getSize().y) {
                
                XPos = (rand() % window.getSize().x) * 1.5;
                YPos = (rand() % window.getSize().y) * 1.5;
            }
            
            if (xSide == 1) {
                XPos *= -1;
            }

            if (ySide == 1) {
                YPos *= -1;
            }

            std::cout << "Enemy spawned at position (x, y): (" << XPos << ", " << YPos << ")" << std::endl;

            enemies.push_back(std::make_shared<Enemy>(&window, player, enemyCount, 1.25, XPos, YPos));
            enemyTimer = 0;
        }

        // Window event polling
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) window.close();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseWheelMoved) {
                if (event.mouseWheel.delta == 1 && maxFireRate + 0.05 <= 2) maxFireRate += 0.05;
                if (event.mouseWheel.delta == -1 && maxFireRate - 0.05 >= -1) maxFireRate -= 0.05;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::U) {
                    restartGame = 1;
                }

                if (event.key.code == sf::Keyboard::P) {
                    window.draw(darkenScreen);
                    window.draw(pauseText);
                    window.draw(pauseTextDescription);
                    window.setMouseCursorVisible(true);
                    window.display();
                    while (event.key.code != sf::Keyboard::R) {
                        sf::sleep(sf::milliseconds(10));
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                            window.close();
                            break;
                        }
                        window.pollEvent(event);
                        clock.restart().asSeconds();
                    }
                    window.setMouseCursorVisible(false);
                }
            }
        }

        // Clear window
        window.clear();

        // Update
        if (fireRate >= maxFireRate) {
            fireRate = maxFireRate;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                bullets.push_back(Bullet(player->getPosition(), window.mapPixelToCoords(sf::Mouse::getPosition(window)), &window, player->getRotation(), player));
                fireRate = 0;
            }
        }

        updateBullets(window, bullets, timeSinceLastFrame, enemies, player, enemyCount);
        updateEnemyBullets(window, enemyBullets, player, timeSinceLastFrame);
        for (std::shared_ptr<Enemy> enemy : enemies) {
            enemy->updateEnemy(enemyBullets, timeSinceLastFrame);
        }
        
        player->updatePlayer(timeSinceLastFrame);

        // Check if game is over
        checkGameOver(player, &window, &darkenScreen, &gameOver, &gameOverDescription, restartGame);

        

        // Render
        updateCursor(window, cursor);
        window.display();

    }

    return 0;
}

void updateCursor(sf::RenderWindow& window, sf::Sprite cursor) {
    cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
    window.draw(cursor);
}

void updateBullets(sf::RenderWindow& window, std::vector<Bullet>& bullets, float time, std::vector<std::shared_ptr<Enemy>>& enemies, std::shared_ptr<Player> player, int& enemyCount) {
    
    bool bulletDestroyed;

    if (!enemies.empty()) {

        for (int i = 0; i < bullets.size(); i++) {

            bulletDestroyed = false;

            for (int j = 0; j < enemies.size(); j++) {
                if (bullets[i].getBounds().intersects(enemies[j]->getBounds())) {
                    bulletDestroyed = true;
                    bullets.erase(bullets.begin() + i--);
                    enemies[j]->lostHealth(25);

                    if (enemies[j]->isDead()) {
                        enemyCount--;
                        enemies.erase(enemies.begin() + j--);
                        player->kills++;
                    }
                    break;
                }
            }
            if (bulletDestroyed) {
                continue;
            }

            if (bullets[i].outOfWindowBounds()) {
                bullets.erase(bullets.begin() + i--);
            }
            else {
                bullets[i].updateBullet(time);

            }
        }
    }
    else {
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].outOfWindowBounds()) {
                bullets.erase(bullets.begin() + i--);
            }
            else {
                bullets[i].updateBullet(time);
            }
        }
    }
}

void updateEnemyBullets(sf::RenderWindow& window, std::vector<Bullet>& enemyBullets, std::shared_ptr<Player> player, float time) {
    for (int i = 0; i < enemyBullets.size(); i++) {

        if (player->getBounds().intersects(enemyBullets[i].getBounds())) {
            enemyBullets.erase(enemyBullets.begin() + i--);
            player->health -= 10;
            continue;
        }

        if (enemyBullets[i].outOfWindowBounds()) {
            if (enemyBullets[i].bulletLife <= 0) {
                enemyBullets.erase(enemyBullets.begin() + i--);
            }
            else {
                enemyBullets[i].bulletLife--;
                enemyBullets[i].updateEnemyBullet(time);
            }
        }
        else {
            enemyBullets[i].updateEnemyBullet(time);
        }
    }
}

void checkGameOver(std::shared_ptr<Player> player, sf::RenderWindow* window, sf::RectangleShape* darkenScreen, sf::Text* gameOver, sf::Text* gameOverDescription, int& restartGame) {
    if (player->health <= 0) {
        sf::Event event;
        window->setMouseCursorVisible(true);
        window->draw(*darkenScreen);
        window->draw(*gameOver);
        window->draw(*gameOverDescription);
        window->display();
        while (window->isOpen()) {
            while (window->pollEvent(event)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) window->close();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
                    restartGame = 1;
                    window->setMouseCursorVisible(false);
                    return;
                }
            }
            
        }
    }
}

void restart(sf::RenderWindow* window, int& enemyCount, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Bullet>& bullets, std::vector<Bullet>& enemyBullets, sf::Clock* clock) {
    
    enemyCount = 0;
    enemies.clear();
    bullets.clear();
    enemyBullets.clear();
    clock->restart();
}