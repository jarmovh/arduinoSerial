
int incomingByte;
int led = 13;
char messageBuffer[32];
int buffPos = 0;
boolean serialDone = false;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  
  getSerialContent();
  if( serialDone )
  {
    //flashLed(3);
    parseMessage();
    serialDone = false;
    
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

  memcpy(command, messageBuffer,2);
  command[2] = '\0';
  
  if( strcmp(command,"01") == 0 )  
    respondToSerial("komento1");  // handle command 0x01
  else if( strcmp(command,"02") == 0 )
    respondToSerial("komento2");  // handle command 0x02
  else
    respondToSerial(command);
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
