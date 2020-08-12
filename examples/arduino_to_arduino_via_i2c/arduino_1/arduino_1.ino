#include "microbuf.h"
#include "ArduinoMessage.h"
#include <Wire.h>

uint8_t msg_counter = 0;

void setup()
{
  Serial.begin(115200);
  Wire.begin(); // I2C master
}

void loop() {
  // create message to send
  
  ArduinoMessage_struct_t msg {};
  for(size_t i=0; i<4; ++i)
  {
    msg.measured_values[i] = 3.3f*i;
  }
  msg.message_counter = msg_counter;
  msg_counter++; // will overflow and start at 0 again

  const auto bytes = msg.as_bytes();
  Wire.beginTransmission(2);
  Wire.write(bytes.data, bytes.size());
  Wire.endTransmission();
  
  Serial.print("- Tried to send ");
  Serial.print(bytes.size());
  Serial.println(" bytes");
  
  delay(1000);
}
