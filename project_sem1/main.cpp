#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "subfunc.hpp"
#include "bullets.hpp"
#include "enemies.hpp"

int MAX_HP = 50;
int PLAYER_HP = MAX_HP;

void checkCollision(sf::Vector2f& player) {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active) {
            
            sf::CircleShape bulletShape(5.f);
            bulletShape.setOrigin({5.f, 5.f});
            bulletShape.setPosition(bullets[i].pos);
            sf::FloatRect bulletBox = bulletShape.getGlobalBounds();
            
            if(bullets[i].owner == 'p') {
                for(int j = 0; j < MAX_ENEMIES; j++) {
                    if(enemies[j].active) {
                        
                        sf::CircleShape enemyShape(15.f);
                        enemyShape.setOrigin({15.f, 15.f});
                        enemyShape.setPosition(enemies[j].pos);
                        sf::FloatRect enemyBox = enemyShape.getGlobalBounds();
                        
                        if(bulletBox.findIntersection(enemyBox)) {
                            bullets[i].active = false;
                            enemies[j].hp--;
                            break;
                        }
                    }
                }
            }
            
            if(bullets[i].owner == 'e') {
                
                sf::CircleShape playerShape(20.f);
                playerShape.setOrigin({20.f, 20.f});
                playerShape.setPosition(player);
                sf::FloatRect playerBox = playerShape.getGlobalBounds();
                
                if(bulletBox.findIntersection(playerBox)) {
                    bullets[i].active = false;
                    PLAYER_HP--;
                }
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({3024, 1964}), "GAME NAME", sf::Style::Default);
    window.setFramerateLimit(60);
    
    sf::RectangleShape hpBarBackground({200.f, 20.f});
    hpBarBackground.setFillColor(sf::Color(50, 50, 50));
    hpBarBackground.setPosition({20.f, 20.f});
    
    sf::RectangleShape hpBar({200.f, 20.f});
    hpBar.setFillColor(sf::Color::Green);
    hpBar.setPosition({20.f, 20.f});

    sf::Vector2f playerPos{800.f, 450.f};
    sf::CircleShape player(20.f);
    player.setOrigin({20.f, 20.f});
    player.setFillColor(sf::Color::Cyan);

    float moveSpeed = 250.f;
    sf::Vector2f fireDir{1.f, 0.f};
    float aimSmooth = 6.f;
    float fireCooldown = 0.1f;
    float fireDelay = 0.f;
    float enemySpawnCooldown = 2.f;
    float enemySpawnDelay = 0.f;
    
    srand(unsigned(time(0)));

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* resized = event->getIf<sf::Event::Resized>())
                window.setSize({resized->size.x, resized->size.y});
        }
        
        

        float dt = clock.restart().asSeconds();
        fireDelay -= dt;
        enemySpawnDelay -= dt;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            if(playerPos.y - moveSpeed * dt > 20.f)
                playerPos.y -= moveSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            if(playerPos.y + moveSpeed * dt < window.getSize().y - 20.f)
                playerPos.y += moveSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            if(playerPos.x - moveSpeed * dt > 20.f)
                playerPos.x -= moveSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            if(playerPos.x + moveSpeed * dt < window.getSize().x - 20.f)
                playerPos.x += moveSpeed * dt;

        sf::Vector2f inputDir{0.f, 0.f};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            inputDir.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            inputDir.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            inputDir.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            inputDir.x += 1;

        if (inputDir.x != 0.f || inputDir.y != 0.f) {
            inputDir = normalize(inputDir);
            fireDir = normalize( lerp(fireDir, inputDir, aimSmooth * dt) );
        }

        float angle = std::atan2(fireDir.y, fireDir.x) * 180.f / 3.1415926f;
        player.setRotation(sf::degrees(angle));
        
        if(/*sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && */fireDelay <= 0) {
            spawnBullet(playerPos, fireDir, 'p');
            fireDelay = fireCooldown;
        }
        
        if(enemySpawnDelay <= 0) {
            spawnEnemy(window.getSize());
            enemySpawnDelay = enemySpawnCooldown;
        }
        
        updateBullet(dt, window.getSize());
        
        updateEnemy(dt, window.getSize(), playerPos);
        
        player.setPosition(playerPos);
        
        checkCollision(playerPos);
        
        float hp_percentage = (float)PLAYER_HP / MAX_HP;
        sf::RectangleShape hpBar({200.f * hp_percentage, 20.f});
        hpBar.setPosition({20.f, 20.f});
        if(hp_percentage >= 0.7)
            hpBar.setFillColor(sf::Color::Green);
        else if(hp_percentage >= 0.25)
            hpBar.setFillColor(sf::Color(255, 165, 0));
        else
            hpBar.setFillColor(sf::Color::Red);
        
        std::cout << PLAYER_HP << ' '<< hp_percentage << std::endl;

        window.clear();
        window.draw(hpBarBackground);
        window.draw(hpBar);
        window.draw(player);
        drawBullets(window);
        drawEnemies(dt, window);
        window.display();
    }

    return 0;
}
