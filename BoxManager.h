//
// Created by bonita on 21/11/23.
//

#ifndef LINUXPORT_BOXMANAGER_H
#define LINUXPORT_BOXMANAGER_H
#include "Vec3.h"

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

};


#endif //LINUXPORT_BOXMANAGER_H
