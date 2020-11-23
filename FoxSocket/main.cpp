#include "FoxTcpServerSocket.h"

#include <STLTimedTask.h>
#include <STLDemoRunnable.h>
#include "FoxTcpServerDemoHandler.h"

int main()
{
	FoxTcpServerSocket socket;
	socket.bindSocketHandler(new FoxTcpServerDemoHandler());

	socket.close();

	socket.start(98231);

	this_thread::sleep_for(chrono::milliseconds(1000 * 20000));

	socket.close();

	socket.start(98231);


	this_thread::sleep_for(chrono::milliseconds(1000 * 1000));

//	socket.create("127.0.0.1",98231);

//	STLTimedTask time(new STLDemoRunnable());
//	time.start(1000);
//	this_thread::sleep_for(chrono::milliseconds(1000 * 60));
//	time.close();

	int i = 0;
}