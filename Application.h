#ifndef LINUXPORT_APPLICATION_H
#define LINUXPORT_APPLICATION_H

#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "BoxManager.h"
#include "Collisions.h"
using namespace std::chrono;

// this is the number of falling physical items.
#define NUMBER_OF_BOXES 50

// this is where the camera is, where it is looking and the bounds of the continaing box. You shouldn't need to alter these

class Application {
public:
    Application();
    ~Application();

    void initScene(int boxCount);
    bool rayBoxIntersection(const Vec3& rayOrigin, const Vec3& rayDirection, const Box& box);
    Vec3 screenToWorld(int x, int y);
    void updatePhysics();
    static void* updatePhysicsStatic(void *ptr);
    void drawQuad(const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec3& v4);
    void drawBox(const Box& box);
    void drawScene();
    void idle();

    std::vector<Box> boxes;
private:
    BoxManager* boxManager = new BoxManager();
    Collisions* collisionHandler = new Collisions();
    pthread_t thread1,thread2;
    int pt1, pt2;
    float deltaTime;
};


#endif //LINUXPORT_APPLICATION_H
