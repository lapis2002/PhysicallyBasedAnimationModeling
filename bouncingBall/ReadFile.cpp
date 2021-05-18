// Basic includes to get this file to work.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;
#include "Variables.h"
#include "ReadFile.h"

double box_width, box_height, box_depth;
double ball_x, ball_y, ball_z, ball_radius, ball_mass;
double time_step;
double coeff_restitution, coeff_friction, coeff_air_resistance;
int sub_steps;
int Time_Steps_Per_Display, Timer_Delay;

// Function to read the input data values
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

void readfile(const char *filename) {
    string str, cmd;
    ifstream in;
    in.open(filename);
    if (in.is_open()) {
        getline(in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines
                stringstream s(str);
                s >> cmd;
                int i;
                double values[20]; 
                bool validinput; // Validity of input

                if (cmd == "Box") {
                    validinput = readvals(s, 3, values); // Position/color for lts.
                    if (validinput) {
                        box_width = values[0];
                        box_height = values[1];
                        box_depth = values[2];
                    }
                }

                else if (cmd == "Ball") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        ball_x = values[0];
                        ball_y = values[1];
                        ball_z = values[2];
                        ball_radius = values[3];
                        ball_mass = values[4];
                    }
                }
                else if (cmd == "TimeStep") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        time_step = values[0];
                        sub_steps = (int) values[0];
                        Time_Steps_Per_Display = max(1, int(sub_steps / time_step + 0.5));
                        Timer_Delay = int(0.5 * time_step * 1000);
                    }
                }
                else if (cmd == "CoeffRes") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
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
        cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
}

int main () {
    return 0;
}