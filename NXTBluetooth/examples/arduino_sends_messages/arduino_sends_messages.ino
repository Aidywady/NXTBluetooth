// include the SoftwareSerial and NXTBluetooth libraries 
#include  <NXTBluetooth.h>

//initialize the NXTBluetooth library (rxPin, txPin, number of inbox buffers, number of outbox buffers)
// note that one can have between 1 and 10 inbox and outbox buffers. also note that the larger the buffer, the less spare ram there will be
// the library uses Software Serial within it so it may not work with all arduino boards, though it works perfectly with the UNO
NXTBluetooth bluetooth(2, 3, 1, 10); 

void setup() {
  // begin the serial communication with the bluetooth module and the computer 
  bluetooth.begin(9600);
  Serial.begin(9600);
  // also send a message to the computer
  Serial.println("Beginning to send messages to NXT");
}

void loop() {
  for (int i = 0; i < 10; i += 1) {
    // create a message string
    String message = String("Message " + String(i + 1) + "!");
    // send the string to the NXT mailbox i
    // note that messages can't exede 59 characters including the null terminator
    bluetooth.write(message, i);
  }
  
  // check if the NXT sent a message and  respond appropriately
  bluetooth.update();
}
