#include <TcpConnection.h>
#include <time.h>

#define COMM_CHECK_TIMEOUT 5
const uint32_t CONNECTION_THREAD_PERIOD_MSEC = 50;


TcpConnection::TcpConnection() : m_pSocket(NULL), m_pWorkingSocket(NULL), m_pServer(NULL),
	m_pRecvThread(NULL), m_pSendThread(NULL), m_pConnectThread(NULL),
	m_Address(0), m_CommPort(0), m_ConnectionState(NoState), m_Connected(false), m_Working(false), m_Initiate(false), m_ConnectSeconds(0),
	m_StartTime(std::chrono::steady_clock::now()), m_ConnectorFunctor(this), m_ReceiveFunctor(this), m_SendFunctor(this)
{
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int WinSockError;

	wVersionRequested = MAKEWORD(2, 2);

	WinSockError = WSAStartup(wVersionRequested, &wsaData);
	if (WinSockError != 0)
	{
		return;
	}
#endif
}

TcpConnection::~TcpConnection()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

bool TcpConnection::init(unsigned long Addr, unsigned short Port, int32_t Time, bool Initiator)
{
	m_Initiate = Initiator;
	m_Address = Addr;
	m_CommPort = Port;
	m_ConnectSeconds = Time;

	m_pRecvThread = new ThreadClassLib::ThreadClass(&m_ReceiveFunctor, true);
	if(m_pRecvThread)
	{
	    m_pRecvThread->registerStopNotificationFunc(&m_ReceiveStopIndicator, nullptr);
	    m_pRecvThread->run();
	}

	m_pSendThread = new ThreadClassLib::ThreadClass(&m_SendFunctor, true);
	if(m_pSendThread)
	{
	    m_pSendThread->registerStopNotificationFunc(&m_SendStopIndicator, nullptr);
	    m_pSendThread->run();
	}

	m_pConnectThread = new ThreadClassLib::ThreadClass(&m_ConnectorFunctor, true);
	if(m_pConnectThread)
	{
		m_StartTime = std::chrono::steady_clock::now();
		m_ConnectionState = StartConnection;
		m_pConnectThread->run();
		m_pConnectThread->start();
	}

	return true;
}

bool TcpConnection::down()
{
	m_pRecvThread->stop();
	m_pRecvThread->exit();
	m_pRecvThread->join();

	m_pSendThread->stop();
	m_pSendThread->exit();
	m_pSendThread->join();

	m_pConnectThread->stop();
	m_pConnectThread->exit();
	m_pConnectThread->join();

	delete m_pRecvThread;
	m_pRecvThread = NULL;

	delete m_pSendThread;
	m_pSendThread = NULL;

	delete m_pConnectThread;
	m_pConnectThread = NULL;

    if(m_pServer)
    {
        delete m_pServer;
        m_pServer = NULL;
    }
    if(m_pSocket)
    {
        delete m_pSocket;
        m_pSocket = NULL;
    }
    if(m_pWorkingSocket)
    {
        delete m_pWorkingSocket;
        m_pWorkingSocket = NULL;
    }
    
	return true;
}

void TcpConnection::connectRoutine()
{
	if(m_ConnectSeconds)
	{
		if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_StartTime).count() > m_ConnectSeconds)
		{
			m_pConnectThread->stop();
		}
	}

	if(m_Working)
	{
		m_pConnectThread->stop();
		return;
	}

	switch(m_ConnectionState)
	{
	case StartConnection:
		m_Connected = false;
		m_Connected = connection(100, NUMBER_OF_CONNECT_TRIALS);
		if(m_Connected)
		{
			m_Working = true;
			m_pConnectThread->stop();

			m_pRecvThread->setCyclic(true);
			m_ReceiveStopIndicator.reset();
			m_pRecvThread->start();
			m_pSendThread->setCyclic(true);
			m_SendStopIndicator.reset();
			m_pSendThread->start();
			m_ConnectionState = NoState;
		}
		break;

	case NoState:
		break;

	default:
		;
	}
}

bool TcpConnection::connection(unsigned long TimeOutMsecs, unsigned long NumOfTries)
{
	SOCK_TRY 
	{
		if(!m_Initiate)
		{
			if(!m_pServer)
			{
				if(INADDR_ANY == m_Address)
				{
					m_pServer = new ServerReuse(static_cast<short>(m_CommPort));
				}
				else
				{
					m_pServer = new ServerReuse(static_cast<short>(m_CommPort), m_Address);
				}
				m_pServer->setNonBlockMode(true);
				m_pServer->listen(1);
			}

// 			if (!(pWorkingSocket && pWorkingSocket->isConnected()))
// 			{
				SOCK_TRY
				{
					if(m_pWorkingSocket)
					{
						delete m_pWorkingSocket;
						m_pWorkingSocket = NULL;
					}
					m_pWorkingSocket = m_pServer->accept();
				}
				SOCK_EXCEPT_CATCH_BEGIN_NOREP
				{
                    m_pWorkingSocket = NULL;
					if ((WSAEWOULDBLOCK != exc.getErrCode()) && (WSAEALREADY != exc.getErrCode()))
					{
						// !!!!! Fatal error EXIT !!!!!
					}
				}
				SOCK_EXCEPT_CATCH_END;
// 			}

			if ((NumOfTries != 0xffffffff) && !(NumOfTries && NumOfTries--))
			{
				m_pConnectThread->stop();
            }

			std::this_thread::sleep_for(std::chrono::milliseconds{TimeOutMsecs});

			if (m_pWorkingSocket && m_pWorkingSocket->isConnected())
			{
				m_pWorkingSocket->setNonBlockMode(false);
				if(m_pServer)
				{
					// pServer->close();
					delete m_pServer;
					m_pServer = NULL;
				}
				m_Connected = true;
			}
		}
		else
		{
			if(!m_pSocket)
			{
				m_pSocket = new ClientSimple;
				m_pSocket->setNonBlockMode(true);
			}
			if(m_pWorkingSocket)
			{
				delete m_pWorkingSocket;
				m_pWorkingSocket = NULL;
			}
			if (!(m_pSocket && m_pSocket->isConnected()))
			{
				SOCK_TRY
				{
					m_pSocket->connect(m_Address, m_CommPort);
				}
				SOCK_EXCEPT_CATCH_BEGIN_NOREP
				{
					if ((WSAEWOULDBLOCK != exc.getErrCode()) && (WSAEALREADY != exc.getErrCode()) && (WSAEINPROGRESS != exc.getErrCode()))
					{
						delete m_pSocket;
						m_pSocket = new ClientSimple;
						m_pSocket->setNonBlockMode(true);
					}
				}
				SOCK_EXCEPT_CATCH_END;

				fd_set ReadSet;
				fd_set ErrSet;
				FD_ZERO(&ReadSet);
				FD_ZERO(&ErrSet);
				FD_SET(m_pSocket->getSock(), &ReadSet);
				FD_SET(m_pSocket->getSock(), &ErrSet);
				timeval SelTime;
				SelTime.tv_sec = TimeOutMsecs / 1000;
				SelTime.tv_usec = (TimeOutMsecs % 1000) * 1000;
				int SelRes = select(static_cast<int>(m_pSocket->getSock()) + 1, &ReadSet, nullptr, nullptr, &SelTime);
				if(SelRes < 0)
				{
				    std::this_thread::sleep_for(std::chrono::milliseconds{20});
				    // !!!!!!!!!!!!!!!!!!!!!
				}
				if(SelRes == 0)
				{
//					pClient = SimpleClientAutoPtr(new SimpleClient);
//					pClient->setNonBlockMode(true);
				}
				if(SelRes > 0 && FD_ISSET(m_pSocket->getSock(), &ReadSet))
				{
					SOCK_TRY
					{
						m_pSocket->getPeerName();
						m_pSocket->setConnected(true);
					}
					SOCK_EXCEPT_CATCH_BEGIN_NOREP
					if(WSAGetLastError() == ENOTCONN)
					{
					    std::this_thread::sleep_for(std::chrono::milliseconds{TimeOutMsecs});
					}
					if(m_pSocket)
					{
						delete m_pSocket;
                        m_pSocket = NULL;
					}
					m_pSocket = new ClientSimple;
					m_pSocket->setNonBlockMode(true);
					SOCK_EXCEPT_CATCH_END
				}
			}

			if ((NumOfTries != 0xffffffff) && !(NumOfTries && NumOfTries--))
			{
				m_pConnectThread->stop();
			}
			else
			{

			}

			if (m_pSocket && m_pSocket->isConnected()) 
			{
				m_pSocket->setNonBlockMode(false);
				m_Connected = true;
				m_pWorkingSocket = m_pSocket;
				m_pSocket = NULL;
			}
		}
	}
	SOCK_EXCEPT_CATCH_BEGIN_NOREP
	{
		return false;
	}
	SOCK_EXCEPT_CATCH_END

	return m_Connected;
}

void TcpConnection::reset()
{
	m_pRecvThread->stop();
	m_pSendThread->stop();
	m_pConnectThread->stop();
	while(m_ReceiveStopIndicator.isRunning() || m_ReceiveStopIndicator.isRunning())
	    std::this_thread::sleep_for(std::chrono::milliseconds{50});
	m_Connected = false;
	m_Working = false;
	m_ConnectionState = StartConnection;
	if(m_pWorkingSocket)
	{
		delete m_pWorkingSocket;
		m_pWorkingSocket = NULL;
	}
	m_StartTime = std::chrono::steady_clock::now();
	if(m_pWorkingSocket)
	{
		delete m_pWorkingSocket;
		m_pWorkingSocket = NULL;
	}
	m_pConnectThread->setCyclic(true);
	m_pConnectThread->start();
}

