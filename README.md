# NXTBluetooth
An Arduino library that makes communicating with the LEGO Mindstorms NXT easy.

## What is the NXTBluetooth library?

This library makes communicating with the LEGO Mindstorms NXT easier than ever by hiding all the bluetooth protocols behind nice and easy to understand functions. All you require for this library to work is an arduino UNO (or any similar arduino board), a serial bluetooth module (e.g. the HC-06 bluetooth module Note: other bluetooth modules should work perfectly too, but have not been tested), a handfull of jumpers and resistors, and a breadboard.

## What are the functions of this library?

There are several public functions in this library, namely:

``` C++
NXTBluetooth bluetooth(2, 3, 1, 1);
```

This is the constructor. it takes 4 arguements **(_int_ rx pin, _int_ tx pin, _int_ the number of inboxes, _int_ the number of outboxes)**. Rx pin is the connected to the bluetooth modules tx and tx pin is the pin connected to the bluetooth module's rx (these pins can be any pins on the arduino board as the library uses SoftwareSerial). the number of inboxes and ouboxes can each be any integer value from 1 to 10, these indicate how large the inbox and outbox buffers must be, with more inboxes and outboxes, more data can be sent and received in a manner of speaking. Note however that the more inboxes and outboxes you use, the less spare RAM your arduino board will have, so try to be frugal with these numbers.

``` C++
bluetooth.begin(9600);
```

This begins the bluetooth communication at the given speed. This function accepts 1 arguement **(_int_ speed)**. Note that to set the speed to a different value, you may also note to use the bluetooth modules AT commands, how to do that, is not explained here.

``` C++
bluetooth.end();
```
This function ends the current instance of the class and requires no arguements. This function also deallocates the RAM used by the inbox and outbox buffers.

``` C++
bluetooth.update();
```

This function MUST be called at the end of every loop to keep communicating with the NXT. No arguements are required in this function. This function is where messages are actually and sent and received to and from the inbox and outbox buffers. To update as freaquently as possible, avoid using the `delay();` function, as well as similar functions. There are many alternatives to this function that can be found on the internet.

``` C++
bluetooth.write("Hello World!", 0);
```

This function writes the input string (in this case "Hello World!" though any other strings, including string type variables, can be used) to the designated outbox which is, in this case outbox 0 (note the outbox is 0 shifted similarly to arrays (ie. 0 -> 1; 1 -> 2; n -> n+1). The designated outbox could be any value from 0 to the number of outboxes - 1. It takes 2 arguements **(_String_ message, _int_ outboxno)**.

``` C++
bluetooth.available(0);
```

This function checks if there is a new message in the designated inbox and returns **true** if there is, otherwise it returns **false**. The designated inbox, in this case, inbox 0, is 0 shifted similarly to arrays. The value of the designated inbox can be any integer number from 0 to the number of inboxes - 1. It takes 1 arguement **(_int_ inboxno)**.

``` C++
bluetooth.read(0, true);
```

This function returns a message from the designated inbox (in this case, inbox 0), and can optionally delete the message once it is read. If there isn't a message in the designated inbox, the function just returns null. If the second arguement is set to true, the message is deleted after it is read, otherwise it is left as . This function takes 2 arguements **(_int_ inboxno, _bool_ delete)**.
