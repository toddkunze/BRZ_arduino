#include <SPI.h>            //standard library
#include <mcp_can.h>        //mcp library must be downloaded and moved into your arduino library folder on the PC
#include <mcp_can_dfs.h>    //mcp library must be downloaded and moved into your arduino library folder on the PC
                            //on my PC the standard path for the library was Documents\Arduino\libraries. You put the unzipped MCP folder into the libraries foler.

int analogPin = A0;         //set analog input pin, A0 is an option
int val = 0;                //initialize a variable
unsigned highbyte;          //output of pressure sensor will need two bytes, this variable will be the high byte
unsigned lowbyte;           //output of pressure sensor will need two bytes, this variable will be the high byte

const int SPI_CS_PIN = 9;   //set SPI pin, if you buy a Seeed CAN shield V2 pin 9 will be the default

MCP_CAN CAN(SPI_CS_PIN);    //initialzes your CAN network
void setup()
{
  CAN.begin(CAN_500KBPS);   //set CAN speed to 500 baud, most vehicle networks with throttle, brake, engine data are on 500baud
}

void loop()
{
  val = analogRead(analogPin);  //read the pressure sensor input pin
  highbyte = highByte(val);     //split the pressure data into high byte and low byte
  lowbyte = lowByte(val);
  unsigned char OilPressure[8] = {highbyte, lowbyte, highbyte, lowbyte, 0, 0, 0, 0};      //creates a vector with the CAN data to be sent. [8] is the length. I sent the data twice because I wanted to process sensor voltage and psi 
  CAN.sendMsgBuf(0x169, 0, 8, OilPressure);                                               //CAN.sendMsgBuf(CAN ID, frame type, length, data). CAN ID 0x... means hexidecimal number, frame type 0 is standard 11 bit CAN. The length can be whatever you want but must match the data vector length
  //remember you are sending raw data, it must be processed to volts or PSI on the equipment side.

  delay(100);                   //pressure sensor data sent every 100ms
}
