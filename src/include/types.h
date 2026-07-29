/*

This file is a list of what data structures are used in the project. 
It is included in all source files that need to use these data structures. 
The purpose of this file is to provide a central location for defining and 
declaring the data structures used in the project, making it easier to maintain 
and update them as needed.

*/ 

#ifndef TYPES_H  // stands for "If not defined TYPES_H" - the preprocessor checks its memory to see if TYPES_H has been defined. If it hasn't, the code between #ifndef and #endif will be included in the compilation process.
#define TYPES_H  // defines TYPES_H to prevent multiple inclusions of this header file in the same compilation unit.

/*

Syntax breakdown:

'struct {}' - Defines a structure, which is a user-defined data type that groups related variables together. The variables inside the structure are called members.
'typedef' - Stands for "type definition" which creates an alias. It tells C "Assign a custom name (an alias) to this data type so I can use it as a first-class type name."
'Vector3D' - The name of the structure being defined. This is the alias that will be used to refer to this structure type in the code.

by combining 'typedef' and 'struct', we can create a new type name (Vector3D) that can be used to declare variables of this structure type without needing to use the 'struct' keyword every time.

*/

/*

1. 3D Cartesian Vector
Used for position (meters), velocity (m/s), and acceleration (m/s^2) in 3D space.

*/
typedef struct {
    double x; // East-West position (meters)
    double y; // North-South position (meters)
    double z; // Up-Down / Altitude-Elevation position (meters)
} Vector3D;

/*

2. Target Threat Contact
Represents an active airborne threat detected by radar.

This is an important concept = Struct Nesting and Memory Layout.
When I place "Vector3D position;" inside TargetThreat, I am telling the compiler to
embed one struct directly inside another.

Here is what is happening in memory:
- When I instantiate a TargetThreat object (like TargetThreat missile;), the C compiler
allocates one single, contiguous block of RAM on the CPU stack.
Because Vector3D is defined as three double fields (x, y, z), and each double takes 8 bytes of memory:
Size of Vector3D = 8 bytes (x) + 8 bytes (y) + 8 bytes (z) = 24 bytes.
- When I write "Vector3D position;" inside of TargetThreat, C doesn't store a pointer or a link 
to another place in memory - it carves out those 24 bytes directly inside the TargetThreat box.

Here is how the bytes sit side-by-side in my computer's RAM chip:
[ TargetThreat Memory Layout ]
┌─────────────────┬──────────────────────────────────────────┬──────────────────────────────────────────┬──────────────────────────────────────────┬─────────────────┬──────────┐
│  id (uint32_t)  │            position (Vector3D)           │            velocity (Vector3D)           │          acceleration (Vector3D)         │  timestamp_us   │  active  │
│     4 bytes     │      x (8b)  |  y (8b)  |  z (8b)        │      x (8b)  |  y (8b)  |  z (8b)        │      x (8b)  |  y (8b)  |  z (8b)        │     8 bytes     │  1 byte  │
└─────────────────┴──────────────────────────────────────────┴──────────────────────────────────────────┴──────────────────────────────────────────┴─────────────────┴──────────┘
  0x1000            0x1008                                     0x1020                                     0x1038                                     0x1050            0x1058

Here is how I access fields in C (Dot Notation):
To access a nested field in my code, I use the dot operator to chain down into the created structure.
[
TargetThreat missile;

missile.id = 101;               // Setting target's ID
missile.position.x = 5000.0;    // Setting the target's X position (Easy/West coordinate) - 5000 meters east
missile.position.y = 1000.0;    // Setting the target's Y position (North/South coordinate) - 1000 meters north
missile.velocity.z = -250.0;    // Seeting the target's Z velocity (Up/Down coordinate) - Descending at 250 m/s
]

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
What is happening under the hood:
The CPU convers missile.position.x into a simple math offset.
1- Find the starting memory address of missile (e.g. address 0x1000).
2- Jump forward to where 'position' starts (+8 bytes offset).
3- Read or write the x field right at that exact byte location.

*/

/*

The Toy Box Analogy:
Think of memory like a big wooden toy box with smaller compartments inside:
┌────────────────────────────────────────────────────────┐
│                   TARGET THREAT CARD                   │
│                                                        │
│  [ ID Tag ] ─── Holds ID #101                        │
│                                                        │
│  [ Position Box ] ─── [ X: 500m ] [ Y: 200m ] [ Z: 50m]│
│                                                        │
│  [ Velocity Box ] ─── [ X: 10m/s] [ Y: 0m/s ] [ Z: 0m/s]│
│                                                        │
│  [ Timestamp ] ──── Holds 12:00:01 PM                  │
│                                                        │
│  [ Active Flag ] ── Holds TRUE                         │
└────────────────────────────────────────────────────────┘
When I write Vector3D position; inside TargetThreat, I am saying:
"Inside this enemy card, build a small 3D Coordinate Box and label it 'position'."

Because a Vector3D box is already built to hold three numbers (x, y, z), my enemy card now
automatically has three slots inside its 'position' box to store:
1- x: How far East/West the threat is (meters)
2- y: How far North/South the threat is (meters)
3- z: How high/low the threat is (meters)

Opening the Boxes in Code:
When I want to change a number inside that card, I use dots like opening smaller and smaller boxes
that are nested inside each other.

missile.position.z = 50.0;            // Sets missile height to 50 meters

Instead of hiding the missile's height in one room, its speed in another room and its ID in the
garage, everything sits glued together in one neat card.

When the computer needs to calculate where the missile is going, it grabs the whole card in one single scoop
making the program faster.

*/
typedef struct {
    uint32_t id;                // Unique identifier for the threat contact
    Vector3D position;          // Current 3D position vector (m)
    Vector3D velocity;          // Current 3D velocity vector (m/s)
    Vector3D acceleration;      // Current 3D acceleration vector (m/s^2)
    uint64_t timestamp_us;      // Measurement timestamp (micorseconds)
    bool active;                // Active tracking state flag.
} TargetThreat;

/*

3. Fire-Control Intercept Solution
Output generated by the C kinematic solver for the weapon platform.

*/
/*

1- Firing Order Slip Analogy:
Imagine the fire-control computer is writing a firing order slip for the automated gun turret
or missile launcher.

┌────────────────────────────────────────────────────────┐
│               FIRE-CONTROL INTERCEPT SLIP              │
│                                                        │
│  [ Target ID ] ──────── Target #101                  │
│                                                        │
│  [ Intercept Point ] ── [ X: 450m ] [ Y: 180m ] [ Z: 30m]
│                                                        │
│  [ Azimuth Angle ] ──── 42.5° (Turn gun Northeast)     │
│                                                        │
│  [ Elevation Angle ] ── 15.0° (Tilt barrel up)         │
│                                                        │
│  [ Time-to-Impact ] ─── 1.8 seconds                    │
│                                                        │
│  [ Solution Valid ] ─── TRUE (Target is in range!)     │
└────────────────────────────────────────────────────────┘

When the C math solver finishes calculating a trajectory, it fills out this exact "slip"
and hands it off to the weapon or the Python dashboard.

2- Field-by-Field Breakdown:

typedef struct {
    uint32_t target_id;           // 4 bytes
    Vector3D intercept_point;     // 24 bytes (x, y, z as double)
    double azimuth_deg;           // 8 bytes
    double elevation_deg;         // 8 bytes
    double time_to_intercept_sec; // 8 bytes
    bool solution_valid;          // 1 byte
} InterceptSolution;

- uint32_t target_id;
    - What it stores: a 32-bit unsigned integer matching the id of the threat being targeted.
    - Why it is important: If there are 10 enemy drones in the sky, the gun system needs to 
      know which specific drone this firing solution belongs to.
- Vector3D intercept_point;
    - What it stores: a 64-bit decimal number representing the comapss angle (0-360 degrees).
    - Why it is needed: Tells the gun turrent how far left or right to rotate horizontally:
        - 0 degrees = Pointing North
        - 90 degrees = Pointing East
        - 180 degrees = Pointing South
        - 270 degrees = Pointing West
- double elevation_deg;
    - What it stores: A 64-bit decimal number representing the tilt angle (-90 to +90 degrees).
    - why it is needed: Tells the gun barrel how high to aim up into the sky relative to the flat horizon.
- double time_to_intercept_sec;
    - What it stores: A 64-bit decimal number measuring seconds (e.g. 2.45 seconds).
    - Why it is needed: Tells the system how long our rounds will spend in the air before impacting the target.
- bool solution_valid;
    - what it stores: A simple true or false flag
    - Why it is needed: What if a target is 50 km away, but our gun can only shoot 5 km? The C engine still 
      calculates the math, but sets solution_valid = false. The automated firing system checks this flag first
      if it's false, it won't pull the trigger.

3- How to interact with it in Code:
Just like before, I use dot notation to access each field in the structure. For example:
[
InterceptSolution solution;

// assign target ID and valid state
solution.target_id = 101;
solution.solution_valid = true;

// Setting the gun angles
solution.azimuth_deg = 42.5;        // Turns gun 42.5 degrees East of North
solution.elevation_deg = 15.0;      // Tilt barrel 15 degrees upward

// Setting the calculated 3D collision coordinates
solution.intercept_point.x = 450.0;
solution.intercept_point.y = 180.0;
solution.intercept_point.z = 30.0;

// Setting time-to-impact
solution.time_to_intercept_sec = 1.8;

4- The complete types.h Picture:
    1- Vector3D = the basic 3D building block
    2- TargetThreat = Ingests raw radar coordinates using embedded Vector3D s.
    3- InterceptSolution = Outputs the calculated firing order using an embedded Vector3D

All 3 structures sit inside src/include/types.h, wrapped safely in my #ifndef TYPES_H header guards.
]

*/

typedef struct {
    uint32_t target_id;             // Target ID this solution belongs to
    Vector3D intercept_point;       // Predicted 3D collision coordinates (m)
    double azimuth_deg;             // Horizontal gun angle (0-360 degrees)
    double elevation_deg;           // Vertical gun angle (-90 to +90 degrees)
    double time_to_intercept_sec;   // Time until projectile/missile impact
    bool solution_valid;            // True if target is within kinematic range
} InterceptSolution;

/*

types.h is simply the blueprint file where I define what a Location (Vector3D), 
an Enemy (TargetThreat), and a Firing Order (InterceptSolution) look like.

Now, whenever any C file in my project includes "types.h", it instantly knows how 
to read and build those three cards.

*/

#endif // TYPES_H