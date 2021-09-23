# NXTBluetooth
An Arduino library that makes communicating with the LEGO Mindstorms NXT easy.

## What is the NXTBluetooth library?

This library makes communicating with the LEGO Mindstorms NXT easier than ever by hiding all the bluetooth protocols behind nice and easy to understand functions. All you require for this library to work is an Arduino UNO (or any similar Arduino board), a serial bluetooth module (e.g. the HC-06 bluetooth module Note: other bluetooth modules should work perfectly too, but have not been tested), a handfull of jumpers and resistors, and a breadboard.

## What are the functions of this library?

There are several public functions in this library, namely:

``` C++
NXTBluetooth bluetooth(2, 3, 1, 1);
```

This is the constructor. It takes 4 arguments **(_int_ rx pin, _int_ tx pin, _int_ the number of inboxes, _int_ the number of outboxes)**. Rx pin is the connected to the bluetooth modules tx and tx pin is the pin connected to the bluetooth module's rx (these pins can be any pins on the Arduino board as the library uses SoftwareSerial). The number of inboxes and ouboxes can each be any integer value from 1 to 10, these indicate how large the inbox and outbox buffers must be, with more inboxes and outboxes, more data can be sent and received in a manner of speaking. Note that the more inboxes and outboxes you use, the less spare RAM your arduino board will have, so try to be frugal with these numbers.

``` C++
bluetooth.begin(9600);
```

This begins the bluetooth communication at the given speed. This function accepts 1 argument **(_int_ speed)**. Note that to set the speed to a different value, you may also need to use the bluetooth module's AT commands; how to do that, is not explained here.

``` C++
bluetooth.end();
```
This function ends the current instance of the class and requires no arguments. This function also deallocates the RAM used by the inbox and outbox buffers.

``` C++
bluetooth.update();
```

This function MUST be called at the end of every loop to keep communicating with the NXT. No arguments are required in this function. This function is where messages are actually and sent and received to and from the inbox and outbox buffers. To update as frequently as possible, avoid using the `delay();` function, as well as similar functions. There are many alternatives to this function that can be found on the internet.

``` C++
bluetooth.write("Hello World!", 0);
```

This function writes the input string (in this case "Hello World!" though any other strings, including string type variables, can be used) to the designated outbox which is, in this case outbox 0 (note the outbox is 0 shifted similarly to arrays (ie. 0 -> 1; 1 -> 2; n -> n+1). The designated outbox could be any value from 0 to the number of outboxes - 1. It takes 2 arguments **(_String_ message, _int_ outboxno)**.

``` C++
bluetooth.available(0);
```

This function checks if there is a new message in the designated inbox and returns **true** if there is, otherwise it returns **false**. The designated inbox, in this case, inbox 0, is 0 shifted similarly to arrays. The value of the designated inbox can be any integer number from 0 to the number of inboxes - 1. It takes 1 argument **(_int_ inboxno)**.

``` C++
bluetooth.read(0, true);
```

This function returns a message from the designated inbox (in this case, inbox 0), and can optionally delete the message once it is read. If there isn't a message in the designated inbox, the function just returns null. If the second arguement is set to true, the message is deleted after it is read, otherwise it is left as. This function takes 2 arguments **(_int_ inboxno, _bool_ delete)**.


## How to get the example code to work

### The Arduino

_apologies, this is still a work in progress. It should be finished by 26/09/2021._

### The NXT

#### 1. installing the NXT Software

Go to the [LEGO Mindstorms NXT Software](https://education.lego.com/en-us/downloads/retiredproducts/nxt/software) page and download the .zip file. This might take some time as it is a 782MB file for Windows. Once the file is downloaded, extract the contents of the .zip. Inside you should find an Autorun.exe. Run this (you may need administrative privileges) and follow its instructions.

#### 2. Opening, Compiling, and Uploading the example code

The LEGO Mindstorms NXT needs code to run. To upload the code, follow these instructions. In this example we will upload the code for _bidirectional_communication_ example, though to do the other examples, one would just go to their respective file pathways and follow the instructions using those pathways instead. 

1. Go to the _bidirectional_communication_ pathway, it should be _Documents\Arduino\libraries\NXTBluetooth\examples\bidirectional_communication_ on a windows machine.
2. find the .rbt file and open it, it should want to be opened with the LEGO Mindstorms NXT Sofware app. If it doesn't, right click on it, select "Open with...", click "More Apps", scroll down and press "Look for another app on this PC", and then find the LEGO Mindstorms NXT .exe in the file explorer (It should be in _C:\Program Files (x86)\LEGO Software\LEGO MINDSTORMS NXT_).
3. You should now connect you NXT to the computer via bluetooth or USB. Then select the button with a picture of a brick, it should look like the top left button in the picture below.
4. A popup menu should now appear, ensure that your NXT is turned on (there must be 6 AA batteries inside it, then you press the big orange button on the NXT and the screen should turn on) and connected to the computer via USB or paired via bluetooth. Press scan and wait. Now a list of options should appear, selct the name of your NXT and press connect.
5. Now that you are connceted to the NXT, you can close the popup menu and then press the button resembling the bottom left button in the forementioned picture (downward pointing arrow). You should wait a bit unitl the NXT makes a beap sound a a dialog says "Completed...".

![NXT upload picture](https://github.com/Aidywady/Library-Pictures/blob/main/NXT%20upload%20picture.png)

You have now successfully compiled and uploaded the code to the NXT.

#### 3. Initializing the bluetooth communication between the NXT and the Arduino

_apologies, this is still a work in progress. It should be finished by 26/09/2021._

#### 4. Running the Code

_apologies, this is still a work in progress. It should be finished by 26/09/2021._
