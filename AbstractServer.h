#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <list>
#include <functional>

typedef uint8_t Socket;

struct HeartBeat{};

    enum SocketState{
        Connected,
        Disconnected,
    };

class AbstractServer
{
protected:
    typedef std::function<void(std::string&)> handlerMsg;
    typedef std::function<void()> handlerDisc;

public:
    virtual void sendData(std::string) = 0;
    virtual void transformData() = 0;    
    virtual std::string recvData() = 0;
    virtual SocketState status() const = 0;
    virtual void startServer() = 0;
	virtual uint8_t closeServer() = 0;
	virtual void setHandleMsg(handlerMsg) = 0;
    virtual ~AbstractServer() = default;
};
