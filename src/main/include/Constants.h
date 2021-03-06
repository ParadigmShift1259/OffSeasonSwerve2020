/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <frc/geometry/Translation2d.h>
#include <frc/kinematics/SwerveDriveKinematics.h>
#include <frc/trajectory/TrapezoidProfile.h>
#include <units/units.h>
#include <wpi/math>

#pragma once

/**
 * The Constants header provides a convenient place for teams to hold robot-wide
 * numerical or bool constants.  This should not be used for any other purpose.
 *
 * It is generally a good idea to place constants into subsystem- or
 * command-specific namespaces within this header, which can then be used where
 * they are needed.
 */

namespace DriveConstants
{
    constexpr int kNumSwerveModules = 4;

    constexpr int kFrontLeftDriveMotorPort    = 1;
    constexpr int kFrontLeftTurningMotorPort  = 2;

    constexpr int kFrontRightDriveMotorPort   = 3;
    constexpr int kFrontRightTurningMotorPort = 4;

    constexpr int kRearRightDriveMotorPort    = 5;
    constexpr int kRearRightTurningMotorPort  = 6;

    constexpr int kRearLeftDriveMotorPort     = 7;
    constexpr int kRearLeftTurningMotorPort   = 8;

    constexpr int kFrontLeftTurningEncoderPort  = 0;
    constexpr int kFrontRightTurningEncoderPort = 1;
    constexpr int kRearRightTurningEncoderPort  = 2;
    constexpr int kRearLeftTurningEncoderPort   = 3;

    constexpr bool kFrontLeftTurningEncoderReversed  = false;
    constexpr bool kRearLeftTurningEncoderReversed   = true;
    constexpr bool kFrontRightTurningEncoderReversed = false;
    constexpr bool kRearRightTurningEncoderReversed  = true;

    constexpr int kFrontLeftDriveEncoderPort  = 1;
    constexpr int kRearLeftDriveEncoderPort   = 3;
    constexpr int kFrontRightDriveEncoderPort = 5;
    constexpr int kRearRightDriveEncoderPort  = 7;

    constexpr bool kFrontLeftDriveMotorReversed  = false;
    constexpr bool kRearLeftDriveMotorReversed   = false;
    constexpr bool kFrontRightDriveMotorReversed = true;
    constexpr bool kRearRightDriveMotorReversed  = true;

    constexpr bool kGyroReversed = false;

    // These are example values only - DO NOT USE THESE FOR YOUR OWN ROBOT!
    // These characterization values MUST be determined either experimentally or
    // theoretically for *your* robot's drive. The RobotPy Characterization
    // Toolsuite provides a convenient tool for obtaining these values for your
    // robot.
    //constexpr auto ks = 1_V;
    //constexpr auto kv = 0.8 * 1_V * 1_s / 1_m;
    //constexpr auto ka = 0.15 * 1_V * 1_s * 1_s / 1_m;

    // Example value only - as above, this must be tuned for your drive!
    //constexpr double kPFrontLeftVel = 0.5;
    //constexpr double kPRearLeftVel = 0.5;
    //constexpr double kPFrontRightVel = 0.5;
    //constexpr double kPRearRightVel = 0.5;

    constexpr double kFrontLeftOffset   = 3.142;         // 3.14;
    constexpr double kFrontRightOffset  = 5.105;         // 5.07;         //5.66;
    constexpr double kRearLeftOffset    = 3.375;         // 3.34;         //4.29;
    constexpr double kRearRightOffset   = 0.665;         // 0.63;         //5.29;

    constexpr double kTurnVoltageToRadians = 2.0 * wpi::math::pi / 4.93;    // Absolute encoder runs 0 to 4.93V
    constexpr double KTurnVoltageToDegrees = 360 / 4.93;

    constexpr double kDriveGearRatio = 8.31;                //!< MK2 swerve modules 11.9 ft/sec
    //constexpr double kDriveGearRatio = 8.16;                //!< MK3 swerve modules w/NEOs 12.1 ft/sec
    //constexpr double kDriveGearRatio = 6.86;                //!< MK3 swerve modules w/NEOs 14.4 ft/sec
    constexpr double kTurnMotorRevsPerWheelRev = 18.0;
}  // namespace DriveConstants

namespace ModuleConstants
{
    constexpr int kEncoderCPR = 1024;
    constexpr double kWheelDiameterMeters = .1016;    // 4"
    // Assumes the encoders are directly mounted on the wheel shafts
    constexpr double kDriveEncoderDistancePerPulse = (kWheelDiameterMeters * wpi::math::pi) / static_cast<double>(kEncoderCPR);

    // Assumes the encoders are directly mounted on the wheel shafts
    //constexpr double kTurningEncoderDistancePerPulse = (wpi::math::pi * 2) / static_cast<double>(kEncoderCPR);

    constexpr double kP_ModuleTurningController = 1.1;
    constexpr double kD_ModuleTurningController = 0.03;

    constexpr double kPModuleDriveController = 0.001;

    constexpr uint kMotorCurrentLimit = 30;
}   // namespace ModuleConstants

namespace AutoConstants
{
    using radians_per_second_squared_t = units::compound_unit<units::radians, units::inverse<units::squared<units::second>>>;

    constexpr auto kMaxSpeed = units::meters_per_second_t(0.7);
    constexpr auto kMaxAcceleration = units::meters_per_second_squared_t(1.0);
    constexpr auto kMaxAngularSpeed = units::radians_per_second_t(wpi::math::pi);
    constexpr auto kMaxAngularAcceleration = units::unit_t<radians_per_second_squared_t>(wpi::math::pi);

    constexpr double kPXController = 0.25;
    constexpr double kPYController = 0.25;
    constexpr double kPThetaController = 0.5;

    extern const frc::TrapezoidProfile<units::radians>::Constraints kThetaControllerConstraints;
}  // namespace AutoConstants

namespace OIConstants
{
    constexpr double kDeadzoneX = 0.10;
    constexpr double kDeadzoneY = 0.10;
    constexpr double kDeadzoneRot = 0.10;
    constexpr int kDriverControllerPort = 0;
}  // namespace OIConstants
