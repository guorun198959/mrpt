/* +---------------------------------------------------------------------------+
   |                 The Mobile Robot Programming Toolkit (MRPT)               |
   |                                                                           |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2013, Individual contributors, see AUTHORS file        |
   | Copyright (c) 2005-2013, MAPIR group, University of Malaga                |
   | Copyright (c) 2012-2013, University of Almeria                            |
   | All rights reserved.                                                      |
   |                                                                           |
   | Redistribution and use in source and binary forms, with or without        |
   | modification, are permitted provided that the following conditions are    |
   | met:                                                                      |
   |    * Redistributions of source code must retain the above copyright       |
   |      notice, this list of conditions and the following disclaimer.        |
   |    * Redistributions in binary form must reproduce the above copyright    |
   |      notice, this list of conditions and the following disclaimer in the  |
   |      documentation and/or other materials provided with the distribution. |
   |    * Neither the name of the copyright holders nor the                    |
   |      names of its contributors may be used to endorse or promote products |
   |      derived from this software without specific prior written permission.|
   |                                                                           |
   | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       |
   | 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED |
   | TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR|
   | PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE |
   | FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL|
   | DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR|
   |  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)       |
   | HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       |
   | STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN  |
   | ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           |
   | POSSIBILITY OF SUCH DAMAGE.                                               |
   +---------------------------------------------------------------------------+ */
#ifndef CActivMediaRobotBase_H
#define CActivMediaRobotBase_H

#include <mrpt/hwdrivers/CGenericSensor.h>
#include <mrpt/hwdrivers/link_pragmas.h>
#include <mrpt/poses/CPose2D.h>
#include <mrpt/slam/CObservationRange.h>
#include <mrpt/hwdrivers/CJoystick.h>

namespace mrpt
{
	namespace hwdrivers
	{
		/** This software driver implements the communications (and some rudimentary control) for ActivMedia robotic bases (Pioneer DX/AT, PeopleBot, etc).
		  *  There is implemented access to robot odometry, ticks counts, velocities, battery charge status, and sonar readings, as well as
		  *   basic velocity control.
		  *
		  *  It is required to check MRPT_BUILD_ARIA in the cmake configuration to enable this class to work properly.
		  *
		  *  See also the application "rawlog-grabber" for a ready-to-use application to gather data from the robot base.
		  *  Through that "common sensor interface", this object can collect these kinds of observations:
		  *    - mrpt::slam::CObservationOdometry : For odometry
		  *    - mrpt::slam::CObservationRange    : For sonars
		  *
		  *  To use this class out of rawlog-grabber, "initialize" must be called to connect to the robot.
		  *  Before that, set the serial port with setSerialPortConfig.
		  *
		  *
		  *
		  *  Example .ini block for rawlog-grabber (format used in "loadConfig"):
		  *
		  *  \code
		  *  PARAMETERS IN THE ".INI"-LIKE CONFIGURATION STRINGS:
		  * -------------------------------------------------------
		  *   [supplied_section_name]
		  *    robotPort_WIN		= COM1
		  *    robotPort_LIN		= /dev/ttyUSB0
		  *    robotBaud			= 115200
		  *    enableSonars         = 0     ; 0:Disabled (default), 1: Enabled
		  *    capture_rate         = 10.0  ; In Hz, the rate at which sonars & odometry are gathered (default=10Hz)
		  *
		  *    joystick_control		= 0		; 0:Disabled (default), 1: Enabled
		  *    joystick_max_v		= 0.1	; Max joystick control speed (m/s)
		  *	   joystick_max_w_degps	= 20	; Max joystick control speed (deg/s)
		  *
		  *  \endcode
		  * \ingroup mrpt_hwdrivers_grp
		  */
		class HWDRIVERS_IMPEXP CActivMediaRobotBase : public CGenericSensor
		{
			DEFINE_GENERIC_SENSOR(CActivMediaRobotBase)
		public:

			/** A structure describing the robot */
			struct HWDRIVERS_IMPEXP TRobotDescription
			{
				TRobotDescription(); //!< Init
				size_t  nFrontBumpers; 	//!< Number of front bumpers
				size_t  nRearBumpers; 	//!< Number of rear bumpers
				size_t  nSonars; 		//!< Number of sonars
			};


			/** Connects to the robot */
			void initialize();

			/** Constructor
			  */
			CActivMediaRobotBase();

			/** Destructor: turns off communications */
			virtual ~CActivMediaRobotBase();

			/** Manually sets the serial port configuration.
			  * \param portName Examples: Windows: "COM1" , Linux: "/dev/ttyUSB0"
			  * \param portBaudRate 9600, 115200, etc..
			  * \sa loadConfig
			  */
			void  setSerialPortConfig(
				const std::string  &portName,
				int		portBaudRate );

			/** Returns the current value of the serial port */
			std::string getSerialPort() const { return m_com_port; }

			/** Returns the current value of the serial port baudrate */
			int getSerialPortBaudRate() const { return m_robotBaud; }


			/** Collect odometry readings and put them in the "observations" queue: DO NOT call this normally, it's useful only for the application rawloggrabber.
			  */
			void doProcess();

			/** Change the current robot odometry pose
			  */
			void changeOdometry(const mrpt::poses::CPose2D &newOdometry);

			/** Get the current robot's odometry
			  * \param out_odom The odometry will be returned here.
			  * \sa getOdometryFull, getOdometryIncrement
			  */
			void getOdometry(poses::CPose2D &out_odom);

			/** Get the current robot's odometry
			  * \param out_odom The odometry will be returned here.
			  * \param out_lin_vel The linear speed, in m/s, positive is forward.
			  * \param out_ang_vel The angular speed, in rad/s, positive is anticlockwise.
			  * \param out_left_encoder_ticks The current overall count of ticks for the left wheel encoder.
			  * \param out_right_encoder_ticks The current overall count of ticks for the right wheel encoder.
			  * \sa getOdometry, getOdometryIncrement
			  */
			void getOdometryFull(
				poses::CPose2D	&out_odom,
				double			&out_lin_vel,
				double			&out_ang_vel,
				int64_t			&out_left_encoder_ticks,
				int64_t			&out_right_encoder_ticks
				);

			/** Get the robot's odometry increment since the last call to this method (the first time the increments are always fixed to zero).
			  * \param out_odom The odometry increment.
			  * \param out_lin_vel The current linear speed, in m/s, positive is forward (Absolute values, not increments)
			  * \param out_ang_vel The angular speed, in rad/s, positive is anticlockwise  (Absolute values, not increments).
			  * \param out_left_encoder_ticks The increment in ticks for the left wheel encoder.
			  * \param out_right_encoder_ticks The increment in ticks for the right wheel encoder.
			  * \sa getOdometry, getOdometryFull
			  */
			void getOdometryIncrement(
				poses::CPose2D	&out_incr_odom,
				double			&out_lin_vel,
				double			&out_ang_vel,
				int64_t			&out_incr_left_encoder_ticks,
				int64_t			&out_incr_right_encoder_ticks
				);

			/** Get the readings from the sonars, only if the observations are new.
			  */
			void getSonarsReadings( bool &thereIsObservation, mrpt::slam::CObservationRange	&obs );

			/** Get the robot battery charge */
			void getBatteryCharge( double &out_batery_volts );

			/** Set the robot linear and angular velocities
			  * \param lin_vel Linear speed, in m/s.
			  * \param ang_vel Angular speed, in rad/s.
			  */
			void setVelocities( const double lin_vel, const double ang_vel);

			void enableSonars();	//!< Enable sonars
			void disableSonars();	//!< Disable sonars


			void getBumpers(vector_bool &bumper_state); //!< Get state of bumpers: at output, the vector will be resized to the number of bumpers, and elements with "true" means bumper is pressed.

			void getRobotInformation(TRobotDescription &info); //!< Get information about the robot and its sensors

			/** Enable/disable manual control of the robot with a Joystick */
			void enableJoystickControl(bool enable=true) { m_enableJoyControl = enable; } 

			/** Get state of manual control with a joystick */
			bool isJoystickControlEnabled() const { return m_enableJoyControl;}

			/** Disable motors but keeps the connection with ARIA*/
			void DisableMotors();
			
			/** Enable motors after a call to DisableMotors()*/
			void EnableMotors();
			
			/** Get the motors state*/
			bool areMotorsEnabled() const;

		protected:
			std::string		m_com_port;		//!< The serial port name to use for communications (COM1, ttyS1,...)
			int				m_robotBaud;	//!< The bauds for ARIA communications to the robot.

			bool			m_firstIncreOdometry;	//!< Used in getOdometryIncrement
			bool			m_enableSonars;
			

			void* /*ArRobot*/				m_robot;
			void* /*ArSonarDevice*/			m_sonarDev;
			void* /*ArSimpleConnector* */	m_simpleConnector;	//!< The connection to the robot

			unsigned int 	m_lastTimeSonars;

			bool		m_enableJoyControl; //!< For use with rawlog-grabber
			float		m_joy_max_v, m_joy_max_w;

			CJoystick	m_joystick;			//!< The joystick opened at first usage.

			mrpt::system::TTimeStamp  m_last_do_process;
			double  	m_capture_rate;  //!< In Hz, the rate at which sonars & odometry are gathered (default=10Hz)

			void disconnectAndDisableMotors();
			void connectAndEnableMotors();
			/** Loads specific configuration for the device from a given source of configuration parameters, for example, an ".ini" file, loading from the section "[iniSection]" (see utils::CConfigFileBase and derived classes)
			  *  See hwdrivers::CActivMediaRobotBase for the possible parameters
			  * \sa setSerialPortConfig
			  */
			void  loadConfig_sensorSpecific(
				const mrpt::utils::CConfigFileBase &configSource,
				const std::string	  &iniSection );


		};	// End of class

	} // End of namespace
} // End of namespace
#endif
