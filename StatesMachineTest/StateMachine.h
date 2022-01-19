#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "StatesTable.h"
#include "SystemState.h"
#include <mutex>

///
/// \brief The StateMachine class
/// Template to implement finite state machine (FSM)
///

template <typename StateIdType, typename TransitionType> class StateMachine
{
public:
    typedef SystemState<StateIdType>* SystemStatePtr;

    ///
    /// \brief StateMachine constructor.
    /// \param InitialState - initial state for state machine.
    ///
    StateMachine(StateIdType InitialState) : m_Current(InitialState), m_Next(InitialState), m_InTransit(false)
    {}

    ///
    /// \brief ~StateMachine dedstructor.
    ///
    virtual ~StateMachine()
    {
        clean();
    }

    ///
    /// \brief start - starts state machine behaviour, enters initial state.
    /// \return - false if state machine has no states, true if OK.
    ///
    bool start()
    {
        typename std::map<StateIdType, SystemStatePtr>::iterator StateIterator = m_States.find(m_Current);
        if(m_States.end() == StateIterator)
            return false;
        return StateIterator->second->enter();
    }

    ///
    /// \brief addState - adds state to state machine.
    /// \param Id - state identificator.
    /// \param pState = pointer to state implementation abstract base class.
    ///
    void addState(StateIdType Id, SystemStatePtr pState)
    {
        m_States[Id] = pState;
    }

    ///
    /// \brief addTransition - adds possible transition between states.
    /// \param Source - state identificator for state transit from.
    /// \param Transition - transition type.
    /// \param Destination - state identificator for transit to.
    /// \return - Success or failure.
    ///
    bool addTransition(StateIdType Source, TransitionType Transition, StateIdType Destination)
    {
        typename std::map<StateIdType, SystemStatePtr>::iterator StateIterator = m_States.find(Source);
        if(m_States.end() == StateIterator)
            return false;
        StateIterator = m_States.find(Destination);
        if(m_States.end() == StateIterator)
            return false;
        m_StatesTable.addTransition(Source, Transition, Destination);
        return true;
    }

    ///
    /// \brief transit - transit between states.
    /// \param Transition - transition type.
    /// \return - true if the transition is possible, otherwise false.
    ///
    bool transit(TransitionType Transition)
    {
        if(!m_StatesTable.getNextState(m_Current, Transition, m_Next))
            return false;
        m_InTransit = true;
        if(m_Next == m_Current)
        {
            m_States[m_Current]->run();
            m_InTransit = false;
            return true;
        }
        if(!transitAllowed())
            return false;
        if(!m_States[m_Current]->exit())
        {
            m_InTransit = false;
            return false;
        }
        m_Current = m_Next;
        if(!m_States[m_Current]->enter())
        {
            m_InTransit = false;
            return false;
        }
        m_InTransit = false;
        return true;
    }

    ///
    /// \brief getCurrentState - obtain current state.
    /// \return - current state.
    ///
    StateIdType getCurrentState()
    {
        return m_States[m_Current]->getState();
    }

//    StateIdType getNextState()
//    {
//        return m_Next;
//    }

    ///
    /// \brief isInTransit - delivers in state transit status
    /// \return - in transit status
    ///
    bool isInTransit()
    {
        return m_InTransit;
    }

    ///
    /// \brief clean - cleans internal data structures
    ///
    void clean()
    {
        for(std::pair<const StateIdType, SystemStatePtr>& p : m_States)
        {
            if(p.second)
                delete p.second;
        }
    }

    ///
    /// \brief transitAllowed - checks if transit from current to the next state is allowed.
    /// \return
    ///
    virtual bool transitAllowed()
    {
        return true;
    }

protected:
    StatesTable<StateIdType, TransitionType> m_StatesTable;
    std::map<StateIdType, SystemStatePtr> m_States;
    StateIdType m_Current;
    StateIdType m_Next;
    bool m_InTransit;
};

///
/// \brief The StateMachineThreadSafe class - thread-safe version of state-nachine
///
template <typename StateIdType, typename TransitionType> class StateMachineThreadSafe : public StateMachine <StateIdType, TransitionType>
{
public:
    ///
    /// \brief StateMachineThreadSafe constructor
    /// \param InitialState - initial state for state machine.
    ///
    StateMachineThreadSafe(StateIdType InitialState) : StateMachine <StateIdType, TransitionType>(InitialState)
    {}
    virtual ~StateMachineThreadSafe() {}

    ///
    /// \brief transit - transit between states.
    /// \param Transition - transition type.
    /// \return - true if the transition is possible, otherwise false.
    ///
    bool transit(TransitionType Transition)
    {
        std::lock_guard<std::mutex> Guard(m_Mutex);
        return StateMachine <StateIdType, TransitionType>::transit(Transition);
    }

    ///
    /// \brief lockTransition
    ///
    void lockTransition()
    {
        m_Mutex.lock();
    }

    ///
    /// \brief unlockTransition
    ///
    void unlockTransition()
    {
        m_Mutex.unlock();
    }

protected:
    std::mutex m_Mutex; ///> Transition mutex
};

#endif /*STATEMACHINE_H*/
