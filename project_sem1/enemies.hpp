#include <cstdlib>

struct Enemy {
    bool active = false;
    sf::Vector2f pos;
    sf::Vector2f dir = {1, 0};
    int pat;
    float t = 0.f;
    int hp = 5;
    float fireCooldawn = 0.8;
    float fireDelay = 0.4;
    float spawnX;
    float spawnY;
};

const int MAX_ENEMIES = 50;
Enemy enemies[MAX_ENEMIES];

void enemyFire(Enemy enemy, const sf::Vector2f& target) {
    sf::Vector2f dir = normalize(target - enemy.pos);
    spawnBullet(enemy.pos, dir, 'e');
}

void spawnEnemy(sf::Vector2u screen) {
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(!enemies[i].active) {
            enemies[i].active = true;
            enemies[i].hp = 5;
            enemies[i].spawnX = (float)(rand() % screen.x);
            enemies[i].spawnY = rand() % 2 == 0 ? 200.f : screen.y - 200.f;
            enemies[i].pos = {enemies[i].spawnX, enemies[i].spawnY};
            enemies[i].pat = rand()%3;
            return;
        }
    }
}

void updateEnemy(float dt, sf::Vector2u screen, const sf::Vector2f& target) {
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
                    enemies[i].pos.x = target.x + std::cos(enemies[i].t) * 300.f;
                    enemies[i].pos.y = target.y + std::sin(enemies[i].t) * 300.f;
                    break;
            }
            
            if(enemies[i].hp == 0)
                enemies[i].active = false;
            
            enemies[i].fireDelay -= dt;
            if(enemies[i].fireDelay <= 0) {
                enemyFire(enemies[i], target);
                enemies[i].fireDelay = enemies[i].fireCooldawn;
            }
        }
    }
}



void drawEnemies(float dt, sf::RenderWindow& window) {
    sf::CircleShape shape(15.f);
    shape.setOrigin({15.f, 15.f});
    shape.setFillColor(sf::Color::Magenta);

    for (int i = 0; i < MAX_ENEMIES; ++i) {
        if (enemies[i].active) {
            shape.setPosition(enemies[i].pos);
            window.draw(shape);
        }
    }
}
