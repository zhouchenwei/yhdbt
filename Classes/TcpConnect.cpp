#include "TcpConnect.h"
#include "cocos2d.h"
USING_NS_CC;

#define comm_flag 0x0

CTcpConnect::CTcpConnect()
{
#ifdef _WIN32
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif // _WIN32
}


CTcpConnect::~CTcpConnect()
{
#ifdef _WIN32
	WSACleanup();
#endif // _WIN32
}

bool CTcpConnect::ConnectServer(const string& ip, unsigned short port)
{
#ifdef _WIN32
	if (_sock)
		closesocket(_sock);
#else
	if (_sock)
		close(_sock);
#endif // _WIN32

	struct  sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	servAddr.sin_addr.s_addr = inet_addr(ip.c_str());

	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock == 0)
		return false;
	if (connect(_sock, (struct sockaddr*)&servAddr, sizeof(servAddr)))
		return false;
	return true;
}


bool CTcpConnect::SendQuery(pair<int, string> msg)
{
	
	return true;
}

bool CTcpConnect::RecvQuery(string& content)
{

	return true;
}

bool CTcpConnect::SendAndRecv(pair<int, string> msg, string& recvs)
{
	if (SendQuery(msg))
		return RecvQuery(recvs);

	return false;
}

void CTcpConnect::GameError()
{
	MessageBox(Configuration::getInstance()->getValue("comm_error").asString().c_str(), "error");
	exit(0);
}
