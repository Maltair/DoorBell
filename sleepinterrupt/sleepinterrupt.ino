/* YourDuinoStarter Example: nRF24L01 Transmit Joystick values
 - WHAT IT DOES: Reads Analog values on A0, A1 and transmits
   them over a nRF24L01 Radio Link to another transceiver.
 - SEE the comments after "//" on each line below
 - CONNECTIONS: nRF24L01 Modules See:
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
   - 
   Analog Joystick or two 10K potentiometers:
   GND to Arduino GND
   VCC to Arduino +5V
   X Pot to Arduino A0
   Y Pot to Arduino A1
   
 - V1.00 11/26/13
   Based on examples at http://www.bajdi.com/
   Questions: terry@yourduino.com */



#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h>
int pin = 2;
volatile int state = LOW; 

/*-----( Import needed libraries )-----*/
#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>
#include "RF24.h"
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10
//#define JOYSTICK_X A0
//#define JOYSTICK_Y A1

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe[2] =  {0xa5a5a5a5a1LL, 0xa5a5a5a5b1LL};//0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
//int joystick[2];  // 2 element array holding Joystick readings
String message[] = "dingdong";
int sizeOfMessage;
int messageRepeat = 1;
bool txResult[3];

void setup(void)
{
    DDRD &= B00000011;       // set Arduino pins 2 to 7 as inputs, leaves 0 & 1 (RX & TX) as is
    //DDRB = B00000000;        // set pins 8 to 13 as inputs
    PORTD |= B11111000;      // enable pullups on pins 3 to 7
    //PORTB |= B11111111;      // enable pullups on pins 8 to 13
    //pinMode(13,OUTPUT);      // set pin 13 as an output so we can use LED to monitor
    //digitalWrite(13,HIGH);   // turn pin 13 LED on
    Serial.begin(9600);
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_2MBPS);
    radio.setAutoAck(1);                     // Ensure autoACK is enabled
    radio.setPayloadSize(32);
    radio.enableDynamicPayloads();
    radio.setChannel(0x60);
    radio.setRetries(15,15);                  // Optionally, increase the delay between retries & # of retries
    radio.setCRCLength(RF24_CRC_16);          // Use 8-bit CRC for performance
    Serial.println("running");
    
    radio.openWritingPipe(pipe[0]);
    radio.openReadingPipe(1, pipe[1]);
    sizeOfMessage = sizeof(message);
    
 }//--(end setup )---

                //
void loop(void)
{
    //delay(100);
    Serial.println("Sleeping");
    delay(300);
    sleepNow();
    Serial.println("Detection");
    //delay(1000);
    //exit from sleep means interrupt has occurred
    //send the message x number of times
    for (int i=0; i < messageRepeat; i++){
      delay(100);
      //Serial.println("Sending/n");
      //radio.write(message, sizeOfMessage );
      //if (!radio.write(&message, sizeOfMessage )){Serial.println("tx failed/n");}
      radio.write(&message, sizeOfMessage);
    }   
      delay(1);    
}//--(end main loop )---
                //
void sleepNow(void)
{
    // Set pin 2 as interrupt and attach handler:
    attachInterrupt(0, pinInterrupt, FALLING); //pin 2 is int0
   
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Choose our preferred sleep mode:
    sleep_enable();    // Set sleep enable (SE) bit:
    sleep_mode();  // Put the device to sleep:
    sleep_disable(); // clear sleep enable (SE) bit
    //digitalWrite(13,HIGH);   // turn LED on to indicate awake
}
                //
void pinInterrupt(void)
{
    detachInterrupt(0);
}
