/**
 *  Communicator that handles Command and Response between Master and Slave Processor.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 04-07-2017
 */

#ifndef Slave_h
#define Slave_h

#include "Arduino.h"
#include "Wire.h"

#include "U8g2_GraphicsEngine.h"
#include "Motion.h"

/**
 * Command Packet Structure:
 * Byte 1: Start Byte - Always 0x55
 * Byte 2: An random generated ID for the Command, same ID is sent in response.
 * Byte 3: Command Byte - Identifies Command
 * Byte 4-9: Parameters - Varies from Command to Command
 * Byte 10: End Byte - Always 0x99
 */
class Command_Packet
{
	public:
		// Declaring Enums inside an embedded class or struct does not pollute Global/Class Scope
		class Commands
		{
			public:
				enum Commands_Enum
				{
					NOT_SET				= 0x00,		// Default value for enum. Slave will return error if sent this.
					DRAW_POINT			= 0x10,		// Draw Point on OLED.
					DRAW_LINE			= 0x12,		// Draw Line on OLED.
					DRAW_CIRCLE			= 0x13,		// Draw Circle on OLED.
					DRAW_DISC 			= 0x14,		// Draw Filled Disc on OLED.
					DRAW_TRIANGLE		= 0x15,		// Draw Circle on OLED.
					DRAW_RECTANGLE		= 0x16,		// Draw Rectangle on OLED.
					DRAW_BOX  			= 0x17,		// Draw Filed Box on OLED.
					DRAW_TEXT        	= 0x18,     // Displays text on the OLED.
          			CLEAR_SCREEN    	= 0x19,     // Clears the Screen
		      		LEFT_MOTOR     		= 0x31,     // Move Left Motor.
					RIGHT_MOTOR    		= 0x32,
					MOVE           		= 0X33,
					MOVE_TO        		= 0x34, 
					STOP           		= 0X35,
					TURN_ANGLE     		= 0X36,
					TURN           		= 0X37
		};
	};
	
		byte id;										// An unique id for each new command.	
		byte Parameter[6];								// Parameter 6 bytes, changes meaning depending on command							
		Commands::Commands_Enum _command;	

		Command_Packet(byte* buffer, bool UseSerialDebug);
		bool CheckParsing(byte b, byte propervalue, char* varname, bool UseSerialDebug);

	private: 
		static const byte COMMAND_START_CODE = 0x55;	// Static byte to mark the beginning of a command packet	-	never changes
		static const byte COMMAND_END_CODE = 0x99;		// Static byte to mark the end of a command packet	-	never changes

		// word _CalculateChecksum();						// Checksum is calculated using byte addition
		// byte GetHighByte(word w);						
		// byte GetLowByte(word w);
};


/**
 * Response Packet Structure:
 * Byte 1: Start Byte - Always 0xAA
 * Byte 2: The Command ID sent by Master.
 * Byte 3: Status - OK or ERROR
 * Byte 4: End Byte - Always 0x11
 */
class Response_Packet
{
	public:
		bool status;

		Response_Packet(byte id);						
		byte* GetPacketBytes();							// returns the bytes to be transmitted

	private: 
		static const byte RESPONSE_START_CODE = 0xAA;	// Static byte to mark the beginning of a command packet	-	never changes
		static const byte RESPONSE_END_CODE = 0x11;		// Static byte to mark the end of a command packet	-	never changes
		static const byte OK = 0x50;
		static const byte ERROR = 0x51;

		byte _id;
};



class Communicator
{
	public:
		Communicator(GraphicEngine&, Motion&);
		void begin(uint8_t);
		void recieveCommand();
		void executeCommand();
		void sendResponse();

	private:
		GraphicEngine& _ge;
		Motion& _motors;

		uint8_t _i2cAddress;
		volatile uint8_t _lastCommandID;
		
		volatile byte commandBuffer[10];
		volatile char dataBuffer[20];
		volatile bool _recieved, _dataRecieved;
};

#endif
