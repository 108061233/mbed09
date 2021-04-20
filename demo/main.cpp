#include "mbed.h"
#include "mbed_rpc.h"

/**
 *  This example program has been updated to use the RPC implementation in the new mbed libraries.
 *  This example demonstrates using RPC over serial
**/

DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
BufferedSerial pc(USBTX, USBRX);
void LEDBlink(Arguments *in, Reply *out);
void BL2(void);
void BL3(void);
RPCFunction rpcLED(&LEDBlink, "LEDBlink");
double x, y;
Thread Blink2;
Thread Blink3; 

int main() {
    //The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class

    // receive commands, and send back the responses
    char buf[256], outbuf[256];

    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");

    Blink2.start(&BL2);
    Blink3.start(&BL3);

    while(1) {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = fgetc(devin);
            if (recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        printf("%s\r\n", outbuf);
    }
}

// Make sure the method takes in Arguments and Reply objects.
void LEDBlink (Arguments *in, Reply *out)   {
    bool success = true;

    // In this scenario, when using RPC delimit the two arguments with a space.
    x = in->getArg<double>();
    y = in->getArg<double>();

    if (success) {
            myled2 = 0;
            myled3 = 0;     
    } else {
        out->putData("Failed to execute LED control.");
    }
}

void BL2()
{
    while(1)
    {
        if (x) {
            myled2 = !myled2;
            ThisThread::sleep_for(500ms);
        } 
        else myled2 = 0;
    }
}

void BL3()
{
    while(1)
    {
        if (y) {
            myled3 = !myled3;
            ThisThread::sleep_for(500ms);
        } 
        else myled3 = 0;
    }
}