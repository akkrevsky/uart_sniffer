#include <SoftwareSerial.h>
#include <OneWire.h>
SoftwareSerial monitor(10, 11); // RX, TX

void setup() 
{
  monitor.begin(9600);
  Serial.begin(9600);
    while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("virtual com-port enable");
  Serial.println("physical com-port enable");
  Serial.println("wait data from virtual port");
}

void loop() 
{
  monitor.listen();

  while (monitor.available() > 0) 
  {
    char inByte = monitor.read();
    Serial.write(inByte);
  }

}


