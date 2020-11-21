#include <MCP23S17.h>
//using namespace MCP23S17;

MCP23S17::PortExpanderSPI expOne;
uint8_t a = 0x01;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("init pe");
  expOne.init(53, 0x00);
  Serial.println("init ok");
  expOne.portMode(EPORTA, ALLOUT);
  Serial.println("port dir ok");
  expOne.writePort(EPORTA, 0xFF);
  Serial.println(" write to port ok");
  delay(200);
  Serial.println("exit init");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(a == 0x00)
    a = 0x01;
  expOne.writePort(EPORTA, a);
  Serial.println(a, HEX);
  a <<= 1;
  delay(100);
}
