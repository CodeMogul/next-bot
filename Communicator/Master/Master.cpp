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

Data_Packet::Data_Packet(char* str)
{
	data[0] = DATA_START_CODE;
	uint8_t i = 1;
	for( ; i < 18, *str; i++, str++)
	{
		data[i] = *str;
	}
	data[i] = '\0';
	data[19] = DATA_END_CODE;
}


//------------------------------------------------------------------

Communicator::Communicator()
{
	randomSeed(analogRead(0));
}

void Communicator::begin(uint8_t slaveAddress)
{
	_slaveAddress = slaveAddress; 
	_commandSent = false;
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

void Communicator::drawText(char *str)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::DrawText);
	cp->Parameter[0] = 0x00;
	cp->Parameter[1] = 0x00;
	cp->Parameter[2] = 0x00;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;

	Data_Packet *dp = new Data_Packet(str);
	sendData(dp->data);
	delete dp;
}

void Communicator::clearScreen()
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::ClearScreen);
	cp->Parameter[0] = 0x00;
	cp->Parameter[1] = 0x00;
	cp->Parameter[2] = 0x00;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::leftMotor(uint8_t dir, uint8_t speed)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::LeftMotor);
	cp->Parameter[0] = dir;
	cp->Parameter[1] = speed;
	cp->Parameter[2] = 0x00;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::rightMotor(uint8_t dir, uint8_t speed)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::RightMotor);
	cp->Parameter[0] = dir;
	cp->Parameter[1] = speed;
	cp->Parameter[2] = 0x00;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::move(uint8_t dir, uint8_t speed)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::Move);
	cp->Parameter[0] = dir;
	cp->Parameter[1] = map(speed, 0, 100, 0, 255);
	cp->Parameter[2] = 0x00;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::moveDistance(uint8_t cm, uint8_t dir, uint8_t speed)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::MoveDistance);
	cp->Parameter[0] = cm;
	cp->Parameter[1] = dir;
	cp->Parameter[2] = speed;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}


void Communicator::stop()
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::Stop);
	cp->Parameter[0] = 0x00;
	cp->Parameter[1] = 0x00;
	cp->Parameter[2] = 0x00;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::turnAngle(uint8_t degree, uint8_t dir, uint8_t speed)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::TurnAngle);
	float fd = degree;
	fd = (fd*255/360);
	degree = fd;
	cp->Parameter[0] = degree;
	cp->Parameter[1] = dir;
	cp->Parameter[2] = speed;
	cp->Parameter[3] = 0x00;
	cp->Parameter[4] = 0x00;
	cp->Parameter[5] = 0x00;

	byte *packetbytes = cp->GetPacketBytes();
	sendCommand(packetbytes);

	delete cp;
	delete packetbytes;
}

void Communicator::turn(uint8_t speed, uint8_t dir)
{
	_lastCommandID = random(255);
	Command_Packet *cp = new Command_Packet(_lastCommandID, Command_Packet::Commands::Turn);
	cp->Parameter[0] = dir;
	cp->Parameter[1] = speed;
	cp->Parameter[2] = 0x00;
	cp->Parameter[3] = 0x00;
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
	_commandSent = true;
	delay(30);
}

void Communicator::sendData(byte* packetbytes)
{
	Wire.beginTransmission(_slaveAddress);
	Wire.write(packetbytes, 20);
	Wire.endTransmission();
	delay(30);
}

bool Communicator::recieveResponse()
{
	if(!_commandSent) return true;

	Wire.requestFrom(int(_slaveAddress), 4);
	while(Wire.available() < 4)	delay(10);

	byte temp[4];
	for(uint8_t i = 0 ; i < 4 ; i++)
		temp[i] = Wire.read();

	Response_Packet *rp = new Response_Packet(temp, true);
	Serial.print("L: ");
	Serial.print(_lastCommandID);
	Serial.print("\tR: ");
	Serial.println(rp->_id);
	bool retval = rp->status && (_lastCommandID == rp->_id);
	delete rp;
	_commandSent = !retval;
	return retval;
}
