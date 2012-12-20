#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMouseEvent> // needed to grab mouse input
#include <QMessageBox> // used to display info dialogs
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include "ui_MainWindow.h"
 #include <QApplication>
#include "GameWidget.h"
#include "MenuManager.h"


class MainWindow : public QMainWindow, public IObserver
{
	Q_OBJECT

private:
	Ui::MainWindowClass ui;
	MenuManager* menuManager;
	GameWidget* gameWidget;
	bool hasMouseLock;

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

	void onUpdate(float delta);
	void onEvent(Event* e);
	void event_showMessageBox(Event_ShowMessageBox* e);

protected:
	// Sends mouse movement as an event.
	void mouseMoveEvent(QMouseEvent* e);
	// Behavior on mouse press
	void mousePressEvent(QMouseEvent *e);
	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e);
	void moveEvent(QMoveEvent *e);
	void resizeEvent(QResizeEvent* e);;
	void showMenu();
	void keyReleaseEvent(QKeyEvent* e);

public slots:
		void slot_setTitle(QString title);
		void slot_toggleFullScreen();

private:
	void toggleMouseLock();
};

#endif // MAINWINDOW_H
