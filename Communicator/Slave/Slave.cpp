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

Communicator::Communicator(GraphicEngine& ge, Motion& motors):_ge(ge), _motors(motors) {}

void Communicator::begin(uint8_t i2cAddress)
{
	_i2cAddress = i2cAddress;
	Wire.begin(i2cAddress);
	_motors.begin();
	_ge.begin();
  _recieved = false;
  _dataRecieved = false;
}

void Communicator::recieveCommand()
{
	if(Wire.available() >= 10)
	{
    byte temp = Wire.read();
    if(temp == 0x55)
    {
      commandBuffer[0] = temp;
      for(uint8_t i = 1 ; i < 10 ; i++)
        commandBuffer[i] = Wire.read();
      _recieved = true;
    }
    else if(temp == 0xDD)
    {
      dataBuffer[0] = (char)temp;
      for(uint8_t i = 1 ; i < 20 ; i++){
        dataBuffer[i] = (char)Wire.read();
      }
      _dataRecieved = true;
    }
	}
}

void Communicator::executeCommand()
{
	if(!_recieved) return;

	Command_Packet *cp = new Command_Packet(commandBuffer, true);
	_lastCommandID = cp->id;
	float tmp_1;
	
	switch(cp->_command)
	{
    case Command_Packet::Commands::LEFT_MOTOR:
      if(cp->Parameter[0] == 0)
        _motors.motor_l->go((-1)*cp->Parameter[1]);
      else
        _motors.motor_l->go(cp->Parameter[1]);
    break;

    case Command_Packet::Commands::RIGHT_MOTOR:
      if(cp->Parameter[0] == 0)
        _motors.motor_r->go((-1)*cp->Parameter[1]);
      else
        _motors.motor_r->go(cp->Parameter[1]);
    break;

    case Command_Packet::Commands::MOVE:
		tmp_1 = map(cp->Parameter[1], 0, 100 , 0, 255);
	if(cp->Parameter[0] == 0)
      {
        _motors.motor_l->go((-1)*tmp_1);
        _motors.motor_r->go((-1)*tmp_1);
      }
      else
      {
        _motors.motor_l->go(tmp_1);
        _motors.motor_r->go(tmp_1);
      }
    break;

    case Command_Packet::Commands::TURN:
      if(cp->Parameter[0] == 0)
      {
        _motors.motor_l->go(cp->Parameter[1]);
        _motors.motor_r->go((-1)*cp->Parameter[1]);
      }
      else
      {
        _motors.motor_l->go((-1)*cp->Parameter[1]);
        _motors.motor_r->go(cp->Parameter[1]);
      }
    break;

		case Command_Packet::Commands::MOVE_TO:
			tmp_1 = (float)cp->Parameter[0];
      tmp_1 /= 100;
			if(cp->Parameter[1] == 0)
				tmp_1 *= -1;
			_motors.move_to(tmp_1);
      while(!_motors.updt()){}
		break;
      
    case Command_Packet::Commands::TURN_ANGLE:
      tmp_1 = (float)cp->Parameter[0];
      tmp_1 /= 255;
      tmp_1 *= (2*3.1415);
      if(cp->Parameter[1] == 1)
        tmp_1 *= -1;
      _motors.rotate_to(tmp_1);
      while(!_motors.updt()){}
    break;

		case Command_Packet::Commands::STOP:
			_motors.stop();
			break;
		
		// case Command_Packet::Commands::DRAW_POINT:
		// 	_ge.drawPixel(cp->Parameter[0], cp->Parameter[1]);
		// 	break;

		case Command_Packet::Commands::DRAW_LINE:
			_ge.drawLine(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2], cp->Parameter[3]);
			break;
		
		case Command_Packet::Commands::DRAW_CIRCLE:
			_ge.drawCircle(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2]);
			break;
		
		case Command_Packet::Commands::DRAW_DISC:
			_ge.drawDisc(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2]);
			break;
		
		case Command_Packet::Commands::DRAW_TRIANGLE:
			_ge.drawTriangle(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2], cp->Parameter[3], cp->Parameter[4], cp->Parameter[5]);
			break;

		case Command_Packet::Commands::DRAW_RECTANGLE:
			_ge.drawRectangle(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2], cp->Parameter[3]);
			break;

		case Command_Packet::Commands::DRAW_BOX:
			_ge.drawBox(cp->Parameter[0], cp->Parameter[1], cp->Parameter[2], cp->Parameter[3]);
			break;

		case Command_Packet::Commands::DRAW_TEXT:
        while(!_dataRecieved){
          delay(5);
        }
        _ge.drawStr(&dataBuffer[1]);
        _dataRecieved = false;
			break;
		
		case Command_Packet::Commands::CLEAR_SCREEN:
			_ge.clear();
			break;
	}
	delete cp;
	_recieved = false;
}

void Communicator::sendResponse()
{
	Response_Packet *rp = new Response_Packet(_lastCommandID); 
	rp->status = (_motors.getMode() == 0) && !_recieved;
	byte *packetBytes = rp->GetPacketBytes();
	Wire.write(packetBytes, 4);

	delete packetBytes;
	delete rp;
}
