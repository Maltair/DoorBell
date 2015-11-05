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
//volatile int state = LOW; 

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10
#define JOYSTICK_X A0
#define JOYSTICK_Y A1

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[2];  // 2 element array holding Joystick readings


void setup(void)
{
    DDRD &= B00000011;       // set Arduino pins 2 to 7 as inputs, leaves 0 & 1 (RX & TX) as is
    //DDRB = B00000000;        // set pins 8 to 13 as inputs
    PORTD |= B11111100;      // enable pullups on pins 2 to 7
    //PORTB |= B11111111;      // enable pullups on pins 8 to 13
    pinMode(13,OUTPUT);      // set pin 13 as an output so we can use LED to monitor
    digitalWrite(13,HIGH);   // turn pin 13 LED on
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(pipe);
}//--(end setup )---

                //
void loop(void)
{
    // Stay awake for 1 second, then sleep.
    // LED turns off when sleeping, then back on upon wake.
    delay(1000);
    Serial.println(20);
    
    
    
    joystick[0] = analogRead(JOYSTICK_X);
    joystick[1] = analogRead(JOYSTICK_Y);
  
    radio.write( joystick, sizeof(joystick) );
    Serial.println(21);
    delay(1000);
    
    sleepNow();
}//--(end main loop )---
                //
void sleepNow(void)
{
  Serial.println(40);
    // Set pin 2 as interrupt and attach handler:
    attachInterrupt(0, pinInterrupt, LOW);
    delay(100);
    //
    // Choose our preferred sleep mode:
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    //
    // Set sleep enable (SE) bit:
    sleep_enable();
    //
    // Put the device to sleep:
    digitalWrite(13,LOW);   // turn LED off to indicate sleep
    Serial.println(41);
    delay(100);
    sleep_mode();
    //
    // Upon waking up, sketch continues from this point.
    Serial.println(42);
    sleep_disable();
    digitalWrite(13,HIGH);   // turn LED on to indicate awake
}
                //
void pinInterrupt(void)
{
    detachInterrupt(0);
  Serial.println(60);
}
