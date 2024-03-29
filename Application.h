#ifndef LINUXPORT_APPLICATION_H
#define LINUXPORT_APPLICATION_H

#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <sys/types.h>
#include "BoxManager.h"
#include "Collisions.h"
using namespace std::chrono;

// this is the number of falling physical items.
#define NUMBER_OF_BOXES 500
// this is where the camera is, where it is looking and the bounds of the box. You shouldn't need to alter these

#define LOOKAT_X 10
#define LOOKAT_Y 10
#define LOOKAT_Z 50

#define LOOKDIR_X 10
#define LOOKDIR_Y 0
#define LOOKDIR_Z 0

class Application {
public:
    Application() = default;
    ~Application() = default;

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
    float GetDeltaTime() { return deltaTime;}
private:
    BoxManager* boxManager = new BoxManager();
    Collisions* collisionHandler = new Collisions();
    pthread_t thread1{},thread2{};
    int pt1{}, pt2{};
    float deltaTime{};
};


#endif //LINUXPORT_APPLICATION_H
