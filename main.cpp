#include "Application.h"

// this is the number of falling physical items.
#define NUMBER_OF_BOXES 50

// this is where the camera is, where it is looking and the bounds of the box. You shouldn't need to alter these

#define LOOKAT_X 10
#define LOOKAT_Y 10
#define LOOKAT_Z 50

#define LOOKDIR_X 10
#define LOOKDIR_Y 0
#define LOOKDIR_Z 0

auto* app = new Application();

#pragma region examples of multithreading

void ForkExample()
{
    //child process returns value zero
    if (fork() == 0) {
        printf("Hello from Child!\n");
    }
    //parent process returns non zero
    else {
        printf("Hello from Parent!\n");
    }
}

#define ReadEnd 0
#define WriteEnd 1

void PipeExample()
{
    int i;
    int fd1[2]; //store 2 ends of pipe

    if (pipe(fd1)==-1) { //attempt to create pipe
        //idk
        std::cout << "\nCould not create pipe.";
    }

    pid_t p = fork();

    if(p>0) { //parent process
        close(fd1[ReadEnd]); //close reading end
        //write(fd1[WriteEnd], "hello from pipe", strlen("hello from pipe")+1); //write string to pipe
        i = 5;
        write(fd1[WriteEnd], &i, sizeof(i));
        close(fd1[WriteEnd]); //close the write end
    }
    else if(p==0) { //child process
        close(fd1[WriteEnd]); //close write end
        //char str[100];
        //read(fd1[ReadEnd], str, 100); //read in data
        read(fd1[ReadEnd], &i, sizeof(i));
        std::cout << i << std::endl;
        //printf("%s\n", i);
        close(fd1[ReadEnd]); //close read end
        exit(0);
    }
}


void *print_message_function( void *ptr )
{
    char *message;
    message = (char *) ptr;
    printf("%s \n", message);
}

void PThreadsExample()
{
    pthread_t thread1,thread2;

    char *message1 = "Thread 1";
    char *message2 = "Thread 2";
    int  pt1, pt2;

    pt1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
    pt2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);

    printf("Thread 1 returns: %d\n",pt1);
    printf("Thread 2 returns: %d\n",pt2);
}

#pragma endregion

void idle() {
    app->idle();
}

// called by GLUT - displays the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(LOOKAT_X, LOOKAT_Y, LOOKAT_Z, LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z, 0, 1, 0);

    app->drawScene();

    glutSwapBuffers();
}

// called the mouse button is tapped
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Get the camera position and direction
        Vec3 cameraPosition(LOOKAT_X, LOOKAT_Y, LOOKAT_Z); // Replace with your actual camera position
        Vec3 cameraDirection(LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z); // Replace with your actual camera direction

        // Get the world coordinates of the clicked point
        Vec3 clickedWorldPos = app->screenToWorld(x, y);

        // Calculate the ray direction from the camera position to the clicked point
        Vec3 rayDirection = clickedWorldPos - cameraPosition;
        rayDirection.normalise();

        // Perform a ray-box intersection test and remove the clicked box
        size_t clickedBoxIndex = -1;
        float minIntersectionDistance = std::numeric_limits<float>::max();

        for (size_t i = 0; i < app->boxes.size(); ++i) {
            if (app->rayBoxIntersection(cameraPosition, rayDirection, app->boxes[i])) {
                // Calculate the distance between the camera and the intersected box
                Vec3 diff = app->boxes[i].position - cameraPosition;
                float distance = diff.length();

                // Update the clicked box index if this box is closer to the camera
                if (distance < minIntersectionDistance) {
                    clickedBoxIndex = i;
                    minIntersectionDistance = distance;
                }
            }
        }

        // Remove the clicked box if any
        if (clickedBoxIndex != -1) {
            app->boxes.erase(app->boxes.begin() + clickedBoxIndex);
        }
    }
}

// called when the keyboard is used
void keyboard(unsigned char key, int x, int y) {
    const float impulseMagnitude = 20.0f; // Upward impulse magnitude

    if (key == ' ') { // Spacebar key
        for (Box& box : app->boxes) {
            box.velocity.y += impulseMagnitude;
        }
    }
}

// the main function.
int main(int argc, char** argv) {

    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Simple Physics Simulation");

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    app->initScene(NUMBER_OF_BOXES);

    //ForkExample();
    //PipeExample();
    //PThreadsExample();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    // it will stick here until the program ends.
    glutMainLoop();
    return 0;
}