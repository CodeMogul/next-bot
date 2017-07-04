/**
 *  Communicator that handles Command and Response between Master and Slave Processor.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 04-07-2017
 */

#include "Master.h" 

Command_Packet::Command_Packet(byte id, Commands::Commands_Enum command)
    :_id(id), _command(command) {}


// returns the 10 bytes of the generated command packet
// remember to call delete on the returned array
byte* Command_Packet::GetPacketBytes()
{
    byte* packetbytes= new byte[10];
    
    packetbytes[0] = COMMAND_START_CODE;
    packetbytes[1] = _id;
    packetbytes[2] = _command;
    
    for(uint8_t i = 0 ; i < 6 ; i++)
        packetbytes[i+3] = Parameter[i];

    packetbytes[9] = COMMAND_END_CODE;

    return packetbytes;
}

//------------------------------------------------------------------


bool Response_Packet::CheckParsing(byte b, byte propervalue, char* varname, bool UseSerialDebug)
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

Response_Packet::Response_Packet(byte* buffer, bool UseSerialDebug)
{
	CheckParsing(buffer[0], RESPONSE_START_CODE, "RESPONSE_START_CODE", UseSerialDebug);
	_id = buffer[1];
	status = (buffer[2] == OK);

	CheckParsing(buffer[3], RESPONSE_END_CODE, "RESPONSE_END_CODE", UseSerialDebug);
}


//------------------------------------------------------------------

Communicator::Communicator()
{
	randomseed(analogRead(0));
}

void Communicator::begin(uint8_t slaveAddress)
{
	_slaveAddress = slaveAddress; 
	Wire.begin();
}

void Communicator::drawPoint(uint8_t x0, uint8_t y0)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::DrawPoint);
	cp->Parameter[0] = x0;
	cp->Parameter[1] = y0;
	cp->Parameter[2] = 0x00;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::DrawLine);
	cp->Parameter[0] = x0;
	cp->Parameter[1] = y0;
	cp->Parameter[2] = x1;
	cp->Parameter[3] = y1;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::drawCircle(uint8_t x, uint8_t y, uint8_t radius)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::DrawCircle);
	cp->Parameter[0] = x;
	cp->Parameter[1] = y;
	cp->Parameter[2] = radius;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::drawDisc(uint8_t x, uint8_t y, uint8_t radius)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::DrawDisc);
	cp->Parameter[0] = x;
	cp->Parameter[1] = y;
	cp->Parameter[2] = radius;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::drawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::DrawTriangle);
	cp->Parameter[0] = x0;
	cp->Parameter[1] = y0;
	cp->Parameter[2] = x1;
	cp->Parameter[3] = y1;
	cp->Parameter[4] = x2;
	cp->Parameter[5] = y2;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::drawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::DrawRectangle);
	cp->Parameter[0] = x;
	cp->Parameter[1] = y;
	cp->Parameter[2] = width;
	cp->Parameter[3] = height;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::drawBox(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::DrawBox);
	cp->Parameter[0] = x;
	cp->Parameter[1] = y;
	cp->Parameter[2] = width;
	cp->Parameter[3] = height;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::sendCommand(byte* packetbytes)
{
	Wire.beginTransmission(_slaveAddress);
	Wire.write(packetbytes, 10);
	Wire.endTransmission();
	delay(10);
}

bool Communicator::recieveResponse()
{
	Wire.requestFrom(int(_slaveAddress), 4);
	while(Wire.available() < 4)	delay(10);

	byte temp[4];
	for(uint8_t i = 0 ; i < 4 ; i++)
		temp[i] = Wire.read();

	Response_Packet *rp = new Response_Packet(temp, true);
	bool retval = rp->status && (_lastCommandID == rp->id);
	delete rp;

	return retval;
}