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

/// @class TestTcpConnection inherits TcpConnection class, implements
/// receive and send pure virtual methods.
class TestTcpConnection : public TcpConnection
{
public:
    /// @struct Synch - notification synchronization primitives.
    struct Synch
    {
        std::mutex m_Mutex; ///< Mutual exclusive lock.
        std::condition_variable m_Event; ///< Condition variable.
    };

    /// Constructor.
    TestTcpConnection();
    /// Destructor.
    virtual ~TestTcpConnection() {}

    /// Initializing method
    /// @param Addr : for client - server's IP address, for server - local IP adderss or INADDR_ANY, both in network byte order.
    /// @param Port : for client - server's TCP port number, for server - local TCP port number, both in network byte order.
    /// @param Time : time in seconds to try to establish TCP connection, 0 - forever.
    /// @param Initiator : client is initiator.
    /// @return - true if initialized successfully, otherwise - false.
    virtual bool init(unsigned long Addr, unsigned short Port, int32_t Time, bool Initiator)
    {
        bool Result = TcpConnection::init(Addr, Port, Time, Initiator);
        m_pConnectThread->nameThread("TcpTest connection thread");
        m_pRecvThread->nameThread("TcpTest receive thread");
        m_pSendThread->nameThread("TcpTest send thread");
        return Result;
    }
    virtual void reset();

    /// Acqires buffer from pool of buffers for sending.
    /// @return - acqired buffer, if there is no free buffer included pointer will be nullptr.
    BuffersPool::ResourcePtrType acquireSendBuffer()
    {
        return m_SendPool.acqireResource();
    }
    /// SEnd buffer to the other side of TCP virtual connection.
    /// @param MsgToSend : Queue element containig buffer to be sent.
    /// @return - success or failure.
    bool send(CharBufferElement& MsgToSend);
    /// Receive buffer from other side of TCP virtual connection.
    /// @param TimeoutMs : time for wait forbuffer arrival in milliseconds.
    /// @return - receved buffer, if no data is received actual size will be 0.
    CharBufferElement recv(uint32_t TimeoutMs);

protected:
    // Connection staff
    bool m_LastConnectionState = false; ///< Connection status latch.

    // Receive staff
    BuffersPool m_ReceivePool; ///< Buffers pool for received data.
    char m_DiscardBuffer[MaxMessageSize()]; ///< Buffer for discarding data in cases of lack of buffers or wrong message format.
    Synch m_ReceveSynch; ///< Sinchronization structure.
    CharBuffersQueueType m_ReceiveQueue; ///< Received data buffers queue.
    /// @enum Receive state machine states.
    enum class ReceiveState {
        AcquireBuffer,//!< AcquireBuffer
        RecvHeader,   //!< RecvHeader
        RecvBody,     //!< RecvBody
        DiscardBody   //!< DiscardBody
    } m_ReceiveState = ReceiveState::AcquireBuffer; ///< Receive state machine current state.
    CharBufferElement m_MsgToReceive; ///< Received data attribute.
    int32_t m_ReceivingBytes = 0; ///< Number of bytes to be received in each receive iteration.
    char* m_pCurrRecvPtr = nullptr; ///< Receive buffer transitional pointer.

    // Send staff
    BuffersPool m_SendPool; ///< Buffers pool for send data.
    CharBuffersQueueType m_SendQueue; ///< Send data buffers queue.
    Synch m_SendSynch; ///< Sinchronization structure.
    /// @enum Send state machine states.
    enum class SendState {
        WaitingFor,
        Sending
    } m_SendState = SendState::WaitingFor; ///< Send state machine current state.
    CharBufferElement m_MsgToSend; ///< Send data attribute.
    int32_t m_SendReminder = 0; ///< Remindaer of bytes to be sent in each send iteration.

    virtual void connectRoutine(); ///< Connect thred routine.
    virtual void receiveRoutine(); ///< Received thred routine.
    virtual void sendRoutine(); ///< Send thred routine.
};

#endif /* COMMON_TESTTCPCONNECTION_H_ */
