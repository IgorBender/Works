#ifndef STATESTABLEE_H
#define STATESTABLEE_H

#include <map>

///
/// \brief The StatesTable class - mapping of states and transitions
///
template <typename StateIdType, typename TransitionType> class StatesTable
{
public:
    typedef std::map<TransitionType, StateIdType> TransitionToState; ///> type of map of transition to destination state
    typedef std::map<StateIdType, TransitionToState> StatesTableMap; ///> type of map source state to transition map

    ///
    /// \brief addTransition
    /// \param Source - system state from which the transition will be.
    /// \param Transition - the transition.
    /// \param Destination - system state to transition will be.
    ///
    void addTransition(StateIdType Source, TransitionType Transition, StateIdType Destination)
    {
        m_StatesTable[Source][Transition] = Destination;
    }

    ///
    /// \brief getNextState - delivers tarnsition possibility and next state after transition.
    /// \param CurrentState - system state from which the transition will be.
    /// \param Transition - the transition.
    /// \param NextState - system state to transition will be.
    /// \return - transition possibility.
    ///
    bool getNextState(StateIdType CurrentState, TransitionType Transition, StateIdType& NextState)
    {
        typename StatesTableMap::iterator i = m_StatesTable.find(CurrentState);
        if(m_StatesTable.end() == i)
            return false;
        typename TransitionToState::iterator j = m_StatesTable[CurrentState].find(Transition);
        if(m_StatesTable[CurrentState].end() == j)
            return false;
        NextState = m_StatesTable[CurrentState][Transition];
        return true;
    }

protected:
    StatesTableMap m_StatesTable; ///> map source state to transition map.
};

#endif /*STATESTABLEE_H*/
