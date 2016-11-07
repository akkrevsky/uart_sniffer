#include <stdlib.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <string.h>


// создаём объект класса DHT11 и пераём номер пина к которому подкючён датчик
DHT11 dht(9);
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

#define SSID "rosatom"
#define PASS "12345678qq"
#define IP "184.106.153.149" // thingspeak.com
//http://api.thingspeak.com/update?key=IWEM7Y9CDQFTV5GV&field1=1
String GET = "GET /update?key=IWEM7Y9CDQFTV5GV&field1=";
SoftwareSerial monitor(10, 11); // RX, TX
String tempF[] = {"11","12","13","14"};

void setup()
{
  monitor.begin(115200);
  Serial.begin(9600);
  Serial.println("Nad Proektom Rabotala Komanda AToM :)");
  Serial.println("");

  //sensors.begin();
  sendDebug("AT");
  delay(5000);
  if(monitor.find("OK")){
    Serial.println("RECEIVED: OK");
  }
    connectWiFi();
    

}

void loop()
{
    // переменная состояния датчика
  int check;
  // мониторинг ошибок
  // считывание данных с датчика DHT11
  check = dht.read();
  switch (check) {
    // всё OK
    case DHT_OK:
      // выводим показания влажности и температуры
      Serial.print("Temperatura = ");
      Serial.print(dht.getTemperatureC());
      Serial.print(" C \t");
      Serial.println("");
      Serial.print("Vlazhnost' = ");
      Serial.print(dht.getHumidity());
      Serial.println("%");
       sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);

  // print the results to the serial monitor:
  Serial.print("Light = ");
  Serial.print(sensorValue);
  Serial.print("\t ");
      Serial.println("");
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(100);
  break;
    // ошибка контрольной суммы
    case DHT_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
      
    // превышение времени ожидания
    case DHT_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;
    // неизвестная ошибка
    default:
      Serial.println("Unknown error");
      break;
  }
 


  float sensor_value = dht.getTemperatureC();
  String  test_string = "";
  test_string += sensor_value;
  updateTemp(test_string );
 
}

void updateTemp(String tenmpF){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  //delay(2000);
  if(monitor.find("Error")){
    Serial.print("RECEIVED: Error");
    return;
  }
  cmd = GET;
  cmd += tenmpF;
  cmd += "\r\n";
  monitor.print("AT+CIPSEND=");
  monitor.println(cmd.length());
  if(monitor.find(">")){
    Serial.print(">");
    Serial.print(cmd);
    monitor.print(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  if(monitor.find("OK")){
    Serial.println("RECEIVED: OK");
  }else{
    Serial.println("RECEIVED: Error");
  }
}
void sendDebug(String cmd){
  Serial.print("SEND: ");
  Serial.println(cmd);
  monitor.println(cmd);
} 
 
boolean connectWiFi(){
  monitor.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if(monitor.find("OK")){
    Serial.println("RECEIVED: OK");
    return true;
  }else{
    Serial.println("RECEIVED: Error");
    return false;
  }
}
