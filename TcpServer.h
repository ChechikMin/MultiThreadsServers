#include "AbstractServer.h"
#include <mutex>
#include <iostream>
#include <thread>
#include <regex>
#include <numeric>
#define NIX(exp) exp
#define WIN(exp)

class TcpServer : public AbstractServer
{
private:
    std::list<Socket> sockets;
    std::string m_host;
    uint32_t m_port;
    std::string msgToClient;
    struct sockaddr_in servaddr, cli;
    SocketState m_servState = Disconnected;
    Socket serverSocket;
    handlerMsg m_handlerMsg;
    std::mutex m;
public:    
    TcpServer (std::string host, uint32_t port);
    virtual void sendData(std::string msg) override;
    virtual std::string recvData() override;
    virtual SocketState status() const override;
    virtual void startServer() override;
    virtual void transformData() override;
	virtual uint8_t closeServer() override;
	virtual void setHandleMsg(handlerMsg) override;
    virtual ~TcpServer() {
        closeServer();};
};

TcpServer ::TcpServer (std::string host, uint32_t port) : m_host(host), m_port(port)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(m_port);

     if ((bind(serverSocket, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

}

void TcpServer ::startServer()
{
    if ((listen(serverSocket, SOMAXCONN)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    
    socklen_t len = sizeof(cli);
    for(;;)
    {
        Socket sock = accept(serverSocket, (struct sockaddr *)&cli, &len);
        sockets.push_back( std::move(sock));
        if (sock < 0) {
            printf("server accept failed...\n");
            exit(0);
    }
        else
            printf("server accept the client...\n");

    m_servState = Connected;
    }
}

SocketState TcpServer :: status () const{
    return m_servState;
}
void TcpServer :: sendData(std::string msg = ""){
    std::unique_lock<std::mutex> lock(m);
    for(auto &socket : sockets)
        send(socket, msgToClient.c_str(), msgToClient.size(), 0);
    msgToClient.clear();
}
std::string TcpServer::recvData(){
    char buff[16];
    int answer;
    std::unique_lock<std::mutex> lock(m);
    for(auto &socket : sockets)
         answer = recv(socket, buff, 16, 0);
         if(answer)
         {
            std::cout<<buff;
            msgToClient = std::string(buff);
         }
    
    return msgToClient;
}
uint8_t TcpServer :: closeServer(){
    std::unique_lock<std::mutex> lock(m);
    for(auto &socket : sockets){
        shutdown(socket,SHUT_RDWR);
        close(socket);
    }
    return 0;
}

void TcpServer :: setHandleMsg(handlerMsg handler){
    m_handlerMsg = handler;
}
void TcpServer :: transformData(){
    m_handlerMsg(msgToClient);
}