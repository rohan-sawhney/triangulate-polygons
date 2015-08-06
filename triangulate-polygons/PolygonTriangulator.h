#ifndef POLYGON_TRIANGULATOR_H
#define POLYGON_TRIANGULATOR_H

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>

struct Point
{
    Point() {}
    Point(const int index0, const double x0, const double y0, const double z0 = 0.0):
         index(index0), x(x0), y(y0), z(z0), isConvex(false), isEar(false) {}
    
    Point(const Point& p): index(p.index), x(p.x), y(p.y), z(p.z), isConvex(p.isConvex), isEar(p.isEar) {}
    
    Point& operator=(const Point& p) {
        index = p.index;
        x = p.x;
        y = p.y;
        z = p.z;
        isConvex = p.isConvex;
        isEar = p.isEar;
        
        return *this;
    }
    
    bool operator==(const Point& p) const {
        if (index == p.index) {
            return true;
        }
        
        return false;
    }
    
    Eigen::Vector3d operator-(const Point& p) const {
        
        return Eigen::Vector3d(x-p.x, y-p.y, z-p.z);
    }
    
    double x, y, z;
    bool isConvex;
    bool isEar;
    int index;
};

struct Face
{
    Face() {}
    Face(Point *p10, Point *p20, Point *p30): p1(p10), p2(p20), p3(p30) {}
    Face(const Face& f): p1(f.p1), p2(f.p2), p3(f.p3) {}
    ~Face() {}
    
    Face& operator=(const Face& f) {
        p1 = f.p1;
        p2 = f.p2;
        p3 = f.p3;
        
        return *this;
    }
    
    Point *p1;
    Point *p2;
    Point *p3;
};

class PolygonTriangulator {
public:
    // default constructor
    PolygonTriangulator();
    
    // triangulates points on a polygonal face using the Ear Clipping algorithm
    std::vector<Face*> triangulateEarClipping(std::vector<Point*>& points0);
    
    // triangulates points on a polygonal face by decomposing them into monotone polygons
    std::vector<Face*> triangulateMonotonePolygons(std::vector<Point*>& points0);
};

#endif
