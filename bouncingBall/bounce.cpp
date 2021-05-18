//
// Author: Austin Brennan
// Date: September 5th, 2016
//
#include "Camera.h"
#include "Scene.h"
#include <cstdio>

// For file reading
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

int WIDTH = 1280;
int HEIGHT = 720;

int persp_win;

Camera *camera;
Scene *scene;

bool showGrid = false;

// draws a simple grid
void makeGrid() {
  glColor3f(0.0f, 0.0f, 0.0f);

  glLineWidth(1.0f);

  for (double i=-12; i<12; i++) {
    for (double j=-12; j<12; j++) {
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i, 0, j+1);
      glEnd();
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i+1, 0, j);
      glEnd();

      if (j == 11){
	glBegin(GL_LINES);
	glVertex3f(i, 0, j+1);
	glVertex3f(i+1, 0, j+1);
	glEnd();
      }
      if (i == 11){
	glBegin(GL_LINES);
	glVertex3f(i+1, 0, j);
	glVertex3f(i+1, 0, j+1);
	glEnd();
      }
    }
  }

  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex3f(-12, 0, 0);
  glVertex3f(12, 0, 0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0, 0, -12);
  glVertex3f(0, 0, 12);
  glEnd();
  glLineWidth(1.0f);
}

// TODO Base this function off of the azim and elev of camera.
void getVisibleFaces(bool *visible_faces, std::vector<Vector3d> face_positions) {
  // set max distance to camera to the distance from the front face to the camera
  double test_distance_to_camera;
  double min_distance_to_camera = (face_positions[0] - camera->Pos).norm();
  int remove_face = 0;

  for (int i=1; i < 6; ++i) {
    test_distance_to_camera = (face_positions[i] - camera->Pos).norm();
    if (test_distance_to_camera < min_distance_to_camera) {
      min_distance_to_camera = test_distance_to_camera;
      remove_face = i;
    }
  }

  *(visible_faces + remove_face) = false;
}

void makeBox() {
  bool visible_faces[6] = {true, true, true, true, true, true};
  getVisibleFaces(visible_faces, scene->box->wall_locations);
  float w = (float) scene->box->width / 2.0f;
  float h = (float) scene->box->height / 2.0f;
  float d = (float) scene->box->depth / 2.0f;

  if (visible_faces[0]) {
    // back face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(w, -1.0f * h, -1.0f * d);
    glVertex3f(w, h, -1.0f * d);
    glVertex3f(-1.0f * w, h, -1.0f * d);
    glVertex3f(-1.0f * w, -1.0f * h, -1.0f * d);
    glEnd();
  }

  if (visible_faces[1]) {
    // front face
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(w, -1.0f * h, d);
    glVertex3f(w, h, d);
    glVertex3f(-1.0f * w, h, d);
    glVertex3f(-1.0f * w, -1.0f * h, d);
    glEnd();
  }

  if (visible_faces[2]) {
    // right face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(w, -1.0f * h, -1.0f * d);
    glVertex3f(w, h, -1.0f * d);
    glVertex3f(w, h, d);
    glVertex3f(w, -1.0f * h, d);
    glEnd();
  }

  if (visible_faces[3]) {
    // left face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f * w, -1.0f * h, d);
    glVertex3f(-1.0f * w, h, d);
    glVertex3f(-1.0f * w, h, -1.0f * d);
    glVertex3f(-1.0f * w, -1.0f * h, -1.0f * d);
    glEnd();
  }

  if (visible_faces[4]) {
    // top face
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(w, h, d);
    glVertex3f(w, h, -1.0f * d);
    glVertex3f(-1.0f * w, h, -1.0f * d);
    glVertex3f(-1.0f * w, h, d);
    glEnd();
  }

  if (visible_faces[5]) {
    // bottom face
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(w, -1.0f * h, -1.0f * d);
    glVertex3f(w, -1.0f * h, d);
    glVertex3f(-1.0f * w, -1.0f * h, d);
    glVertex3f(-1.0f * w, -1.0f * h, -1.0f * d);
    glEnd();
  }
}

void initCamera() {
  // set up camera
  // parameters are eye point, aim point, up vector
  camera = new Camera(Vector3d(0, 5, 5), Vector3d(0, 0, 0),
		      Vector3d(0, 1, 0));

  // grey background for window
  glClearColor(0.62, 0.62, 0.62, 0.0f);
  glShadeModel(GL_SMOOTH);
  glDepthRange(0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Helper function to read the input data values
// Use is optional, but should be very helpful in parsing.
bool readvals(stringstream &s, const int numvals, double *values) {
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void initSimulation(char *param_filename) {
    double box_width, box_height, box_depth,
          ball_x, ball_y, ball_z, 
          ball_vx, ball_vy, ball_vz,
          ball_radius, ball_mass,
          timestep, coeff_restitution, coeff_friction,
          coeff_air_resistance;
    Vector3d acceleration;
    int substeps;

    string str, cmd;
    ifstream in;
    in.open(param_filename);
    if (in.is_open()) {
        getline(in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines
                stringstream s(str);
                s >> cmd;
                double values[20]; 
                bool validinput; // Validity of input

                if (cmd == "Box") {
                    validinput = readvals(s, 3, values); // Position/color for lts.
                    if (validinput) {
                        printf("read box\n");
                        box_width = values[0];
                        box_height = values[1];
                        box_depth = values[2];
                    }
                }

                else if (cmd == "Ball") {
                    validinput = readvals(s, 8, values);
                    if (validinput) {
                        printf("read ball\n");
                        ball_x = values[0];
                        ball_y = values[1];
                        ball_z = values[2];
                        ball_vx = values[3];
                        ball_vy = values[4];
                        ball_vz = values[5];
                        ball_radius = values[6];
                        ball_mass = values[7];
                    }
                }
                else if (cmd == "Acceleration") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        printf("read a\n");
                        acceleration.x = values[0];
                        acceleration.y = values[1];
                        acceleration.z = values[2];
                    }
                }
                else if (cmd == "TimeStep") {
                    validinput = readvals(s, 2, values); 
                    if (validinput) {
                        printf("read time\n");

                        timestep = values[0];
                        substeps = (int) values[1];
                    }
                }
                else if (cmd == "CoeffRes") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        printf("read coeff res\n");
                        coeff_restitution = values[0];
                    }
                }
                else if (cmd == "CoeffFrict") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        coeff_friction = values[0];
                    }
                }
                else if (cmd == "CoeffWind") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        coeff_air_resistance = values[0];
                    }
                }
            }
            getline(in, str);
        }
    }
    else {
        cerr << "Unable to Open Input Data File " << param_filename << "\n";
        throw 2;
    }
  printf("finish reading file");
  scene = new Scene(box_width, box_height, box_depth,
                    ball_x, ball_y, ball_z,
                    ball_vx, ball_vy, ball_vz,
                    ball_radius, ball_mass, substeps, timestep,
                    coeff_restitution, coeff_friction, coeff_air_resistance);

  printf("check scene\n");
  printf("box %2f %2f %2f\n", scene->box->width, scene->box->height, scene->box->depth);
  printf("ball pos %2f %2f %2f v %2f %2f %2f rad %2f mass %2f\n", scene->ball->pos.x, scene->ball->pos.y, scene->ball->pos.z, scene->ball->velocity.x, scene->ball->velocity.y, scene->ball->velocity.z, scene->ball->radius, scene->ball->mass);
  printf("time %2f %d\n", scene->h, scene->substeps);
  printf("coeff res %2f frict %2f air %2f\n", scene->coeff_res, scene->coeff_frict, scene->coeff_air);
}

void simulateBall() {
  // printf("%2f, %2d\n",scene->h, scene->substeps);
  for(int i = 0; i < scene->substeps; ++i) {
      // printf("simulate\n");

    scene->updateSimulation();
  }
  glutPostRedisplay();
}


void perspDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw the camera created in perspective
  camera->PerspectiveDisplay(WIDTH, HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (showGrid)
    makeGrid();

  makeBox();

  glColor3f(1,0,0.5);
  GLUquadric *quad;
  quad = gluNewQuadric();
  glTranslatef(scene->ball->pos.x,scene->ball->pos.y,scene->ball->pos.z);
  gluSphere(quad,scene->ball->radius,100,20);

  glFlush();
  glutSwapBuffers();
}

void mouseEventHandler(int button, int state, int x, int y) {
  // let the camera handle some specific mouse events (similar to maya)
  camera->HandleMouseEvent(button, state, x, y);
  glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
  // let the camera handle some mouse motions if the camera is to be moved
  camera->HandleMouseMotion(x, y);
  glutPostRedisplay();
}

void keyboardEventHandler(unsigned char key, int x, int y) {
  switch (key) {
  case 'r': case 'R':
    // reset the camera to its initial position
    camera->Reset();
    break;

  case 'f': case 'F':
    camera->SetCenterOfFocus(Vector3d(0, 0, 0));
    break;

  case 'g': case 'G':
    showGrid = !showGrid;
    break;

  case 'w':
    scene->gravity = {0.0f, 9.8f, 0.0f};
    cout << "Gravity is now up" << endl;
    break;

  case 'a':
    scene->gravity = {-9.8f, 0.0f, 0.0f};
    cout << "Gravity is now left" << endl;
    break;

  case 's':
    scene->gravity = {0.0f, -9.8f, 0.0f};
    cout << "Gravity is now down" << endl;
    break;

  case 'd':
    scene->gravity = {9.8f, 0.0f, 0.0f};
    cout << "Gravity is now right" << endl;
    break;

  case 'z':
    scene->gravity = {0.0f, 0.0f, 9.8f};
    cout << "Gravity is now forward" << endl;
    break;

  case 'x':
    scene->gravity = {0.0f, 0.0f, -9.8f};
    cout << "Gravity is now back" << endl;
    break;

  case 'q': case 'Q':	// q or esc - quit
  case 27:		// esc
    exit(0);
  }

  glutPostRedisplay();
}

int main(int argc, char *argv[]) {

  if(argc != 2){
    fprintf(stderr, "usage: BouncingBall paramfile\n");
    exit(1);
  }

  // set up opengl window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(50, 50);
  persp_win = glutCreateWindow("Ball Bouncing in 6 Sided Box");

  // initialize the camera and such
  initCamera();

  initSimulation(argv[1]);

  // set up opengl callback functions
  glutDisplayFunc(perspDisplay);
  glutIdleFunc(simulateBall);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(keyboardEventHandler);


  glutMainLoop();
  return(0);
}

