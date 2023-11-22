#ifndef LINUXPORT_COLLISIONS_H
#define LINUXPORT_COLLISIONS_H

#include "BoxManager.h"
#include "Vec3.h"

class Collisions {
private:

public:
    Collisions() = default;
    ~Collisions() = default;
    bool checkCollision(const Box& a, const Box& b);
    void resolveCollision(Box& a, Box& b);
};


#endif //LINUXPORT_COLLISIONS_H
