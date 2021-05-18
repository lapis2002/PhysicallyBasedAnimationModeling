// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

#include<glm/glm.hpp>
#include<string>

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN glm::vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN glm::vec3 up;  // The (regularly updated) vector coordinates of the up 

#ifdef MAINPROGRAM 
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point 
int amountinit = 5;
int w = 500, h = 500 ; // width and height 
 
#endif 

EXTERN double box_width, box_height, box_depth;
EXTERN double ball_x, ball_y, ball_z, ball_vx, ball_vy, ball_vz, ball_radius, ball_mass;
EXTERN double coeff_restitution, coeff_friction, coeff_air_resistance;
EXTERN int substeps;
EXTERN double timestep;
EXTERN int Time_Steps_Per_Display, Timer_Delay;