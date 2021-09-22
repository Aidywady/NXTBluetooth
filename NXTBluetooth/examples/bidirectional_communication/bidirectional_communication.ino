// include the SoftwareSerial and NXTBluetooth libraries 
#include  <NXTBluetooth.h>

//initialize the NXTBluetooth library (rxPin, txPin, number of inbox buffers, number of outbox buffers)
// note that one can have between 1 and 10 inbox and outbox buffers. also note that the larger the buffer, the less spare ram there will be
// the library uses Software Serial within it so it may not work with all arduino boards, though it works perfectly with the UNO
NXTBluetooth bluetooth(2, 3, 1, 1); 

void setup() {
  // begin the serial communication with the bluetooth module and the computer 
  bluetooth.begin(9600);
  Serial.begin(9600);
  // also send a message to the computer
  Serial.println("Beginning communication with NXT");
}

void loop() {
  // convert the current time in millis to seconds to the 3rd decimal place, and convert that to a string
  String runTime = String((float) millis() / 1000, 3);
  // send the string to the NXT mailbox 1
  // note that messages can't exede 59 characters including the null terminator
  bluetooth.write(runTime, 0);

  // check for a new message in mailbox 1
  if (bluetooth.available(0)) { 
    // read the message from the NXT in mailbox 1 and display it in the serial monitor.
    // also delete the message once it is read
    Serial.println(bluetooth.read(0, true));
  }
  
  // check if the NXT sent a message and  respond appropriately
  bluetooth.update();
}
