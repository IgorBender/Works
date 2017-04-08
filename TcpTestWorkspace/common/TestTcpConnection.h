/*
 * TestTcpConnection.h
 *
 *  Created on: Mar 31, 2017
 *      Author: Igor Bender
 */

#ifndef COMMON_TESTTCPCONNECTION_H_
#define COMMON_TESTTCPCONNECTION_H_

#include "TcpConnection.h"
#include "CharBuffersPool.h"
#include "InterfaceDefs.h"

class TestTcpConnection : public TcpConnection
{
public:
    /// @struct Synch - notification synchronization primitives.
    struct Synch
    {
        std::mutex m_Mutex; ///< Mutual exclusive lock.
        std::condition_variable m_Event; ///< Condition variable.
    };

    TestTcpConnection();
    virtual ~TestTcpConnection();

    virtual bool init(unsigned long Addr, unsigned short Port, int32_t Time, bool Initiator)
    {
        bool Result = TcpConnection::init(Addr, Port, Time, Initiator);
        m_pConnectThread->nameThread("TcpTest connection thread");
        m_pRecvThread->nameThread("TcpTest receive thread");
        m_pSendThread->nameThread("TcpTest send thread");
        return Result;
    }
    virtual void connectRoutine();
    virtual void receiveRoutine();
    virtual void sendRoutine();
    virtual void reset();

    BuffersPool::ResourcePtrType acquireSendBuffer()
    {
        return m_SendPool.acqireResource();
    }
    bool send(CharBufferElement& MsgToSend);
    CharBufferElement recv(uint32_t TimeoutMs);

protected:
    // Connection staff
    bool m_LastConnectionState = false;

    // Receive staff
    BuffersPool m_ReceivePool;
    char m_DiscardBuffer[MaxMessageSize()];
    Synch m_ReceveSynch;
    CharBuffersQueueType m_ReceiveQueue;
    enum class ReceiveState {
        AcquireBuffer,
        RecvHeader,
        RecvBody,
        DiscardBody
    } m_ReceiveState = ReceiveState::AcquireBuffer;
    CharBufferElement m_MsgToReceive;
    int32_t m_ReceivingBytes = 0;
    char* m_pCurrRecvPtr = nullptr;

    // Send staff
    BuffersPool m_SendPool;
    CharBuffersQueueType m_SendQueue;
    Synch m_SendSynch;
    enum class SendState {
        WaitingFor,
        Sending
    } m_SendState = SendState::WaitingFor;
    CharBufferElement m_MsgToSend;
    int32_t m_SendReminder = 0;
};

#endif /* COMMON_TESTTCPCONNECTION_H_ */
