#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cctype>
#include <iostream>
#include "headers/structs.hpp"
#include "headers/subfunc.hpp"
#include "headers/particles.hpp"
#include "headers/bullets.hpp"
#include "headers/laser.hpp"
#include "headers/enemies.hpp"

char GAME_STATE = 'm';
float MAX_HP;
float PLAYER_HP;
int KILL_TARGET;

void reset() {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active)
            bullets[i].active = false;
    }
    
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(enemies[i].active)
            enemies[i].active = false;
    }
    
    for(int i = 0; i < MAX_PARTICLES; i++) {
        if(particles[i].active)
            particles[i].active = false;
    }
    
    if(laser.active)
        laser.active = false;
}

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
        sf::CircleShape enemyShape(enemyRadius);
        enemyShape.setOrigin({enemyRadius, enemyRadius});
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
    
    sf::Text MESSAGE(font);
    MESSAGE.setCharacterSize(80);
    MESSAGE.setFillColor(sf::Color::White);
    centerText(MESSAGE, window);
    
    sf::Text menuTitle(font);
    menuTitle.setCharacterSize(60);
    menuTitle.setFillColor(sf::Color::White);
    menuTitle.setString("Choose kill target");
    centerText(menuTitle, window);
    menuTitle.move({0, -150});
    
    sf::Text option3(font);
    option3.setString("3 enemies");
    option3.setCharacterSize(50);
    option3.setFillColor(sf::Color::White);
    centerText(option3, window);
    option3.move({0, -40.f});
    
    sf::Text option15(font);
    option15.setString("15 enemies");
    option15.setCharacterSize(50);
    option15.setFillColor(sf::Color::White);
    centerText(option15, window);
    
    sf::Text option50(font);
    option50.setString("50 enemies");
    option50.setCharacterSize(50);
    option50.setFillColor(sf::Color::White);
    centerText(option50, window);
    option50.move({0, 40.f});
    
    sf::Color normalColor = sf::Color::White;
    sf::Color hoverColor  = sf::Color(255, 220, 0);
    
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
    quitText.move({0, 100.f});
    
    sf::Text score(font);
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::White);
    
    sf::Text resumeText(font);
    resumeText.setCharacterSize(20);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setString("Press N to resume");
    centerText(resumeText, window);
    resumeText.move({0, 140.f});
    
    sf::Text killTarget(font);
    killTarget.setCharacterSize(40);
    killTarget.setFillColor(sf::Color::Yellow);
    killTarget.setPosition({20, 90});
    
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
    float enemySpawnCoef = 0.01;
    float laserCooldown = 15.f;
    float laserDelay = laserCooldown;
    DeathFlash deathFlash;
    
    srand(unsigned(time(0)));

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        
            if(GAME_STATE == 'm') {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                    window.close();
                
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if (option3.getGlobalBounds().contains(mouse)) {
                        MAX_HP = 10.f;
                        PLAYER_HP = MAX_HP;
                        KILL_TARGET = 3;
                        enemySpawnCoef = 0.1;
                        GAME_STATE = 'r';
                    }
                    else if (option15.getGlobalBounds().contains(mouse)) {
                        MAX_HP = 50.f;
                        PLAYER_HP = MAX_HP;
                        KILL_TARGET = 15;
                        enemySpawnCoef = 0.005;
                        GAME_STATE = 'r';
                    }
                    
                    else if (option50.getGlobalBounds().contains(mouse)) {
                        MAX_HP = 150.f;
                        PLAYER_HP = MAX_HP;
                        KILL_TARGET = 50;
                        enemySpawnCoef = 0.0001;
                        GAME_STATE = 'r';
                    }
                    
                    killTarget.setString("Target: " + std::to_string(KILL_TARGET));
                }
            }
        }
        
        float dt = clock.restart().asSeconds();
        
        if(GAME_STATE == 'r') {
            
            fireDelay -= dt;
            enemySpawnDelay -= dt;
            laserDelay -= dt;
            if( enemySpawnCooldown - dt > 1.5)
                enemySpawnCooldown -= 0.1 * dt;
            
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
                laserDelay = laserCooldown;
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
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                GAME_STATE = 'p';
                std::cout << 'p' << std::endl;
                continue;
            }
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
        
        if(PLAYER_HP == 0 && GAME_STATE == 'r') {
            GAME_STATE = 'd';
            deathFlash.active = true;
            deathFlash.t = 1.f;
            MESSAGE.setString("GAME OVER");
            centerText(MESSAGE, window);
            score.setString("Score: " + std::to_string(KILL_COUNT));
            centerText(score, window);
            score.move({0, -60.f});
        }
        
        if((GAME_STATE == 'd' && !deathFlash.active) || GAME_STATE == 'w') { if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
                GAME_STATE = 'b';
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                window.close();
        }
        
        if(KILL_COUNT == KILL_TARGET && GAME_STATE == 'r') {
            GAME_STATE = 'w';
            MESSAGE.setString("YOU WIN");
            centerText(MESSAGE, window);
        }
        
        if(GAME_STATE == 'p') {
            MESSAGE.setString("Pause");
            centerText(MESSAGE, window);
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
                GAME_STATE = 'b';
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                window.close();
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N))
                GAME_STATE = 'r';
        }
        
        if (GAME_STATE == 'm') {
            sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            option3.setFillColor(normalColor);
            option15.setFillColor(normalColor);
            option50.setFillColor(normalColor);

            if (option3.getGlobalBounds().contains(mouse))
                option3.setFillColor(hoverColor);
            if (option15.getGlobalBounds().contains(mouse))
                option15.setFillColor(hoverColor);
            if (option50.getGlobalBounds().contains(mouse))
                option50.setFillColor(hoverColor);
            
            window.clear();
            window.draw(background);
            window.draw(afterGameBg);
            window.draw(menuTitle);
            window.draw(quitText);
            window.draw(option3);
            window.draw(option15);
            window.draw(option50);
            window.display();
            continue;
        }
        
        if(GAME_STATE == 'b') {
            reset();
        
            playerPos = {window.getSize().x / 2.f, window.getSize().y / 2.f};
            player.setPosition(playerPos);
            fireDir = {0.f, -1.f};
            player.setRotation(sf::degrees(90.f));
        
            GAME_STATE = 'm';
            KILL_COUNT = 0;
            enemySpawnCooldown = 4.f;
        }
            
        window.clear();
        window.draw(background);
        window.draw(hpBarBackground);
        window.draw(hpBar);
        window.draw(killCount);
        window.draw(killTarget);
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
        if(GAME_STATE == 'p')
            window.draw(resumeText);
        if(GAME_STATE == 'd')
            window.draw(score);
        if((GAME_STATE == 'd' && !deathFlash.active) || GAME_STATE == 'w' || GAME_STATE == 'p') {
            window.draw(restartText);
            window.draw(quitText);
        }
        window.display();
    }

    return 0;
}
