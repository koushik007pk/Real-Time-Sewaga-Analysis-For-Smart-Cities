#include <MQUnifiedsensor.h>
#include <dht.h>
#define TriggerPin 7
#define EchoPin 8
#define RedPin 9
#define GreenPin 6
#define BluePin 5
#define dht_apin A1
#define WaterDigital 4
#define GasSensor A0
dht DHT;
float distance,correctedDistance;
float timeTaken;
float temperature,humidity;
void setup() {
// put your setup code here, to run once:
pinMode(TriggerPin, OUTPUT);
pinMode(EchoPin, INPUT);
pinMode(RedPin, OUTPUT);
pinMode(GreenPin, OUTPUT);
pinMode(BluePin, OUTPUT);
pinMode(WaterDigital, OUTPUT);
digitalWrite(WaterDigital, LOW);
Serial.begin(9600);
}
void loop() {
// put your main code here, to run repeatedly:
digitalWrite(TriggerPin, LOW);
delayMicroseconds(5);
digitalWrite(TriggerPin, HIGH);
delayMicroseconds(10);
timeTaken = pulseIn(EchoPin, HIGH);
distance = (timeTaken*0.0331)/2;
Serial.print("{\"calculated_Distance\":");
Serial.print(distance);
DHT.read11(dht_apin);
Serial.print("current humidity = ");
Serial.print(DHT.humidity);
Serial.print("% ");
Serial.print("temperature = ");
Serial.print(DHT.temperature);
Serial.println("C ");
correctedDistance =
((timeTaken)*((331+(0.606*temperature)+(0.01243*humidity))*
0.0001))/2;
Serial.print(",\"Actual_Distance\":");
Serial.print(correctedDistance);
RGBIndicator(round(correctedDistance));
boolean GasState = GasDetected(GasSensor);
Serial.print(",\"gas_detected\":");
if (GasState)
{
Serial.print("\"yes\"");
}
else
{
Serial.print("\"no\"");
}
Serial.println("}");
delay(5000);
}
void RGBIndicator(int objectDistance)
{
if(objectDistance <= 10)
{
analogWrite(RedPin, 255);
analogWrite(BluePin, 0);
analogWrite(GreenPin, 0);
}
else if(objectDistance >10 and objectDistance
<=20)
{
analogWrite(RedPin, 0);
analogWrite(BluePin, 255);
analogWrite(GreenPin, 0);
}
else
{
analogWrite(RedPin, 0);
analogWrite(BluePin, 0);
analogWrite(GreenPin, 255);
}
}
boolean GasDetected(int GasPin)
{
float gasAnalog = analogRead(GasPin);
Serial.println("gasAnalog value:");
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
