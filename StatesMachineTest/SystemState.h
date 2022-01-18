#ifndef SYSTEMSTATE_H
#define SYSTEMSTATE_H

///
/// \class SystemState
/// Abstract system state class.
/// \param StateIdType : system state ID type.
template <typename StateIdType> class SystemState
{
public:
    /// Constructor.
    /// \param State: system state ID.
    SystemState(StateIdType State) : m_State(State) {}
    /// Destructor
    virtual ~SystemState() {}
    /// Called at state enter.
    /// \return Status
    virtual bool enter() = 0;
    /// Called at state exit.
    /// \return Status
    virtual bool exit() = 0;
    /// Called at state inner transition.
    /// \return Status
    virtual bool run()
    {
        return true;
    }
    /// Gets state ID
    /// \return State ID.
    virtual StateIdType getState()
    {
        return m_State;
    }

protected:
    StateIdType m_State; ///< State ID
};

///
/// \class AppSystemState
/// Bounds system state object to other class object.
template <typename StateIdType, typename StateMachine> class AppSystemState : public SystemState<StateIdType>
{
public:
    /// Constructor
    /// \param State: system state ID.
    /// \param pApp: pointer to other object.
	AppSystemState(StateIdType State, StateMachine* pApp) : SystemState<StateIdType>(State), m_pApp(pApp)
	{}
    virtual ~AppSystemState() {}

protected:
	StateMachine* m_pApp; ///< Pointer to other object
};


#endif /*SYSTEMSTATE_H*/
