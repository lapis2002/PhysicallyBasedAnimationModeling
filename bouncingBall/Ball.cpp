#include "Ball.h"

Ball::Ball(double x, double y, double z, double v_x, double v_y, double v_z, double rad, double m) {
  pos = Vector3d(x, y, z);
  velocity = Vector3d(v_x, v_y, v_z);
  radius = rad;
  mass = m;
}
