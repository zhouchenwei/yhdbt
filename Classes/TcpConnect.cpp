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
	unique_lock<mutex> lk(_muxSend);
	int cmd = msg.first;
	string content = msg.second;
	int dateLen = content.length();
	int flag = comm_flag;
	string sendText = string((char*)&flag, 4) + string((char*)&cmd, 4) + string((char*)&dateLen, 4) + content;
	size_t len = 0;
	while (len < sendText.length()) {
		int n = send(_sock, sendText.c_str() + len, sendText.length() - len, 0);
		if (n <= 0) {
			GameError();
			return false;
		}
		len += n;
	}
	return true;
}

bool CTcpConnect::RecvQuery(string& content)
{
	char buf[8] = { 0 };
	int recvlen = 0;
	while (recvlen < 8){
		int n = recv(_sock, buf+recvlen, 8-recvlen, 0);
		if (n <= 0) {
			GameError();
			return false;
		}
		recvlen += n;
	}
	int flag = *((int*)buf);
	int len = *((int*)(buf+4));
	if (flag != comm_flag || len < 0 || len > 5000) {
		GameError();
		return false;
	}

	recvlen = 0;
	content.resize(len);
	while (recvlen < len) {
		int n = recv(_sock, (char*)(content.c_str() + recvlen), len - recvlen, 0);
		if (n <= 0) {
			GameError();
			return false;
		}
		recvlen += n;
	}
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
