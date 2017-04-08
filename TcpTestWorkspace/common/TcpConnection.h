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

enum ConnectStateEnum
{
	StartConnection		= 0,
	NoState				= 1
};

//typedef std::pair < int , int > IntPairType;
//typedef std::unique_ptr < ClientSimple > SockAutoPtr;
//typedef std::unique_ptr < StreamSocket > StreamSockAutoPtr;
//typedef std::unique_ptr < ServerReuse > ReuseServerAutoPtr;


class TcpConnection
{
public:
	TcpConnection();
	virtual ~TcpConnection();

	virtual bool init(unsigned long Addr, unsigned short Port, int32_t Time, bool Initiator);
	bool down();
	bool isConnected()
	{
		if(m_pWorkingSocket)
			return m_pWorkingSocket->isConnected();
		return false;
	}
	bool isWorking()
	{
		return m_Working;
	}
	virtual bool connection(unsigned long TimeOutMsecs, unsigned long NumOfTries);
// 	void readMessage();

	void disconnect()
	{
		m_Connected = false;
		m_Working = false;
		if(m_pWorkingSocket)
		{
            delete m_pWorkingSocket;
			m_pWorkingSocket = NULL;
			// TODO - Update !!!!! 
		}
	}
	virtual void reset();


protected:
	ClientSimple* m_pSocket;
	StreamSocket* m_pWorkingSocket;
	ServerReuse* m_pServer;
	ThreadClassLib::ThreadClass* m_pRecvThread;
	ThreadClassLib::ThreadClass* m_pSendThread;
	ThreadClassLib::ThreadClass* m_pConnectThread;
	unsigned long m_Address;
	unsigned short m_CommPort;
	ConnectStateEnum m_ConnectionState;
	volatile bool m_Connected;
	volatile bool m_Working;
	bool m_Initiate;
	int32_t m_ConnectSeconds;
	std::chrono::steady_clock::time_point m_StartTime;

	virtual void connectRoutine();
	virtual void receiveRoutine() = 0;
	virtual void sendRoutine() = 0;

    struct ConnectFunctor : public ThreadClassLib::ThreadClass::ThreadRoutineType
    {
        ConnectFunctor(TcpConnection* p) : m_pTcpConnection(p) {}
        virtual void operator()()
        {
            m_pTcpConnection->connectRoutine();
        }
        TcpConnection* m_pTcpConnection;
    } m_ConnectorFunctor;

    struct ReceiveFunctor : public ThreadClassLib::ThreadClass::ThreadRoutineType
    {
        ReceiveFunctor(TcpConnection* p) : m_pTcpConnection(p) {}
        virtual void operator()()
        {
            m_pTcpConnection->receiveRoutine();
        }
        TcpConnection* m_pTcpConnection;
    } m_ReceiveFunctor;

    struct SendFunctor : public ThreadClassLib::ThreadClass::ThreadRoutineType
    {
        SendFunctor(TcpConnection* p) : m_pTcpConnection(p) {}
        virtual void operator()()
        {
            m_pTcpConnection->sendRoutine();
        }
        TcpConnection* m_pTcpConnection;
    } m_SendFunctor;

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
	        m_Running.store(true);
	    }
	} m_ReceiveStopIndicator, m_SendStopIndicator;
};

#endif // _TCPLCONNECTION_H
