# Anonymous Autonomous Art Installation
## File Structure
A gallery setup for the chairs consists of a folder with the following files: 
```
choreoCommands.h              // all specified choreo commands
handwrittenCommands.h         // the autonomous commands for handwritten commands (used as default commands)
babydriverConstants.h         // constants for baby_driver.cpp
cameraConstants.h             // constants for camera_eyes.cpp FIXME
lidarConstants.h              // constants for conductor.cpp FIXME
[optional] baby_driver.cpp    // optional custom baby_driver logic
[optional] camera_eyes.cpp    // optional custom baby_driver logic
[optional] antisocial or social files inside the lidar path   // optional custom (anti)social logic
```

Each file will have these components

choreoCommands.h: 
* overloaded choreos (`choreoSpin, choreoReverse, choreoEmpty`)
* additional custom choroes with unique shortcodes

handwrittenCommands.h
* specific autonomous commands for each handwritten command
      e.g. `fwd = f050f050`               FIXME

babydriverConstants.h
* overloaded `compareCommands` comparator
  * can either compare individually, or take a list of commands and compare placements in that list
* TODO: specify constants to add

cameraConstants.h
* TODO: specify other constants to add

lidarConstants.h
* TODO: specify other constants to add

\[optional\] baby_driver.cpp
* custom logic for a new baby_driver.cpp
* remember to include babydriverConstants.h!        TODO: add to verifier
* likely will need to track: 
  * most recent lidar command
  * most recent camera command
  * currently running choreo
  * current choreo stage
  * current running command (state of chair)
  * timer (to see when exceeded duration of a choreo stage)

\[optional\] camera_eyes.cpp
* custom logic for a new camera_eyes.cpp
* remember to include cameraConstants.h!        TODO: add to verifier

\[optional\] antisocial or social files
* TODO: specify format this takes


## Commands
### Types of Commands
* **Safety:** something is near the chair, and the chair should stop
* **Default:** the chair is reacting to its environment and moving accordingly (e.g. avoiding white paper, engaging in (anti)social behavior)
* **Choreography:** the chair is going through a series of default commands, each for a specific duration. Safety commands may or may not stop this choreography early. 

Safety commands are a specific subset of default commands, and uninterrupted commands are made up of a series of default commands. All commands are represented by strings. Commands can be either **autonomous** or **handwritten**. 

### Autonomous Commands
All autonomous commands are composed of 12 characters.

**Character 1:** Represents the source of the command
```
C: Camera
L: Lidar
H: Hub
M: Motors
```
**Character 2:** Represents the Priority of the command
```
A, B, C, ..., Z           where A is the highest priority, and Z is the lowest
```
**Character 3:** Represents the Type of Command
```
C: Choreo
D: Default
S: Safety (stop)
```
**Character 4:** (choreo) Represents the Choreo Shortcode (if this is a choreo command)
```
A, B, C, ..., Z
```
**Character 5:** (left motor) Represents the Direction of the Left Motor
```
f: forward
r: reverse
```
**Character 6 - 8:** (left motor) Represents the Speed of the Left Motor
```
000, 001, ..., 256        where 000 is stopped and 256 is full speed
```
**Character 9:** (right motor) Represents the Direction of the Right Motor
```
f: forward
r: reverse
```
**Character 10 - 12:** (right motor) Represents the Speed of the Right Motor
```
000, 001, ..., 256        where 000 is stopped and 256 is full speed
```

### Handwritten commands
The following is a list of valid handwritten commands. Note that they all have less than 12 characters. This is how we distinguish handwritten commands from autonomous commands.
```
fwd       // forward
ffwd      // fast forward
bwd       // backward
fbwd      // fast backward
stop      // stop
left      // left
right     // right
fwdl      // veer forward left
fwdr      // veer forward right
bwdl      // veer backward left
bwdr      // veer backward right
pivotl    // pivot left (counterclockwise)
pivotr    // pivot right (clockwise)
toggle    // stop handwritten commands and ask chair to listen to autonomous commands instead
L###R###  // custom speeds and directions for left and right motors.
          // L/R should be either 'f' for forward or 'r' for reverse
          // ### should be between 000 - 256 and represent the speed of each motor in the chosen directions
 ```

### Choreography Commands
Choreography commands are composed of multiple default or autonomous commands being run in succession. They are structured as follows: 
```
struct startCondition {
  bool immediately = 0;               // this choreo should start immediately when it is received as a command
  bool add_to_queue = 0;              // this choreo should be handled like a default command as prioritized when it is received
  int after_x_old_cam_images = -1;    // (stuck) this choreo should start after x old camera images have been sent in a row
  int after_x_stop_cmds_lidar = -1;   // (stuck) this choreo should start after x stop commands from the lidar have been sent
  int after_x_seconds = -1;           // this choreo should start x seconds after being received
  // more can be added here (e.g. for encoder motors)
}

struct stage {
  string command;
  int duration;                           
}

struct choreo {
  bool interruptable;               // whether a safety command should stop this choreo
  startCondition start_condition;   // when this choreo should be started
  int numStages;                    // how many stages are in this choreo (size of stages)
  char shortCode;                   // a unique identifier between {A, ..., Z}
  vector<stage> stages;             // stages to be run in order
}
```
Choreos are specified in the `choreoCommands.h` file. Each `choreoCommands.h` file must contain the following overloaded choreos, as we use them in a lot of the chairs' functionality: 
```
choreo choreoSpin;        // shortcode: S   Chair spins in place
choreo choreoReverse;     // shortcode: R   Chair reverses after being stuck
choreo choreoEmpty;       // shortcode: E   Empty choreo for when a default command is running
```

#### Choreo Verifier Checks
When a new setup is loaded for the chairs, the verifier will check the following choreo elements: 
* the overloaded choreos are present (`choreoSpin, choreoReverse, choreoEmpty`)
* all choreo shortcodes are unique and in the correct range {A, ..., Z}
* start condition requirements
  * if `immediately` is set to true, all other variables are set to false or -1
  * if `add_to_queue` is set to true, all other variables are set to false or -1
  * only one of `after_x_old_cam_images, after_x_stop_cmds_lidar, after_x_seconds` is specified, with a positive integer
  * all commands in choreo commands are valid autonomous or handwritten commands
