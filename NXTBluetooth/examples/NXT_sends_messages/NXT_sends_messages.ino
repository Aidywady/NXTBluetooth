// include the SoftwareSerial and NXTBluetooth libraries 
#include  <NXTBluetooth.h>

//initialize the NXTBluetooth library (rxPin, txPin, number of inbox buffers, number of outbox buffers)
// note that one can have between 1 and 10 inbox and outbox buffers. also note that the larger the buffer, the less spare ram there will be
// the library uses Software Serial within it so it may not work with all arduino boards, though it works perfectly with the UNO
NXTBluetooth bluetooth(2, 3, 10, 1);

void setup() {
  // begin the serial communication with the bluetooth module and the computer  
  bluetooth.begin(9600);
  Serial.begin(9600);
  // also send a message to the computer
  Serial.println("Beginning receiving messages from the NXT");
}

void loop() {
  // check all the inboxes for new messages
  for (int i = 0; i < 10; i += 1) {
    // check for a new message in mailbox i
    if (bluetooth.available(i)) { 
      // if there is a new message, read the message from the NXT in mailbox i and display it in the serial monitor.
      // also delete the message once it is read
      Serial.println(bluetooth.read(i, true));
    }
  }

  // check if the NXT sent a message and  respond appropriately
  bluetooth.update();
}
