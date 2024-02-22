#pragma once
#include "Painter.hpp"
#include "Point.hpp"
#include "Velocity.hpp"

class Ball {
    Point center;
    double radius;
    Velocity velocity;
    Color color;
    bool isCollidable;

private:
    friend std::istream& operator>>(std::istream& stream, Ball& ball);

public:
    Ball() = default;
    Ball(const Point& center, double radius, const Velocity& velocity, const Color& color, bool isCollidable);
    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    bool IsCollidable() const;
};
