struct Vec2 {
    float x, y;
    Vec2(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}

    // Conversion constructor to SDLFramework::Vector2
    operator SDLFramework::Vector2() const {
        return SDLFramework::Vector2(x, y);
    }
};
