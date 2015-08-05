#include "PolygonTriangulator.h"

PolygonTriangulator::PolygonTriangulator()
{
    
}

std::vector<Face> PolygonTriangulator::trianguateConvexPoints(std::vector<Eigen::Vector3d> points)
{
    std::vector<Face> faces;
    
    // compute centroid of points
    Eigen::Vector2d centroid = Eigen::Vector3d::Zero();
    for (size_t i = 0; i < points.size(); i++) {
        centroid += points[i];
    }
    centroid /= points.size();
    
    // sort points with respect to the centroid
    
    
    // create faces with the 1st point as the shared vertex
    for (size_t i = 1; i < points.size(); i++) {
        faces.push_back(Face(points[0], points[i], points[i+1]));
    }
    
    return faces;
}

std::vector<Face> PolygonTriangulator::triangulateEarClipping(std::vector<Eigen::Vector3d> points)
{
    std::vector<Face> faces;
    
    return faces;
}

std::vector<Face> PolygonTriangulator::triangulateMonotonePolygons(std::vector<Eigen::Vector3d> points)
{
    std::vector<Face> faces;
    
    return faces;
}