/*
  bidirectional_communication

  This example demonstrates both the NXT and the Arduino sending messages at the same time. The NXT will display the current millis() value 
  of the arduino on its LCD screen (in seconds to the 3rd decimal place though), while the arduino will display the current amount of time that the 
  LEGO Mindstorms NXT has been running the code (also in seconds to the 3rd decimal place)

  This example code requires a bluetooth serial module such as the HC-06 to be connected to arduino.

  pin connections:  bluetooth rx -> arduino pin 3. Note that a resistor voltage divider may be necissary for modules opperating at 3.3V
                    bluetooth tx -> arduino pin 2
                    bluetooth vin -> either 3.3V or 5V pin of arduino depending on the module
                    bluetooth gnd -> arduino gnd

  The LEGO Mindstorms NXT must be connected to the bluetooth module manually before the code is run. The LEGO Mindstorms also requires its 
  own code to run. This code can be found in the same pathway as this program (usually the following path on a windows machine: 
  Documents\Arduino\libraries\NXTBluetooth\examples\bidirectional_communication). The code must be opened, compiled, and uploaded using the
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
