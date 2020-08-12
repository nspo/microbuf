#include "ArduinoMessage.h"
#include <Wire.h>

ArduinoMessage_struct_t msg {};

microbuf::array<uint8_t,ArduinoMessage_struct_t::data_size> bytes {}; // storage for received data
bool newDataReceived = false;

void setup()
{
  Wire.begin(2); // I2C slave
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
}

void loop()
{
  if(newDataReceived) {
    bool worked = msg.from_bytes(bytes);
    Serial.print("Received new data - conversion worked: ");
    Serial.print(worked);
    Serial.print(", message counter: ");
    Serial.println(msg.message_counter);
    newDataReceived = false;  
  }
}

void receiveEvent(int howMany)
{
  size_t num_read = 0;
  while(Wire.available() && num_read <= ArduinoMessage_struct_t::data_size) {
    bytes[num_read] = Wire.read();  
    ++num_read;
  }
  //Serial.print("read n bytes: "); Serial.println(num_read);
  
  if(num_read == ArduinoMessage_struct_t::data_size) {
    // correct size received
    newDataReceived = true;
  }
  
  // discard rest of the data if there is any
  while(Wire.available()) {
    Wire.read();  
  }
}
