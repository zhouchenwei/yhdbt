#ifndef _H_CTCPCONNECT_H_
#define _H_CTCPCONNECT_H_
#include <string>
#include <map>
#include <mutex>
#include "Singleton.h"

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
typedef SOCKET socket_t;
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef int socket_t;
#endif

using namespace std;

class CTcpConnect
{
public:
	CTcpConnect();
	~CTcpConnect();

	bool	ConnectServer(const string& ip, unsigned short port);
	bool	SendQuery(pair<int, string> msg);
	bool	RecvQuery(string& content);
	bool    SendAndRecv(pair<int, string> msg, string& recvs);

	static void	GameError();

private:
	socket_t		_sock = 0;
	mutex			_muxSend;
};

typedef CSingleton<CTcpConnect> tcpConn;
#endif
