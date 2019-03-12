#pragma once

#include <frc/WPILib.h>
#include "RobotMap.h"

class Dashboard {
public:
	Dashboard(std::string commandPrefix, int robotStatusCount, int robotValueCount, 
	          int dashButtonCount, int dashValueCount);
	~Dashboard();

	std::string 	CountReply();																					// Returns current state of Robot, DbData, DbButton values
	std::string 	DataString(int32_t number, int delimiter);						// Returns string with integer followed by a delimiter
	std::string 	DataString(double number, int delimiter);							// Returns string with double followed by a delimiter
	std::string 	GetCommandPrefix();
	bool					GetDashButton(DashButton buttonIndex);								// Returns state of a Dashboard Button
	bool					GetDashButtonPress(DashButton buttonIndex);						// Returns True if Dashboard Button has been Pressed
	double				GetDashValue(DashValue valueIndex);										// Returns Dashboard value at specified index
	std::string 	GetReply();																						// Returns GET reply for all Robot Values
	bool					GetRobotStatus(RobotStatus statusIndex);							// Returns current Robot Status
	std::string 	PullReply();																					// Returns PULL reply for all Dashboard values

	void        	SaveDashValues();
	bool					SetDashButton(int group, int32_t value);							// Sets Dashboard Button group at specified index
	bool					SetDashValue(int valueIndex, double value);						// Sets Dashboard Value at specified index
	bool					SetRobotStatus(RobotStatus statusIndex, bool value);	// Sets Robot Status at specified index
	bool					SetRobotValue(RobotValue valueIndex, double value);		// Sets Robot Value at specified index
	void					SetRobotMode(RobotMode mode);													// Sets Robot Mode
	void					StartHost();																					// Starts Host thread
	static void 	TcpLoop(Dashboard *host);															// TCP communications loop (Runs in separate thread)
	void					WriteToLog(std::string entry);												// Write entry into Robot Log

private:
	struct button {
		int32_t state;
		int32_t pressed;
	};

	std::string m_commandPrefix;
	int						m_dashboardButtonCount;
	int						m_dashboardValueCount;
	int						m_robotStatusCount;
	int						m_robotValueCount;
	int						m_robotMode;

	button*				m_dashboardButton;
	double*				m_dashboardValue;
	char					m_log[100];
	int32_t*			m_robotStatus;
	double*				m_robotValue;
};

