Laser laser;

void triggerLaser(const sf::Vector2u screen, const sf::Vector2f target) {
    if(!laser.active) {
        laser.active = true;
        laser.damaging = false;
        laser.t = 0;
        laser.area = sf::FloatRect({target.x - 75.f, 0.f}, {150.f, (float)screen.y});
    }
}

void updateLaser(Laser *laser, float dt) {
    if(laser->active) {
        laser->t += dt;
        
        if(!laser->damaging && laser->t >= laser->telegraphTime) {
            laser->damaging = true;
            laser->t = 0.f;
        }
        
        if(laser->damaging && laser->t >= laser->activeTime) {
            laser->damaging = false;
            laser->active = false;
        }
    }
}

void drawLaser(Laser laser, sf::RenderWindow *window) {
    if(laser.active) {
        sf::RectangleShape shape;
        shape.setPosition({laser.area.position.x, laser.area.position.y});
        shape.setSize({laser.area.size.x, laser.area.size.y});
        
        float intensity = (std::sin(laser.t * 10.f) * 0.5f + 0.5f) * 40.f;
        if(!laser.damaging)
            shape.setFillColor(sf::Color(255, 0, 0, intensity));
        
        if(laser.damaging)
            shape.setFillColor(sf::Color(255, 0, 0, 200));
        
        window->draw(shape);
    }
}
