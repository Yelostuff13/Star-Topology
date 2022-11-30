int ledPin = 3;
int analogPin = 5; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาที่5
int val = 0;
#define sensor A0
#define relay  3
#include <SPI.h>
#include <RH_RF95.h>
RH_RF95 rf95;

void setup()
{
  pinMode(ledPin, OUTPUT);  // sets the pin as output
  //pinMode(ledPin3, OUTPUT);  // sets the pin as output
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
    rf95.setFrequency(433.0);  
}

void loop()
{ 
  int analog = analogRead(sensor);
  val = analogRead(analogPin);
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  
  if (rf95.waitAvailableTimeout(3000))
   {
    if(rf95.recv(buf, &len))
    {
     int dataLength;
     String Request = (char*)buf;
     if (Request == "C1"){
        Serial.println("Client 1 Got Request,Answering Server...");
        
        String data = "NODE 1 : Humidity = "+String(analog);
        int dataLength = data.length(); dataLength++;
        uint8_t total[dataLength];
        data.toCharArray(total, dataLength);
        Serial.println(data);
        Serial.print("Sensor = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
        Serial.println(analog); // พิมพ์ค่าของตัวแปร val
        rf95.send(total, dataLength);
        rf95.waitPacketSent();
        
        if(analog < 700){
         digitalWrite(relay,LOW);
         Serial.println("PUMP   = OFF");
        }
        else{
         digitalWrite(relay,HIGH);
         Serial.println("PUMP   = ON");
        }
        
      }
    }
   }
   else
   {
    Serial.println("No reply, is rf95_server running");
   }
   delay(500);
}
