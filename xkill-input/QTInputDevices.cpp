#include "QTInputDevices.h"

#include <math.h>

#include "InputButtonObject.h"
#include "InputAxisObject.h"
#include "InputTriggerObject.h"

#include "InputObjectArray.h"

#include "InputActions.h"

#include <xkill-utilities/Converter.h>

QTInputDevices::QTInputDevices() :
	InputDevice(GUID(), "")
{
	createInputLayout();
	createInputObjectsFromLayout();
}

QTInputDevices::QTInputDevices(GUID deviceGUID, std::string name, unsigned int playerID) : 
	InputDevice(deviceGUID, name, playerID)
{
	createInputLayout();
	createInputObjectsFromLayout();
}

QTInputDevices::~QTInputDevices()
{

}

void QTInputDevices::Update(float deltaTime)
{
	InputDevice::Update(deltaTime);
	updateState();
}

void QTInputDevices::setStandardMappings()
{
	if(axes_.size() >= 2)
	{
		axes_[0]->addMapping(InputAction::ACTION_F_LOOK_LR);
		axes_[0]->setDeadZone(0.0f);
		axes_[0]->setSensitivity(0.01f);
		axes_[1]->addMapping(InputAction::ACTION_F_LOOK_UD);
		axes_[1]->setDeadZone(0.0f);
		axes_[1]->setSensitivity(0.01f);
	}

	if(buttons_.size() >= (unsigned int)inputLayout_.nrOfButtons)
	{
		buttons_[0]->addMapping(InputAction::ACTION_B_WALK_FORWARD);
		buttons_[1]->addMapping(InputAction::ACTION_B_WALK_LEFT);
		buttons_[2]->addMapping(InputAction::ACTION_B_WALK_BACKWARD);
		buttons_[3]->addMapping(InputAction::ACTION_B_WALK_RIGHT);
		buttons_[4]->addMapping(InputAction::ACTION_B_JUMP_JETPACK);
		buttons_[5]->addMapping(InputAction::ACTION_B_NEXT_AMMUNITIONTYPE);
		buttons_[6]->addMapping(InputAction::ACTION_B_NEXT_FIRINGMODE);
		buttons_[9]->addMapping(InputAction::ACTION_B_SPRINT);

		buttons_[10]->addMapping(InputAction::ACTION_B_TIME_SPEED_UP);
		buttons_[11]->addMapping(InputAction::ACTION_B_TIME_SPEED_DOWN);
		buttons_[12]->addMapping(InputAction::ACTION_B_RELOAD);
		buttons_[13]->addMapping(InputAction::ACTION_B_PREV_AMMUNITIONTYPE);
		buttons_[14]->addMapping(InputAction::ACTION_B_PREV_FIRINGMODE);

		buttons_[15]->addMapping(InputAction::ACTION_B_FIRE);
		buttons_[16]->addMapping(InputAction::ACTION_B_LOW_SENSITIVITY);
		buttons_[18]->addMapping(InputAction::ACTION_B_NEXT_AMMUNITIONTYPE);
		buttons_[19]->addMapping(InputAction::ACTION_B_PREV_AMMUNITIONTYPE);
		buttons_[20]->addMapping(InputAction::ACTION_B_NEXT_FIRINGMODE);
		buttons_[21]->addMapping(InputAction::ACTION_B_PREV_FIRINGMODE);

		buttons_[22]->addMapping(InputAction::ACTION_B_SPRINT);
		buttons_[23]->addMapping(InputAction::ACTION_B_SHOW_SCOREBOARD);
	}
}

InputDevice::InputDeviceType QTInputDevices::GetType()
{
	return QT_INPUT_DEVICE;
}

std::string QTInputDevices::getStandardMappingsString()
{
	QTInputDevices qtDevice;

	qtDevice.setStandardMappings();
	std::vector<InputObject*> inputObjects = qtDevice.inputObjectArray_->inputObjects;

	std::string str = "";

	for(unsigned int i = 0; i < inputObjects.size(); i++)
	{
		std::vector<int>* mappings = inputObjects.at(i)->getMappings();

		for(unsigned int j = 0; j < mappings->size(); j++)
		{
			str += Converter::IntToStr(mappings->at(j));
		}

		str += inputObjects[i]->getName();
		str += Converter::FloatToStr(inputObjects[i]->getSensitivity());
		str += Converter::IntToStr(static_cast<int>(inputObjects[i]->isInverted()));
	}

	return str;
}

void QTInputDevices::updateState()
{
	//Will be done through events in InputComponent
}

void QTInputDevices::createInputLayout()
{
	inputLayout_.nrOfHatSwitches = 0;
	inputLayout_.nrOfButtons = 17 + 7; // 7 = Mouse buttons
	inputLayout_.nrOfTriggers = 0;
	inputLayout_.nrOfAxes = 2;
}

void QTInputDevices::createInputObjectsFromLayout()
{
	for(int i = 0; i < inputLayout_.nrOfAxes; i++)
	{
		InputAxisObject* axis = new InputAxisObject(-0x7FFF, 0x7FFF, true);
		axes_.push_back(axis);
		inputObjectArray_->inputObjects.push_back(axis);
	}

	if(inputLayout_.nrOfAxes >= 2)
	{
		axes_[0]->setName("Mouse X");
		axes_[1]->setName("Mouse Y");
	}
	
	for(int i = 0; i < inputLayout_.nrOfButtons; i++)
	{
		InputButtonObject* button = new InputButtonObject(i);
		buttons_.push_back(button);
		inputObjectArray_->inputObjects.push_back(button);
	}

	if(buttons_.size() >= (unsigned int)inputLayout_.nrOfButtons)
	{
		buttons_[0]->setKey('W');
		buttons_[1]->setKey('A');
		buttons_[2]->setKey('S');
		buttons_[3]->setKey('D');
		buttons_[4]->setKey(0x20); //Space
		buttons_[4]->setName("Space");
		buttons_[5]->setKey('Q');
		buttons_[6]->setKey('E');
		buttons_[7]->setKey('K');
		buttons_[8]->setKey('F');
		buttons_[9]->setKey('T');
		buttons_[10]->setKey(43);
		buttons_[11]->setKey(45);
		buttons_[12]->setKey('R');
		buttons_[13]->setKey('Z');
		buttons_[14]->setKey('C');

		buttons_[15]->setKey('�');	//Fix
		buttons_[15]->setName("Left mouse button");
		mouseButtons_.push_back(buttons_[15]);

		buttons_[16]->setKey('�');
		buttons_[16]->setName("Right mouse button");
		mouseButtons_.push_back(buttons_[16]);

		buttons_[17]->setKey('�');
		buttons_[17]->setName("Middle mouse button");
		mouseButtons_.push_back(buttons_[17]);

		buttons_[18]->setKey('�');
		buttons_[18]->setName("Mouse button 4");
		mouseButtons_.push_back(buttons_[18]);

		buttons_[19]->setKey('�');
		buttons_[19]->setName("Mouse button 5");
		mouseButtons_.push_back(buttons_[19]);

		buttons_[20]->setKey('�');
		buttons_[20]->setName("Mouse scroll up");
		mouseButtons_.push_back(buttons_[20]);

		buttons_[21]->setKey('�');
		buttons_[21]->setName("Mouse scroll down");
		mouseButtons_.push_back(buttons_[21]);

		buttons_[22]->setKey('�');
		buttons_[22]->setName("Shift");
		
		buttons_[23]->setKey('�');
		buttons_[23]->setName("Tab");
	}
}

void QTInputDevices::setAxis(unsigned int index, float value)
{
	if(axes_.size() > index)
	{
		axes_[index]->AddValue(value);
	}
}

void QTInputDevices::setButton(char key, bool value)
{
	for(unsigned int i = 0; i < buttons_.size(); i++)
	{
		if(buttons_[i]->getKey() == key)
		{
			buttons_[i]->SetValue(value, false);
		}
	}
}

void QTInputDevices::setMouseButton(unsigned int nr, bool value)
{
	for(unsigned int i = 0; i < mouseButtons_.size(); i++)
	{
		if(static_cast<int>(std::pow(2, i)) & nr)
		{
			mouseButtons_[i]->SetValue(value);
		}
	}
}

void QTInputDevices::setAxesToZero()
{
	for(unsigned int i = 0; i < axes_.size(); i++)
	{
		axes_[i]->SetValue(0.0f);
	}
}

void QTInputDevices::updateButtons()
{
	for(unsigned int i = 0; i < buttons_.size(); i++)
		buttons_[i]->SetValue(buttons_[i]->isDown());
}

void QTInputDevices::updateScroll() // Shouldn't hard code the scroll indices
{
	if(buttons_[20]->isDown() && !buttons_[20]->isPressed())
		buttons_[20]->SetValue(false, false);
	if(buttons_[21]->isDown() && !buttons_[21]->isPressed())
		buttons_[21]->SetValue(false, false);
}

InputButtonObject* QTInputDevices::getButtonObject(unsigned int index)
{
	InputButtonObject* button = nullptr;

	for(unsigned int i = 0; i < buttons_.size(); i++)
	{
		if(buttons_[i]->getKey() == (char)index)
		{
			button = buttons_[i];
			break;
		}
	}

	if(button == nullptr)
	{
		button = new InputButtonObject(index);
		buttons_.push_back(button);
		inputObjectArray_->inputObjects.push_back(button);
		inputLayout_.nrOfButtons++;
	}

	return button;
}
  
void QTInputDevices::RunForceFeedback()
{

}
bool QTInputDevices::StopForceFeedback()
{
	return true;
}
void QTInputDevices::SetForceFeedback(float leftMotorScale, float rightMotorScale)
{

}
bool QTInputDevices::IsForceFeedbackCapable()
{
	return false;
}