using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor FlywheelBottom;
extern motor FlywheelTop;
extern controller Controller1;
extern motor DriveFL;
extern motor DriveBL;
extern motor DriveFR;
extern motor DriveBR;
extern motor Indexer;
extern motor Intake;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );