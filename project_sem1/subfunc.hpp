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
