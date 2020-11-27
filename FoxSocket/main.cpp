#include "FoxTcpServerSocket.h"

#include <STLTimedTask.h>
#include <STLDemoRunnable.h>
#include "FoxTcpServerDemoHandler.h"
#include "FoxTcpClientDemoHandler.h"
#include "FoxUdpServerDemoHandler.h"
#include "FoxTcpClientSocket.h"
#include "FoxUdpServerSocket.h"

int main()
{
	FoxUdpServerSocket udpsocket;
	udpsocket.bindSocketHandler(new FoxUdpServerDemoHandler());

	udpsocket.close();

	udpsocket.create(98233);

	this_thread::sleep_for(chrono::milliseconds(1000 * 2000));

	udpsocket.close();

	udpsocket.create(98233);

	/*
	std::string data = "send data";

	FoxTcpClientSocket client;
	client.bindSocketHandler(new FoxTcpClientDemoHandler());

	this_thread::sleep_for(chrono::milliseconds(1000 * 1));

	client.connect("127.0.0.1", 98231);	
	
	client.send(data.c_str(), data.size()+1);

	this_thread::sleep_for(chrono::milliseconds(1000 * 100000));
	client.close();

	client.connect("127.0.0.1", 98231);
	client.send(data.c_str(), data.size() + 1);
	this_thread::sleep_for(chrono::milliseconds(1000 * 10000000));
	client.close();

	this_thread::sleep_for(chrono::milliseconds(1000 * 10));
*/
	

/*

	FoxTcpServerSocket socket;
	socket.bindSocketHandler(new FoxTcpServerDemoHandler());

	socket.close();

	socket.create(98232);

	this_thread::sleep_for(chrono::milliseconds(1000 * 200000));

	socket.close();

	socket.create(98231);
	*/

	this_thread::sleep_for(chrono::milliseconds(1000 * 1000));

//	socket.create("127.0.0.1",98231);

//	STLTimedTask time(new STLDemoRunnable());
//	time.start(1000);
//	this_thread::sleep_for(chrono::milliseconds(1000 * 60));
//	time.close();

	int i = 0;
}