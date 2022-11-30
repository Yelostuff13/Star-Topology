#include <SPI.h>
#include <RH_RF95.h>
RH_RF95 rf95;
int led = 13;
unsigned long int millisBefore;
int turn = 1;
void setup()
{
  pinMode(led, OUTPUT); 
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
  rf95.setFrequency(433.0);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
 
  //delay(1500);
}
void loop()
{
  if (millis() - millisBefore < 1000) {
    if (turn ==1 ){
      Serial.println("send Request to Client 1");
      sendRequest("C1");
      waitForAnswer();
      turn = 2;
    }
    waitForAnswer();
  } else if ((millis() - millisBefore > 1000) && (millis() - millisBefore < 2000)) {
    if (turn == 2){
      Serial.println("Send Request to Client 2");
      sendRequest("C2");
      waitForAnswer();
      turn = 3;
    }
  } else if ((millis() - millisBefore > 3000) && (millis() - millisBefore < 4000)) {
    if (turn == 3){
      Serial.println("Send Request to Client 3");
      sendRequest("C3");
      waitForAnswer();
      turn = 4;
    }
  } else if ((millis() - millisBefore > 4000)) {
    if (turn == 4){
      Serial.println("Send Request to Client 4");
      sendRequest("C4");
      waitForAnswer();
      sendRequest("C1");
      waitForAnswer();
      millisBefore = millis();
      turn = 1; 
    } 
  } 
}  

void waitForAnswer() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf95.waitAvailableTimeout(500))
  {
    if (rf95.recv(buf, &len))
    {
      Serial.print("Received at Server: ");
      Serial.println((char*)buf);
      String dataTotal = (char*)buf;
      Serial.println(dataTotal);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}

void sendRequest(String request) {
  String dataTotal = request;
  int dataLength = dataTotal.length(); dataLength ++;
  uint8_t total[dataLength];
  dataTotal.toCharArray(total, dataLength);
  rf95.send(total, dataLength);
  rf95.waitPacketSent();
}  







 
