
#include "TcpServer.h"
#include <memory>
#define C_ALL(X)

using namespace std;
bool flag = true;
const string msg = "TcpServer: ok!\n";

int main(int argc, char** argv){

    std::string host(argv[1]);
    int port = std::atoi(argv[2]);

    auto toInt = [](int sum,smatch s ){
        return std::stoi(s.str()) + sum;
    };
    auto toStr = [](std::string str, smatch s){
        return s.str() + std::move(str);
    };

    std::function<void(std::string&)> handlerMsg = [&](std::string& msgToClient) -> void
    {
        msgToClient = "TcpServer: ok!\n";
        // const regex r("R[-|+?[0-9]+]*");
        // auto words_begin = 
        // std::sregex_iterator(msgToClient.begin(), msgToClient.end(), r);
        // auto words_end = std::sregex_iterator();
 
        // // std::cout << "Found " 
        // //       << std::distance(words_begin, words_end) 
        // //       << " words:\n";
 
        // for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        //     std::smatch match = *i;                                                 
        //     std::string match_str = match.str(); 
        //     //std::cout << match_str << '\n';

        
        //int tmp = 0;
        //std::accumulate(words_begin, words_end, tmp, toInt);
        
        //msgToClient += std::to_string(tmp);
        //std::cout<<msgToClient ;
        //msgToClient += std::accumulate(words_begin, words_end,msgToClient, toStr);
    };
try
{
    std::shared_ptr< TcpServer> tcpServer ( new TcpServer(host, port));
    tcpServer->setHandleMsg(handlerMsg);
    thread t1(&TcpServer::startServer, std::ref(*tcpServer));

    if ( tcpServer->status() == SocketState::Connected )
    {        
        
            while ( strncmp("exit", tcpServer->recvData().c_str(), 4) != 0 )
            {
                tcpServer->transformData();
                tcpServer->sendData();
            }
    }
    t1.join();
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}

return 0;
}
