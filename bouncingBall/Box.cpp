#include "Box.h"
#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
Box::Box(double width, double height, double depth) {
    this->width = width;
    this->height = height;
    this->depth = depth;
    double w = width / 2;
    double h = height / 2;
    double d = depth / 2;


    wall_locations = {Vector3d( 0.0f, 0.0f, -1.0 * d),    // back face
                    Vector3d( 0.0f, 0.0f, d),           // front face
                    Vector3d( w, 0.0f, 0.0f),           // right face
                    Vector3d(-1.0f * w, 0.0f, 0.0f),    // left face
                    Vector3d( 0.0f, h, 0.0f),          // top face
                    Vector3d( 0.0f, -1.0f * h, 0.0f)}; // bottom face

    wall_normals = {Vector3d( 0.0f,  0.0f,  1.0f),  // back face
                    Vector3d( 0.0f,  0.0f, -1.0f),  // front face
                    Vector3d(-1.0f,  0.0f,  0.0f),  // right face
                    Vector3d( 1.0f,  0.0f,  0.0f),  // left face
                    Vector3d( 0.0f, -1.0f,  0.0f),  // top face
                    Vector3d( 0.0f,  1.0f,  0.0f)}; // bottom face
    
}