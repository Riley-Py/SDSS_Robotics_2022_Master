using namespace vex;

extern brain Brain;

// VEXcode devices
extern smartdrive Drivetrain;
extern controller Controller1;
extern motor Flywheel;
extern motor Intake;
extern motor DiskPusher;
extern motor Roller;
extern digital_out Pneumatic;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );