/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\sbhop                                            */
/*    Created:      Thu Aug 04 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// FlywheelBottom       motor         4               
// FlywheelTop          motor         5               
// Controller1          controller                    
// DriveFL              motor         9               
// DriveBL              motor         1               
// DriveFR              motor         2               
// DriveBR              motor         10              
// Indexer              motor         6               
// Intake               motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "cmath"
#include "vector"

using namespace vex;

// GLOBAL VARIABLES
double flywheelFF = 12.0/(530 * 5.0);
double flywheelP = 0.003;
double flywheelSetpoint = 1950.0;

/* FLYWHEEL */

// Returns the flywheel speed in rotations per second
double get_flywheel_rpm() {
  return FlywheelBottom.velocity(rpm) * (5.0/1.0);
}

// Sets the flywheel motors to a specified voltage
void set_flywheel_voltage(double demand) {
  FlywheelBottom.spin(forward, demand, volt);
  FlywheelTop.spin(forward, demand, volt);
}

// Sets the flywheel to a specified speed in rotations per second
void set_flywheel_rpm(double rpm) {
  if (std::abs(rpm) > 0) {
    double error = rpm - get_flywheel_rpm();
    double ff = flywheelFF * rpm;
    double p = flywheelP * error;
    double output = ff + p;
    if (output < -12) output = -12;
    if (output > 12) output = 12;
    set_flywheel_voltage(output);
  } else {
    set_flywheel_voltage(0);
  }
}

/* INDEXER */

// Set indexer to a specified voltage
void set_indexer_voltage(double demand) {
  Indexer.spin(forward, demand, volt);
}



/* DRIVE */

void drive(double leftDemand, double rightDemand) {
  DriveFL.spin(forward, leftDemand, percent);
  DriveFR.spin(forward, rightDemand, percent);
  DriveBL.spin(forward, leftDemand, percent);
  DriveBR.spin(forward, rightDemand, percent);
}


/* ROBOT CODE FLOW*/

// robotInit() is run at the beginning of robot initialization
void robotInit() {
  FlywheelBottom.setStopping(coast);
  FlywheelTop.setStopping(coast);
  
  DriveFL.setStopping(brake);
  DriveFR.setStopping(brake);
  DriveBL.setStopping(brake);
  DriveBR.setStopping(brake);
}

// robotPeriodic() is run periodically during the program's execution
void robotPeriodic() {
  // Print the current rpm
  Controller1.Screen.print(get_flywheel_rpm());
  Controller1.Screen.newLine();

  // Shoot Command
  if (Controller1.ButtonY.pressing()) {
    set_flywheel_rpm(flywheelSetpoint);
    if (std::abs(flywheelSetpoint - get_flywheel_rpm()) < 50) {
      set_indexer_voltage(6.0);
    }
  } else {
    set_flywheel_rpm(0);
    set_indexer_voltage(0);
  }

  // Handle Drive
  double leftInput = Controller1.Axis3.position(percent) * 0.6;
  double rightInput = Controller1.Axis2.position(percent) * 0.6;

  drive(leftInput, rightInput);
}
// Set intake to a specified voltage
void set_intake_voltage(double demand) {
  Intake.spin(forward, demand, volt);

  // Intake Command
  if (Controller1.ButtonR1.pressing()) {
      set_intake_voltage(6.0);
    }
   else {
    set_intake_voltage(0);
    }
}

int main() {
  vexcodeInit();
  robotInit();

  while (true) {
    robotPeriodic();
  }
 
}

