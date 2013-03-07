#pragma once

#include <QtGui/QMainWindow>
#include <xkill-utilities/Util.h>

#include <QtGui/QMainWindow>
#include <xkill-utilities/WeaponStructs.h>
#include <QtGui/QProgressBar>
#include <vector>

#include "ui_Menu_HUD.h"

class Attribute_SplitScreen;

class HudMessage
{
private:
	float _lifetime;
	QLabel* _label;
	Float2 _targetPosition;

public:
	HudMessage(Event_PostHudMessage* e, QWidget* parent);
	~HudMessage()
	{
		delete _label;
	}
	int getHeight()
	{
		return _label->height();
	}

	void setPosition(Float2 position);

	void setTargetPosition(Float2 position)
	{
		// Offset relative to center of label
		position.x -= _label->width() * 0.5f;
		position.y -= _label->height() * 0.5f;

		_targetPosition = position;
	}

	void updatePosition();
	
	void update()
	{
		updatePosition();
		
		// Decrement timer
		if(_lifetime > 0.0f)
			_lifetime -= SETTINGS->trueDeltaTime;
	}
	bool isExpired()
	{
		// Check expiration condition
		if(_lifetime <= 0.0f)
			return true;

		return false;
	}
};

class HudMessage_Manager : IObserver
{
private:
	SimpleQueue<HudMessage*> stack;
	QWidget* parent;
	Float2 position;
	AttributePtr<Attribute_SplitScreen> splitScreen;

public:
	HudMessage_Manager()
	{
		SUBSCRIBE_TO_EVENT(this, EVENT_POST_HUD_MESSAGE);
	}
	~HudMessage_Manager()
	{
		UNSUBSCRIBE_TO_EVENTS(this);

		for(int i=0; i<stack.count(); i++)
			delete stack.at(i);
	}
	void init(QWidget* parent, AttributePtr<Attribute_SplitScreen> splitScreen)
	{
		this->parent = parent;
		this->splitScreen = splitScreen;
	}

	void move(Float2 position)
	{
		this->position = position;
	}

	void update()
	{
		for(int i=0; i<stack.count(); i++)
		{
			stack.at(i)->update();
			if(stack.at(i)->isExpired())
				removeTopMessage();
		}
	}

	void addMessage(Event_PostHudMessage* e);

	void removeTopMessage()
	{
		delete stack.pop();
	}

	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_POST_HUD_MESSAGE:
			addMessage((Event_PostHudMessage*)e);
			break;
		default:
			break;
		}
	}
};

class ScoreboardEntry
{
public:
	QLabel* label_process;
	QLabel* label_cycles;
	QLabel* label_priority;

	bool isChanged;

	std::string playerName;
	int cycles;
	int priority;

	AttributePtr<Attribute_Player> ptr_player;

	ScoreboardEntry()
	{
		isChanged = true;
	}
};

class ScoreBoard
{
private:
	std::vector<ScoreboardEntry> entries;
	AttributePtr<Attribute_Player> ptr_current_player;

	int valueAt(int index)
	{
		return entries.at(index).ptr_player->cycles;
	}
	void bubbleSort()
	{
		int numEntries = entries.size();
		bool swapped = true;

		int j = 0;
		while(swapped) 
		{
			swapped = false;
			j++;
			for(int i=0; i<numEntries-j; i++)
			{
				if(valueAt(i) < valueAt(i + 1))
				{
					// Swap values
					AttributePtr<Attribute_Player> tmp = entries.at(i).ptr_player;
					entries.at(i).ptr_player = entries.at(i+1).ptr_player;
					entries.at(i+1).ptr_player = tmp;

					swapped = true;
				}
			}
		}
	}

	void quickSort(int index_start, int index_end)
	{
		int left = index_start;
		int right = index_end;

		// Pick pivot (should be random for best O(n))
		int pivot = valueAt(index_end);

		// Repeat until ends meet
		while(left < right) 
		{
			// Find lower and upper values to switch
			while(valueAt(left) < pivot)
			{
				left++;
			}
			while(valueAt(right) > pivot)
			{
				right--;
			}

			// Check if chosen values are still valid
			if(left < right) 
			{
				// Swap values
				AttributePtr<Attribute_Player> tmp = entries.at(left).ptr_player;
				entries.at(left).ptr_player = entries.at(right).ptr_player;
				entries.at(right).ptr_player = tmp;

				left++;
				right--;
			}
		}

		// Continue sorting left and right
		if(index_start < right)
			quickSort(index_start, right);
		if(left < index_end)
			quickSort(left, index_end);
	}
	void sortPlayers()
	{
		// Sort using bubblesort
		bubbleSort();

		//// Sort using quicksort
		//quickSort(0, entries.size() - 1);
	}
	void syncLabelsWithPlayers()
	{
		for(int i=0; i<entries.size(); i++)
		{
			ScoreboardEntry* e = &entries.at(i);

			// Detect if label has changed
			if(e->ptr_player->playerName != e->playerName)
				e->isChanged = true;
			if(e->ptr_player->cycles != e->cycles)
				e->isChanged = true;
			if(e->ptr_player->priority != e->priority)
				e->isChanged = true;
			e->isChanged = true;

			// Update label
			if(e->isChanged)
			{
				e->isChanged = false;

				// Set text
				e->label_process->setText(e->ptr_player->playerName.c_str());
				e->label_cycles->setNum(e->ptr_player->cycles);
				e->label_priority->setNum(e->ptr_player->priority);

				// Empty style sheets
				std::string sheet_process = "";
				std::string sheet_cycles = "";
				std::string sheet_priority = "";

				// Apply extra stuff if we're at the current player
				if(e->ptr_player == ptr_current_player)
				{
					sheet_process += "background-color: rgba(255, 255, 255, 100); font-weight: bold;";
					sheet_cycles += "background-color: rgba(255, 255, 255, 100); font-weight: bold;";
					sheet_priority += "background-color: rgba(255, 255, 255, 100); font-weight: bold;";
				}

				// Apply extra stuff if we have most cycles
				if(e->ptr_player->cycles == maxCycles)
				{
					sheet_cycles += "background-color: rgba(0, 255, 0, 100);";
				}

				// Apply extra stuff if we have most priority
				if(e->ptr_player->priority == maxPriority)
				{
					sheet_priority += "background-color: rgba(0, 255, 0, 100);";
				}

				// Apply style sheet
				e->label_process->setStyleSheet(sheet_process.c_str());
				e->label_cycles->setStyleSheet(sheet_cycles.c_str());
				e->label_priority->setStyleSheet(sheet_priority.c_str());
			}
		}
	}
public:
	int maxCycles;
	int maxPriority;
	int secondMaxPriority;

	void init(AttributePtr<Attribute_Player> ptr_current_player)
	{
		this->ptr_current_player = ptr_current_player;
	}
	void findMaxValues()
	{
		int numEntries = entries.size();

		// Find max cycles
		maxCycles = 0;
		for(int i=0; i<numEntries; i++)
		{
			ScoreboardEntry* e = &entries.at(i);

			if(e->ptr_player->cycles > maxCycles)
				maxCycles = e->ptr_player->cycles;
		}

		// Find max priority
		maxPriority = 0;
		int maxPriority_index = 0;
		for(int i=0; i<numEntries; i++)
		{
			ScoreboardEntry* e = &entries.at(i);

			if(e->ptr_player->priority > maxPriority)
			{
				maxPriority = e->ptr_player->priority;
				maxPriority_index = i;
			}
		}

		// Find second max priority
		secondMaxPriority = 0;
		for(int i=0; i<numEntries; i++)
		{
			ScoreboardEntry* e = &entries.at(i);

			if(e->ptr_player->priority > maxPriority && maxPriority_index != i)
				secondMaxPriority = e->ptr_player->priority;
		}
	}
	void addEntry(ScoreboardEntry entry)
	{
		entry.playerName = entry.ptr_player->playerName;
		entry.cycles = entry.ptr_player->cycles;
		entry.priority = entry.ptr_player->priority;

		entries.push_back(entry);
	}
	void refresh()
	{
		sortPlayers();
		findMaxValues();
		syncLabelsWithPlayers();
	}
};

class Menu_HUD : public QWidget, IObserver
{
private:
	AttributePtr<Attribute_SplitScreen> ptr_splitScreen;
	Ui::Menu_HUD ui;
	HudMessage_Manager hudMessage_manager;
	ScoreBoard scoreboard;

	float scoreboardFade;
	float healthFade;
	float ammoFade;
	float firingModeFade;
	int index_crosshair;
	int index_firingMode;
	int prev_ammoRatio;
	int prev_reloadRatio;

public:
	Menu_HUD(AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent);
	~Menu_HUD();

	void mapToSplitscreen();
	void refresh();
	void initScoreboard();
	void onEvent(Event* e);
};

class Menu_HUDManager : public QObject , IObserver
{
	Q_OBJECT

private:
	std::vector<Menu_HUD*> huds;
	QWidget* parent;
	bool isEnabled;

public:
	Menu_HUDManager(QWidget* parent);
	void mapHudsToSplitscreen();
	void onEvent(Event* e);
};