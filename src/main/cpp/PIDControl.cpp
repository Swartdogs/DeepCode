#include "PIDControl.h"

#include <frc/WPILib.h>
#include <stdio.h>
#include <math.h>
#include "Robot.h"

// ************************* PUBLIC FUNCTIONS *************************

PIDControl::PIDControl(std::string name) {
	m_P.threshold       = 0;
	m_P.kAbove          = 0;
	m_P.kBelow          = 0;
	m_P.kNow            = 0;

	m_I.threshold       = 0;
	m_I.kAbove          = 0;
	m_I.kBelow          = 0;
	m_I.kNow            = 0;

	m_D.threshold       = 0;
	m_D.kAbove          = 0;
	m_D.kBelow          = 0;
	m_D.kNow            = 0;

	m_deadband          = 0;
	m_errorIsPositive   = false;
	m_errorPrev         = 0;
	m_errorTotal        = 0;
	m_inputMax          = 0;
	m_inputMin          = 0;
	m_name              = name;
	m_outputMax         = 1.0;
	m_outputMin         = -1.0;
	m_outputLowErrorMin = 0;
	m_rampMin           = 0;
	m_rampNow           = 0;
	m_rampStep          = 0;
	m_setpoint          = 0;
	m_showOutput        = false;
	m_useRamp           = false;
}

PIDControl::~PIDControl() {
}

bool PIDControl::AtSetpoint() {
	return (fabs(m_errorPrev) <= m_deadband);
}

double PIDControl::Calculate(double input) {											    // Calculate output based on input
	double error = m_setpoint - input;		                              // Current Error
	double errorDiff = error - m_errorPrev;                             // Difference between current error and previous error
	std::string limit = "";

	if (m_P.threshold > 0) {															              // Threshold > 0 indicates use of 2 coefficients
		(fabs(error) < m_P.threshold) ? m_P.kNow = m_P.kBelow :           // 'P' coefficient below threshold
			m_P.kNow = m_P.kAbove;								                          //				   above threshold
	}

	if (m_I.threshold > 0) {                                            // Same for 'I' coefficient
		(fabs(error) < m_I.threshold) ? m_I.kNow = m_I.kBelow : m_I.kNow = m_I.kAbove;
	}

	if (m_D.threshold > 0) {                                            // Same for 'D' coefficient
		(fabs(error) < m_D.threshold) ? m_D.kNow = m_D.kBelow : m_D.kNow = m_D.kAbove;
	}

	if (m_I.kNow == 0) {                                                // Update Total Error accumulation
		m_errorTotal = 0;
  }	else {                                                            // Limit 'I' output to within Min-to-Max Output range
		m_errorTotal = std::max(m_outputMin / m_I.kNow, std::min(m_errorTotal + error, m_outputMax / m_I.kNow));
	}
	                                                                    // Calculate Output
	double output = m_P.kNow * error + m_I.kNow * m_errorTotal + m_D.kNow * errorDiff;

	if (m_useRamp) {														                        // Output ramping enabled
		m_rampNow = std::min(m_rampNow + m_rampStep, 1.0);                // Limit output change to size of ramp Step
		m_useRamp = (m_rampNow < output && m_rampNow < 1.0);              // Disable ramping when output >= ramp value

		output = std::copysign(std::min(fabs(output), m_rampNow), output);
		limit += "Ramp ";
	}

	if (output > m_outputMax)	{                                         // Limit output to within Minimum-Maximum range
		output = m_outputMax;
		limit += "OutMax ";
	} else if (output < m_outputMin) {
		output = m_outputMin;
		limit += "OutMin ";
	} else if (!m_useRamp) {                                            // Ramping disabled or completed
		if (fabs(output) < m_outputLowErrorMin) {                         // Keep output >= Low Error minimum
			output = m_errorIsPositive ? m_outputLowErrorMin : -m_outputLowErrorMin;
			limit += "LowErrorMin ";
		}
	}

	if (m_showOutput)	{                                                 // Show Output
		if (limit.length() > 1)	limit = limit.substr(0, limit.length() - 1);

		sprintf(m_log, "%s PID: Error=%7.1f  P=%6.2f  I=%6.2f  D=%6.2f  Output=%6.2f (%s)", m_name.c_str(), error,
				m_P.kNow * error, m_I.kNow * m_errorTotal, m_D.kNow * errorDiff, output, limit.c_str());
		Robot::m_robotLog.WritePid(m_log);
	}

	m_errorPrev = error;                                                // Set Previous error for next calculation

	return output;
}

double PIDControl::GetError() {
	return m_errorPrev;
}

void PIDControl::Reset() {
	m_errorTotal = 0;
	m_rampNow = m_rampMin;
}

void PIDControl::SetCoefficient(char kWhich, double threshold, double kAbove, double kBelow) {
	switch (kWhich) {
    case 'D':                                                         // D Coefficient ('D' or 'd')
    case 'd':
      m_D.threshold = fabs(threshold);
      m_D.kAbove = fabs(kAbove);
      m_D.kBelow = fabs(kBelow);
      m_D.kNow = m_D.kAbove;
      break;

    case 'I':                                                         // I Coefficient ('I' or 'i')
    case 'i':
      m_I.threshold = fabs(threshold);
      m_I.kAbove = fabs(kAbove);
      m_I.kBelow = fabs(kBelow);
      m_I.kNow = m_I.kAbove;
      break;

    case 'P':                                                         // P Coefficient ('P' or 'p')
    case 'p':
      m_P.threshold = fabs(threshold);
      m_P.kAbove = fabs(kAbove);
      m_P.kBelow = fabs(kBelow);
      m_P.kNow = m_P.kAbove;
      break;

    default:;                                                         // Ignore if not P, I, or D
	}
}

void PIDControl::SetInputRange(double inputMinimum, double inputMaximum) {
	m_inputMin = inputMinimum;
	m_inputMax = inputMaximum;
}

void PIDControl::SetOutputRange(double outputMinimum, double outputMaximum, double lowErrorMinimum) {
	m_outputMin = std::copysign(std::min(fabs(outputMinimum), 1.0), outputMinimum);
	m_outputMax = std::copysign(std::min(fabs(outputMaximum), 1.0), outputMaximum);
	m_outputLowErrorMin = fabs(lowErrorMinimum);
}

void PIDControl::SetOutputRamp(double rampMinimum, double rampStep) {
	m_rampMin = fabs(rampMinimum);
	m_rampStep = fabs(rampStep);
}

void PIDControl::SetSetpoint(double setpoint, double inputNow, bool resetPID) {
	if (m_inputMax > m_inputMin) {
		if (setpoint > m_inputMax) {
			setpoint = m_inputMax;
		} else if (setpoint < m_inputMin) {
			setpoint = m_inputMin;
		}
	}

	m_setpoint = setpoint;					                                    // Store new setpoint
	m_errorPrev = m_setpoint - inputNow;	                              // Determine previous error for use in next 'D' calculation
	m_errorIsPositive = (m_errorPrev >= 0);                             // Determine +/- sign of output used with Low Error Minimum

	if (resetPID)	{						                                          // Reset PID parameters (Default = True)
		m_errorTotal = 0;	                                                // Total Error used for 'I' calculation
		m_rampNow = m_rampMin;                                            // Variables used for output ramping
		m_useRamp = (m_rampMin > 0);
	}
}

void PIDControl::SetSetpointDeadband(double deadband) {
	m_deadband = fabs(deadband);
}

void PIDControl::SetShowOutput(bool enabled) {
	m_showOutput = enabled;
}
