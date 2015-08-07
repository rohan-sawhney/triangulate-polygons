#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "PolygonTriangulator.h"
#define MIN_DIST 2000

int gridX = 600;
int gridY = 600;
bool leftDrag = false;

PolygonTriangulator pt;
std::vector<Point*> points;
std::vector<Face*> faces;

void printInstructions()
{
    std::cout << "Drag mouse to draw polygon.\n"
              << "Hit spacebar to toggle between the ear clipping\n"
              << "method and monotone polygon algorithm.\n"
              << "Self intersecting polygons are invalid.\n"
              << "Polygon must be draw in CCW order." << std::endl;
}

void clearPoints()
{
    // clear points
    if (points.size() > 0) {
        for (int i = 0; i < points.size(); i++) {
            delete points[i];
        }
        points.clear();
    }
}

void clearFaces()
{
    // clear faces
    if (faces.size() > 0) {
        for (int i = 0; i < faces.size(); i++) {
            delete faces[i];
        }
        faces.clear();
    }
}

bool lineIntersection(const Point& p1s, const Point& p1e,
                      const Point& p2s, const Point& p2e)
{
    Eigen::Vector3d l1 = p1e - p1s;
    Eigen::Vector3d l2 = p2e - p2s;
    
    double u = (p1s.y*l2.x() + l2.y()*p2s.x - p2s.y*l2.x()  - l2.y()*p1s.x) /
               (l1.x()*l2.y() - l2.x()*l1.y());
    double v = (p1s.x + l1.x() * u - p2s.x) / l2.x();
    
    if (u > 0 && u <= 1 && v > 0 && v <= 1) {
        return true;
    }
    
    return false;
}

bool checkPolygonSelfIntersections()
{
    size_t size = points.size();
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if (i != j) {
                size_t nextI = i+1;
                if (nextI == size) nextI = 0;
                
                size_t nextJ = j+1;
                if (nextJ == size) nextJ = 0;
                
                if(lineIntersection(*points[i], *points[nextI], *points[j], *points[nextJ])) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, gridX, gridY, 0.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (faces.size() > 0) {
        glPointSize(4.0);
        glLineWidth(1.0);
        for (size_t i = 0; i < faces.size(); i++) {
            glColor4f(0.0f, 0.0f, 1.0f, 0.6f);
            glBegin(GL_TRIANGLES);
            glVertex2f(faces[i]->p1->x, faces[i]->p1->y);
            glVertex2f(faces[i]->p2->x, faces[i]->p2->y);
            glVertex2f(faces[i]->p3->x, faces[i]->p3->y);
            glEnd();
            
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_LINES);
            glVertex2f(faces[i]->p1->x, faces[i]->p1->y);
            glVertex2f(faces[i]->p2->x, faces[i]->p2->y);
            
            glVertex2f(faces[i]->p2->x, faces[i]->p2->y);
            glVertex2f(faces[i]->p3->x, faces[i]->p3->y);
            
            glVertex2f(faces[i]->p3->x, faces[i]->p3->y);
            glVertex2f(faces[i]->p1->x, faces[i]->p1->y);
            glEnd();
            
            glColor3f(1.0f, 1.0f, 0.0f);
            glBegin(GL_POINTS);
            glVertex2f(faces[i]->p1->x, faces[i]->p1->y);
            glVertex2f(faces[i]->p2->x, faces[i]->p2->y);
            glVertex2f(faces[i]->p3->x, faces[i]->p3->y);
            glEnd();
        }
        
    } else {
        glLineWidth(3.0);
        if (points.size() > 1) {
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_LINES);
            for (size_t i = 0; i < points.size()-1; i++) {
                glVertex2d(points[i]->x, points[i]->y);
                glVertex2d(points[i+1]->x, points[i+1]->y);
            }
            glEnd();
        }
    }
    
    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        clearPoints();
        clearFaces();
        leftDrag = true;
        
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (checkPolygonSelfIntersections()) {
            clearPoints();
        
        } else {
            faces = pt.triangulateEarClipping(points);
        }
        
        leftDrag = false;
    }
    
    glutPostRedisplay();
}

void motion(int x, int y)
{
    if (leftDrag) {
        int size = (int)points.size();
        Point *p = new Point(size, x, y);
        if (size == 0) {
            points.push_back(p);
            
        } else {
            double d = (*p - *points[size-1]).squaredNorm();
            if (d > MIN_DIST) {
                points.push_back(p);
            }
        }
        
    }
    
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 27 :
            clearPoints();
            clearFaces();
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
    glutCreateWindow("Polygon Triangulation - Ear Clipping");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}
