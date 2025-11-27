sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t) {
    return a + (b - a) * t;
}

float length(sf::Vector2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(sf::Vector2f v) {
    float len = length(v);
    return (len == 0.f) ? sf::Vector2f{0,0} : v / len;
}

void centerText(sf::Text& text, sf::RenderWindow& screen) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    text.setPosition({screen.getSize().x / 2.f, screen.getSize().y / 2.f});
}

void menu() {
    
}

sf::Texture bgTex("textures/background.jpg");

sf::Texture playerTex("textures/flight_unit.png");
float playerRadius = playerTex.getSize().x * 0.05 / 2.f;

sf::Texture enemyTex("textures/enemy.png");
float enemyRadius = enemyTex.getSize().x * 0.05 / 2.f;

sf::Font font("fonts/arialmt.ttf");
