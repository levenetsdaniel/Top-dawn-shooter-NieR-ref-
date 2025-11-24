struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    bool active = false;
    float lifeTime;
    int green = 0;
    int blue = 0;
};

struct Bullet {
    sf::Vector2f pos;
    sf::Vector2f vel;
    bool active = false;
    char owner;
};

struct Laser {
    bool active = false;
    bool damaging = false;
    sf::FloatRect area;
    float telegraphTime = 4.f;
    float activeTime = 2.f;
    float t = 0.f;
};

struct Enemy {
    bool active = false;
    sf::Vector2f pos;
    sf::Vector2f dir = {1, 0};
    int pat;
    float t = 0.f;
    float hp = 5.f;
    float amplitude = 150.f;
    float fireCooldawn = 0.8;
    float fireDelay = 0.4;
    float spawnX;
    float spawnY = 200.f;
    float angle = 0.f;
};

struct DeathFlash {
    float t = 0.f;
    bool active = false;
    sf::RectangleShape flash;
};
