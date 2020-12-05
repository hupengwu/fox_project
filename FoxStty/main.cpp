#include "FoxStty.h"
#include "FoxDemoHandler.h"

int main()
{
   
    FoxStty foxSerialPort;
    foxSerialPort.bindHandler(new FoxDemoHandler());
    foxSerialPort.open("ttyS1");
    foxSerialPort.setTimeOut(10 * 1000 * 1000);
    foxSerialPort.setParam();

  //  usleep(1000000000);
    int send = 0;
    foxSerialPort.sendData("hupengwu",8, send);
    usleep(1000000000);

    /*
    SerialTest serail_test;

    if (serail_test.ValueInitOpenSerial(COM1) < 0)
    {
        return -1;
    }

    while (1)
    {
       // serail_test.Write();
        serail_test.Read();
     //   setbuf(stdin, NULL);
    //    getchar();
        usleep(100);
    }
    */
    /*
	FoxStty sttyn;
	sttyn.demo();*/
}