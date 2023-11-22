#include "BoxManager.h"

Box BoxManager::CreateBox() {
    Box box;

    // Assign random x, y, and z positions within specified ranges
    box.position.x = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f));
    box.position.y = 10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f));
    box.position.z = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f));

    box.size = {1.0f, 1.0f, 1.0f};

    // Assign random x-velocity between -1.0f and 1.0f
    float randomXVelocity = -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));
    box.velocity = {randomXVelocity, 0.0f, 0.0f};

    // Assign a random color to the box
    box.colour.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    box.colour.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    box.colour.z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    return box;
}

void BoxManager::Update(Box& box, float deltaTime) {
    // Update velocity due to gravity
    box.velocity.y += gravity * deltaTime;

    // Update position based on velocity
    box.position.x += box.velocity.x * deltaTime;
    box.position.y += box.velocity.y * deltaTime;
    box.position.z += box.velocity.z * deltaTime;

    // Check for collision with the floor
    if (box.position.y - box.size.y / 2.0f < floorY) {
        box.position.y = floorY + box.size.y / 2.0f;
        float dampening = 0.7f;
        box.velocity.y = -box.velocity.y * dampening;
    }

    // Check for collision with the walls
    if (box.position.x - box.size.x / 2.0f < minX || box.position.x + box.size.x / 2.0f > maxX) {
        box.velocity.x = -box.velocity.x;
    }
    if (box.position.z - box.size.z / 2.0f < minZ || box.position.z + box.size.z / 2.0f > maxZ) {
        box.velocity.z = -box.velocity.z;
    }
}
