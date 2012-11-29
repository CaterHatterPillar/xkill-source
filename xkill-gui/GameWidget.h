#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget.h>
#include <QResizeEvent>
#include <QMouseEvent>

#include <QTime>
#include <QTimer> // needed to implement frame rate

#include "GameTimer.h"


class GameWidget : public QWidget
{
	Q_OBJECT

private:
	GameTimer gameTimer;
	QTimer* updateTimer;
	bool hasMouseLock;

public:
	GameWidget(QWidget* parent = 0, Qt::WFlags flags = 0) : QWidget(parent, flags)
	{
		// make widget non-transparent & draw directly onto screen
		QWidget::setAttribute(Qt::WA_OpaquePaintEvent);
		QWidget::setAttribute(Qt::WA_PaintOnScreen);

		// init updateTimer
		updateTimer = new QTimer(this);
		updateTimer->setInterval(0);
		connect(updateTimer, SIGNAL(timeout()), this, SLOT(slot_onUpdate()));
		updateTimer->start();

		// init gameTimer
		gameTimer.reset();
	};
	~GameWidget()
	{
	};
	
	QPaintEngine* paintEngine() const {return 0;}; // overrides Qt:s own paint engine; prevents flicker

public slots:
	// Runs every time gameTimer times out
	void slot_onUpdate()
	{
		gameTimer.tick();
		float delta = gameTimer.getDeltaTime();
		computeFPS();
	};

protected:
	void paintEvent(QPaintEvent* e){}; // should not be implemented
	void resizeEvent(QResizeEvent* e)
	{
		QWidget::resizeEvent(e);
		int width = size().width();
		int height = size().height();
		//renderer->onResize(width, height);
	};

private:
	void computeFPS()
	{
		static int num_frames = 0;
		static float timeElapsed = 0.0f;
		num_frames++;

		// Compute averages FPS and MS over one second period.
		if((gameTimer.getTotalTime()-timeElapsed) >= 1.0f)
		{
			// calculate statistics
			float fps = (float)num_frames; // fps = frameCnt / 1
			float msPerFrame = 1000.0f/fps;

			// convert statistics into QString
			QString stats;
			stats = "FPS:  %1  Frame Time:  %2 (ms)";
			stats = stats.arg(fps).arg(msPerFrame);

			// send signal
			emit signal_fpsChanged(stats);

			// reset stats for next average.
			num_frames = 0;
			timeElapsed += 1.0f;
		}
	}

signals:
	void signal_fpsChanged(QString value);
};

#endif