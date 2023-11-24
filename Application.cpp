#include "Application.h"

void Application::initScene(int boxCount) {
    for (int i = 0; i < boxCount; ++i) {
        boxes.push_back(boxManager->CreateBox());
    }
}

// a ray which is used to tap (by default, remove) a box - see the 'mouse' function for how this is used.
bool Application::rayBoxIntersection(const Vec3 &rayOrigin, const Vec3 &rayDirection, const Box &box) {
    float tMin = (box.position.x - box.size.x / 2.0f - rayOrigin.x) / rayDirection.x;
    float tMax = (box.position.x + box.size.x / 2.0f - rayOrigin.x) / rayDirection.x;

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (box.position.y - box.size.y / 2.0f - rayOrigin.y) / rayDirection.y;
    float tyMax = (box.position.y + box.size.y / 2.0f - rayOrigin.y) / rayDirection.y;

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (box.position.z - box.size.z / 2.0f - rayOrigin.z) / rayDirection.z;
    float tzMax = (box.position.z + box.size.z / 2.0f - rayOrigin.z) / rayDirection.z;

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    return true;
}

void Application::updatePhysics() {
    for (Box& box : boxes) {
        boxManager->Update(box, deltaTime); //update box

        // Check for collisions with other boxes
        for (Box& other : boxes) {
            if (&box == &other) continue;
            if (collisionHandler->checkCollision(box, other)) {
                collisionHandler->resolveCollision(box, other);
                break;
            }
        }
    }
}

void *Application::updatePhysicsStatic(void *ptr) {
    static_cast<Application*>(ptr)->updatePhysics();
    return NULL;
}

// used in the 'mouse' tap function to convert a screen point to a point in the world
Vec3 Application::screenToWorld(int x, int y) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return Vec3((float)posX, (float)posY, (float)posZ);
}

// draw the sides of the containing area
void Application::drawQuad(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3, const Vec3 &v4) {
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v4.x, v4.y, v4.z);
    glEnd();
}

// draw the physics object
void Application::drawBox(const Box &box) {
    glPushMatrix();
    glTranslatef(box.position.x, box.position.y, box.position.z);
    GLfloat diffuseMaterial[] = { box.colour.x, box.colour.y, box.colour.z, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glScalef(box.size.x, box.size.y, box.size.z);
    glRotatef(-90, 1, 0, 0);
    glutSolidCube(1.0);
    //glutSolidTeapot(1);
    //glutSolidCone(1, 1, 10, 10);
    glPopMatrix();
}

void Application::drawScene() {
// Draw the side wall
    GLfloat diffuseMaterial[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

    // Draw the left side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vec3 leftSideWallV1(minX, 0.0f, maxZ);
    Vec3 leftSideWallV2(minX, 50.0f, maxZ);
    Vec3 leftSideWallV3(minX, 50.0f, minZ);
    Vec3 leftSideWallV4(minX, 0.0f, minZ);
    drawQuad(leftSideWallV1, leftSideWallV2, leftSideWallV3, leftSideWallV4);

    // Draw the right side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vec3 rightSideWallV1(maxX, 0.0f, maxZ);
    Vec3 rightSideWallV2(maxX, 50.0f, maxZ);
    Vec3 rightSideWallV3(maxX, 50.0f, minZ);
    Vec3 rightSideWallV4(maxX, 0.0f, minZ);
    drawQuad(rightSideWallV1, rightSideWallV2, rightSideWallV3, rightSideWallV4);


    // Draw the back wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vec3 backWallV1(minX, 0.0f, minZ);
    Vec3 backWallV2(minX, 50.0f, minZ);
    Vec3 backWallV3(maxX, 50.0f, minZ);
    Vec3 backWallV4(maxX, 0.0f, minZ);
    drawQuad(backWallV1, backWallV2, backWallV3, backWallV4);

    for (const Box& box : boxes) {
        drawBox(box);
    }
}

void Application::idle() {
    static auto last = steady_clock::now();
    auto old = last;
    last = steady_clock::now();
    const duration<float> frameTime = last - old;
    deltaTime = frameTime.count();

    //update physics
    pt1 = pthread_create(&thread1, NULL, Application::updatePhysicsStatic, this);

    // tell glut to draw - note this will cap this function at 60 fps
    pt2 = pthread_create(&thread2, NULL, reinterpret_cast<void *(*)(void *)>(glutPostRedisplay), NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}


