#include <cstdlib>

struct Enemy {
    bool active = false;
    sf::Vector2f pos;
    sf::Vector2f dir = {1, 0};
    int pat;
    float t = 0.f;
    float hp = 5.f;
    float fireCooldawn = 0.8;
    float fireDelay = 0.4;
    float spawnX;
    float spawnY;
    float angle = 0.f;
};

const int MAX_ENEMIES = 50;
Enemy enemies[MAX_ENEMIES];
int KILL_COUNT = 0;
sf::Texture enemyTex("textures/enemy.png");
float enemyRadius = enemyTex.getSize().x * 0.05 / 2.f;

void enemyFire(Enemy enemy, const sf::Vector2f dir) {
    spawnBullet(enemy.pos, dir, enemyRadius, 'e');
}

void spawnEnemy(sf::Vector2u screen) {
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(!enemies[i].active) {
            enemies[i].active = true;
            enemies[i].hp = 5;
            enemies[i].angle = 0.f;
            enemies[i].spawnX = (float)(rand() % screen.x);
            enemies[i].spawnY = rand() % 2 == 0 ? 200.f : screen.y - 200.f;
            enemies[i].pos = {enemies[i].spawnX, enemies[i].spawnY};
            enemies[i].pat = rand()%3;
            return;
        }
    }
}

void updateEnemy(float dt, sf::Vector2u screen, const sf::Vector2f target) {
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(enemies[i].active) {
            enemies[i].t += dt;
            
            switch(enemies[i].pat) {
                case 0:
                    if(enemies[i].pos.x < 0)
                        enemies[i].dir = {1, 0};
                    else if(enemies[i].pos.x > screen.x)
                        enemies[i].dir = {-1, 0};
                    enemies[i].pos += enemies[i].dir * 200.f * dt;
                    break;
                    
                case 1:
                    if(enemies[i].pos.x < 0)
                        enemies[i].dir = {1, 0};
                    else if(enemies[i].pos.x > screen.x)
                        enemies[i].dir = {-1, 0};
                    enemies[i].pos.x += enemies[i].dir.x * 200.f * dt;
                    enemies[i].pos.y = enemies[i].spawnY + std::sin(enemies[i].t * 3.f) * 150.f;
                    break;
                    
                case 2:
                    float x = target.x + std::cos(enemies[i].t) * 300.f;
                    if(x > screen.x) x = screen.x;
                    if(x < 0) x = 0;
                    float y = target.y + std::sin(enemies[i].t) * 300.f;
                    if(y > screen.y) y = screen.y;
                    if(y < 0) y = 0;
                    enemies[i].pos.x = x;
                    enemies[i].pos.y = y;
                    break;
            }
            
            if(enemies[i].hp == 0) {
                KILL_COUNT++;
                spawnParticles(enemies[i].pos, 50);
                enemies[i].active = false;
            }
            
            sf::Vector2f fireDir = normalize(target - enemies[i].pos);
            enemies[i].angle = (std::atan2(fireDir.x, fireDir.y) * 180.f) / 3.1415926f;
            enemies[i].fireDelay -= dt;
            if(enemies[i].fireDelay <= 0) {
                enemyFire(enemies[i], fireDir);
                enemies[i].fireDelay = enemies[i].fireCooldawn;
            }
        }
    }
}



void drawEnemies(sf::RenderWindow& window) {
//    sf::CircleShape shape(15.f);
//    shape.setOrigin({15.f, 15.f});
//    shape.setFillColor(sf::Color::Magenta);
    
    sf::Sprite sprite{enemyTex};
    sprite.setOrigin({enemyTex.getSize().x / 2.f, enemyTex.getSize().y / 2.f});
    sprite.setScale({0.05, 0.05});

    for (int i = 0; i < MAX_ENEMIES; ++i) {
        if (enemies[i].active) {
//            shape.setPosition(enemies[i].pos);
//            window.draw(shape);
            
            sprite.setPosition(enemies[i].pos);
            sprite.setRotation(-sf::degrees(enemies[i].angle) + sf::degrees(90.f));
            window.draw(sprite);
        }
    }
}
