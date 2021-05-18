#ifndef BOUNCING_BALL_SCENE_H
#define BOUNCING_BALL_SCENE_H
#include "Ball.h"
#include "Box.h"

class Scene {
  private:
  int detectCollision(Vector3d ball_pos, Vector3d new_ball_pos,  Vector3d ball_velocity, double *f);
  bool checkCollision ();
  void updateState(double f, Vector3d normal, Vector3d *velocity_new, Vector3d * pos_new);
  public:
    Box *box;
    Ball *ball;
    double h; // timestep
    int substeps;
    Vector3d gravity = Vector3d(0.0f, -9.8f, 0.0f);
    Vector3d acceleration = Vector3d(0.0f, 0.0f, 0.0f);
    double coeff_res; // coefficient of restitution
    double coeff_frict; // coefficient of friction
    double coeff_air; // air resistance

    // constructors
    Scene(double box_width, double box_height, double box_depth,
          double x, double y, double z,
          double v_x, double v_y, double v_z,
          double radius, double mass,
          int substeps, double time_step,
          double coeff_res, double coeff_frict, double coeff_air);

    // member functions
    // int detectCollision(Vector3d ball_pos, Vector3d new_ball_pos, double *s);
    void updateSimulation();
};


#endif //BOUNCING_BALL_SCENE_H
