#pragma once

#include "InputDevice.h"

class InputObject;
class InputAxisObject;
class InputButtonObject;
class InputTriggerObject;

//! A wrapper for a XInput device
/*!

*/

class XInputDevice : public InputDevice
{
	XInputDevice();
public:
	XInputDevice(int deviceNr, GUID deviceGUID, std::string name, unsigned int playerID);
	virtual ~XInputDevice(void);

	virtual void Update(float deltaTime);

	virtual void RunForceFeedback();
	virtual bool StopForceFeedback();
	void SetForceFeedback(float leftMotorScale, float rightMotorScale);
	virtual bool IsForceFeedbackCapable();

	virtual void setStandardMappings();

	/*!
	\return XINPUT_DEVICE
	*/
	virtual InputDeviceType GetType();

private:
	int deviceNr_;
	int rightFFMotor_, leftFFMotor_;
	bool forceFeedbackOn_;

	virtual std::string getStandardMappingsString();

	virtual void updateState();
	virtual void createInputLayout();
	virtual void createInputObjectsFromLayout();
};

