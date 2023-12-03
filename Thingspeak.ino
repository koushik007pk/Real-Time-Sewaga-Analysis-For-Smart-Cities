#include <SoftwareSerial.h>
#include <MQUnifiedsensor.h>
#include <dht.h>
#define RX 2
#define TX 3
#define TriggerPin 7
#define EchoPin 8
#define RedPin 9
#define GreenPin 6
#define BluePin 5
#define dht_apin A1
#define WaterDigital 4
#define GasSensor A0
#include <EEPROM.h>
dht DHT;
float distance,correctedDistance;
float timeTaken;
float temperature,humidity;
int gas;
String AP = "Redmi K20 Pro"; // AP NAME
String PASS = "0987654321"; // AP PASSWORD
String API = "SF6CS24JZG2TL06O"; // API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";
String field3 = "field3";
String field4 = "field4";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;
SoftwareSerial wmod(RX,TX);
void setup() {
Serial.begin(9600);
wmod.begin(115200);
pinMode(TriggerPin, OUTPUT);
pinMode(EchoPin, INPUT);
pinMode(GreenPin, OUTPUT);
pinMode(BluePin, OUTPUT);
pinMode(WaterDigital, OUTPUT);
digitalWrite(WaterDigital, LOW);
sendCommand("AT",5,"OK");
sendCommand("AT+CWMODE=1",5,"OK");
sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS
+"\"",20,"OK");
}
void loop() {
//ULTRASONIC SENSOR STARTS
digitalWrite(TriggerPin, LOW);
delayMicroseconds(5);
digitalWrite(TriggerPin, HIGH);
delayMicroseconds(10);
timeTaken = pulseIn(EchoPin, HIGH);
distance = (timeTaken*0.0331)/2;
Serial.print("{\"calculated_Distance\":");
Serial.print(distance);
//UlTRASONIC SENSOR ENDS
//DHT 11 STARTS
DHT.read11(dht_apin);
humidity = DHT.humidity;
temperature = DHT.temperature;
correctedDistance =
((timeTaken)*((331+(0.606*temperature)+(0.01243*humidity))*
0.0001))/2;
//DHT 11 ENDS
//GAS SENSOR STARTS
boolean GasState = GasDetected(GasSensor);
if (GasState){
gas = 1;
}
else{
gas = 0 ;
}
delay(1500);
//GAS SENSOR ENDS
String getData2 = "GET /update?api_key="+ API +"&"+
field1 +"="+String(distance);
sendCommand("AT+CIPMUX=1",5,"OK");
sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+
PORT,15,"OK");
sendCommand("AT+CIPSEND=0,"
+String(getData2.length()+4),4,">");
wmod.println(getData2);delay(500);countTrueCommand++;
sendCommand("AT+CIPCLOSE=0",5,"OK");
String getData3 = "GET /update?api_key="+ API +"&"+
field2 +"="+String(humidity);
sendCommand("AT+CIPMUX=1",5,"OK");
sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+
PORT,15,"OK");
sendCommand("AT+CIPSEND=0,"
+String(getData3.length()+4),4,">");
wmod.println(getData3);delay(500);
countTrueCommand++;
sendCommand("AT+CIPCLOSE=0",5,"OK");
String getData4 = "GET /update?api_key="+ API
+"&"+ field3 +"="+String(temperature);
sendCommand("AT+CIPMUX=1",5,"OK");
sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST
+"\","+ PORT,15,"OK");
sendCommand("AT+CIPSEND=0,"
+String(getData4.length()+4),4,">");
wmod.println(getData4);delay(500);countTrueCommand+
+;
sendCommand("AT+CIPCLOSE=0",5,"OK");
String getData5 = "GET /update?api_key="+ API
+"&"+ field4 +"="+ String(gas);
sendCommand("AT+CIPMUX=1",5,"OK");
sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST
+"\","+ PORT,15,"OK");
sendCommand("AT+CIPSEND=0,"
+String(getData5.length()+4),4,">");
wmod.println(getData5);delay(500);countTrueCommand+
+;
sendCommand("AT+CIPCLOSE=0",5,"OK");
}
int getSensorData(){
return random(1000); // Replace with your own
sensor code
}
boolean GasDetected(int GasPin)
{
float gasAnalog = analogRead(GasPin);
Serial.println(" gasAnalog value:");
Serial.println(gasAnalog);
float gasVoltage = (gasAnalog/1024)*5.0;
if(gasVoltage > 2)
{
Serial.print(",\"Poisonous Gas Detected\":");
}
else
{
Serial.print(",\"Poisonous Gas Not
Detected\":");
}
}
void sendCommand(String command, int maxTime, char
readReplay[]) {
Serial.print(countTrueCommand);
Serial.print(". at command => ");
Serial.print(command);
Serial.print(" ");
while(countTimeCommand < (maxTime*1))
{
wmod.println(command);//at+cipsend
if(wmod.find(readReplay))//ok
{
found = true;
break;
}
countTimeCommand++;
}
if(found == true)
{
Serial.println("OYI");
countTrueCommand++;
countTimeCommand = 0;
}
if(found == false)
{
Serial.println("Fail");
countTrueCommand = 0;
countTimeCommand = 0;
}
found = false;
}
