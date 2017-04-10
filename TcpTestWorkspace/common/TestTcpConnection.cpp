/*
 * TestTcpConnection.cpp
 *
 *  Created on: Mar 31, 2017
 *      Author: Igor Bender
 */

#include <TestTcpConnection.h>
#include <algorithm>

TestTcpConnection::TestTcpConnection() : m_ReceivePool(MaxMessageSize(), 4), m_SendPool(MaxMessageSize(), 4)
{}

void TestTcpConnection::connectRoutine()
{
    if(m_LastConnectionState && !isConnected())
    {
        std::cout << "Disconnected..." << std::endl;
        m_LastConnectionState = false;
    }
    TcpConnection::connectRoutine();
    if(!m_LastConnectionState && isConnected())
    {
        sockaddr_in SockData = m_pWorkingSocket->getSockName();
        std::cout << "Connected " << inet_ntoa(SockData.sin_addr) << ":" << ntohs(SockData.sin_port) << " <--> ";
        SockData = m_pWorkingSocket->getPeerName();
        std::cout << inet_ntoa(SockData.sin_addr) << ":" << ntohs(SockData.sin_port) << std::endl;
        m_LastConnectionState = true;
    }
}

void TestTcpConnection::receiveRoutine()
{
    if(!m_pWorkingSocket)
        return;

    fd_set RecvSet;
    fd_set ErrSet;
    FD_ZERO(&RecvSet);
    FD_ZERO(&ErrSet);
    FD_SET(m_pWorkingSocket->getSock(), &RecvSet);
    FD_SET(m_pWorkingSocket->getSock(), &ErrSet);
#ifndef _WIN32
    timeval Timeout = { .tv_sec = 1, .tv_usec = 0 };
#else
	timeval Timeout;
	Timeout.tv_sec = 1;
	Timeout.tv_usec = 0;
#endif
    int32_t SelectResult = select(m_pWorkingSocket->getSock() + 1, &RecvSet, nullptr, &ErrSet, &Timeout);
    if(0 > SelectResult)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
        return;
    }
    if(0 == SelectResult)
        return;
    if(FD_ISSET(m_pWorkingSocket->getSock(), &ErrSet))
    {
        reset();
        m_MsgToReceive.m_pBuff.reset();
        reset();
        return;
    }
    if(!FD_ISSET(m_pWorkingSocket->getSock(), &RecvSet))
        return;
    switch(m_ReceiveState)
    {
    case ReceiveState::AcquireBuffer:
        m_MsgToReceive.m_pBuff = m_ReceivePool.acqireResource();
        if(nullptr == m_MsgToReceive.m_pBuff.get())
            m_pCurrRecvPtr = m_DiscardBuffer;
        else
            m_pCurrRecvPtr = static_cast<char*>(*(m_MsgToReceive.m_pBuff->m_pPtr));
        m_ReceiveState = ReceiveState::RecvHeader;
        m_ReceivingBytes = sizeof(MessageHeader);
        break;

    case ReceiveState::RecvHeader:
        SOCK_TRY
        {
            int32_t Received = m_pWorkingSocket->receive(m_pCurrRecvPtr + sizeof(MessageHeader) - m_ReceivingBytes, m_ReceivingBytes);
            if(0 >= Received)
            {
                reset();
                return;
            }
            m_ReceivingBytes -= Received;
            if(0 < m_ReceivingBytes)
                return;
            else
            {
                if(sizeof(MessageHeader) == reinterpret_cast<MessageHeader*>(m_pCurrRecvPtr)->MessageSize)
                {
                    std::unique_lock<std::mutex> Lock(m_ReceveSynch.m_Mutex);
                    m_MsgToReceive.m_ActualSize = sizeof(MessageHeader);
                    m_ReceiveQueue.push(m_MsgToReceive);
                    m_ReceveSynch.m_Event.notify_one();
                    m_MsgToReceive.m_pBuff.reset();
                    m_ReceiveState = ReceiveState::AcquireBuffer;
                }
                else
                {
                    if(m_MsgToReceive.m_pBuff->m_pPtr->memsize() > reinterpret_cast<MessageHeader*>(m_pCurrRecvPtr)->MessageSize)
                    {
                        m_ReceiveState = ReceiveState::RecvBody;
                        m_MsgToReceive.m_ActualSize = reinterpret_cast<MessageHeader*>(m_pCurrRecvPtr)->MessageSize;
                        m_ReceivingBytes = reinterpret_cast<MessageHeader*>(m_pCurrRecvPtr)->MessageSize - sizeof(MessageHeader);
                    }
                    else
                    {
                        m_ReceiveState = ReceiveState::DiscardBody;
                        m_ReceivingBytes = reinterpret_cast<MessageHeader*>(m_pCurrRecvPtr)->MessageSize - sizeof(MessageHeader);
                    }
                }
            }
        }
        SOCK_EXCEPT_CATCH_BEGIN_NOREP
        reset();
        SOCK_EXCEPT_CATCH_END
        break;

    case ReceiveState::RecvBody:
        SOCK_TRY
        {
            int32_t Received = m_pWorkingSocket->receive(m_pCurrRecvPtr + reinterpret_cast<MessageHeader*>(m_pCurrRecvPtr)->MessageSize - m_ReceivingBytes, m_ReceivingBytes);
            m_ReceivingBytes -= Received;
            if(0 < m_ReceivingBytes)
                return;
            else
            {
                std::unique_lock<std::mutex> Lock(m_ReceveSynch.m_Mutex);
                m_ReceiveQueue.push(m_MsgToReceive);
                m_ReceveSynch.m_Event.notify_one();
                m_MsgToReceive.m_pBuff.reset();
                m_ReceiveState = ReceiveState::AcquireBuffer;
            }
        }
        SOCK_EXCEPT_CATCH_BEGIN_NOREP
        reset();
        SOCK_EXCEPT_CATCH_END
        break;

    case ReceiveState::DiscardBody:
        SOCK_TRY
        {
	    int32_t Received = m_pWorkingSocket->receive(m_DiscardBuffer, std::min<int32_t>(static_cast<int32_t>(sizeof(m_DiscardBuffer)), m_ReceivingBytes));
            m_ReceivingBytes -= Received;
            if(0 < m_ReceivingBytes)
                return;
            else
                m_ReceiveState = ReceiveState::AcquireBuffer;
        }
        SOCK_EXCEPT_CATCH_BEGIN_NOREP
        reset();
        SOCK_EXCEPT_CATCH_END
        break;

    default:
        ;
    }
}

void TestTcpConnection::sendRoutine()
{
    switch(m_SendState)
    {
    case SendState::WaitingFor:
        {
            std::unique_lock<std::mutex> Lock(m_SendSynch.m_Mutex);
            if(m_SendQueue.empty())
            {
                std::cv_status WaitStatus = m_SendSynch.m_Event.wait_for(Lock, std::chrono::seconds{1});
                if(std::cv_status::timeout == WaitStatus)
                    return;
            }
            if(m_SendQueue.empty())
                return;
            m_MsgToSend = m_SendQueue.front();
            m_SendQueue.pop();
            m_SendReminder = m_MsgToSend.m_ActualSize;
        }
        if(m_pWorkingSocket && m_pWorkingSocket->isConnected())
        {
            SOCK_TRY
            {
                int32_t Sent = m_pWorkingSocket->send(static_cast<char*>(*(m_MsgToSend.m_pBuff->m_pPtr)), m_MsgToSend.m_ActualSize);
                if(0 < (m_SendReminder - Sent))
                {
                    m_SendState = SendState::Sending;
                    m_SendReminder -= Sent;
                }
                else
                    m_MsgToSend.m_pBuff.reset();
            }
            SOCK_EXCEPT_CATCH_BEGIN_NOREP
            reset();
            m_MsgToSend.m_pBuff.reset();
            SOCK_EXCEPT_CATCH_END
        }
        break;

    case SendState::Sending:
        if(m_pWorkingSocket && m_pWorkingSocket->isConnected())
        {
            SOCK_TRY
            {
                int32_t Sent = m_pWorkingSocket->send(m_MsgToSend.m_pBuff->m_pPtr->m_pBuffer + m_MsgToSend.m_ActualSize - m_SendReminder, m_SendReminder);
                if(0 == (m_SendReminder - Sent))
                {
                    m_SendState = SendState::WaitingFor;
                    m_MsgToSend.m_pBuff.reset();
                }
                m_SendReminder -= Sent;
            }
            SOCK_EXCEPT_CATCH_BEGIN_NOREP
            reset();
            m_MsgToSend.m_pBuff.reset();
            SOCK_EXCEPT_CATCH_END
        }
        break;

    default:
        ;
    }
}

void TestTcpConnection::reset()
{
    TcpConnection::reset();
    m_SendState = SendState::WaitingFor;
    m_SendReminder = 0;
    m_ReceiveState = ReceiveState::AcquireBuffer;
    m_ReceivingBytes = sizeof(MessageHeader);
}

bool TestTcpConnection::send(CharBufferElement& MsgToSend)
{
    if(!m_pSendThread->isRunning())
    {
        return false;
    }
    std::unique_lock<std::mutex> Lock(m_SendSynch.m_Mutex);
    m_SendQueue.push(MsgToSend);
    m_SendSynch.m_Event.notify_one();
    return true;
}

CharBufferElement TestTcpConnection::recv(uint32_t TimeoutMs)
{
    CharBufferElement RecvMsg;
    std::unique_lock<std::mutex> Lock(m_ReceveSynch.m_Mutex);
    if(m_ReceiveQueue.empty())
    {
        std::cv_status WaitStatus = m_ReceveSynch.m_Event.wait_for(Lock, std::chrono::milliseconds{TimeoutMs});
        if(std::cv_status::timeout == WaitStatus)
            return RecvMsg;
    }
    if(m_ReceiveQueue.empty())
        return RecvMsg;
    RecvMsg = m_ReceiveQueue.front();
    m_ReceiveQueue.pop();
    return RecvMsg;
}
