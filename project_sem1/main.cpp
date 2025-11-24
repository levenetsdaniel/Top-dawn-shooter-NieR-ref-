#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "headers/structs.hpp"
#include "headers/subfunc.hpp"
#include "headers/particles.hpp"
#include "headers/bullets.hpp"
#include "headers/laser.hpp"
#include "headers/enemies.hpp"

const float MAX_HP = 10.f;
float PLAYER_HP = MAX_HP;
char GAME_STATE = 'r';
DeathFlash deathFlash;
sf::Text MESSAGE(font);

void checkCollision(sf::Vector2f& player) {
    sf::CircleShape playerShape(playerRadius);
    playerShape.setOrigin({playerRadius, playerRadius});
    playerShape.setPosition(player);
    sf::FloatRect playerBox = playerShape.getGlobalBounds();
    
    sf::RectangleShape laserShape;
    laserShape.setPosition({laser.area.position.x, laser.area.position.y});
    laserShape.setSize({laser.area.size.x, laser.area.size.y});
    sf::FloatRect laserBox = laserShape.getGlobalBounds();
    
    if(laser.damaging && laserBox.findIntersection(playerBox))
        PLAYER_HP -= 0.5;
    
    for(int i = 0; i < MAX_ENEMIES; i++) {
        sf::CircleShape enemyShape(15.f);
        enemyShape.setOrigin({15.f, 15.f});
        enemyShape.setPosition(enemies[i].pos);
        sf::FloatRect enemyBox = enemyShape.getGlobalBounds();
        
        if(laser.damaging && laserBox.findIntersection(enemyBox))
            enemies[i].hp = 0;
    }
    
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active) {
            
            sf::CircleShape bulletShape(5.f);
            bulletShape.setOrigin({5.f, 5.f});
            bulletShape.setPosition(bullets[i].pos);
            sf::FloatRect bulletBox = bulletShape.getGlobalBounds();
            
            if(bullets[i].owner == 'p') {
                for(int j = 0; j < MAX_ENEMIES; j++) {
                    if(enemies[j].active) {
                        
                        sf::CircleShape enemyShape(enemyRadius);
                        enemyShape.setOrigin({enemyRadius, enemyRadius});
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
    
    sf::Sprite background {bgTex};
    background.setOrigin({(float)window.getSize().x, (float)window.getSize().y});
    background.setScale({(float)bgTex.getSize().x / window.getSize().x, (float)bgTex.getSize().y / window.getSize().y});
    
    sf::RectangleShape afterGameBg({(float)window.getSize().x, (float)window.getSize().y});
    afterGameBg.setFillColor(sf::Color(0, 0, 0, 150));
    afterGameBg.setPosition({0.f, 0.f});
    
    
    sf::RectangleShape hpBarBackground({200.f, 20.f});
    hpBarBackground.setFillColor(sf::Color(50, 50, 50));
    hpBarBackground.setPosition({20.f, 20.f});
    
    sf::RectangleShape hpBar({200.f, 20.f});
    hpBar.setFillColor(sf::Color::Green);
    hpBar.setPosition({20.f, 20.f});

    sf::Sprite player{playerTex};
    player.setOrigin({playerTex.getSize().x / 2.f, playerTex.getSize().y / 2.f});
    sf::Vector2f playerPos{window.getSize().x / 2.f, window.getSize().y / 2.f};
    player.setScale({0.1, 0.1});
    
    MESSAGE.setCharacterSize(80);
    MESSAGE.setFillColor(sf::Color::White);
    
    sf::Text restartText(font);
    restartText.setCharacterSize(20);
    restartText.setFillColor(sf::Color::White);
    restartText.setString("Press R to restart");
    centerText(restartText, window);
    restartText.move({0, 60.f});
    
    sf::Text quitText(font);
    quitText.setCharacterSize(20);
    quitText.setFillColor(sf::Color::White);
    quitText.setString("Press Q to quit");
    centerText(quitText, window);
    quitText.move({0, 80.f});
    
    sf::Text killCount(font);
    killCount.setCharacterSize(40);
    killCount.setFillColor(sf::Color::White);
    killCount.setPosition({20, 40});

    float moveSpeed = 250.f;
    sf::Vector2f fireDir{0.f, -1.f};
    float aimSmooth = 3.f;
    float fireCooldown = 0.1f;
    float fireDelay = 0.f;
    float enemySpawnCooldown = 4.f;
    float enemySpawnDelay = 0.f;
    float laserCooldawn = 15.f;
    float laserDelay = laserCooldawn;
    
    srand(unsigned(time(0)));

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        float dt = clock.restart().asSeconds();
        
        if(GAME_STATE == 'r') {
            
            fireDelay -= dt;
            enemySpawnDelay -= dt;
            laserDelay -= dt;
            
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
                fireDir = normalize(lerp(fireDir, inputDir, aimSmooth * dt));
            }
            
            float angle = (std::atan2(fireDir.y, fireDir.x)) * 180.f/ 3.1415926f;
            player.setRotation(sf::degrees(angle) + sf::degrees(90.f));
            
            if(fireDelay <= 0) {
                spawnBullet(playerPos, fireDir,  20.f, 'p');
                fireDelay = fireCooldown;
            }
            
            if(enemySpawnDelay <= 0) {
                spawnEnemy(window.getSize());
                enemySpawnDelay = enemySpawnCooldown;
            }
            
            if(laserDelay <= 0) {
                triggerLaser(window.getSize(), playerPos);
                laserDelay = laserCooldawn;
            }
            
            player.setPosition(playerPos);
            
            updateBullet(dt, window.getSize());
            
            updateEnemy(dt, window.getSize(), playerPos);
            
            updateLaser(&laser, dt);
            
            updateParticles(dt);
            
            checkCollision(playerPos);
            
            killCount.setString("Kills: " + std::to_string(KILL_COUNT));
            
            float hp_percentage = PLAYER_HP / MAX_HP;
            hpBar.setSize({200.f * hp_percentage, 20.f});
            hpBar.setPosition({20.f, 20.f});
            if(hp_percentage >= 0.7)
                hpBar.setFillColor(sf::Color::Green);
            else if(hp_percentage >= 0.25)
                hpBar.setFillColor(sf::Color(255, 165, 0));
            else
                hpBar.setFillColor(sf::Color::Red);
        }
            
        if(GAME_STATE == 'd') {
            if(deathFlash.active) {
                deathFlash.t -= dt * 1.8;
                
                if(deathFlash.t <= 0) {
                    deathFlash.t = 0.f;
                    deathFlash.active = false;
                }
                else {
                    deathFlash.flash.setSize({(float)window.getSize().x, (float)window.getSize().y});
                    float intensity = 255.f * deathFlash.t;
                    deathFlash.flash.setFillColor(sf::Color(255, 0, 0, intensity));
                }
                
            }
        }
        
        if(PLAYER_HP == 0 && GAME_STATE != 'd') {
            GAME_STATE = 'd';
            deathFlash.active = true;
            deathFlash.t = 1.f;
            MESSAGE.setString("GAME OVER");
            centerText(MESSAGE, window);
        }
        if(GAME_STATE == 'd' && !deathFlash.active && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
            GAME_STATE = 'm';
        
        if(KILL_COUNT == 2 && GAME_STATE != 'w') {
            GAME_STATE = 'w';
            MESSAGE.setString("YOU WIN");
            centerText(MESSAGE, window);
        }
        
        if(GAME_STATE == 'd' || GAME_STATE == 'w')
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                window.close();
            
        window.clear();
        window.draw(background);
        window.draw(hpBarBackground);
        window.draw(hpBar);
        window.draw(killCount);
        window.draw(player);
        drawBullets(window);
        drawEnemies(window);
        drawLaser(laser, window);
        drawParticle(window);
        if(deathFlash.active)
            window.draw(deathFlash.flash);
        if(GAME_STATE != 'r') {
            window.draw(afterGameBg);
            window.draw(MESSAGE);
        }
        if((GAME_STATE == 'd' && !deathFlash.active) || GAME_STATE == 'w') {
            window.draw(restartText);
            window.draw(quitText);
        }
        window.display();
    }

    return 0;
}
