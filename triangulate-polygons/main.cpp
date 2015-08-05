#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

int gridX = 600;
int gridY = 600;
int gridZ = 600;

bool draw3dHull = false;


void printInstructions()
{
    std::cout << std::endl;
}

void init()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, gridX, 0, gridY, -gridZ, gridZ);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 27 :
            exit(0);
    }
    
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    printInstructions();
   
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(gridX, gridY);
    glutCreateWindow("Triangulating Polygons");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}
