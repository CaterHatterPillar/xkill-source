#include "SoundComponent.h"

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-sound/FMODEventSystem.h> //check architecture project depending on sound
#include <xkill-sound/FMODEventSystemProgrammerReportParser.h> //check architecture project depending on sound

#include <iostream>

#include "FileParser.h"

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

SoundComponent::SoundComponent()
{
	mFMODEventSystem = NULL;
}

SoundComponent::~SoundComponent()
{
	SAFE_DELETE(mFMODEventSystem);
}

bool SoundComponent::init()
{
	mFMODEventSystem = new FMODEventSystem();
	mFMODEventSystem->Init("../xkill-dependencies/sound/", "testproject.fev", 64);

	FMODEventSystemProgrammerReportParser fmodEventSystemProgrammerReportParser;
	if(!fmodEventSystemProgrammerReportParser.parseProgrammerReport(mFMODEventSystem))
	{
		std::cout << "parsing of FMOD Designer's programmer's report failed." << std::endl;
		return false;
	}

	FileParser fp;
	fp.setFileName("sound.cfg");
	fp.setFilePath("../../xkill-resources/xkill-configs/");
	if(fp.startReading())
	{
		while(!fp.isEmpty())
		{
			std::string tmp = fp.getNextRow();
		}
	}

	fp.startWriting();
	fp.writeRow("Hej");
	fp.writeRow("H�r �r vi");
	fp.writeRow("Sveriges b�sta lag");
	fp.writeRow("Vi ska ta guld �n en g�ng");
	fp.writeRow("Och vi firar hela natten l�ng");
	fp.writeRow("Tillsammans");
	fp.doneWriting();

	return true;
}

void SoundComponent::onEvent(Event* e)
{
	//mFMODEventSystem->StartSoundEventAt(0);
}

void SoundComponent::onUpdate(float delta)
{
	mFMODEventSystem->Update();
}