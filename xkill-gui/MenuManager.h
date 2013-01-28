#pragma once

#include <QtGui/QKeyEvent> // needed to grab mouse input
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/WeaponStructs.h>

#include "Menu_Main.h"
#include "Menu_ScoreBoard.h"
#include "Menu_InGame.h"
#include <vector>

class Attribute_SplitScreen;

class HUDWindow : public QMainWindow
{
private:
	int id;
	QLabel *label_health;
    QLabel *label_ammo;
	QLabel *label_weaponType;
    QLabel *label_ammoType;
	Ammunition::AmmunitionType ammo;
	FiringMode::FiringModeType weapon;
	
	QHBoxLayout *horizontalLayout;

public:
	HUDWindow(QWidget* parent, int id);

	void parentMoveEvent(Attribute_SplitScreen* splitScreen);
	void update(Attribute_SplitScreen* player);
};

class HUDManager
{
private:
	std::vector<HUDWindow*> huds;
	QWidget* parent;
public:
	HUDManager();
	HUDManager(QWidget* parent);

	void update();

	void createHUD();

	void parentMoveEvent();
};


class MenuManager : public IObserver
{
private:
	Menu_Main* mainMenu;
	Menu_ScoreBoard* scoreBoard;
	Menu_InGame* inGameMenu;
	HUDManager hudManager;

public:
	MenuManager(QWidget* parent);

	void onEvent(Event* e);

	void onUpdate(float delta);

	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e);

	void keyReleaseEvent(QKeyEvent* e);

	void moveEvent();
};
