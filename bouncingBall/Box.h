#ifndef BOUNCING_BALL_BOX_H
#define BOUNCING_BALL_BOX_H

#include <vector>
#include "Matrix.h"
#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
class Box {
  public:
    double width; // width
    double height; // height
    double depth; // depth
    std::vector<Vector3d> wall_locations;
    std::vector<Vector3d> wall_normals;

    Box(double width, double height, double depth);
};


#endif //BOUNCING_BALL_BOX_H
