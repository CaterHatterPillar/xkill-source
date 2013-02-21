#include "Menu_Main2.h"

#include <QtGui/QStandardItemModel>

struct keyFrame
{
	Float2 pos;
	float duration;
};

void Menu_Main2::loadOpeningGif()
{
	openingAnimation = new QMovie(this);
	openingAnimation->setCacheMode(QMovie::CacheAll);
	std::string fileName = "../../xkill-resources/xkill-gui/images/animations/menu_opening.gif"; 
	openingAnimation->setFileName(fileName.c_str());
	if(!openingAnimation->isValid()) // error checking
		SHOW_MESSAGEBOX("Could not open " + fileName + ". Either the file is missing, or \"imageformats/qgif4.dll\" is missing.");
	openingAnimation->setParent(this); // prevents memory leaks
	ui.label_openingAnimation->setMovie(openingAnimation);
	openingAnimation->start();
	SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_OPENING_ANIMATION));
	connect(openingAnimation, SIGNAL(frameChanged(int)),	this, SLOT(openingAnimation_frameChanged(int)));
}

Menu_Main2::Menu_Main2( QWidget* parent ) : QMainWindow()
{
	// Init
	loadCustomFonts();
	this->parent = parent;
	ui.setupUi(this);
	QWidget::setWindowFlags(Qt::FramelessWindowHint);
	alwaysOnTop(true);
	QWidget::setAttribute(Qt::WA_TranslucentBackground);
	QWidget::show();
	loadOpeningGif();

	// Events
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_MOVE);
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);
	SUBSCRIBE_TO_EVENT(this, EVENT_ENABLE_MENU);


	//
	// Setup GUI
	//

	// center background 
	ui.label_background->move(0,0);
	ui.label_openingAnimation->move(0,0);

	// hide unused menus
	ui.frame_opening->hide();
	ui.frame_main->hide();
	ui.frame_start->hide();
	ui.frame_customize->hide();
	ui.frame_settings->hide();
	ui.frame_video->hide();
	ui.frame_audio->hide();
	ui.frame_input->hide();
	ui.frame_credits->hide();

	// show main menu
	push_menu(ui.frame_opening);

	// signals
	connect(ui.pushButton_start, SIGNAL(clicked()),	this, SLOT(slot_menu_start()));
	connect(ui.pushButton_startgame, SIGNAL(clicked()),	this, SLOT(slot_menu_startgame()));
	connect(ui.pushButton_customize, SIGNAL(clicked()),	this, SLOT(slot_menu_customize()));
	connect(ui.pushButton_settings, SIGNAL(clicked()),	this, SLOT(slot_menu_settings()));
	connect(ui.pushButton_video, SIGNAL(clicked()),	this, SLOT(slot_menu_video()));
	connect(ui.pushButton_audio, SIGNAL(clicked()),	this, SLOT(slot_menu_audio()));
	connect(ui.pushButton_input, SIGNAL(clicked()),	this, SLOT(slot_menu_input()));
	connect(ui.pushButton_credits, SIGNAL(clicked()),	this, SLOT(slot_menu_credits()));
	connect(ui.pushButton_quit, SIGNAL(clicked()),	this, SLOT(slot_menu_quit()));
	connect(ui.horizontalSlider_numPlayers, SIGNAL(valueChanged(int)),	this, SLOT(setNumPlayers(int)));

	// standard values
	ui.horizontalSlider_numPlayers->setValue(SETTINGS->numPlayers);

	// init sub-menus
	input_Menu = new Menu_Input(&ui, this);
	input_Menu->Init(new QStandardItemModel(0, 2, this), new QStandardItemModel(0, 1, this));
	ammo_Menu = new Menu_Ammo(&ui, this);
	firingMode_Menu = new Menu_FiringMode(&ui, this);
	sound_Menu = new Menu_Sound(&ui, this);

	// init level menu
	filePath = QString("../../xkill-resources/xkill-scripts/levels.xml");
	//editorModel->setHorizontalHeaderItem(1, new QStandardItem("ID"));
	QStringList columnNames;
	Event_GetFileList* fileList = new Event_GetFileList("../../xkill-resources/xkill-level/", ".mdldesc");
	SEND_EVENT(fileList);
	std::vector<std::string> filenames = fileList->filenames;
	delete fileList;
	levelListModel = new QStandardItemModel(0, filenames.size(), this);
	for(unsigned int i = 0; i < filenames.size(); i++)
	{
		std::string filename = filenames[i];
		int strIndex = std::string::npos;
		strIndex = filename.find_first_of(".");

		if(strIndex != std::string::npos)
			filename = filename.substr(0, strIndex);

		QString qStr = filename.c_str();
		QStandardItem* stdItem = new QStandardItem(qStr);

		levelNames.push_back(filename);
		levelListModel->appendRow(stdItem);
	}

	SEND_EVENT(&Event_LoadLevel(levelNames[0]));
	SETTINGS->currentLevel = levelNames[0];
	//ui.comboBox_LevelSelect->setModel(levelListModel);

}

void Menu_Main2::mousePressEvent( QMouseEvent *e )
{
	if(e->button() == Qt::RightButton)
	{
		pop_menu();
	}
	if(e->button() == Qt::LeftButton)
	{
		// Skip opening, if at opening (index 0)
		if(menuStack.size()==1)
			endOpening();
	}
}

void Menu_Main2::loadCustomFonts()
{
	QStringList list;
	list << "arcade_interlaced.ttf" << "arcade_rounded.ttf" << "arista_light.ttf";;
	int fontID(-1);
	bool fontWarningShown(false);
	for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator) 
	{
		QFile res(":/xkill/fonts/" + *constIterator);
		if(res.open(QIODevice::ReadOnly) == false)
		{
			if(fontWarningShown == false)
			{
				std::string fontName = (*constIterator).toStdString();
				SHOW_MESSAGEBOX("Problem loading custom font \"" + fontName + "\".");
				fontWarningShown = true;
			}
		} 
		else 
		{
			fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
			if (fontID == -1 && fontWarningShown == false)
			{
				SHOW_MESSAGEBOX("Problem loading custom font");
				fontWarningShown = true;
			}
		}
	}
}

void Menu_Main2::push_menu( QFrame* menu )
{
	// Hide previous menu, if any
	if(menuStack.size() > 0)
	{
		QFrame* topMenu = menuStack.back();
		topMenu->hide();
	}

	// Show new menu
	menu->move(0,0);
	menu->resize(width(), height());
	menu->show();
	menuStack.push_back(menu);

	SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_BUTTON_CLICK));
}

void Menu_Main2::pop_menu()
{
	// Make sure rot-menu is not poped
	if(menuStack.size() > 2)
	{
		// Pop current menu
		QFrame* topMenu = menuStack.back();
		topMenu->hide();
		menuStack.pop_back();

		// Show previous menu
		QFrame* menu = menuStack.back();
		menu->move(0,0);
		menu->resize(width(), height());
		menu->show();
	}
}

void Menu_Main2::menuResize()
{
	QFrame* topMenu = menuStack.back();

	// Resize background
	ui.label_openingAnimation->resize(width(), height());
	ui.label_background->resize(width(), height());
	ui.label_openingAnimation->lower();
	ui.label_background->lower();

	// Resize current menu
	topMenu->resize(width(), height());
}

void Menu_Main2::alwaysOnTop( bool on )
{
	if(on)
	{
		// Enable Window Stay on Top flag
		this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
	}
	else
	{
		// Disable Window Stay on Top flag
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
	}
}

void Menu_Main2::event_windowMove( Event_WindowMove* e )
{
	move(e->pos.x, e->pos.y);
}

void Menu_Main2::keyPressEvent( QKeyEvent *e )
{
	if(e->key() == Qt::Key_Escape)
	{
		pop_menu();
	}

	QCoreApplication::sendEvent(parent, e);
}

void Menu_Main2::keyReleaseEvent( QKeyEvent *e )
{
	QCoreApplication::sendEvent(parent, e);
}

void Menu_Main2::event_windowResize( Event_WindowResize* e )
{
	// Resize window
	resize(e->width, e->height);

	// Make sure menu is resized as well
	menuResize();
}

void Menu_Main2::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_ENABLE_MENU:
		if(((Event_EnableMenu*)e)->enableMenu)
		{
			// Refresh menu
			//input_Menu->setSettingsMenu();
			ammo_Menu->setSettingsMenu();
			firingMode_Menu->setSettingsMenu();
			//sound_Menu->setSettingsMenu();

			// Display menu
			this->show();
		}
		else
			this->hide();
		break;
	case EVENT_WINDOW_MOVE:
		event_windowMove((Event_WindowMove*)e);
		break;
	case EVENT_WINDOW_RESIZE:
		event_windowResize((Event_WindowResize*)e);
		break;
	default:
		break;
	}
}

Menu_Main2::~Menu_Main2()
{
	UNSUBSCRIBE_TO_EVENTS(this);

	delete input_Menu;
	delete ammo_Menu;
	delete firingMode_Menu;
	delete sound_Menu;
}

void Menu_Main2::closeEvent( QCloseEvent* event )
{
	SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));
}