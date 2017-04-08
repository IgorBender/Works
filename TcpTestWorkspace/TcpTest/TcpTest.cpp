/*
 * TcpTest.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: Igor Bender
 */

#include <iostream>
#include <sstream>
#include <TestTcpConnection.h>
#include <ThreadClass.h>

using namespace std;

struct RecvFunctor : public ThreadClassLib::ThreadClass::ThreadRoutineType
{
    RecvFunctor(TestTcpConnection* pConnection, bool& Exit) : m_pConnection(pConnection), m_Exit(Exit) {}
    virtual void operator()()
    {
        CharBufferElement Msg;
        Msg = m_pConnection->recv(1000);
        if(0 < Msg.m_ActualSize && !m_Exit)
        {
            MessageHeader* pHead = reinterpret_cast<MessageHeader*>(static_cast<char*>(*(Msg.m_pBuff->m_pPtr)));
            if(EXIT_MSG_TYPE == pHead->MessageType)
            {
                cout << "Exit..." << endl;
                m_Exit = true;
                BuffersPool::ResourcePtrType SendBuf = m_pConnection->acquireSendBuffer();
                if(nullptr == SendBuf->m_pPtr)
                    return;
                CharBufferElement SendMsg(Msg.m_ActualSize, 0, SendBuf);
                memcpy(static_cast<char*>(*(SendMsg.m_pBuff->m_pPtr)), static_cast<char*>(*(Msg.m_pBuff->m_pPtr)), Msg.m_ActualSize);
                m_pConnection->send(SendMsg);
            }
            else
            {
                char Tmp[128];
                memcpy(Tmp, static_cast<char*>(*(Msg.m_pBuff->m_pPtr)) + sizeof(MessageHeader), Msg.m_ActualSize - sizeof(MessageHeader));
                Tmp[Msg.m_ActualSize - sizeof(MessageHeader)] = '\0';
                cout << Tmp << endl;
            }
            if(!m_Exit)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds{200});
                BuffersPool::ResourcePtrType SendBuf = m_pConnection->acquireSendBuffer();
                if(nullptr == SendBuf->m_pPtr)
                    return;
                CharBufferElement SendMsg(Msg.m_ActualSize, 0, SendBuf);
                memcpy(static_cast<char*>(*(SendMsg.m_pBuff->m_pPtr)), static_cast<char*>(*(Msg.m_pBuff->m_pPtr)), Msg.m_ActualSize);
                m_pConnection->send(SendMsg);
            }
        }
    }
    TestTcpConnection* m_pConnection = nullptr;
    bool& m_Exit;
};

int main(int argc, char* argv[])
{
    cout << "Usage : " << argv[0] << " server/client port address" << endl;
    uint32_t Address = INADDR_ANY;
    if(argc == 4)
        Address = inet_addr(argv[3]);
    if(INADDR_NONE == Address)
        Address = INADDR_ANY;
    uint16_t Port = 0;
    std::stringstream Conversion;
    Conversion << argv[2] << std::ends;
    Conversion >> Port;
    if(!Conversion)
    {
        cout << "Wrong port number..." << endl;
        exit(1);
    }
    bool IsInitiator = !(std::string("server") == std::string(argv[1]));

    TestTcpConnection TcpConnection;
    bool Exit = false;
    RecvFunctor Rcv(&TcpConnection, Exit);
    ThreadClassLib::ThreadClass RecvThread(&Rcv, true);
    RecvThread.nameThread("Test receive");
    RecvThread.run();
    RecvThread.start();

    TcpConnection.init(Address, htons(Port), 0, IsInitiator);

    while(!TcpConnection.isWorking())
        this_thread::sleep_for(chrono::milliseconds{100});

    if(IsInitiator)
    {
        BuffersPool::ResourcePtrType SendBuf = TcpConnection.acquireSendBuffer();
        if(nullptr == SendBuf->m_pPtr)
            return 1;
        MessageType2* pMsg = reinterpret_cast<MessageType2*>(static_cast<char*>(*(SendBuf->m_pPtr)));
        pMsg->Header.MessageType = 0;
        pMsg->Header.MessageSize = strlen("hello") + sizeof(MessageHeader);
        strcpy(reinterpret_cast<char*>(pMsg->Body.Field1), "hello");
        CharBufferElement SendMsg(strlen("hello") + sizeof(MessageHeader), 0, SendBuf);
        TcpConnection.send(SendMsg);

    }
    uint32_t Counter = 0;
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds{1});
        if(Exit)
            break;
        if(IsInitiator && 3 == Counter++)
        {
            BuffersPool::ResourcePtrType SendBuf = TcpConnection.acquireSendBuffer();
            if(nullptr == SendBuf->m_pPtr)
                return 1;
            cout << "Send exit." << endl;
            ExitMessage* pMsg = reinterpret_cast<ExitMessage*>(static_cast<char*>(*(SendBuf->m_pPtr)));
            pMsg->MessageType = EXIT_MSG_TYPE;
            pMsg->MessageSize = sizeof(ExitMessage);
            char Tmp[128];
            memcpy(Tmp, SendBuf->m_pPtr->m_pBuffer, 32);
            CharBufferElement SendMsg(sizeof(ExitMessage), 0, SendBuf);
            TcpConnection.send(SendMsg);
        }
    }
    RecvThread.stop();
    RecvThread.exit();
    RecvThread.join();
    TcpConnection.down();

    return 0;
}

