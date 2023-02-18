#include "main.h"

Controller controller;

std::shared_ptr<ChassisController> drivetrain =
	ChassisControllerBuilder()
		.withMotors({1, 2}, {-3, -4})
		.withDimensions(AbstractMotor::gearset::green, {{3.25_in, 15_in}, imev5GreenTPR})
		.build();

ControllerButton flywheelForwardButton(ControllerDigital::R2);
ControllerButton flywheelBackwardButton(ControllerDigital::Y);
Motor flywheel(5, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

ControllerButton intakeForwardButton(ControllerDigital::L1);
ControllerButton intakeBackwardButton(ControllerDigital::L2);
Motor intake(7);
Motor intake2(8);

ControllerButton diskPusherForwardButton(ControllerDigital::R1);
ControllerButton diskPusherBackwardButton(ControllerDigital::B);
Motor diskPusher(-6, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

IMU inertial(8);

ControllerButton pneumaticButton(ControllerDigital::X);
pros::ADIDigitalOut pneumatic('H');

ControllerButton upButton(ControllerDigital::up);
ControllerButton downButton(ControllerDigital::down);

constexpr int flywheelSpeed3{ 9000 };
constexpr int flywheelSpeed2{ 8000 };
constexpr int flywheelSpeed1{ 7000 };

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	inertial.reset();
	diskPusher.setBrakeMode(AbstractMotor::brakeMode::brake);
	//competition_initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

int auton{ 0 };

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
	pros::lcd::set_text(1, "In front of roller");
	pros::lcd::set_text(2, "Other sides");

	while(true) {
		auton = pros::lcd::read_buttons();
		if(auton != 0) {
			break;
		}
		pros::delay(20);
	}

	pros::lcd::shutdown();

	autonomous();
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
	if(auton == 4) {
		drivetrain->getModel()->forward(100);
		intake.moveVoltage(5000);
		pros::delay(400);
		intake.moveVoltage(0);
		drivetrain->moveDistance(-10_in);
		drivetrain->turnAngle(-160_deg);
		intake.moveVoltage(12000);
		drivetrain->moveDistance(7_ft);
		drivetrain->turnAngle(100_deg);
		flywheel.moveVoltage(11000);
		pros::delay(2000);
		diskPusher.moveVoltage(8000);
		flywheel.moveVoltage(11000);
		pros::delay(2000);
		diskPusher.moveVoltage(8000);
		flywheel.moveVoltage(11000);
		pros::delay(2000);
		diskPusher.moveVoltage(8000);
	}
	if(auton == 2) {

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
	int flywheelSpeed{ 9000 };

	int timeSinceFlywheelSettingLastPressed{ pros::millis() };

	bool flywheelToggle{ false };
	bool flywheelLatch{ false };

	while(true) {
		drivetrain->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::leftX), 0.3);
		
		if(flywheelToggle) {
			flywheel.moveVoltage(flywheelSpeed);
		} else if(flywheelBackwardButton.isPressed()) {
			flywheel.moveVoltage(-flywheelSpeed);
		} else {
			flywheel.moveVoltage(0);
		} 

		if(flywheelForwardButton.isPressed()) {
			if(!flywheelLatch) {
				flywheelToggle = !flywheelToggle;
				flywheelLatch = !flywheelLatch;
			}
		} else {
			flywheelLatch = false;
		}

		if(intakeForwardButton.isPressed()) {
			intake.moveVoltage(12000);
			intake2.moveVoltage(12000);
		} else if(intakeBackwardButton.isPressed()) {
			intake.moveVoltage(-12000);
			intake2.moveVoltage(12000);
		} else {
			intake.moveVoltage(0);
			intake2.moveVoltage(0);
		}

		if(diskPusherForwardButton.isPressed()) {
			diskPusher.moveVoltage(12000);
			if(diskPusher.getPosition() >= 420) {
				diskPusher.moveVoltage(0);
			}
		} else if(diskPusherBackwardButton.isPressed()) {
			diskPusher.moveVoltage(-12000);
		} else {
			diskPusher.moveVoltage(0);
			diskPusher.tarePosition();
		}

		if(pneumaticButton.isPressed()) {
			pneumatic.set_value(true);
		}

		if(pros::millis() - timeSinceFlywheelSettingLastPressed > 100) {
			if(upButton.isPressed()) {
				flywheelSetting++;
				timeSinceFlywheelSettingLastPressed = pros::millis();
			} else if(downButton.isPressed()) {
				flywheelSetting--;
				timeSinceFlywheelSettingLastPressed = pros::millis();
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

		controller.setText(1, 1, std::to_string(flywheelSetting) + " " + std::to_string(static_cast<int>(flywheel.getTemperature())) + "°C");

		pros::delay(20);
	}
}
