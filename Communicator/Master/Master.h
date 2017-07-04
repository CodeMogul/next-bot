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
	
		byte _id;										// An unique id for each new command.	
		byte Parameter[6];								// Parameter 6 bytes, changes meaning depending on command							
		Commands::Commands_Enum _command;	
		byte* GetPacketBytes();							// returns the bytes to be transmitted
		//void ParameterFromInt(int i);

		Command_Packet(byte id, Commands::Commands_Enum command);

	private: 
		static const byte COMMAND_START_CODE = 0x55;	// Static byte to mark the beginning of a command packet	-	never changes
		static const byte COMMAND_END_CODE = 0x99;		// Static byte to mark the end of a command packet	-	never changes

		// word _CalculateChecksum();						// Checksum is calculated using byte addition
		// byte GetHighByte(word w);						
		// byte GetLowByte(word w);
};

//	----------------------------------------------------------------------------------

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
		bool status;									// Status of Command.
		byte _id;			

		Response_Packet(byte* buffer, bool UseSerialDebug);
		bool CheckParsing(byte b, byte propervalue, char* varname, bool UseSerialDebug);

	private: 
		static const byte RESPONSE_START_CODE = 0xAA;	// Static byte to mark the beginning of a command packet	-	never changes
		static const byte RESPONSE_END_CODE = 0x11;		// Static byte to mark the end of a command packet	-	never changes
		static const byte OK = 0x50;
		static const byte ERROR = 0x51;
};

//	----------------------------------------------------------------------------------

class Communicator
{
	public:
		Communicator();
		void begin(uint8_t);
		void sendCommand(byte*);
		bool recieveResponse();

		/** Sends command to draw a Line on OLED Display.
         *  
         *  @param x x-coordinate of the pixel.
         *  @param y y-coordinate of the pixel.
         *
         */
		void drawPoint(uint8_t, uint8_t);

        /** Sends command to draw a Line on OLED Display.
         *  
         *  @param x0 x-coordinate of one point.
         *  @param y0 y-coordinate of one point.
         *  @param x1 x-coordinate of other point.
         *  @param y1 y-coordinate of other point.
         *  
         */
        void drawLine(uint8_t, uint8_t, uint8_t, uint8_t);

        /** Sends command to draw a Circle on OLED Display.
         *  
         *  @param x0 x-coordinate of the center.
         *  @param y0 y-coordinate of the center.
         *  @param radius Radius of the circle.
         *  
         */
        void drawCircle(uint8_t, uint8_t, uint8_t);

        /** Sends command to draw a Circle that is filled on OLED Display.
         *  
         *  @param x0 x-coordinate of the center.
         *  @param y0 y-coordinate of the center.
         *  @param radius Radius of the disc.
         *  
         */
        void drawDisc(uint8_t, uint8_t, uint8_t);

        /** Sends command to draw a Triangle on OLED Display.
         *  
         *  @param x0 x-coordinate of the first point.
         *  @param y0 y-coordinate of the first point.
         *  @param x1 x-coordinate of the second point.
         *  @param y1 y-coordinate of the second point.
         *  @param x2 x-coordinate of the third point.
         *  @param y2 y-coordinate of the third point
         *  
         */
        void drawTriangle(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

        /** Sends command to draw a Rectangle on OLED Display.
         *  
         *  @param x0 x-coordinate of the left corner.
         *  @param y0 y-coordinate of the left corner.
         *  @param radius Width of the rectangle.
         *  @param radius Height of the rectangle.
         *  
         */
        void drawRectangle(uint8_t, uint8_t, uint8_t, uint8_t);

        /** Sends command to draw a Rectangle that is filled on OLED Display.
         *  
         *  @param x0 x-coordinate of the left corner.
         *  @param y0 y-coordinate of the left corner.
         *  @param radius Width of the box.
         *  @param radius Height of the box.
         *  
         */
	    void drawBox(uint8_t, uint8_t, uint8_t, uint8_t);

	private:
		uint8_t _slaveAddress, _lastCommandID;
};

#endif