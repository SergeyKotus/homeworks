#pragma once
#include <chrono>
#include "Ball.hpp"

#define _DUST_LIFETIME 250
#define _DUST_RADIUS 5.0
#define _DUST_VELOCITY_MODULE 1000
#define _DUST_COLOR Color(0.90, 0.30, 0.10)
#define _APPLICATION_RUNTIME 10    

#pragma warning(push)
#pragma warning(disable : 4458)

class Dust : public Ball {    
    static inline auto begin = std::chrono::system_clock::now();//время начала работы программы
    size_t lifeTime;//время жизни частицы пыли в тиках
public:
    inline Dust(const Point& center, const Velocity& velocity, size_t lifeTime = _DUST_LIFETIME) : Ball(center, _DUST_RADIUS, velocity, _DUST_COLOR, false), lifeTime{lifeTime} {};

    inline void ReduseLifeTime() {
        if(this->lifeTime > 0)
            this->lifeTime--;   
    }

    inline size_t GetLifeTime() {
        return this->lifeTime;
    }

    inline void ResetLifeTime() {
        this->lifeTime = 0;
    }

    //проверяем по времени, обновляются ли еще отрисовываемые объекты
    inline static bool DustsShouldBeCleaned() {
        return (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - Dust::begin)).count() > _APPLICATION_RUNTIME;
    }
};

#pragma warning(pop)