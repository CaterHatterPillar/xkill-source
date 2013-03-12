//#pragma once
//
//#include <xkill-utilities/Util.h>
//#include <QtGui/QLabel>
//
//
//class HudPowerupMessage
//{
//private:
//
//public:
//	HudPowerupDispayer(Event_PostHudMessage* e, QWidget* parent);
//};
//
//class HudPowerupMessage_Manager : IObserver
//{
//private:
//	SimpleQueue<HudMessage*> stack;
//	QWidget* parent;
//	Float2 position;
//	AttributePtr<Attribute_SplitScreen> splitScreen;
//
//public:
//	HudMessage_Manager();
//	~HudMessage_Manager();
//	void init(QWidget* parent, AttributePtr<Attribute_SplitScreen> splitScreen);
//	void onEvent(Event* e);
//
//	void update();
//	void move(Float2 position);
//	void addMessage(Event_PostHudMessage* e);
//	void removeTopMessage();
//};