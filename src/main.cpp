#include "main.h"

pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup leftWheels({11, 12});
pros::Motor rightFrontWheel(13, true);
pros::Motor rightBackWheel(14, true);
pros::MotorGroup rightWheels({rightFrontWheel, rightBackWheel});
pros::Motor flywheel(16, MOTOR_GEAR_BLUE);
pros::Motor intake(18);
pros::Motor diskPusher(17, true);
pros::Motor roller(19, MOTOR_GEAR_RED);
pros::Imu inertial(1);
pros::ADIDigitalOut pneumatic('A');

bool auton1{ false };
bool auton2{ false };
bool auton3{ false };

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	inertial.reset();
	inertial.set_heading(0);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Auton 1");
	pros::lcd::set_text(2, "Auton 2");
	pros::lcd::set_text(3, "Auton 3");

	if(pros::lcd::read_buttons() == 1) {
		auton1 = true;
	}  else if(pros::lcd::read_buttons() == 4) {
		auton2 = true;
	} else if(pros::lcd::read_buttons() == 8) {
		auton3 = true;
	}
	
	pros::lcd::shutdown();
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	if(auton1) {
		pros::screen::print(TEXT_MEDIUM, 1, "auton1");
	} else if(auton2) {
		pros::screen::print(TEXT_MEDIUM, 1, "auton2");
	} else if(auton3) {
		pros::screen::print(TEXT_MEDIUM, 1, "auton3");
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	bool drivetrainStopped{ true };
	bool intakeStopped{ true };
	bool diskPusherStopped{ true };
	bool flywheelStopped{ true };
	bool rollerStopped{ true };

	while(true) {
		int leftWheelsSpeed{ controller.get_analog(ANALOG_LEFT_Y) + controller.get_analog(ANALOG_LEFT_X) };
		int rightWheelsSpeed{ controller.get_analog(ANALOG_LEFT_Y) - controller.get_analog(ANALOG_LEFT_X) };

		if(abs(leftWheelsSpeed) < 5 && abs(rightWheelsSpeed) < 5) {
			if(!drivetrainStopped) {
				leftWheels.brake();
				rightWheels.brake();
				drivetrainStopped = true;
			}
		} else {
			drivetrainStopped = false;
		}

		if(!drivetrainStopped) {
			leftWheels = leftWheelsSpeed;
			rightWheels = rightWheelsSpeed;
		}

		if(controller.get_digital(DIGITAL_L1)) {
			intake = 127;
			intakeStopped = false;
		} else if(controller.get_digital(DIGITAL_L2)) {
			intake = -127;
			intakeStopped = false;
		} else if(!intakeStopped) {
			intake.brake();
			intakeStopped = true;
		}

		if(controller.get_digital(DIGITAL_R2)) {
			flywheel = 127;
			flywheelStopped = false;
		} else if(controller.get_digital(DIGITAL_Y)) {
			flywheel = -127;
			flywheelStopped = false;
		} else if(!flywheelStopped) {
			flywheel.brake();
			flywheelStopped = true;
		}

		if(controller.get_digital(DIGITAL_R1)) {
			diskPusher = 95;
			diskPusherStopped = false;
		} else if(controller.get_digital(DIGITAL_B)) {
			diskPusher = -95;
			diskPusherStopped = false;
		} else if(!diskPusherStopped) {
			diskPusher.brake();
			diskPusherStopped = true;
		}

		int rollerSpeed{ controller.get_analog(ANALOG_RIGHT_Y) };

		if(abs(rollerSpeed) < 5) {
			if(rollerStopped) {
				roller.brake();
				rollerStopped = false;
			}
		} else {
			rollerStopped = true;
		}

		if(rollerStopped) {
			roller = rollerSpeed;
		}

		if(controller.get_digital(DIGITAL_X)) {
			pneumatic.set_value(true);
		}

		pros::screen::print(TEXT_MEDIUM, 1, "Flywheel temperature limit flag: %i", flywheel.is_over_temp());

		pros::delay(20);
	}
}
