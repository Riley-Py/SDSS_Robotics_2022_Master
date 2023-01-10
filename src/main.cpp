// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    1, 2, 3, 4, 9   
// Controller1          controller                    
// Flywheel             motor         5               
// Intake               motor         6               
// DiskPusher           motor         7               
// Roller               motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    1, 2, 3, 4      
// Controller1          controller                    
// Flywheel             motor         5               
// Intake               motor         6               
// DiskPusher           motor         7               
// Roller               motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

void spinRoller() {
  Roller.setVelocity(Controller1.Axis2.position(percent), percent);
  Roller.spin(forward);
}

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.setTurnVelocity(100, percent);

  Flywheel.setVelocity(100, percent);

  Intake.setVelocity(100, percent);

  DiskPusher.setVelocity(100, percent);

  Roller.setVelocity(100, percent);

  //Sets the degrees to start to 0 (where the inertial is positioned with respect to the x-axis)
  Drivetrain.setHeading(0, degrees);

  
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................

  //Need colour sensor to code the rollers here 

  /*As well, we need to make a decision on the vision sensor
  as that will dictate whether we use it to pick up disks or 
  doing it manually ourselves*/

  //Delete these comments when we no longer need them
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while(1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    //Flywheel controls
    if(Controller1.ButtonR2.pressing()) {
      Flywheel.spin(forward);
    } else if(Controller1.ButtonX.pressing()) {
      Flywheel.spin(reverse);
    } else {
      Flywheel.stop();
    }

    //Pusher into the flywheel
    if(Controller1.ButtonR1.pressing()) {
      DiskPusher.spin(forward);
    } else if(Controller1.ButtonB.pressing()) {
      DiskPusher.spin(reverse);
    } else {
      DiskPusher.stop();
    }

    Controller1.Axis2.changed(spinRoller);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while(true) {
    wait(100, msec);
  }
}
