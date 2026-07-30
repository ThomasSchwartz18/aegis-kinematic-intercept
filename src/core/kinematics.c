#include "kinematics.h"
#include "math.h"             // Standard C library so we can use the sqrt() function

/*

3D Vector Math Utility Functions

3D VECTOR UTILITIES REAL-WORLD CHEAT SHEET
============================================================================
Function     | v1 Parameter        | v2 / Scalar Parameter | Physical Output / Meaning
-------------+---------------------+-----------------------+------------------------------------------
vec3_sub     | Target Position     | Gun/Launcher Position | Vector line pointing from Gun -> Target
vec3_add     | Current Position    | Movement (Vel * Time) | Predicted Future Position in 3D Space
vec3_scale   | Velocity Vector m/s | Time Duration (sec)   | Total Distance Traveled Vector (meters)
vec3_mag     | Any 3D Vector       | N/A                   | Straight-line Distance / Length (meters)
============================================================================

*/

Vector3D vec3_add(Vector3D v1, Vector3D v2) {
    Vector3D result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

Vector3D vec3_sub(Vector3D v1, Vector3D v2) {
    Vector3D result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

Vector3D vec3_scale(Vector3D v, double scalar) {

    Vector3D result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

double vec3_magnitude(Vector3D v) {
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

/*

Step 1-
To calculate the collision point of a projectile and a moving target,
I need to first measure how far away the target is RIGHT NOW and divide
that distance by the speed of my bullet.

Time-to-Intercept(tti) = Distance to Target / Bullet Speed

Step 2-
If the target takes 2.0 seconds to reach, it won't be at its current 
position - it will be where its velocity carries it over that 2.0 
seconds.

I run a loop 3 times (running 3 times narrows the mathematical error
down to millimeters without causing too much CPU work) 
to lock down the exact time:
1- Predict target's new position at time tti:
    Future Position = Current Position + (Velocity of target * tti)
2- Measure distance to that new future position.
3- Recalculate tti based on that new distance.

Step 3-
Once I know the 3D intercept coordinate (x, y, z), human gun operators
(and mechanical turrets) don't use X, Y, Z. They use Azimuth and 
Elevation angles. So I convert the 3D intercept

▲ Z (Elevation / Up-Down)
       │
       │     / Target
       │    /
       │   /  Angle = Elevation (asin)
       │  /
       └─── ── ── ── ──► X (East)
      /  \
     /    \ Angle = Azimuth (atan2)
    /      ▼
   ▼ Y (North)

Azimuth = Horizontal compass rotation (0 degrees = North,
90 degrees = East, 180 degrees = South, 270 degrees = West)
calculated using atan(x, y) from C's math.h library.

Elevation = Vertical barrel tilt above the horizon (-90 degrees 
to +90 degrees) calculated using asin(z / total_distance).

*/

/* 

VOCABULARY:
Lead Angle - Aiming ahead of a moving target so the projectile and
    target collide.
Convergence Loop - A loop that runs a calculation multiple times,
    getting closer to the perfect answer with each pass.
Azimuth - Compass angle in degrees telling the turret which way to 
    turn left/right.
Elevation - Angle in degrees telling the gun barrel how far to tilt
    up/down.
Radians vs. Degrees - Standard C math functions (atan2, asin) retun 
    angles in radians. We must multiply by (180 / M_PI) to convert 
    to degrees for humans to read.

*/

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*

Fire Control Intercept Solver

*/

