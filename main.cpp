#include "mbed.h"
#include "bbcar.h"
#include "drivers/DigitalOut.h"

#include "erpc_simple_server.h"
#include "erpc_basic_codec.h"
#include "erpc_crc16.h"
#include "UARTTransport.h"
#include "DynamicMessageBufferFactory.h"
#include "bbcar_control_server.h"

Ticker speed_ticker;
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BBCar car(pin5, pin6, servo_ticker);
DigitalInOut FR(D2), R(D3), L(D4), FL(D7);
BufferedSerial pc(USBTX, USBRX);

Thread t(osPriorityLow);
Ticker encoder_ticker;
DigitalIn encoder(D11);
DigitalInOut pin10(D10);
volatile int steps;
volatile int last;
int right = 0;
int left = 0;
int nextturn = 3;
float spd = 0;
float lastdis = 0;

ep::UARTTransport uart_transport(D1, D0, 9600);
ep::DynamicMessageBufferFactory dynamic_mbf;
erpc::BasicCodecFactory basic_cf;
erpc::Crc16 crc16;
erpc::SimpleServer rpc_server;

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

void speedcal()
{
    spd = steps * 6.5 * 3.14 / 32 - lastdis; 
    lastdis = steps * 6.5 * 3.14 / 32;
}

void info()
{
    while(1)
    {
    printf("distance travelled = %f\n", steps * 6.5 * 3.14 / 32);
    printf("current speed = %f\n", spd);
    ThisThread::sleep_for(2s);
    }
}


void rpc()
{
    rpc_server.run();
}

BBCarService_service car_control_service;

int main()
{
    // printf("start\n");
    parallax_ping  ping1(pin10);
    pc.set_baud(9600);
    steps = 0;
    last = 0;
    encoder_ticker.attach(&encoder_control, 1ms);
    speed_ticker.attach(&speedcal, 1s);
    uart_transport.setCrc16(&crc16);
    rpc_server.setTransport(&uart_transport);
    rpc_server.setCodecFactory(&basic_cf);
    rpc_server.setMessageBufferFactory(&dynamic_mbf);
    rpc_server.addService(&car_control_service);
    t.start(rpc);
    while (1) {
        FR.output();
        FL.output();
        R.output();
        L.output();
        FR = 1;
        R = 1;
        L = 1;
        FL = 1;
        wait_us(230);
        FR.input();
        FL.input();
        R.input();
        L.input();
        wait_us(230);
        int pattern = FL * 1000 + L * 100 + R * 10 + FR;
        // printf("%d\n", pattern);
        // printf("%lf\n",float(ping1));
        if (float(ping1) < 27)
        {
            car.rotation1(50);
            ThisThread::sleep_for(2100ms);
        }
        if (pattern == 1000) {
            right = 0;
            left = 0;
            car.turn(200, 0.0000000001);
            pin5 = 0;
        }

        else if (pattern == 1100) {
            right = 0;
            left = 0;
            car.turn(100, 0.0001);
        }
        else if (pattern == 100) {
            right = 0;
            left = 0;
            car.turn(160, 0.7);
        }
        else if (pattern == 110) {
            right = 0;
            left = 0;
            car.goStraight(120);
        }
        else if (pattern == 10) {
            right = 0;
            left = 0;
            car.turn(160, -0.4);
        }
        else if (pattern == 11) {
            right = 0;
            left = 0;
            car.turn(200, -0.00000001);
        }
        else if (pattern == 1) {
            right = 0;
            left = 0;
            car.rotation1(200);
            pin6 = 0;
        } 
        else if (pattern == 111)
        {
            // printf("!!!!!\n");
            right++;
            if (right >= 5)
            {
            // printf("right\n");
            nextturn = 0;
            car.goStraight(120);
            }
        }
        else if (pattern == 1110)
        {
            // printf("?????\n");
            left++;
            if (left >= 5)
            {
            // printf("left\n");
            nextturn = 1;
            car.goStraight(120);
            }
        }
        else if (pattern == 1111)
        {
            if (nextturn == 0)
            {
                car.turn(200, -0.00000001);
                ThisThread::sleep_for(500ms);
            }
            else if (nextturn == 1)
            {
                car.turn(100, 0.0001);
                ThisThread::sleep_for(500ms);
            }
            else car.goStraight(120);
        }
        else if (pattern == 0)
        {
            car.stop();
        }
        else car.goStraight(150);
        ThisThread::sleep_for(20ms);
    }

}

