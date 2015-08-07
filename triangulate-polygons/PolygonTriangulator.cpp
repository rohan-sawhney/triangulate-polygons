#include "PolygonTriangulator.h"
#include <list>

PolygonTriangulator::PolygonTriangulator()
{
    
}

bool isConvex(Point *p1, Point *p2, Point *p3)
{
    // change to local coordinates
    Eigen::Vector3d a = *p2 - *p1;
    Eigen::Vector3d b = *p3 - *p1;
    b = a.cross(b);
    
    // a, b, c corresponds to the new axis
    a.normalize();
    b.normalize();
    Eigen::Vector3d c = a.cross(b);
    
    return a.x()*c.y() - a.y()*c.x() > 0;
}

bool triContainsPoint(Point *t1, Point *t2,
                      Point *t3, Point *p)
{
    // compute vectors
    Eigen::Vector3d v0 = *t2 - *t1;
    Eigen::Vector3d v1 = *t3 - *t1;
    Eigen::Vector3d v2 = *p - *t1;
    
    // compute dot products
    double dot00 = v0.dot(v0);
    double dot01 = v0.dot(v1);
    double dot02 = v0.dot(v2);
    double dot11 = v1.dot(v1);
    double dot12 = v1.dot(v2);
    
    // compute barycentric coordinates
    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    
    // check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

bool earIsValid(Point *curr, Point *prev, Point *next,
                const std::list<Point*>& concavePoints)
{
    if (prev->isEar || next->isEar) {
        return false;
    }
        
    for (std::list<Point*>::const_iterator it = concavePoints.begin(); it != concavePoints.end(); it++) {
        if (*it != prev && *it != next) {
            if (triContainsPoint(prev, curr, next, *it)) {
                return false;
            }
        }
    }
    
    return true;
}

void initializeLists(std::list<Point*>& concavePoints, std::list<Point*>& ears, std::vector<Point*>& points)
{
    // label convex and concave points
    std::vector<Point*> convexPoints;
    int size = (int)points.size();
    for (int i = 0; i < size; i++) {
        int prev = i-1;
        if (prev < 0) prev = size-1;
        
        int next = i+1;
        if (next == size) next = 0;
        
        if (isConvex(points[prev], points[i], points[next])) {
            points[i]->isConvex = true;
            convexPoints.push_back(points[i]);
            
        } else {
            points[i]->isConvex = false;
            concavePoints.push_back(points[i]);
        }
    }
    
    // mark ears
    for (int i = 0; i < (int)convexPoints.size(); i++) {
        int curr = convexPoints[i]->index;
        
        int prev = curr-1;
        if (prev < 0) prev = size-1;
        
        int next = curr+1;
        if (next == size) next = 0;
        
        if (earIsValid(points[curr], points[prev], points[next], concavePoints)) {
            ears.push_front(points[curr]);
            points[curr]->isEar = true;
        }
    }
}

void updateAdjacentPoint(Point *curr, Point *prev, Point *next,
                         std::list<Point*>& concavePoints, std::list<Point*>& ears)
{
    if (curr->isEar) {
        // if point is a ear, it does not necessarily remain a ear
        // check if there is a concave vertex inside the ear triangle
        if (!earIsValid(curr, prev, next, concavePoints)) {
            curr->isEar = false;
            ears.remove(curr);
        }
        
    } else if (curr->isConvex) {
        // convex point can be now a ear
        if (earIsValid(curr, prev, next, concavePoints)) {
            curr->isEar = true;
            ears.push_front(curr);
        }
        
    } else {
        // concave point might become a convex point (and a ear)
        if (isConvex(prev, curr, next)) {
            // remove from concave list
            concavePoints.remove(curr);
            
            curr->isConvex = true;
            
            // check if convex point is a ear
            if (earIsValid(curr, prev, next, concavePoints)) {
                curr->isEar = true;
                ears.push_front(curr);
            }
        }
    }
}

std::vector<Face*> PolygonTriangulator::triangulateEarClipping(std::vector<Point*>& points0)
{
    std::vector<Face*> faces;
    std::vector<Point*>& points = points0;
    
    int size = (int)points.size();
    if (size < 4) {
        if (size == 3) {
            faces.push_back(new Face(points[0], points[1], points[2]));
        }
        return faces;
    }
    
    std::list<Point*> concavePoints;
    std::list<Point*> ears;
    initializeLists(concavePoints, ears, points);

    // Clip ears
    while (ears.size() != 0) {
        
        // remove current ear
        Point *p = ears.front();
        p->isEar = false;
        ears.remove(p);
        
        // add face
        int curr = p->index;
        
        int prev = curr-1;
        if (prev < 0) prev = size-1;
        
        int next = curr+1;
        if (next == size) next = 0;
        
        if (points.size() > 2) {
            faces.push_back(new Face(points[prev], p, points[next]));
        }
        
        // update previous point
        curr = prev;
            
        int prevPrev = prev-1;
        if (prevPrev < 0) prevPrev = size-1;
            
        updateAdjacentPoint(points[curr], points[prevPrev], points[next], concavePoints, ears);
            
        // update next point
        curr = next;
            
        int nextNext = next+1;
        if (nextNext == size) nextNext = 0;
            
        updateAdjacentPoint(points[curr], points[prev], points[nextNext], concavePoints, ears);
        
        // update point list
        points.erase(points.begin() + p->index);
        size = (int)points.size();
        for (int i = 0; i < size; i++) {
            points[i]->index = i;
        
        }
    }
    
    return faces;
}
