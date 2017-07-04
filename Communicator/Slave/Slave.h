/**
 *  Communicator that handles Command and Response between Master and Slave Processor.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 04-07-2017
 */

#ifndef Master_h
#define Master_h

#include "Arduino.h"
#include "Wire.h"
#include "U8g2_GraphicsEngine.h"

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
					NotSet				= 0x00,		// Default value for enum. Slave will return error if sent this.
					DrawPoint			= 0x10,		// Draw Point on OLED.
					DrawLine			= 0x12,		// Draw Line on OLED.
					DrawCircle			= 0x13,		// Draw Circle on OLED.
					DrawDisc 			= 0x14,		// Draw Filled Disc on OLED.
					DrawTriangle		= 0x15,		// Draw Circle on OLED.
					DrawRectangle		= 0x16,		// Draw Rectangle on OLED.
					DrawBox 			= 0x17,		// Draw Filed Box on OLED.
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
		Communicator(GraphicEngine&);
		void begin(uint8_t);
		void recieveCommand();
		void executeCommand();
		void sendResponse();

	private:
		GraphicEngine& _ge;
		uint8_t _i2cAddress;
		byte commandBuffer[10];
		uint8_t _lastCommandID;
		bool _recieved;
};

#endif