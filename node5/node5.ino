int ledPin1 = 7; //led Rx
int ledPin = 3; // led Humidity
int analogPin = 5; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาที่5
int val = 0;
#include <SPI.h>
#include <RH_RF95.h>
RH_RF95 rf95;

void setup()
{
  pinMode(ledPin, OUTPUT);  // sets the pin as output
  pinMode(ledPin1, OUTPUT); //LED  
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
    rf95.setFrequency(433.0);  
}

void loop()
{ 
  val = analogRead(analogPin);
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf95.waitAvailableTimeout(3000))
   {
    if(rf95.recv(buf, &len))
    {
     int dataLength;
     String Request = (char*)buf;
     if (Request == "A5"){
      digitalWrite(ledPin1, HIGH);
      Serial.println("led ON");
     }
     if (Request == "A55"){
      digitalWrite(ledPin1, LOW);
      Serial.println("led OFF");
     }      
     if (Request == "C5"){
        Serial.println("Client 5 Got Request,Answering Server...");
        
        String data = "NODE 5 : Humidity = "+String(val);
        int dataLength = data.length(); dataLength++;
        uint8_t total[dataLength];
        data.toCharArray(total, dataLength);
        Serial.println(data);
        Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
        Serial.println(val); // พิมพ์ค่าของตัวแปร val
        rf95.send(total, dataLength);
        rf95.waitPacketSent();
        
        if (val < 700) { 
          //ค่า val ต่ำกว่า 700 = ความชื้นมากกว่า 700 = ไม่ต้องต้องลดน้ำต้นไม้
          digitalWrite(ledPin, LOW); // สั่งให้ LED ที่ Pin2 ดับ
          //digitalWrite(ledPin3, HIGH); // สั่งให้ LED ที่ Pin3 ติดสว่าง
          Serial.println("----- RED LED = OFF -----");
          Serial.println("HIGHT humidity, no watering.");
          Serial.println("-----------------------------");
        }
        else {
          //ค่า val มากกว่า 700 = ความชื้นต่ำกว่า 700 = ต้องลดน้ำต้นไม้
          digitalWrite(ledPin, HIGH); // สั่งให้ LED ที่ Pin2 ติดสว่าง
         // digitalWrite(ledPin3, LOW); // สั่งให้ LED ที่ Pin3 ดับ
          Serial.println("----- RED LED = ON -----");
          Serial.println("Low humidity, please water.");
          Serial.println("-----------------------------");
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
