#include "Ball.hpp"
#include <cmath>

#pragma warning(push)
#pragma warning(disable : 4458)

Ball::Ball(const Point& center, double radius, const Velocity& velocity, const Color& color, bool isCollidable) {
    this->center = center;
    this->radius = radius;
    this->velocity = velocity;
    this->color = color;
    this->isCollidable = isCollidable;    
}

std::istream& operator>>(std::istream& stream, Ball& ball) {
    Point center;
    Point vec;
    Color color;
    double radius;
    bool isCollidable; 
    stream >> center >> vec >> color >> radius >> std::boolalpha >> isCollidable;
    ball.center = center;
    ball.velocity = Velocity(vec);
    ball.color = color;
    ball.radius = radius;
    ball.isCollidable = isCollidable;
    return stream;
}

/**
 * Задает скорость объекта
 * @param velocity новое значение скорости
 */
void Ball::setVelocity(const Velocity& velocity) {    
    this->velocity = velocity;
}

/**
 * @return скорость объекта
 */
Velocity Ball::getVelocity() const {    
    return this->velocity;
}

/**
 * @brief Выполняет отрисовку объекта
 * @details объект Ball абстрагирован от конкретного
 * способа отображения пикселей на экране. Он "знаком"
 * лишь с интерфейсом, который предоставляет Painter
 * Рисование выполняется путем вызова painter.draw(...)
 * @param painter контекст отрисовки
 */
void Ball::draw(Painter& painter) const {    
    painter.draw(this->center, this->radius, this->color);
}

/**
 * Задает координаты центра объекта
 * @param center новый центр объекта
 */
void Ball::setCenter(const Point& center) {    
    this->center = center;
}

/**
 * @return центр объекта
 */
Point Ball::getCenter() const {    
    return this->center;
}

/**
 * @brief Возвращает радиус объекта
 * @details обратите внимание, что метод setRadius()
 * не требуется
 */
double Ball::getRadius() const {    
    return this->radius;
}

/**
 * @brief Возвращает массу объекта
 * @details В нашем приложении считаем, что все шары
 * состоят из одинакового материала с фиксированной
 * плотностью. В этом случае масса в условных единицах
 * эквивалентна объему: PI * radius^3 * 4. / 3.
 */
double Ball::getMass() const {        
    const double PI = std::acos(-1);
    return PI * pow(this->radius, 3) * 4.0 / 3.0;    
}

bool Ball::IsCollidable() const {
    return this->isCollidable;
}

#pragma warning(pop)