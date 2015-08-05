#ifndef POLYGON_TRIANGULATOR_H
#define POLYGON_TRIANGULATOR_H

#include <stdlib.h>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>

struct Face
{
    Face() {}
    Face(Eigen::Vector3d p10, Eigen::Vector3d p20, Eigen::Vector3d p30): p1(p10), p2(p20), p3(p30) {}
    Face(const Face& f): p1(f.p1), p2(f.p2), p3(f.p3) {}
    ~Face() {}
    
    Face& operator=(const Face& f) {
        p1 = f.p1;
        p2 = f.p2;
        p3 = f.p3;
        
        return *this;
    }
    
    Eigen::Vector3d p1, p2, p3;
};

class PolygonTriangulator {
public:
    // default constructor
    PolygonTriangulator();
    
    // triangulates points on a convex polygonal face
    std::vector<Face> trianguateConvexPoints(std::vector<Eigen::Vector3d> points);
    
    // triangulates points on a polygonal face using the Ear Clipping algorithm
    std::vector<Face> triangulateEarClipping(std::vector<Eigen::Vector3d> points);
    
    // triangulates points on a polygonal face by decomposing them into monotone polygons
    std::vector<Face> triangulateMonotonePolygons(std::vector<Eigen::Vector3d> points);
};

#endif
