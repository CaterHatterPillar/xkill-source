#include "MenuManager.h"

#include <xkill-utilities/EventManager.h>


ATTRIBUTES_DECLARE_ALL;

HUDWindow::HUDWindow(QWidget* parent, int id) : QMainWindow(parent)
{
	this->id = id;

	//QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
	QWidget::setWindowFlags(Qt::SplashScreen);
	setWindowFlags(Qt::WindowStaysOnBottomHint);
	QWidget::setStyleSheet("QWidget{color: #fff; background-color: #000;}");
	show();



	//
	// build menu
	//

	horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(0);
	horizontalLayout->setMargin(0);
	verticalLayout	= new QVBoxLayout();
	verticalLayout->setSpacing(0);
	horizontalLayout->setMargin(0);
	horizontalLayout->addLayout(verticalLayout);

	// health
	progressBar_health = new QProgressBar();
	verticalLayout->addWidget(progressBar_health);
	progressBar_health->setValue(50);
	progressBar_health->setMaximumSize(QSize(50, 10));
	progressBar_health->setTextVisible(false);
	progressBar_health->setStyleSheet(QString::fromUtf8("QProgressBar::chunk \n"
"{\n"
"	background-color: rgb(0, 170, 0);\n"
"	border: 1px solid black;\n"
"}\n"
"\n"
"QProgressBar\n"
"{\n"
"	border: 0px solid black;\n"
"	background: rgb(0, 0, 0);\n"
"}"));
	

	// ammo
	progressBar_ammo = new QProgressBar();
	verticalLayout->addWidget(progressBar_ammo);
	progressBar_ammo->setValue(50);
    progressBar_ammo->setMaximumSize(QSize(50, 10));
	progressBar_ammo->setTextVisible(false);
    progressBar_ammo->setStyleSheet(QString::fromUtf8("QProgressBar::chunk \n"
"{\n"
"	background-color: rgb(200, 170, 0);\n"
"	border: 1px solid black;\n"
"}\n"
"\n"
"QProgressBar\n"
"{\n"
"	border: 0px solid black;\n"
"	background: rgb(0, 0, 0);\n"
"}"));
	

	// ammo type
	label_ammoType = new QLabel();
	label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_explosive2.png")));
	
	label_ammoType->setScaledContents(true);
	horizontalLayout->addWidget(label_ammoType);
	
	// weapon type
	label_weaponType = new QLabel();
	label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_auto.png")));
	
	label_weaponType->setScaledContents(true);
	horizontalLayout->addWidget(label_weaponType);

	QWidget* mainWidget = new QWidget();
	mainWidget->setLayout(horizontalLayout);
	setCentralWidget(mainWidget);

	resize(horizontalLayout->minimumSize());
}

void HUDWindow::parentMoveEvent(Attribute_SplitScreen* splitScreen)
{
	float sizeScale = (float) splitScreen->ssHeight / 1000;
	sizeScale = 0.75f*sizeScale + 0.25f;

	int x = 20*sizeScale + splitScreen->ssTopLeftX;
	int y = splitScreen->ssTopLeftY + splitScreen->ssHeight - this->height() - 20*sizeScale;
	move(x, y);
}

void HUDWindow::update(Attribute_SplitScreen* splitScreen)
{
	Attribute_Player*		player		=	itrPlayer		.at(splitScreen->ptr_player);
	Attribute_Health*		health		=	itrHealth		.at(player->ptr_health);
	Attribute_WeaponStats*	weaponStats	=	itrWeaponStats	.at(player->ptr_weaponStats);

	Ammunition* ammunition = &weaponStats->ammunition[weaponStats->currentAmmunitionType];
	FiringMode* firingMode = &weaponStats->firingMode[weaponStats->currentFiringModeType];

	float sizeScale = (float) splitScreen->ssHeight / 1000;
	sizeScale = 0.50f*sizeScale + 0.50f;
	QSize iconSize((int)32*sizeScale, (int)32*sizeScale);
	label_weaponType->setMaximumSize(iconSize);
	label_ammoType->setMaximumSize(iconSize);
	progressBar_health->setMaximumSize(QSize(200*sizeScale, 16*sizeScale));
	progressBar_ammo->setMaximumSize(QSize(200*sizeScale, 16*sizeScale));
	progressBar_health->setMinimumSize(QSize(200*sizeScale, 16*sizeScale));
	progressBar_ammo->setMinimumSize(QSize(200*sizeScale, 16*sizeScale));

	// health & ammo bars
	int healthRatio = (int)((health->health / health->startHealth) * 100);
	int ammoRatio = (int)(((float)firingMode->nrOfShotsLeftInClip / firingMode->clipSize) * 100);
	progressBar_health->setValue(healthRatio);
	progressBar_ammo->setValue(ammoRatio);
	progressBar_health->update();
	progressBar_ammo->update();

	// ammo icon
	if(ammo != weaponStats->currentAmmunitionType)
	{
		ammo = weaponStats->currentAmmunitionType;

		if(ammo == Ammunition::AmmunitionType::BULLET)
		{
			label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_bullet.png")));
		}
		if(ammo == Ammunition::AmmunitionType::SCATTER)
		{
			label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_scatter.png")));
		}
		if(ammo == Ammunition::AmmunitionType::EXPLOSIVE)
		{
			label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_explosive.png")));
		}
	}

	// weapon icon
	if(weapon != weaponStats->currentFiringModeType)
	{
		weapon = weaponStats->currentFiringModeType;

		if(weapon == FiringMode::FiringModeType::SINGLE)
		{
			label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_single.png")));
		}
		if(weapon == FiringMode::FiringModeType::SEMI)
		{
			label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_semi.png")));
		}
		if(weapon == FiringMode::FiringModeType::AUTO)
		{
			label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_auto.png")));
		}
	}
	resize(horizontalLayout->minimumSize());
}

HUDManager::HUDManager()
{
	//Intentionally left blank
}

HUDManager::HUDManager(QWidget* parent)
{
	ATTRIBUTES_INIT_ALL;

	this->parent = parent;
}

void HUDManager::update()
{
	// Balance attributes / vs huds
	int num_splitScreen = itrSplitScreen.size();
	while(num_splitScreen>huds.size())
	{
		huds.push_back(new HUDWindow(parent, huds.size()));
	}
	while(num_splitScreen<huds.size())
	{
		delete huds.back();
		huds.pop_back();
	}

	// Update huds
	int index = 0;
	while(itrSplitScreen.hasNext())
	{
		Attribute_SplitScreen* splitScreen = itrSplitScreen.getNext();
		huds[index]->update(splitScreen);
		huds[index]->parentMoveEvent(splitScreen);
		index++;
	}
}

void HUDManager::createHUD()
{
	for(int i=0; i<5; i++)
	{
		huds.push_back(new HUDWindow(parent, i));
	}
}

void HUDManager::parentMoveEvent()
{
	update();
}


MenuManager::MenuManager( QWidget* parent )
{
	scoreBoard = new Menu_ScoreBoard(parent);
	inGameMenu = new Menu_InGame(parent);
	mainMenu = new Menu_Main(parent);
	hudManager = HUDManager(parent);
	
	mainMenu->toggleMenu(true);

	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_GAME_OVER);
}

void MenuManager::keyPressEvent( QKeyEvent* e )
{
	if(GET_STATE() == STATE_DEATHMATCH)
	{
		//JON: Part of STATE_DEATHMATCH in synced FSM. I.e DeathmatchMenuState::onEvent should listen to keyPressEvents
		switch (e->key()) 
		{
		case Qt::Key_Escape:
			inGameMenu->toggleMenu(); //JON: Set flag in for triggering state transition on update?
			inGameMenu->setWindowFlags(Qt::WindowStaysOnTopHint);
			break;
		case Qt::Key_Tab:
			scoreBoard->toggleMenu(true);
			break;
		default:
			break;
		}
	}
	if(GET_STATE() == STATE_GAMEOVER)
	{
		switch (e->key())
		{
		case Qt::Key_Escape:
			//SEND_EVENT(&Event(EVENT_END_DEATHMATCH));

			GET_STATE() = STATE_MAINMENU;
			SEND_EVENT(&Event_EndDeathmatch());
			SEND_EVENT(&Event_StartDeathmatch(0));	//To get a black background, for now run the game with zero players
			break;
		default:
			break;
		}
	}
}

void MenuManager::keyReleaseEvent( QKeyEvent* e )
{
	switch (e->key()) 
	{

	case Qt::Key_Tab:
		scoreBoard->toggleMenu(false);
		break;
	default:
		break;
	}
}

void MenuManager::onEvent( Event* e )
{
	EventType type = e->getType();
	static int refreshRate = 2;
	static int test = refreshRate;
	switch(type) 
	{
	case EVENT_UPDATE:
		// HACK: Makes the menu update every 20 frame
		test--;
		if(test<0)
		{
			hudManager.update();
			scoreBoard->onUpdate(1.0f);
			test = refreshRate;
		}
		break;
	case EVENT_END_DEATHMATCH:
		scoreBoard->toggleMenu(false);
		inGameMenu->toggleMenu(false);
		mainMenu->toggleMenu(true);
		break;
	case EVENT_GAME_OVER:
		scoreBoard->toggleMenu(true);
		scoreBoard->onUpdate(0.01f);
		inGameMenu->toggleMenu(false);
		mainMenu->toggleMenu(false);
	default:
		break;
	}
}

void MenuManager::onUpdate( float delta )
{
	//Intentionally left blank
}

void MenuManager::moveEvent()
{
	mainMenu->parentMoveEvent();
	scoreBoard->parentMoveEvent();
	inGameMenu->parentMoveEvent();
	hudManager.parentMoveEvent();
}

