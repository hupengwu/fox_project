#include "FoxTcpServerSocket.h"

#include <STLTimedTask.h>
#include <STLDemoRunnable.h>
#include "FoxTcpServerDemoHandler.h"
#include "FoxTcpClientDemoHandler.h"
#include "FoxTcpClientSocket.h"

int main()
{
	std::string data = "send data";

	FoxTcpClientSocket client;
	client.bindSocketHandler(new FoxTcpClientDemoHandler());

	client.connect("127.0.0.1", 98231);	
	this_thread::sleep_for(chrono::milliseconds(1000 * 10));
	client.send(data.c_str(), data.size()+1);
	client.close();

	client.connect("127.0.0.1", 98231);
	client.send(data.c_str(), data.size() + 1);
	this_thread::sleep_for(chrono::milliseconds(1000 * 100));
	client.close();

	this_thread::sleep_for(chrono::milliseconds(1000 * 10));

	

	this_thread::sleep_for(chrono::milliseconds(1000 * 20000));

/*	FoxTcpServerSocket socket;
	socket.bindSocketHandler(new FoxTcpServerDemoHandler());

	socket.close();

	socket.start(98231);

	this_thread::sleep_for(chrono::milliseconds(1000 * 20000));

	socket.close();

	socket.start(98231);


	this_thread::sleep_for(chrono::milliseconds(1000 * 1000));
*/
//	socket.create("127.0.0.1",98231);

//	STLTimedTask time(new STLDemoRunnable());
//	time.start(1000);
//	this_thread::sleep_for(chrono::milliseconds(1000 * 60));
//	time.close();

	int i = 0;
}