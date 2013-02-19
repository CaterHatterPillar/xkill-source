#pragma once

#include <QtGui/QMainWindow>
#include <QtGui/QKeyEvent>
#include <QtGui/QMovie>
#include <QtGui/QFontDatabase>
#include <xkill-utilities/Util.h>
#include "ui_MainMenu2.h"
#include <vector>

#include "Menu_Input.h"
#include "Menu_Ammo.h"
#include "Menu_FiringMode.h"
#include "Menu_Sound.h"

class Menu_Main2 : public QMainWindow, IObserver
{
	Q_OBJECT

private:
	Ui::MainWindow ui;
	QWidget* parent;
	std::vector<QFrame*> menuStack; // Used as state machine to return to the previous menu
	QMovie* openingAnimation;

	// Level menu
	std::vector<std::string> levelNames;
	QStandardItemModel* levelListModel;
	QString filePath;

	Menu_Input*			input_Menu;
	Menu_Ammo*			ammo_Menu;
	Menu_FiringMode*	firingMode_Menu;
	Menu_Sound*			sound_Menu;

	void loadCustomFonts();
	void loadOpeningGif();
	void push_menu(QFrame* menu);
	void pop_menu();
	void menuResize();
public:
	Menu_Main2(QWidget* parent);
	~Menu_Main2();

	void onEvent(Event* e);
	void event_windowMove(Event_WindowMove* e);
	void alwaysOnTop(bool on);
	void closeEvent(QCloseEvent* event);
	void event_windowResize(Event_WindowResize* e);

protected:
	void mousePressEvent(QMouseEvent* e);
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

private slots:
	void slot_menu_startgame()
	{
		// Apply menu settings
		SETTINGS->cycleLimit = ui.horizontalSlider_cycleLimit->value();
		SETTINGS->timeLimit = ui.horizontalSlider_timeLimit->value() * 60.0f;
		SETTINGS->numPlayers = ui.horizontalSlider_numPlayers->value();
		AttributeIterator<Attribute_Player> itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
		while(itrPlayer.hasNext())
		{
			itrPlayer.getNext()->respawnTimer.setStartTime(static_cast<float>(ui.horizontalSlider_respawnTime->value()));
		}

		// Start game
		SEND_EVENT(&Event(EVENT_STARTGAME));
	}
	void openingAnimation_frameChanged(int frameNumber )
	{
		int lastFrame = openingAnimation->frameCount()-1;

		// Stop animaiton when last frame is reached
		if(frameNumber >= lastFrame)
		{
			endOpening();
		}
	}
	void endOpening()
	{
		int lastFrame = openingAnimation->frameCount()-1;
		openingAnimation->jumpToFrame(lastFrame);
		openingAnimation->stop();
		ui.label_openingAnimation->hide();

		slot_menu_main();
	}
	void setNumPlayers(int numPlayers)
	{
		SETTINGS->numPlayers = numPlayers;
	}
	void slot_menu_main()
	{
		push_menu(ui.frame_main);
	}
	void slot_menu_start()
	{
		push_menu(ui.frame_start);
	}
	void slot_menu_settings()
	{
		push_menu(ui.frame_settings);
	}
	void slot_menu_video()
	{
		push_menu(ui.frame_video);
	}
	void slot_menu_audio()
	{
		push_menu(ui.frame_audio);
	}
	void slot_menu_customize()
	{
		push_menu(ui.frame_customize);
	}
	void slot_menu_input()
	{
		push_menu(ui.frame_input);
	}
	void slot_menu_credits()
	{
		push_menu(ui.frame_credits);
	}
	void slot_menu_quit()
	{
		SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));
	}
};