#include "Collisions.h"

bool Collisions::checkCollision(const Box &a, const Box &b) {
    return (std::abs(a.position.x - b.position.x) * 2 < (a.size.x + b.size.x)) &&
           (std::abs(a.position.y - b.position.y) * 2 < (a.size.y + b.size.y)) &&
           (std::abs(a.position.z - b.position.z) * 2 < (a.size.z + b.size.z));
}

void Collisions::resolveCollision(Box &a, Box &b) {
    Vec3 normal = { a.position.x - b.position.x, a.position.y - b.position.y, a.position.z - b.position.z };
    float length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

    // Normalize the normal vector
    normal.normalise();

    float relativeVelocityX = a.velocity.x - b.velocity.x;
    float relativeVelocityY = a.velocity.y - b.velocity.y;
    float relativeVelocityZ = a.velocity.z - b.velocity.z;

    // Compute the relative velocity along the normal
    float impulse = relativeVelocityX * normal.x + relativeVelocityY * normal.y + relativeVelocityZ * normal.z;

    // Ignore collision if objects are moving away from each other
    if (impulse > 0) {
        return;
    }

    // Compute the collision impulse scalar
    float e = 0.01f; // Coefficient of restitution (0 = inelastic, 1 = elastic)
    float dampening = 0.9f; // Dampening factor (0.9 = 10% energy reduction)
    float j = -(1.0f + e) * impulse * dampening;

    // Apply the impulse to the boxes' velocities
    a.velocity.x += j * normal.x;
    a.velocity.y += j * normal.y;
    a.velocity.z += j * normal.z;
    b.velocity.x -= j * normal.x;
    b.velocity.y -= j * normal.y;
    b.velocity.z -= j * normal.z;
}
