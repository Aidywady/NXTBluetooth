/*
  NXT_Bluetooth.cpp - Library for communicationg with the LEGO Mindstorms NXT via bluetooth.
  Created by Aidan R. W. Gebbie, September 19, 2021.
  Released into the public domain.
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "NXTBluetooth.h"

/*============================================================================*/	
/* constructor code */

NXTBluetooth::NXTBluetooth(uint8_t rx, uint8_t tx, uint8_t noOfInboxes, uint8_t noOfOutboxes)
{
	// initialize the number of inboxes and outboxes
	_noOfInboxes = noOfInboxes;
	_noOfOutboxes = noOfOutboxes;
	
	// make sure that the inboxes are within the bounds and correct the if they are not
	if (_noOfInboxes > 10) _noOfInboxes = 10;
	else if (_noOfInboxes < 1) _noOfInboxes = 1;
	
	// do the same with the outboxes
	if (_noOfOutboxes > 10) _noOfOutboxes = 10;
	else if (_noOfOutboxes < 1) _noOfOutboxes = 1;
	
	// initialize the serial communication pins
	BluetoothSerial = new SoftwareSerial(rx, tx);
}

/*============================================================================*/	
/* public serial functions */
/* These are for begining and ending serial communication while still checking that the bluetoothSerial object was initialized */

void NXTBluetooth::begin(uint16_t speed) 
{
	// allocate the correct amount of memory to the inbox and outbox buffers
	_inbox = (uint8_t *) malloc(MAILBOX_BUFFER_SIZE * _noOfInboxes * sizeof(uint8_t));
	_outbox = (uint8_t *) malloc(MAILBOX_BUFFER_SIZE * _noOfOutboxes * sizeof(uint8_t));
	
	// set up a baud rate at the give speed
	// remember to check that BluetoothSerial has been initialized first to avoid errors
	if (BluetoothSerial) 
	{
		// begin the communication at the given speed
		BluetoothSerial->begin(speed);
	}
}
 
void NXTBluetooth::end() 
{
	// once again, remember to check that BluetoothSerial has been initialized first to avoid errors
	if (BluetoothSerial) 
	{
		// deallocate the memory for the buffers
		free(_inbox);
		free(_outbox);
		
		// then end the serial communication
		BluetoothSerial->end();
	}
}

/*============================================================================*/	
/* private serial functions */
/* these functions are private and are used to neatly rap their serial functions in if statements to check that the serial communication has actually been initialized so to avoid errors */


bool NXTBluetooth::bluetoothAvailable() 
{
	// once again, remember to check that BluetoothSerial has been initialized first to avoid errors
	if (BluetoothSerial) 
	{
		return BluetoothSerial->available();
	}
	else return false;
}

byte NXTBluetooth::bluetoothRead() 
{
	// once again, remember to check that BluetoothSerial has been initialized first to avoid errors
	if (BluetoothSerial) 
	{
		return BluetoothSerial->read();
	}
	else return NULL;
}

void NXTBluetooth::bluetoothWrite(uint8_t message) 
{
	// once again, remember to check that BluetoothSerial has been initialized first to avoid errors
	if (BluetoothSerial) 
	{
		BluetoothSerial->write(message);
	}
}

//*============================================================================*/	
/* buffer functions */
/* these functions are used to check, read from, and write to the inbox and outbox buffers. Note that they don't actually send or receive messages. */


// returns whether or not the selected inbox has a new message
bool NXTBluetooth::available(uint16_t inboxno) 
{
	// check that inboxno is within the bounds, if not, correct it
	if (inboxno > (_noOfInboxes - 1)) inboxno = _noOfInboxes - 1;
	else if (inboxno < 0) inboxno = 0;
	
	// set the inboxno to its true location in the array 
	inboxno *= MAILBOX_BUFFER_SIZE;
	
	// if there is a message in the mailbox, return true
	if (_inbox[(inboxno + 1)] == WRITE_REQUEST) return true;
	
	// else, return false
	else return false;
}

// reads the message in a selected inbox and returns it. One can also choose to delete the message once it is read
String NXTBluetooth::read(uint16_t inboxno, bool del) 
{
	// check that inboxno is within the bounds, if not, correct it
	if (inboxno > (_noOfInboxes - 1)) inboxno = _noOfInboxes - 1;
	else if (inboxno < 0) inboxno = 0;
	
	// set the inboxno to its true location in the arrays
	inboxno *= MAILBOX_BUFFER_SIZE;
	
	// check that there actually is a message
	if (_inbox[(inboxno + 1)] == WRITE_REQUEST) 
	{
		// get the length of the message
		uint8_t messagelength = _inbox[(inboxno + 3)]; 
		// create a character array that is of the right size
		char temp[messagelength];
		
		// copy the actual message from the inbox to the character array
		for (uint8_t i = 0; i < messagelength; i += 1) temp[i] = _inbox[(inboxno + 4 + i)];
		// convert the array to a string
		String message = String(temp);
		
		// if del is true, delete the message from the inbox
		if (del) 
		{
			for (uint8_t i = 0; i < MAILBOX_BUFFER_SIZE; i += 1) _inbox[(inboxno + i)] = 0x00;
		}
		
		// return the message string
		return message;
	}
	// if there isn't a message, return null
	else return "";
}

// writes a message to a selected outbox, this message will be sent when the lego mindstorms requests to read the contents of that outbox
void NXTBluetooth::write(String message, uint16_t outboxno) 
{
	// make sure that the message is not too long (see the LEGO Mindstorms Bluetooth Developers Kit for details)
	if (message.length()  < 59) 
	{
		if (outboxno > (_noOfOutboxes - 1)) outboxno = _noOfOutboxes - 1;
		else if (outboxno < 0) outboxno = 0;
		
		// set the outboxno to its true location in the array
		outboxno *= MAILBOX_BUFFER_SIZE;
		
		// create a char that is 59 characters long (the data can be up to 59 characters long as the first 5 bytes of the the message that is sent are the details of the message)
		char temp[59];
		// also clear the array as local arrays appear not to be cleared
		memset(temp, 0, sizeof(temp));
		
		// write the input message to the array
		message.toCharArray(temp, sizeof(temp)); 
		
		// clear the details of the message in the outbox as these will be set later
		for (uint8_t i = 0; i < 4; i += 1) _outbox[(outboxno + i)] = 0x00;
		
		// set the 5th byte of the message details to the length of the actual message data + 1 (include a null terminator)
		_outbox[(outboxno + 4)] = message.length() + 1;
		
		// write the message data (the temp array) to the outbox
		for (uint8_t i = 0; i < sizeof(temp); i += 1) _outbox[(outboxno + 5 + i)] = temp[i];
	}
}


//*============================================================================*/	
/* update functions */
/* this function updates the communication link by checking for incoming messages and responding appropriately (ie. if it is a write message, save it to the inbox, if it is a read message, respond by sending the message form the corresponding outbox)
NOTE: this function MUST be called once every loop iteration, and there should idealy be no wait functions in the loop so as to update the communication as frequently as possible */

void NXTBluetooth::update() 
{
	// check if a new message is being sent
	if (bluetoothAvailable()) 
	{
		// if so create a temporary array and clear it
		uint8_t temporarymessage[MAILBOX_BUFFER_SIZE];
		memset(temporarymessage, 0, sizeof(temporarymessage));  
		// save the first byte of the message to a variable
		uint8_t messagelength1 = bluetoothRead();
		// wait for the second byte
		
		while (!bluetoothAvailable());
		// merge the two bytes to work out the total message length
		uint16_t messagelength = messagelength1 + (bluetoothRead() << 8);
		// set the current byte of the actual message to 0
		uint16_t currentbyte = 0;
		// and set the time the last byte was sent to now
		uint32_t lasttime = millis();
		
		// and now receive the actual message, but if there is a pause greater than 10 milliseconds between the transmisions, exit the loop
		while ((currentbyte < messagelength) && ((millis() - lasttime) <= TIMOUT_DELAY)) 
		{    
			// wait for the next byte, but stop waiting if it takes longer than 10 milliseconds
			while ((!bluetoothAvailable()) && ((millis() - lasttime) <= TIMOUT_DELAY));  
			
			// if we didn't take too long:
			if ((millis() - lasttime) <= TIMOUT_DELAY) 
			{
				// save the current byte to the temporary array,  
				temporarymessage[currentbyte] = bluetoothRead(); 
				// update the time the last byte was sent to now,
				lasttime = millis();
				// and increment the current byte we received
				currentbyte += 1;
			} 
		}
		
		// if we received the full message (i.e. it is not corrupted)
		if (currentbyte = messagelength) 
		{
			// if the secend byte is the write message command (see the LEGO Mindstorms Bluetooth Developers Kit for details about these commands)
			if (temporarymessage[1] == WRITE_REQUEST) 
			{
				// set the inbox to write to to the inbox the lego mindstorms meant to send the message too.
				// note that if there are fewer than 10 inboxes in the array (e.g. to save space), "rap it arround" 
				// (e.g. if there are only 5 inboxes, send a message meant for inbox 6, to inbox 1, inbox 7 to inbox 2, etc.) 
				uint16_t inboxno = (temporarymessage[2] % _noOfInboxes) * MAILBOX_BUFFER_SIZE;
				
				// write the message to the formentioned inbox
				for (uint8_t i = 0; i < MAILBOX_BUFFER_SIZE; i += 1) _inbox[(inboxno + i)] = temporarymessage[i];
				
				// if the NXT wants a reply (see see the LEGO Mindstorms Bluetooth Developers Kit for details)
				if (temporarymessage[0] == 0x00) 
				{
					// reply appropriately (see see the LEGO Mindstorms Bluetooth Developers Kit for details)
					bluetoothWrite((byte) 0x02);
					bluetoothWrite((byte) WRITE_REQUEST);
					bluetoothWrite((byte) 0x00);
				}
			}
			
			// if the second byte is the read message command (see the LEGO Mindstorms Bluetooth Developers Kit for details)
			else if (temporarymessage[1] == READ_REQUEST) 
			{
				// ensure that the lego mindstorms wants a reply
				if (temporarymessage[0] == 0x00) 
				{
					// set the outbox to read from to the outbox the lego mindstorms meant to read from. 
					// Once again, rap arround if there are fewer than 10 outboxes
					uint16_t outboxno = ((temporarymessage[2] - 10) % _noOfOutboxes) * MAILBOX_BUFFER_SIZE;
					
					// set the first for bytes of the message to the appropriate values, providing some basic information about the message
					// see ee the LEGO Mindstorms Bluetooth Developers Kit for details
					_outbox[(outboxno + 0)] = 0x02;
					_outbox[(outboxno + 1)] = READ_REQUEST;
					_outbox[(outboxno + 2)] = 0x00;
					_outbox[(outboxno + 3)] = temporarymessage[3];
				
					// indicate the total length of the actual transmission (excluding the first 2 bytes)
					bluetoothWrite((byte) 0x40);
					bluetoothWrite((byte) 0x00);
				
					// send the message
					for (uint8_t i = 0; i < MAILBOX_BUFFER_SIZE; i += 1) bluetoothWrite(_outbox[(outboxno + i)]);
				
					// if the NXT wants the message deleted when its done, clear the specific section of the outbox
					if (temporarymessage[4] != 0x00) 
					{
					for (uint8_t i = 0; i < MAILBOX_BUFFER_SIZE; i += 1) _outbox[(outboxno + i)] = 0;
					}
				}
				
			}
		}
	}  
}