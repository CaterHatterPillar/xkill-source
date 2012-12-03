#include "InputAxisObject.h"

InputAxisObject::InputAxisObject(int minValue, int maxValue)
{
	minValue_ = minValue;
	maxValue_ = maxValue;

	deadZone_ = 0.25f;
	inverted_ = false;

	value_ = 0.0f;
}

InputAxisObject::~InputAxisObject(void)
{
}

void InputAxisObject::SetValue(float value)
{
	if(value > 1.0f)
		value_ = 1.0f;
	else if(value < -1.0f)
		value_ = -1.0f;
	else
		value_ = value;
}

void InputAxisObject::SetValue(int value)
{
	value_ = formatValue(value);
}

float InputAxisObject::GetValue()
{
	return value_;
}

InputObject::InputObjectType InputAxisObject::GetType()
{
	return AXIS_OBJECT;
}

void InputAxisObject::SetDeadZone(float deadZone)
{
	if(deadZone > 1.0f)
		deadZone_ = 1.0f;
	else if(deadZone < 0.0f)
		deadZone_ = 0.0f;
	else
		deadZone_ = deadZone;
}

void InputAxisObject::SetInverted(bool inverted)
{
	inverted_ = inverted;
}

float InputAxisObject::formatValue(int value) //Fix deadzone, is square for the moment
{
	double doubleAxis = value;
	double interval = (double)(maxValue_ - minValue_);

	doubleAxis = doubleAxis - ((double)minValue_);
	doubleAxis = doubleAxis - (interval/2.0);
	doubleAxis = doubleAxis / (interval/2.0);

	if(doubleAxis < -0.95)
		doubleAxis = -1.0;
	else if(doubleAxis > 0.95)
		doubleAxis = 1.0;

	if(std::abs(doubleAxis) < deadZone_)
		doubleAxis = 0.0;

	if(inverted_)
		doubleAxis = -doubleAxis;

	return (float)doubleAxis;
}