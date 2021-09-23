/*
  NXT_sends_message

  Makes the NXT send a message to each of the 10 arduino inboxes, the arduino then displays the messages in the Serial Monitor.

  This example code requires a bluetooth serial module such as the HC-06 to be connected to arduino.

  pin connections:  bluetooth rx -> arduino pin 3. Note that a resistor voltage divider may be necissary for modules opperating at 3.3V
                    bluetooth tx -> arduino pin 2
                    bluetooth vin -> either 3.3V or 5V pin of arduino depending on the module
                    bluetooth gnd -> arduino gnd

  The LEGO Mindstorms NXT must be connected to the bluetooth module manually before the code is run. The LEGO Mindstorms also requires its 
  own code to run. This code can be found in the same pathway as this program (usually the following path on a windows machine: 
  Documents\Arduino\libraries\NXTBluetooth\examples\NXT_sends_messages). The code must be opened, compiled, and uploaded using the
  NXT software (this software can be downloaded and installed from here: https://education.lego.com/en-us/downloads/retiredproducts/nxt/software)

  Steps on how to upload the code to the lego mindstorms and initiate the bluetooth communication can be found here:
  https://github.com/Aidywady/NXTBluetooth/blob/main/README.md
  
  created 22 September 2021
  by Aidan Gebbie

  This example code is in the public domain.
*/

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
