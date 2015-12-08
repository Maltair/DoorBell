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
   Arduino pin 2 for wake interupt
   Arduino pin 3 for rf module power supply
*/

#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h>
volatile int state = LOW; 

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include "RF24.h"

/*-----( Declare Constants and Pin Numbers )-----*/
#define INT_PIN 2
#define RADIO_POWER_PIN 3
#define CE_PIN   9
#define CSN_PIN 10

#define messageRepeat 7

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe[2] =  {0xa5a5a5a5a1LL, 0xa5a5a5a5b1LL};//0xE8E8F0F0E1LL; // Define the transmit pipe

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
String message[] = "dingdong";
int sizeOfMessage;
int count;

void setup(void)
{
    pinMode(CE_PIN, OUTPUT);
    pinMode(CSN_PIN, OUTPUT);
    pinMode(INT_PIN, INPUT_PULLUP);
    pinMode(RADIO_POWER_PIN, INPUT);
    Serial.begin(9600);
    sizeOfMessage = sizeof(message); 
 }//--(end setup )---

void loop(void)
{
    Serial.println("Sleeping");
    delay(100);    //wait for serial to send
    sleepNow();
    Serial.println("Detection"); //sleep was exited by an interupt
    startRadio();
    for (int i=0; i < messageRepeat; i++){
      while (!radio.write(message, sizeOfMessage) && count < messageRepeat)
      {  
        delay(15);
        Serial.print(count%10);
        count++;
      } 
      count = 0;
    }  
    stopRadio();
    Serial.println("done");
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
}

void pinInterrupt(void)
{
    detachInterrupt(0);
}

void startRadio(void)
{
    pinMode(RADIO_POWER_PIN, OUTPUT);
    digitalWrite(RADIO_POWER_PIN, HIGH); //Send power to the radio
    delay(200);    //allow time to wake ] up
    radio.begin(); 
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_2MBPS);
    radio.setAutoAck(1);                     
    radio.setPayloadSize(32);
    radio.enableDynamicPayloads();
    radio.setChannel(0x60);
    radio.setRetries(15,15);                  
    radio.setCRCLength(RF24_CRC_16);          
    radio.openWritingPipe(pipe[0]);
    radio.openReadingPipe(1, pipe[1]);
}

void stopRadio()
{
    pinMode(RADIO_POWER_PIN, INPUT);  //let radio power bleed out, dont pull low and short out. no need
}

