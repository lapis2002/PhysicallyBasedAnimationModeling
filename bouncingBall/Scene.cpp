#include <cfloat>
#include "Scene.h"
#include <cstdio>
const double epsilon = 0.00000001f; // used to avoid division by zero

Scene::Scene(double box_width, double box_height, double box_depth,
             double x, double y, double z,
             double v_x, double v_y, double v_z,
             double radius, double mass,
             int substeps, double time_step,
             double coeff_res, double coeff_frict, double coeff_air) {
  this->box = new Box(box_width, box_height, box_depth);
  this->ball = new Ball(x, y, z, v_x, v_y, v_z, radius, mass);
  this->h = time_step;
  this->substeps = substeps;
  this->coeff_res = coeff_res;
  this->coeff_frict = coeff_frict;
  this->coeff_air = coeff_air;
  this->acceleration = this->gravity;
}


int Scene::detectCollision(Vector3d ball_pos, Vector3d new_ball_pos, Vector3d ball_velocity, double *f) {
    int collision_wall = -1; // -1 indicates no collision
    Vector3d wall_location;
    Vector3d wall_normal;
    Vector3d point_on_ball;
    Vector3d point_on_new_ball;
    double distance_to_wall, total_distance;
    double min_f = 2.0f; // set min_s to a number arbitrarilly greater than 1.0
    double temp_f;

    for (int i=0; i < 6; ++i) {
        wall_location = box->wall_locations[i];
        wall_normal = box->wall_normals[i];

        // get the point on ball surface closest to the plane
        point_on_ball = -1.0f * wall_normal * ball->radius + ball_pos;
        point_on_new_ball = -1.0f * wall_normal * ball->radius + new_ball_pos;

        distance_to_wall = (point_on_ball - wall_location) * wall_normal;
        total_distance = (point_on_ball - point_on_new_ball) * wall_normal;

        if (abs(total_distance) < epsilon) // avoid division by zero
            continue;

        // find the first collision to occur
        temp_f = distance_to_wall / total_distance;
        if (temp_f >= 0.0f && temp_f < 1.0f && temp_f < min_f && wall_normal * ball_velocity < 0) {
            // printf("distance to wall %2f, update f %2f\n", distance_to_wall, temp_f);
            *f = temp_f;
            collision_wall = i;
            min_f = temp_f;
        }
  }

  return collision_wall;
}

// TODO add multiple collision reaction.
void Scene::updateSimulation() {
    // bool resting;
    Vector3d pos_new, velocity_new;
    int collision_wall;
    double f = 1; // fraction between distance of wall to ball
            // and total distance travelled in normal direction
    
    double timestep_remain = h;
    double timestep = timestep_remain;
    // printf("test timestep %2f, timestep_remain %2f\n", timestep, timestep_remain);
    int t = 2;
    while (t--) {
        // printf("%2f %2f\n", timestep_remain, timestep);
        acceleration = gravity - ((coeff_air/ball->mass) * ball->velocity);
        velocity_new = ball->velocity + timestep_remain * acceleration;
        pos_new = ball->pos + timestep_remain *  ball->velocity;
        // printf("timestep remain %2f\n", timestep_remain);
        // printf("new v %2f, %2f, %2f\n", velocity_new.x, velocity_new.y, velocity_new.z);

        // printf("a %2f, %2f, %2f\n", acceleration.x, acceleration.y, acceleration.z);
        // printf("old pos %2f, %2f, %2f\n", ball->pos.x,  ball->pos.y, ball->pos.z);
        // printf("new pos %2f, %2f, %2f\n", pos_new.x, pos_new.y, pos_new.z);
        collision_wall = detectCollision(ball->pos, pos_new, ball->velocity, &f);
        if (collision_wall != -1 ) { // collision occurred!
            // printf("%2f %2f\n", timestep_remain, timestep);
            // printf("collide\n");
            updateState(f, box->wall_normals[collision_wall], &velocity_new, &pos_new);
            timestep *= f;            
            // printf("%2f %2f\n", timestep_remain, timestep);
            // if (timestep > epsilon) printf("true");
            // printf("a %2f, %2f, %2f \n", acceleration.x, acceleration.y, acceleration.z);
            // printf("collision %d v=(%2f, %2f, %2f) at (%2f, %2f, %2f)\n", collision_wall, ball->velocity.x, ball->velocity.y, ball->velocity.z, ball->pos.x, ball->pos.y, ball->pos.z);
        }
        ball->pos = pos_new;
        ball->velocity = velocity_new;
        timestep_remain -= timestep;
    }
        // // printf("%2f %2f\n", timestep_remain, timestep);
        // acceleration = gravity - ((coeff_air/ball->mass) * ball->velocity);
        // velocity_new = ball->velocity + timestep_remain * acceleration;
        // pos_new = ball->pos + timestep_remain *  ball->velocity;

        // collision_wall = detectCollision(ball->pos, pos_new, &f);
        // if (collision_wall != -1) { // collision occurred!
        //     // printf("%2f %2f\n", timestep_remain, timestep);

        //     updateState(f, box->wall_normals[collision_wall], &velocity_new, &pos_new);
        //     timestep *= f;            
        //     printf("%2f %2f\n", timestep_remain, timestep);
        //     if (timestep > epsilon) printf("true");
        //     // printf("a %2f, %2f, %2f \n", acceleration.x, acceleration.y, acceleration.z);
        //     printf("collision %d v=(%2f, %2f, %2f) at (%2f, %2f, %2f)\n", collision_wall, ball->velocity.x, ball->velocity.y, ball->velocity.z, ball->pos.x, ball->pos.y, ball->pos.z);
        //     acceleration = gravity - ((coeff_air/ball->mass) * ball->velocity);
        //     Vector3d tmp_v = Vector3d(ball->velocity.x, ball->velocity.y, ball->velocity.z);
        //     ball->velocity = ball->velocity + (1 - f) * h * acceleration;
        //     ball->pos = ball->pos + (1.0f - f) * h * tmp_v;
        // }
        
        // ball->pos = pos_new;
        // ball->velocity = velocity_new;
        
        // timestep_remain -= timestep;
}

void Scene::updateState (double f, Vector3d normal, Vector3d *velocity_new, Vector3d * pos_new) {
    Vector3d velocity_collision, pos_collision,
             velocity_collision_normal, velocity_collision_tangent,
             velocity_new_normal, velocity_new_tangent;

    velocity_collision = ball->velocity + f * h * acceleration;
    pos_collision = ball->pos + f * h * ball->velocity;

    velocity_collision_normal = (velocity_collision * normal) * normal;
    velocity_collision_tangent = velocity_collision - velocity_collision_normal;

    velocity_new_normal = -1.0f * coeff_res * velocity_collision_normal;
    velocity_new_tangent = (1.0f - coeff_frict) * velocity_new_tangent;

    * velocity_new = velocity_new_normal + velocity_new_tangent;
    *pos_new = pos_collision;

    acceleration = gravity - ((coeff_air/ball->mass) * *velocity_new);
}
