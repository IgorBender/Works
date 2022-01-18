#ifndef STATESTABLEE_H
#define STATESTABLEE_H

#include <map>

template <typename StateIdType, typename TransitionType> class StatesTable
{
public:
    typedef std::map<TransitionType, StateIdType> TransitionToState;
    typedef std::map<StateIdType, TransitionToState> StatesTableMap;

    void addTransition(StateIdType Source, TransitionType Transition, StateIdType Destination)
    {
        m_StatesTable[Source][Transition] = Destination;
    }
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
    StatesTableMap m_StatesTable;
};

#endif /*STATESTABLEE_H*/
