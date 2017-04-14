#ifndef _TCPLCONNECTION_H
#define _TCPLCONNECTION_H

#include <ClientSimple.h>
#include <ServerReuse.h>

#include <ThreadClass.h>

#include <sys/timeb.h>

#include <map>
#include <memory>
#include <chrono>

const uint32_t NUMBER_OF_CONNECT_TRIALS = 0xffffffff;

/// @class TcpConnection - abstract class of TCP virtual line, implements connection routine.
class TcpConnection
{
public:
    /// @enum - connection state machine state.
    enum class ConnectState
    {
        StartConnection		= 0,   ///< StartConnection
        NoState				= 1    ///< NoState
    };

    /// Constructor.
	TcpConnection();
	/// Destructor.
	virtual ~TcpConnection();

	/// Initializing method
	/// @param Addr : for client - server's IP address, for server - local IP adderss or INADDR_ANY, both in network byte order.
	/// @param Port : for client - server's TCP port number, for server - local TCP port number, both in network byte order.
	/// @param Time : time in seconds to try to establish TCP connection, 0 - forever.
	/// @param Initiator : client is initiator.
	/// @return - true if initialized successfully, otherwise - false.
	virtual bool init(unsigned long Addr, unsigned short Port, int32_t Time, bool Initiator);
	/// Closing connection method.
	/// @return : success or failure.
	bool down();
	/// @return : connection status.
	bool isConnected()
	{
		if(m_pWorkingSocket)
			return m_pWorkingSocket->isConnected();
		return false;
	}
//	void disconnect()
//	{
//		m_Connected = false;
//		if(m_pWorkingSocket)
//		{
//            delete m_pWorkingSocket;
//			m_pWorkingSocket = NULL;
//		}
//	}
	virtual void reset();

protected:
	ClientSimple* m_pSocket = nullptr; ///< Client socket pointer.
	StreamSocket* m_pWorkingSocket = nullptr; ///< Pointer to connected socket for both client and server.
	ServerReuse* m_pServer = nullptr; ///< Server socket pointer.
	ThreadClassLib::ThreadClass* m_pRecvThread = nullptr; ///< Pointer to receive thread class object.
	ThreadClassLib::ThreadClass* m_pSendThread = nullptr; ///< Pointer to receive send class object.
	ThreadClassLib::ThreadClass* m_pConnectThread = nullptr; ///< Pointer to connect thread class object.
	unsigned long m_Address = 0; ///< IP addres : for client - remote, for server - local.
	unsigned short m_CommPort = 0; ///< TCP port : for client - remote, for server - local.
	ConnectState m_ConnectionState = ConnectState::NoState; ///< conection state machine current state.
	volatile bool m_Connected = false; ///< Connected status.
	bool m_Initiate = false; ///< If true - client, otherwise - server.
	int32_t m_ConnectSeconds = 0; ///< Connection trying duration in seconds.
	std::chrono::steady_clock::time_point m_StartTime; ///< Connection starting time-point.

	virtual void connectRoutine(); ///< Connection thread routine.
	virtual void receiveRoutine() = 0; ///< Receive thread routine.
	virtual void sendRoutine() = 0; ///< Send thread routine.

	/// @struct ConnectFunctor - wrapper on connection thread routine.
    struct ConnectFunctor : public ThreadClassLib::ThreadClass::ThreadRoutineType
    {
        ConnectFunctor(TcpConnection* p) : m_pTcpConnection(p) {}
        virtual void operator()()
        {
            m_pTcpConnection->connectRoutine();
        }
        TcpConnection* m_pTcpConnection;
    } m_ConnectorFunctor;

    /// @struct ReceiveFunctor - wrapper on receive thread routine.
    struct ReceiveFunctor : public ThreadClassLib::ThreadClass::ThreadRoutineType
    {
        ReceiveFunctor(TcpConnection* p) : m_pTcpConnection(p) {}
        virtual void operator()()
        {
            m_pTcpConnection->receiveRoutine();
        }
        TcpConnection* m_pTcpConnection;
    } m_ReceiveFunctor;

    /// @struct SendFunctor - wrapper on send thread routine.
    struct SendFunctor : public ThreadClassLib::ThreadClass::ThreadRoutineType
    {
        SendFunctor(TcpConnection* p) : m_pTcpConnection(p) {}
        virtual void operator()()
        {
            m_pTcpConnection->sendRoutine();
        }
        TcpConnection* m_pTcpConnection;
    } m_SendFunctor;

    /// @struct StopIndicator - stop indicator functor type.
	struct StopIndicator : public ThreadClassLib::ThreadClass::ThreadStopNotificatonType
	{
	    std::atomic<bool> m_Running;
	    void reset()
	    {
	        m_Running.store(false);
	    }
	    bool isRunning()
	    {
	        return m_Running.load();
	    }
	    virtual void operator()(void* p)
	    {
            m_p = p; // To eliminate compiler warning about unused parameter.
	        m_Running.store(true);
	    }
        void* m_p = nullptr; // To eliminate compiler warning about unused parameter.
	}
	m_ReceiveStopIndicator, ///< Receive stop indicator.
	m_SendStopIndicator; ///< Send stop indicator.

	/// @param TimeOutMsecs : duration of each connection attempt in milliseconds.
	/// @param NumOfTries : number of connection attempts.
	/// @return connect attempt result.
    virtual bool connection(unsigned long TimeOutMsecs, unsigned long NumOfTries);
};

#endif // _TCPLCONNECTION_H
