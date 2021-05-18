//
// Created by awbrenn on 8/31/16.
//

#ifndef BOUNCING_BALL_BALL_H
#define BOUNCING_BALL_BALL_H

#include "Matrix.h"

class Ball {
  public:
    Ball(double x, double y, double z, double v_x, double v_y, double v_z, double radius, double mass);

    Vector3d pos;
    Vector3d velocity;

    double radius;
    double mass;
    double viscosity;
};


#endif //BOUNCING_BALL_BALL_H
