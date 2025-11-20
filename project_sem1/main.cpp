#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "headers/subfunc.hpp"
#include "headers/particles.hpp"
#include "headers/bullets.hpp"
#include "headers/enemies.hpp"
#include "headers/laser.hpp"
#include "headers/background.hpp"

const float MAX_HP = 500.f;
float PLAYER_HP = MAX_HP;

sf::Texture bgTex("textures/background.jpg");

sf::Texture playerTex("textures/flight_unit.png");
float playerRadius = playerTex.getSize().x * 0.05 / 2.f;

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
                        
                        sf::CircleShape enemyShape(enemyTex.getSize().x * 0.05 / 2.f);
                        enemyShape.setOrigin({enemyRadius, enemyRadius});
                        enemyShape.setPosition(enemies[j].pos);
                        sf::FloatRect enemyBox = enemyShape.getGlobalBounds();
                        
//                        float w = enemyTex.getSize().x * 0.05;
//                        float h = enemyTex.getSize().x * 0.05;
//                        float x = enemies[i].pos.x - w/2.f;
//                        float y = enemies[i].pos.y - h/2.f;
//                        sf::FloatRect enemyBox({x, y}, {w, h});
                        
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
    
    sf::RectangleShape hpBarBackground({200.f, 20.f});
    hpBarBackground.setFillColor(sf::Color(50, 50, 50));
    hpBarBackground.setPosition({20.f, 20.f});
    
    sf::RectangleShape hpBar({200.f, 20.f});
    hpBar.setFillColor(sf::Color::Green);
    hpBar.setPosition({20.f, 20.f});

    sf::Vector2f playerPos{800.f, 450.f};
//    sf::CircleShape player(20.f);
//    player.setOrigin({20.f, 20.f});
//    player.setFillColor(sf::Color::Cyan);
    
    sf::Sprite player{playerTex};
    player.setOrigin({playerTex.getSize().x / 2.f, playerTex.getSize().y / 2.f});
    player.setScale({0.1, 0.1});

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
            if (const auto* resized = event->getIf<sf::Event::Resized>())
                window.setSize({resized->size.x, resized->size.y});
        }
        
        

        float dt = clock.restart().asSeconds();
        fireDelay -= dt;
        enemySpawnDelay -= dt;
        laserDelay -= dt;
        
//        updateBackground(dt);
//        if(prevIndex < bgIndex) {
//            std::string zeros = (bgIndex < 10 ? "00" : (bgIndex < 100 ? "0" : ""));
//            std::string filename = "textures/frames/frame_" + zeros + std::to_string(bgIndex) + ".jpg";
//            sf::Texture bgTex(filename);
//            sf::Sprite background{bgTex};
//            background.setOrigin({(float)window.getSize().x, (float)window.getSize().y});
//            background.setScale({(float)bgTex.getSize().x / window.getSize().x, (float)bgTex.getSize().y / window.getSize().y});
//        }

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
        
//        sf::Vector2f trailDir = -(playerPos - player.getPosition());

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
        
//        spawnFireTrail(playerPos, trailDir, 5);
        
        updateBullet(dt, window.getSize());
        
        updateEnemy(dt, window.getSize(), playerPos);
        
        updateLaser(dt);
        
        updateParticles(dt);
        
        checkCollision(playerPos);
        
        float hp_percentage = PLAYER_HP / MAX_HP;
        sf::RectangleShape hpBar({200.f * hp_percentage, 20.f});
        hpBar.setPosition({20.f, 20.f});
        if(hp_percentage >= 0.7)
            hpBar.setFillColor(sf::Color::Green);
        else if(hp_percentage >= 0.25)
            hpBar.setFillColor(sf::Color(255, 165, 0));
        else
            hpBar.setFillColor(sf::Color::Red);

        window.clear();
        window.draw(background);
        window.draw(hpBarBackground);
        window.draw(hpBar);
        window.draw(player);
        drawBullets(window);
        drawEnemies(window);
        drawLaser(window);
        drawParticle(window);
        window.display();
    }

    return 0;
}
