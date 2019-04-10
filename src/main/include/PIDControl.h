#pragma once

#include <frc/WPILib.h>

class PIDControl {
public:
	PIDControl(std::string name = "");
	~PIDControl();

	bool		AtSetpoint();
	double	Calculate(double input);
	double 	GetError();
	void		Reset();
	void 		SetCoefficient(char kWhich, double errorThreshold, double kAbove, double kBelow);
	void		SetInputRange(double inputMinimum, double inputMaximum);
	void		SetOutputRange(double outputMinimum, double outputMaximum, double lowErrorMinimum = 0);
	void 		SetOutputRamp(double rampMinimum, double rampStep);
	void		SetSetpoint(double setpoint, double inputNow, bool resetPID = true);
	void		SetSetpointDeadband(double deadband);
	void		SetShowOutput(bool enabled);

private:
	struct Coefficient {
		double		threshold;
		double		kAbove;
		double		kBelow;
		double		kNow;
	};

	Coefficient	m_P;
	Coefficient m_I;
	Coefficient m_D;

	double  		m_deadband;
	bool				m_errorIsPositive;
	double			m_errorPrev;
	double			m_errorTotal;
	double			m_inputMax;
	double			m_inputMin;
	char				m_log[100];
	std::string	m_name;
	double			m_outputMax;
	double 			m_outputMin;
	double			m_outputLowErrorMin;
	double  		m_rampMin;
	double  		m_rampNow;
	double  		m_rampStep;
	double			m_setpoint;
	bool				m_showOutput;
	bool				m_useRamp;
};







