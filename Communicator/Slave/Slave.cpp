/**
 *  Communicator that handles Command and Response between Master and Slave Processor.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 04-07-2017
 */

#include "Slave.h"

Command_Packet::Command_Packet(byte* buffer, bool UseSerialDebug)
{
	CheckParsing(buffer[0], COMMAND_START_CODE, "COMMAND_START_CODE", UseSerialDebug);
	id = buffer[1];
	_command = buffer[2];

	for(uint8_t i = 0 ; i < 6 ; i++)
		Parameter[i] = buffer[i+3];

	CheckParsing(buffer[9], COMMAND_END_CODE, "COMMAND_END_CODE", UseSerialDebug);
}

bool Command_Packet::CheckParsing(byte b, byte propervalue, char* varname, bool UseSerialDebug)
{
	bool retval = (b == propervalue);
	if ((UseSerialDebug) && (!retval))
	{
		Serial.print("Response_Packet parsing error ");
		Serial.print(varname);
		Serial.print(" ");
		Serial.print(propervalue, HEX);
		Serial.print(" != ");
		Serial.println(b, HEX);
	}
	return retval;
}

// --------------------------------------------------------------

Response_Packet::Response_Packet(byte id): _id(id){}

byte* Response_Packet::GetPacketBytes()
{
	byte* packetbytes= new byte[4];
    
    packetbytes[0] = RESPONSE_START_CODE;
    packetbytes[1] = _id;
    packetbytes[2] = (status) ? OK : ERROR;
    packetbytes[3] = RESPONSE_END_CODE;

    return packetbytes;
}


// --------------------------------------------------------------

Communicator::Communicator(GraphicEngine& ge):_ge(ge) {}

void Communicator::begin(uint8_t i2cAddress)
{
	_i2cAddress = i2cAddress;
	Wire.begin(i2cAddress);
}

void Communicator::recieveCommand()
{
	if(Wire.available() >= 10)
	{
		for(uint8_t i = 0 ; i < 10 ; i++)
			commandBuffer[i] = Wire.read();
		_recieved = true;
	}
}

void Communicator::executeCommand()
{
	if(!_recieved) return;

	Command_Packet *cp = new Command_Packet(commandBuffer, true);
	_lastCommandID = cp->id;
	switch(cp->_command)
	{
		// case Command_Packet::Commands::DrawPoint:
		// 	_ge.drawPixel(cp->Parameter[0], cp->Parameter[1]);
		// 	break;

		case Command_Packet::Commands::DrawLine:
			_ge.drawLine(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2], cp->Parameter[3]);
			break;
		
		case Command_Packet::Commands::DrawCircle:
			_ge.drawCircle(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2]);
			break;
		
		case Command_Packet::Commands::DrawDisc:
			_ge.drawDisc(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2]);
			break;
		
		case Command_Packet::Commands::DrawTriangle:
			_ge.drawTriangle(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2], cp->Parameter[3], cp->Parameter[4], cp->Parameter[5]);
			break;

		case Command_Packet::Commands::DrawRectangle:
			_ge.drawRectangle(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2], cp->Parameter[3]);
			break;

		case Command_Packet::Commands::DrawBox:
			_ge.drawBox(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2], cp->Parameter[3]);
			break;
	}
	delete cp;
	_recieved = false;
}

void Communicator::sendResponse()
{
	Response_Packet *rp = new Response_Packet(_lastCommandID); 
	byte *packetBytes = rp.GetPacketBytes();
	rp->status = true;

	Wire.write(packetBytes, 4);
	delay(10);

	delete packetBytes;
	delete rp;
}