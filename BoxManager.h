#ifndef LINUXPORT_BOXMANAGER_H
#define LINUXPORT_BOXMANAGER_H
#include "Vec3.h"
#include <vector>

#define minX -10.0f
#define maxX 30.0f
#define minZ -30.0f
#define maxZ 30.0f

const float floorY = 0.0f;
// gravity - change it and see what happens (usually negative!)
const float gravity = -19.81f;


// the box (falling item)
struct Box {
    Vec3 position;
    Vec3 size;
    Vec3 velocity;
    Vec3 colour;
};

class BoxManager {
private:

public:
    BoxManager() = default;
    ~BoxManager() = default;

    Box CreateBox();
    void Update(Box& box, float deltaTime);
};

#endif //LINUXPORT_BOXMANAGER_H
