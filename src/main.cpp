#include "main.h"

Controller controller;

std::shared_ptr<ChassisController> drivetrain =
	ChassisControllerBuilder()
		.withMotors({11, 12}, {-13, -14})
		.withDimensions(AbstractMotor::gearset::green, {{4_in, 15_in}, imev5GreenTPR})
		.build();

ControllerButton flywheelForwardButton(ControllerDigital::R2);
ControllerButton flywheelBackwardButton(ControllerDigital::Y);
Motor flywheel(16, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

ControllerButton intakeForwardButton(ControllerDigital::L1);
ControllerButton intakeBackwardButton(ControllerDigital::L2);
Motor intake(18);

ControllerButton diskPusherForwardButton(ControllerDigital::R1);
ControllerButton diskPusherBackwardButton(ControllerDigital::B);
Motor diskPusher(-17);

IMU inertial(1);

ControllerButton pneumaticButton(ControllerDigital::X);
pros::ADIDigitalOut pneumatic('A');

ControllerButton upButton(ControllerDigital::up);
ControllerButton downButton(ControllerDigital::down);

int auton{ 0 };

constexpr int flywheelSpeed3{ 10000 };
constexpr int flywheelSpeed2{ 9000 };
constexpr int flywheelSpeed1{ 8000 };

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	inertial.reset();
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

	while(true) {
		auton = pros::lcd::read_buttons();
		if(auton != 0) {
			break;
		}
		pros::delay(20);
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
	if(auton == 1) {
		pros::screen::print(TEXT_MEDIUM, 1, "auton1");
	} 
	if(auton == 2) {
		pros::screen::print(TEXT_MEDIUM, 1, "auton2");
	} 
	if(auton == 4) {
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
	int flywheelSetting{ 3 };
	int flywheelSpeed{ 10000 };

	int timeSinceLastPressed{ pros::millis() };

	while(true) {
		drivetrain->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::leftX), 0.5);

		if(flywheelForwardButton.isPressed()) {
			flywheel.moveVoltage(flywheelSpeed);
		} else if(flywheelBackwardButton.isPressed()) {
			flywheel.moveVoltage(flywheelSpeed);
		} else {
			flywheel.moveVoltage(0);
		}

		if(intakeForwardButton.isPressed()) {
			intake.moveVoltage(12000);
		} else if(intakeBackwardButton.isPressed()) {
			intake.moveVoltage(-12000);
		} else {
			intake.moveVoltage(0);
		}

		if(diskPusherForwardButton.isPressed()) {
			diskPusher.moveVoltage(8000);
		} else if(diskPusherBackwardButton.isPressed()) {
			diskPusher.moveVoltage(-8000);
		} else {
			diskPusher.moveVoltage(0);
		}

		if(pneumaticButton.isPressed()) {
			pneumatic.set_value(true);
		}

		if(pros::millis() - timeSinceLastPressed > 100) {
			if(upButton.isPressed()) {
				flywheelSetting++;
				timeSinceLastPressed = pros::millis();
			} else if(downButton.isPressed()) {
				flywheelSetting--;
				timeSinceLastPressed = pros::millis();
			}
		}

		if(flywheelSetting == 1) {
			flywheelSpeed = flywheelSpeed1;
		} else if(flywheelSetting == 2) {
			flywheelSpeed = flywheelSpeed2;
		} else if(flywheelSetting == 3) {
			flywheelSpeed = flywheelSpeed3;
		} else {
			if(flywheelSetting > 3) {
				flywheelSpeed = flywheelSpeed3;
				flywheelSetting = 3;
			} else {
				flywheelSpeed = flywheelSpeed1;
				flywheelSetting = 1;
			}
		}

		controller.setText(1, 1, std::to_string(flywheel.getVoltage()));

		pros::delay(20);
	}
}