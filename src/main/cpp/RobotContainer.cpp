/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "RobotContainer.h"

#include <frc/controller/PIDController.h>
#include <frc/geometry/Translation2d.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/trajectory/Trajectory.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/SwerveControllerCommand.h>
#include <frc2/command/button/JoystickButton.h>
#include <units/units.h>

#include "Constants.h"
#include "subsystems/DriveSubsystem.h"

using namespace DriveConstants;

RobotContainer::RobotContainer(Logger& log)
    : m_log(log)
    , m_drive(log)
{
    // Initialize all of your commands and subsystems here

    // Configure the button bindings
    ConfigureButtonBindings();

    // Set up default drive command
    m_drive.SetDefaultCommand(frc2::RunCommand(
        [this] {

//#define USE_BUTTONS
#ifdef USE_BUTTONS
            // Push button control to diagnose swerve angle accuracy
            double xInput = 0.0;
            const double c_buttonInputSpeed = 0.5;
            if (m_driverController.GetYButton())
            {
                xInput = c_buttonInputSpeed;
            }
            else if (m_driverController.GetAButton())
            {
                xInput = -1.0 * c_buttonInputSpeed;
            }
            double yInput = 0.0;
            if (m_driverController.GetXButton())
            {
                yInput = c_buttonInputSpeed;
            }
            else if (m_driverController.GetBButton())
            {
                yInput = -1.0 * c_buttonInputSpeed;
            }

            double rotInput = 0.0;
            //            U
            //            0
            //            |
            //    UL 315\ | /45 UR
            //           \|/
            // L 270------+------90 R
            //           /|\ 
            //    DL 225/ | \135 DR
            //            |
            //           180
            //            D
            auto dPadPointOfView = m_driverController.GetPOV();
            if (dPadPointOfView >= 225 && dPadPointOfView <= 315)
            {
                rotInput = 1.0;
            }
            else if (dPadPointOfView >= 45 && dPadPointOfView <= 135)
            {
                rotInput = -1.0;
            }
#else
            // up is xbox joystick y pos
            // left is xbox joystick x pos
            auto xInput = Deadzone(m_driverController.GetY(frc::GenericHID::kLeftHand) * -1.0, OIConstants::kDeadzoneX);
            auto yInput = Deadzone(m_driverController.GetX(frc::GenericHID::kLeftHand) * -1.0, OIConstants::kDeadzoneY);
            auto rotInput = Deadzone(m_driverController.GetX(frc::GenericHID::kRightHand), OIConstants::kDeadzoneRot);

#endif

            m_inputXentry.SetDouble(xInput);
            m_inputYentry.SetDouble(yInput);
            m_inputRotentry.SetDouble(rotInput);

            /// \todo Scale +/-1.0 xbox input to kMaxSpeed
            m_drive.Drive(units::meters_per_second_t(xInput),
                          units::meters_per_second_t(yInput),
                          units::radians_per_second_t(rotInput),
                          false);
        },
        {&m_drive}
    ));

    ShuffleboardTab& tab = Shuffleboard::GetTab("XboxInput");
    m_inputXentry = tab.Add("X", 0).GetEntry();
    m_inputYentry = tab.Add("Y", 0).GetEntry();
    m_inputRotentry = tab.Add("Rot", 0).GetEntry();

    // The roboRIO does not have a battery powered RTC. However, the DS sends the time when it connects, which the roboRIO uses to set the system time. If you wait until the DS connects, you can have correct timestamps, without a RTC.
    double matchTime = frc::Timer::GetMatchTime();
    printf("Match time %.3f\n", matchTime);

    // Shuffleboard::GetTab("Preround").Add("Partner can scale", false)
    //                                 .WithWidget(frc::BuiltInWidgets::kSplitButtonChooser)
    //                                 .WithSize(2, 1)     // Widget size on shuffleboard
    //                                 .WithPosition(0,0); // Widget position on shuffleboard
}

void RobotContainer::ConfigureButtonBindings()
{
    // Configure your button bindings here
}

frc2::Command *RobotContainer::GetAutonomousCommand()
{
    m_drive.ResetOdometry(frc::Pose2d(0_m, 0_m, frc::Rotation2d(0_deg)));

    // Set up config for trajectory
    frc::TrajectoryConfig config(AutoConstants::kMaxSpeed,
                                 AutoConstants::kMaxAcceleration);
    // Add kinematics to ensure max speed is actually obeyed
    config.SetKinematics(m_drive.kDriveKinematics);

    // An example trajectory to follow.  All units in meters.
    auto exampleTrajectory = frc::TrajectoryGenerator::GenerateTrajectory(
        // Start at the origin facing the +X direction
        frc::Pose2d(0_m, 0_m, frc::Rotation2d(0_deg)),
        // Pass through these two interior waypoints, making an 's' curve path
        {frc::Translation2d(1_m, 1_m), frc::Translation2d(2_m, -1_m)},
        // End 3 meters straight ahead of where we started, facing forward
        frc::Pose2d(3_m, 0_m, frc::Rotation2d(0_deg)),
        // Pass the config
        config
    );

    auto exampleTrajectory2 = frc::TrajectoryGenerator::GenerateTrajectory(
        // Start at the origin facing the +X direction
        frc::Pose2d(0_m, 0_m, frc::Rotation2d(0_deg)),
        // Pass through these two interior waypoints, making an 's' curve path
        {frc::Translation2d(1_m, 0_m), frc::Translation2d(2_m, 0_m)},
        // End 2 meters straight ahead of where we started, facing forward
        frc::Pose2d(3_m, 0_m, frc::Rotation2d(0_deg)),
        // Pass the config
        config
    );

    frc2::SwerveControllerCommand<4> swerveControllerCommand(
        exampleTrajectory, [this]() { return m_drive.GetPose(); },

        m_drive.kDriveKinematics,

        frc2::PIDController(AutoConstants::kPXController, 0, 0),
        frc2::PIDController(AutoConstants::kPYController, 0, 0),
        frc::ProfiledPIDController<units::radians>(
            AutoConstants::kPThetaController, 0, 0,
            AutoConstants::kThetaControllerConstraints),

        [this](auto moduleStates) { m_drive.SetModuleStates(moduleStates); },

        {&m_drive}
    );

    // no auto
    return new frc2::SequentialCommandGroup(
        std::move(swerveControllerCommand), std::move(swerveControllerCommand),
        frc2::InstantCommand(
            [this]() {
                m_drive.Drive(units::meters_per_second_t(0.0),
                              units::meters_per_second_t(0.0),
                              units::radians_per_second_t(0.0), false);
            },
            {}
        )
    );
}
