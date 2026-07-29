/*

If types.h defines my nouns (Vector3D, TargetThreat, and InterceptSolution), then
kinematics.h defines our verbs - the math functions we can perform on those nouns.

Two groups for this file:
1- 3D Vector Operations
    - Vector Addition
    - Vector Subtraction
    - Vector Scaling
    - Vector Magnitude/Distance

2- The Intercept Solver
    - solve_intercept: Takes a TargetThreat and the firing plarform's position,
      projectile speed, and gravity into account, returning an InterceptSolution

*/

#ifndef KINEMATICS_H
#define KINEMATICS_H
#include "types.h"             // used to inherit types.h structures (Vector3D, TargetThreat, InterceptSolution)

/*

Group 1: The Vector Math Utilities
In physics and 3D graphics, I cannot just do v1 + v2 with custom structs like I can in Python. 
I have to declare explicit C functions to do vector math for me.

*/
/*

VECTOR ADDITION (vec3_add)

What this is doing:
    - Adds two 3D vectors together: V = (x1 + x2, y1 + y2, z1 + z2)
    - Why it is needed: I need to add two 3D vectors together to 
      calculate the next position of a moving target, or to calculate 
      the next position of a projectile in flight.

*/
Vector3D vec3_add(Vector3D v1, Vector3D v2);

/*

VECTOR SUBTRACTION (vec3_sub)

What this is doing:
    - Subtracts two 3D vectors: V = (x1 - x2, y1 - y2, z1 - z2)
    - Why it is needed: To find the relative vector from our gun to the 
      target. If target is at (1000, 500, 200) and out gun is at (0, 0, 0),
      vec3_sub gives us the direction and distance line pointing straight at
      the target.

*/
Vector3D vec3_sub(Vector3D v1, Vector3D v2);

/*

VECTOR SCALING (vec3_scale)

What is this doing:
    - Multiplies all three coordinates of a vector by a single number (a scaler): 
      V = (x * s, y * s, z * s)
    - Why it is needed: Time scaling. If a target travels at a velocity of V (m/s)
      where will it be in 2.5 seconds? We scale the vector: vec3_scale(V, 2.5).

*/
Vector3D vec3_scale(Vector3D v, double scaler);

/*

VECTOR MAGNITUDE / DISTANCE (vec3_magnitude)

What is this doing:
    - Calculates the total straight-line length of a 3D vector using the 3D Pythagorean Theorem: 
      |V| = sqrt(x^2 + y^2 + z^2)
    - Why it is needed: I need to measure the physical distance in meters between our gun and the
      target or intercept point.

*/
double vec3_magnitude(Vector3D v);

/*

Group 2: The Fire-Control Intercept Solver
This is the main function that takes a TargetThreat and calculates the firing 
solution for our gun or missile launcher.

*/
/*

The centerpiece function of my whole C engine.

What this is doing:
    - What goes in (Parameters):
        - TargetThreat target: The enemy missile card (its position, speed, direction, ID)
        - Vector3D origin_pos: Where our defensive fun turret is located in 3D space.
        - double projectile_speed: How fast our defensive round fly through the air (m/s)
    - What comes out (Return Value):
        - InterceptSolution: Returns a fully filled-out firing order slip containing the aim 
          point, azimuth/elevaion angles, time-to-impact, and whether the solution is valid.
    - Why it is needed: This is the main function that calculates the firing solution for 
      our gun or missile launcher.

*/
InterceptSolution solve_intercept(TargetThreat target, Vector3D origin_pos, double projectile_speed);

/*

Every line ends with a semicolon and not curly brackets. This is because this is a header file and not
the C engine files.

This file simply tells C: "This function exists, and it takes these parameters and returns this type 
of data." The actual math is in the C engine files.

*/

#endif // KINEMATICS_H