#include <SPI.h>
#include <mcp_can.h>
#include <mcp_can_dfs.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);

void setup()
{
  CAN.begin(CAN_500KBPS);
}

void loop()
{
  getCCmode();
}

void getCCmode()
{
  int CCmode = 1;
  while(CCmode == 1)
  {
    CAN.readMsgBuf(&len, rxBuf);
    rxId = CAN.getCanId();
    if(rxId == 0x144)
    {
      if(rxBuf[0] == 0xE0)
      {
        pedaldance();
      }
      CCmode = 0;
    }
  }
}

void pedaldance()
{
  int CCcount = 1;
  while(CCcount != 0)
  {
    CAN.readMsgBuf(&len, rxBuf);
    rxId = CAN.getCanId();
    if(rxId == 0x144)                 //cruise control buttons on 0x144
    {
      if(rxBuf[0] == 0xE0)            //cruise cancel switch active
      {
        CCcount++;
        delay(500);
      }
      else
      {
        CCcount--;
      }
      if(CCcount == 7)
      {
        sendpedaldance();
      }
      if(CCcount == 20)
      {
        clear_DTC();
      }
    }
  }
}

void sendpedaldance()
{
  unsigned char SendDance1[8] = {0x01, 0x3E, 0, 0, 0, 0, 0, 0};  
  unsigned char SendDance2[8] = {0x03, 0x3B, 0x0B, 0x80, 0, 0, 0, 0};
  CAN.sendMsgBuf(0x7E0, 0, 8, SendDance1);
  delay(100);
  CAN.sendMsgBuf(0x7B0, 0, 8, SendDance2);
}

void clear_DTC()
{
  unsigned char clear_DTC[8] = {0x01, 0x04, 0, 0, 0, 0, 0, 0};  
  CAN.sendMsgBuf(0x7E0, 0, 8, clear_DTC);
}
