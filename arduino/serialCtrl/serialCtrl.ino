
int incomingByte;
int led = 13;
char messageBuffer[32];
int buffPos = 0;
boolean serialDone = false;
int fade = 0;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(5,OUTPUT);
}

void loop() {
  
  //analogWrite(pwmB, fade);
  
  getSerialContent();
  if( serialDone )
  {
    flashLed(3);
    parseMessage();
    serialDone = false;
    fade += 10;
    
    //clear the message buffer
    for(int i=0; i<32; i++)
    {
      messageBuffer[i]=NULL;
    }
  }  
  
}

void parseMessage()
{
  char command[3];
  char param[3];
  char value[3];

  memcpy(command, messageBuffer,2);
  memcpy(param, messageBuffer+2,2);
  memcpy(value, messageBuffer+4,2);
  
  command[2] = '\0';
  param[2] = '\0';
  value[2] = '\0';
  
  if( strcmp(command,"01") == 0 )  
  {
    respondToSerial("komento1");    // handle command 0x01
    commandSetPwm(param,value);     // set Pwm at IO param to value(0x00-0xFF)
  }
  else if( strcmp(command,"02") == 0 )
    respondToSerial("komento2");  // handle command 0x02
  else
    respondToSerial(command);
}

void commandSetPwm(char *io, char *str)
{
  int ioPort = 0;
  long int pulseWidth = 0;
  char * pEnd;
  
  String ioS = String(io);
  
  //3, 5, 6, 9, 10, and 11
  //Parse which IO port to use
  if( ioS == "03" )
  {
    ioPort = 3;
    respondToSerial("io3");
  }
  else if( ioS == "05" )
  {
    ioPort = 5;
    respondToSerial("io5");
  }
  
  //convert str from hex to dec
  pulseWidth = strtol(str,&pEnd,16);
  //pulseWidth is set from 0 to 255
  analogWrite(ioPort,pulseWidth);
}

void respondToSerial(String msg)
{
  Serial.println(msg);
}

void getSerialContent()
{
  while( Serial.available() > 0 )
  {
    char inChar = Serial.read();
    messageBuffer[buffPos] = inChar;
    buffPos++;
    
    if( inChar == '\n' || buffPos == 31 )
    {
      serialDone = true;
      buffPos = 0;
      break; 
    }
  }
}

void flashLed(int times)
{
  for(int i=0; i<times; i++)
  {
    digitalWrite(led,HIGH);
    delay(100);
    digitalWrite(led,LOW);
    delay(100);
  }
}
