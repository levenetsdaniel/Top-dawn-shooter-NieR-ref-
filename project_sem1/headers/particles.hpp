struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    bool active = false;
    float lifeTime;
    int green = 0;
    int blue = 0;
};

const int MAX_PARTICLES = 300;
Particle particles[MAX_PARTICLES];

void spawnParticles(const sf::Vector2f pos, int count) {
    for(int i = 0; i < MAX_PARTICLES && count > 0; i++) {
        if(!particles[i].active) {
            count--;
            particles[i].active = true;
            particles[i].pos = pos;
            particles[i].green = 0;
            particles[i].blue = 0;
            
            float alpha = (rand() % 360) * 0.0174533;
            float speed = 80.f + (float)(rand() % 121);
            
            particles[i].vel = {std::cos(alpha) * speed, std::sin(alpha) * speed};
            particles[i].lifeTime = 0.6 + (float)(rand() % 41) / 100.f;
        }
    }
}

void spawnFireTrail(const sf::Vector2f pos, const sf::Vector2f dir, int count) {
    for(int i = 0; i < MAX_PARTICLES; i++) {
        if(!particles[i].active) {
            count--;
            particles[i].active = true;
            particles[i].pos = pos;
            particles[i].green = 100 + rand() % 156;
            particles[i].blue = 0;
            
            float alpha = pow(-1, rand()%2) * (rand() % 30) * 0.0174533;
            float speed = 80.f + (float)(rand() % 121);
            
            particles[i].vel = {std::cos(alpha) * speed, std::sin(alpha) * speed};
            particles[i].lifeTime = 0.6 + (float)(rand() % 41) / 100.f;
        }
    }
}

void updateParticles(float dt) {
    for(int i = 0; i < MAX_PARTICLES; i++) {
        if(particles[i].active) {
            particles[i].pos += particles[i].vel * dt;
            particles[i].vel.x *= 0.9;
            particles[i].vel.y *= 0.9;
            
            particles[i].lifeTime -= dt;
            if(particles[i].lifeTime <= 0)
                particles[i].active = false;
        }
    }
}

void drawParticle(sf::RenderWindow& window) {
    sf::CircleShape part(3.f);
    part.setOrigin({3.f, 3.f});
    
    for(int i = 0; i < MAX_PARTICLES; i++) {
        if(particles[i].active) {
            part.setPosition(particles[i].pos);
            
            float alpha = particles[i].lifeTime * 255.f;
            part.setFillColor(sf::Color(255, particles[i].green, particles[i].blue, alpha));
            window.draw(part);
        }
    }
}
