struct Bullet {
    sf::Vector2f pos;
    sf::Vector2f vel;
    bool active = false;
    char owner;
};

const int MAX_BULLETS = 200;
Bullet bullets[MAX_BULLETS];

void spawnBullet(const sf::Vector2f pos, const sf::Vector2f dir, float dist, char owner) {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].pos = pos + dist * dir;
            bullets[i].vel = normalize(dir) * 600.f;
            bullets[i].owner = owner;
            return;
        }
    }
}

void updateBullet(float dt, sf::Vector2u screen) {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active) {
            bullets[i].pos += bullets[i].vel * dt;
            
            if(bullets[i].pos.x < 0 || bullets[i].pos.x > screen.x || bullets[i].pos.y < 0 || bullets[i].pos.y > screen.y) {
                bullets[i].active = false;
            }
        }
    }
}

void drawBullets(sf::RenderWindow& window) {
    sf::CircleShape shape(5.f);
        shape.setOrigin({5.f, 5.f});
        shape.setFillColor(sf::Color::Yellow);

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active){
                shape.setPosition(bullets[i].pos);
                if(bullets[i].owner == 'p')
                    shape.setFillColor(sf::Color::Yellow);
                else if(bullets[i].owner == 'e')
                    shape.setFillColor(sf::Color::Red);
                window.draw(shape);
            }
        }
}
