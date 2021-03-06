#include "FiniteStateMachine.h"
#include <iterator>

bool FiniteStateMachine::AddState( FiniteState* state )
{
	if (state == 0)
	{
		DEBUGPRINT( "[FAIL] FiniteStateMachine::AddState, "
			<< "Parameter 'state' can not be null");
		return false;
	}

	auto result = states_.insert(
		std::pair<StateType, FiniteState*>(state->GetType(), state));
	
	if(result.second)
	{
		DEBUGPRINT( "[SUCCESS] FiniteStateMachine::AddState, "
			<< "Adding FiniteState with enum value " 
			<< state->GetType() 
			<< " succeded");
	}
	else
	{
		DEBUGPRINT( "[FAIL] FiniteStateMachine::AddState, "
			<< "Adding FiniteState with enum value " 
			<< state->GetType() 
			<< " was unsuccessful, did you try to add it twice? " 
			<< "Note: A particular state may only be added once per state machine");
	}

	return result.second;
}

bool FiniteStateMachine::SetDefaultState( StateType state )
{
	bool wasSet = false;

	if(states_.count(state) > 0)
	{
		defaultState_ = states_[state];
		wasSet = true;
	}

	if (wasSet)
	{
		DEBUGPRINT( "[SUCCESS] FiniteStateMachine::SetDefaultState, "
			<< "Default state successfully set to FiniteState with enum value " << state );
	}
	else
	{
		DEBUGPRINT( "[FAIL] FiniteStateMachine::SetDefaultState, "
			<< "Attempt to set default state to FiniteState with enum value " 
			<< state << " was unsuccessful. Did you remember to add it first?" );
	}

	return wasSet;
}

void FiniteStateMachine::Update( float dt )
{
	//no states to handle, nothing to do
	if (states_.size() == 0)
	{
		DEBUGPRINT( "Warning: FiniteStateMachine::Update is being called on an empty machine" );
		return;
	}

	//if no current state: attempt to set it to default
	if (currentState_.empty() && defaultState_)
	{
		currentState_.push(defaultState_);
		defaultState_->Enter();
		SEND_EVENT(&Event_StateChanged(defaultState_->GetType(), this));
	}

	//no valid  state could be established, nothing to do
	if (currentState_.empty())
	{
		DEBUGPRINT( "Warning: FiniteStateMachine::Update, current state is invalid. Provide a default state to fix this issue" );
		return;
	}

	bool isReplacementState = true;
	StateType oldStateType = currentState_.top()->GetType();
	StateType goalStateType = currentState_.top()->CheckTransitions(isReplacementState);
	if (HandleTransition(goalStateType, isReplacementState))
	{
		SEND_EVENT(&Event_StateChanged(goalStateType, this));
		SEND_EVENT(&Event_SyncStateCommand(this, oldStateType, currentState_.top()->GetType(), isReplacementState));
	}

	if (!currentState_.empty())
	{
		currentState_.top()->Update(dt);
	}
}

StateType FiniteStateMachine::CheckTransitions( bool& out_isReplacementState ) const
{
	return type_; //returning my own StateType means no transition will take place if I am the current state.
}

void FiniteStateMachine::Enter()
{
	if(currentState_.empty() && defaultState_) //no current state exists, see if we have a default state we can use
	{
		currentState_.push(defaultState_);
	}

	if(!currentState_.empty()) //if we have a usable current state
	{
		currentState_.top()->Enter();
	}
}

void FiniteStateMachine::Exit()
{
	if (!currentState_.empty())
	{
		currentState_.top()->Exit();
		currentState_.pop();
	}
	
	if (defaultState_)
	{
		// Prepare for next use by resetting to default state. 
		currentState_.push(defaultState_);
		// Enter should be called later using FiniteStateMachine::Enter()
	}
}

void FiniteStateMachine::Reset()
{
	//empty stack of current states
	while (!currentState_.empty())
	{
		currentState_.top()->Exit();
		currentState_.pop();
	}

	//reset all added states
	for (auto pair : states_)
	{
		pair.second->Reset();
	}

	if (defaultState_)
	{
		currentState_.push(defaultState_);
		currentState_.top()->Enter();
	}
}

void FiniteStateMachine::Nuke()
{
	//empty stack of current states
	while (!currentState_.empty())
	{
		currentState_.top()->Exit();
		currentState_.pop();
	}

	//deallocate all states and clear map
	for (std::map<StateType, FiniteState*>::iterator it = states_.begin(); 
		it != states_.end(); 
		it++)
	{
		it->second->Nuke(); // Nested FSMs are possible: recursively nuke nested states.
		delete it->second;
	}
	states_.clear();

	defaultState_ = 0; //all states removed, no valid default state exists
}

FiniteStateMachine::FiniteStateMachine( StateType type, FiniteState* defaultAndCurrentState ):
	FiniteState(type),
	defaultState_(0)
{
	if (defaultAndCurrentState)
	{
		AddState(defaultAndCurrentState);
		defaultState_ = defaultAndCurrentState;
		currentState_.push(defaultAndCurrentState); 
	}
}

FiniteStateMachine::~FiniteStateMachine()
{
	//States are injected and memory deallocation is therefore handled by caller (See Wikipedia RAII)
	//Deallocation can however be performed by a specific call to the method Nuke()
}

bool FiniteStateMachine::HandleTransition( StateType goalStateType, bool replaceCurrentState )
{
	bool stateChanged = false;
	StateType currentStateType = currentState_.top()->GetType();
	
	//switch if a transition is both possible and necessary
	bool stateShouldChange = goalStateType != currentStateType;
	bool goalStateExists = states_.count(goalStateType) > 0;
	if (stateShouldChange && goalStateExists)
	{ 
		if (replaceCurrentState)
		{	//replace stack top
			currentState_.top()->Exit();
			currentState_.top() = states_[goalStateType];
		}
		else //add to stack
		{
			currentState_.push(states_[goalStateType]);
		}
		currentState_.top()->Enter();
		stateChanged = true;
	}
	else if (goalStateType == SPECIAL_STATE_BACK)
	{ //handle special transition
		currentState_.top()->Exit();
		currentState_.pop();

		if (!currentState_.empty())
		{
			goalStateType = currentState_.top()->GetType();
			stateChanged = true;
		}
		else if (currentState_.empty() && defaultState_)
		{
			currentState_.push(defaultState_);
			goalStateType = defaultState_->GetType();
			stateChanged = true;

			DEBUGPRINT("FiniteStateMachine::HandleTransition, "
				<< "SPECIAL_STATE_BACK resulted in a fallback to the default state. "
				<< "Was this your intention?");
		}
		else
		{
			DEBUGPRINT("Warning: FiniteStateMachine::HandleTransition, "
				<< "SPECIAL_STATE_BACK resulted in invalid current state as no default fallback state has been set. "
				<< "Provide a default state to fix this issue. No event message will be dispathed.");
		}
	}
	else //requested transition could not be performed
	{
		DEBUGPRINT( "Warning: FiniteStateMachine::Update, The requested transition could not be performed." 
			<< " From state with enum value " << currentStateType
			<< " To state with enum value " << goalStateType);
	}

	return stateChanged;
}

void FiniteStateMachine::onEvent( Event* e )
{
	//Pass the event on if current state is another FiniteStateMachine
	if (dynamic_cast<FiniteStateMachine*>(currentState_.top())) //true if dynamic type is FiniteStateMachine or subclass 
		//Note 1: dynamic_cast returns null if the cast fails and a pointer, i.e positive integer, otherwise. Thus the expression evaluates to true if successful.
		//Note 2: The alternative, typeid(currentState_.top()) == typeid(FiniteStateMachine) , is true only if the dynamic type is exactly FiniteStateMachine, derived types would yeild false.
	{
		currentState_.top()->onEvent(e);
		DEBUGPRINT("FiniteStateMachine::onEvent(Event*), Current state is another StateMachine - Passed event on to that Machine.");
		return;
	}

	//Event intended for us, handle received event
	switch (e->getType())
	{
	case EVENT_STATE_CHANGED:
			handleEvent_StateChanged(static_cast<Event_StateChanged*>(e));
		break;
	case EVENT_SYNC_STATE_COMMAND:
			handleEvent_SyncStateCommand(static_cast<Event_SyncStateCommand*>(e));
		break;
	default:
		break;
	}
}

void FiniteStateMachine::handleEvent_StateChanged( Event_StateChanged* e )
{
	if (e->sender != this)
	{
		DEBUGPRINT("FiniteStateMachine::onEvent(Event*), "
			<< "Recieved EVENT_STATE_CHANGED event. New state has enum value " 
			<< e->newState);
	}
}

void FiniteStateMachine::handleEvent_SyncStateCommand( Event_SyncStateCommand* e )
{
	
	if (e->sender != this 
		&& e->sender->GetType() == this->type_
		&& e->fromState == currentState_.top()->GetType())
		//If I'm different instance of the same FiniteStateMachine and in the correct (matching) state
	{
		HandleTransition(e->toState, e->isReplacementState);
		// Send StateChanged event?
	}
}
