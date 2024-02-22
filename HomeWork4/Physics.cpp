#include "Physics.hpp"

double dot(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}

void Physics::setWorldBox(const Point& topLeft, const Point& bottomRight) {
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
}

void Physics::update(std::vector<Ball>& balls, std::vector<Dust>& dusts, const size_t ticks) const {

    for (size_t i = 0; i < ticks; ++i) {

        //Убираем пыль с истекшим сроком существования
        for (std::vector<Dust>::iterator it = dusts.begin(); it != dusts.end();) {
            it->ReduseLifeTime();
            if(it->GetLifeTime() == 0)
                it = dusts.erase(it);
            else
                ++it;
        }

        move(balls, dusts);
        collideWithBox(balls, dusts);
        collideBalls(balls, dusts);
    }
}

void Physics::dustGeneration(const Ball& a, const Ball& b, std::vector<Dust>& dusts) const {
    //вычисляем центр генерации пыли
    Point center = a.getCenter() + (b.getCenter() - a.getCenter()) * a.getRadius()/(a.getRadius() + b.getRadius());

    //генерируем пыль
    const double PI = std::acos(-1);    
    for(double rad = 0.0; rad < 2 * PI; rad += 0.4)
    {        
        dusts.push_back(Dust(center, Velocity(_DUST_VELOCITY_MODULE, rad)));
    }
}

void Physics::collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    for (auto a = balls.begin(); a != balls.end(); ++a) {
        if(a->IsCollidable()) {
            for (auto b = std::next(a); b != balls.end(); ++b) {
                if(b->IsCollidable()) {
                    const double distanceBetweenCenters2 =
                        distance2(a->getCenter(), b->getCenter());
                    const double collisionDistance = a->getRadius() + b->getRadius();
                    const double collisionDistance2 =
                        collisionDistance * collisionDistance;

                    if (distanceBetweenCenters2 < collisionDistance2) {
                        processCollision(*a, *b, distanceBetweenCenters2);
                        dustGeneration(*a, *b, dusts);
                    }
                }
            }
        }
    }
}

void Physics::collideWithBox(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    // определяет, находится ли v в диапазоне (lo, hi) (не включая границы)
    auto isOutOfRange = [](double v, double lo, double hi) {
        return v < lo || v > hi;
    };

    for (Ball& ball : balls) {
        if(ball.IsCollidable()) {
            const Point p = ball.getCenter();
            const double r = ball.getRadius();            

            if (isOutOfRange(p.x, topLeft.x + r, bottomRight.x - r)) {
                Point vector = ball.getVelocity().vector();
                vector.x = -vector.x;
                ball.setVelocity(vector);
            } else if (isOutOfRange(p.y, topLeft.y + r, bottomRight.y - r)) {
                Point vector = ball.getVelocity().vector();
                vector.y = -vector.y;
                ball.setVelocity(vector);
            }
        }
    }

    for (Dust& dust : dusts) {
        const Point p = dust.getCenter();
        const double r = dust.getRadius();

        if(isOutOfRange(p.x, topLeft.x + r, bottomRight.x - r) ||
           isOutOfRange(p.y, topLeft.y + r, bottomRight.y - r))
            {
                dust.ResetLifeTime();        
            }
    }
}

void Physics::move(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    for (Ball& ball : balls) {
        Point newPos =
            ball.getCenter() + ball.getVelocity().vector() * timePerTick;
        ball.setCenter(newPos);
    }

    for (Dust& dust : dusts) {
        Point newPos =
            dust.getCenter() + dust.getVelocity().vector() * timePerTick;
        dust.setCenter(newPos);
    }
}

void Physics::processCollision(Ball& a, Ball& b,
                               double distanceBetweenCenters2) const {
    // нормированный вектор столкновения
    const Point normal =
        (b.getCenter() - a.getCenter()) / std::sqrt(distanceBetweenCenters2);

    // получаем скорость в векторном виде
    const Point aV = a.getVelocity().vector();
    const Point bV = b.getVelocity().vector();

    // коэффициент p учитывает скорость обоих мячей
    const double p =
        2 * (dot(aV, normal) - dot(bV, normal)) / (a.getMass() + b.getMass());

    // задаем новые скорости мячей после столкновения
    a.setVelocity(Velocity(aV - normal * p * a.getMass()));
    b.setVelocity(Velocity(bV + normal * p * b.getMass()));
}
