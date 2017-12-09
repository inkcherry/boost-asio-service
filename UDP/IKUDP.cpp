﻿#include "IKUDP.h"
#include "IKUDP.h"
#include <system_error>
#define DAYTIME_BUF_SIZE    64  /* 缓冲区的大小 */
#define DAYTIME_DEF_COUNT    10  

void IKUDP:: _init_wsadata() {
	WSAStartup(WINSOCK_VERSION, &wsa);
	iksock = socket(AF_INET, SOCK_DGRAM, 0);
	if (iksock == INVALID_SOCKET)
			throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket");

}
IKUDP::IKUDP():iksock(0),flag(0)
{
	_init_wsadata();
	_init_buffer();
}
//IKUDP::IKUDP(addr_config &config_):port(config_.port), flag(0)
//{
//	_init_wsadata();
//	_init_buffer();
//}
void IKUDP::_init_buffer()
{
	buffer_config = std::make_pair(64, nullptr);
	//buffer_config.second = new char[buffer_config.first];  
}

IKUDP::~IKUDP()
{
	closesocket(iksock);
	WSACleanup();
}
bool IKUDP::Send(const std::string& buffer) 
{
	return Send(buffer.c_str(), buffer.length());
}


bool IKUDP::Send(const char *buffer, int buffer_len)
{

	if (addr.sin_addr.s_addr == INADDR_NONE)
		throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
	int ret = sendto(iksock, buffer, buffer_len, 0,
		(struct sockaddr *)&addr, sizeof(addr));
	if (ret == buffer_len)      //完全发送算作发送成功
		return true;
	else  //发送失败
	{
		std::cout << "send error with" << WSAGetLastError() << std::endl;
		return false;
	}
}

sockaddr_in IKUDP::Recv(char *buffer)  
{  
	sockaddr_in peer_addr;  //from（默认的监听addr)
	int peer_addr_size = sizeof(peer_addr);
	int ret=recvfrom(iksock,buffer,buffer_config.first, flag,
		(struct sockaddr *)&peer_addr, &peer_addr_size);
	//UDP无连接 故recvfrom返回值始终为0
    //有连接时 TCP 0表示断开连接
	if (ret < 0)
		std::cout << "recv error with"<<WSAGetLastError()<<std::endl;
	return  peer_addr;
}

void IKUDP::ReSet_buffer_size(int &buffer_size_)
{
	//delete buffer_config.second;  //删除原来定义的位置
	buffer_config.first = buffer_size_; 
	/*buffer_config.second = new char[buffer_size_];*/
}
