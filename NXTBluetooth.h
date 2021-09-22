/*
  NXT_Bluetooth.h - Library for communicationg with the LEGO Mindstorms NXT via bluetooth.
  Created by Aidan R. W. Gebbie, September 19, 2021.
  Released into the public domain.
*/

#ifndef NXTBluetooth_h
#define NXTBluetooth_h

#include <Arduino.h>
#include <SoftwareSerial.h>

#define MAILBOX_BUFFER_SIZE 64
#define TIMOUT_DELAY 10
#define WRITE_REQUEST 0x09
#define READ_REQUEST 0x13



class NXTBluetooth
{
	// public functions
	public:
		NXTBluetooth(uint8_t rx, uint8_t tx, uint8_t noOfInboxes, uint8_t noOfOutboxes);
		void begin(uint16_t speed);
		void end();
		void update();
		void write(String message, uint16_t outboxno);
		String read(uint16_t inboxno, bool del);
		bool available(uint16_t inboxno);
		
	// private functions and variables
	private:
		void bluetoothWrite(uint8_t message);
		byte bluetoothRead();
		bool bluetoothAvailable();
		
		SoftwareSerial *BluetoothSerial;
		
		uint8_t _noOfInboxes;
		uint8_t _noOfOutboxes;
		uint8_t *_inbox;
		uint8_t *_outbox;
};

#endif		