//
// Created by bonita on 21/11/23.
//

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
