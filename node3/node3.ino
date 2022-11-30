#include <SPI.h>
#include <RH_RF95.h>
RH_RF95 rf95;
int sensorPin = A0;//พอร์ตที่ต่อกับ sensor ขา DO (data out)
int sensorValue;
const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;

void setup() {
  Serial.begin(115200);
  while (!Serial) ;
  if (!rf95.init())
    Serial.println("init failed");
  rf95.setFrequency(433.0);
}
void loop() {
  //float f = dht.readTemperature(true);
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  soilMoistureValue = map(soilMoistureValue, 0, 1023, 0, 100);
  if (rf95.waitAvailableTimeout(3000))
   {
    if(rf95.recv(buf, &len))
    {
     int dataLength;
     String Request = (char*)buf;
     if (Request == "C3"){
        Serial.println(soilMoistureValue);
        Serial.print(soilMoistureValue);
        Serial.println("%");
        Serial.println("Client 3 Got Request,Answering Server...");

        String data = "NODE 3 : Humidity = "+String(soilMoistureValue) ;
        
        //String data = "Hello 4";
        int dataLength = data.length(); dataLength++;
        uint8_t total[dataLength];
        data.toCharArray(total, dataLength);
        Serial.println(data);
        rf95.send(total, dataLength);
        rf95.waitPacketSent();
     }
    }
   }
   else
   {
    Serial.println("No reply, is rf95_server running");
   }
   delay(500);
}
