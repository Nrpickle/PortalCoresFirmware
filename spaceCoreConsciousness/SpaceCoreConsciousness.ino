/*
  SPACE CORE FRIMWARE
  March 2015
  Nick McComb / Dylan Thrush
  
  @desc
  This file is the magical consciousness of the PortalCore project
  
*/

#include <SoftwareSerial.h>

#define DEBUG


SoftwareSerial ESPComm(11, 10); //Rx, Tx

#define FEEDBACK 1
#define NO_FEEDBACK 0

enum Commands {
  NEXT = 0x01,
  PREVIOUS = 0x02,
  SPECIFY_TRACK = 0x03,
  VOLUME_UP = 0x04,
  VOLUME_DOWN = 0x05,
  VOLUME_NUMBER = 0x06,
  SPECIFY_EQ = 0x07,
  REPEAT_A_TRACK = 0x08,
  PLAY_SOURCE = 0x09,
  SLEEP = 0x0A,
  RESET = 0x0C,
  PLAY = 0x0D,
  PAUSE = 0x0E,
  SPECIFY_FOLDER_AND_TRACK = 0x0F,
  CONTINUOUS = 0x11,
  STOP = 0x16,
  RANDOM_TRACK = 0x18,
  REPEAT_CURRENT = 0x19,
  SINGLE_PLAY = 0x19,
  QUERY_VOLUME = 0x43,
  QUERY_EQ = 0x44,
  QUERY_TRACKS_USB = 0x47,
  QUERY_TRACKS_uSD = 0x48,
  QUERY_CURRENT_TRACK_USB = 0x4B,
  QUERY_CURRENT_TRACK_uSD = 0x4C,
  QUERY_TRACKS_FOLDER = 0x4E
};

//Setup function
void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
}

//Main loop
void loop(){
  
  initDevice();
  
  while(1);
  
}

void initDevice(void){
  sendPacket(VOLUME_NUMBER, NO_FEEDBACK, 0, 0x05);  //Sets the volume to 5
}

//Sends a packet to the SOMO
void sendPacket(unsigned char cmd, unsigned char feedback, unsigned char para1, unsigned char para2){
  const unsigned char start = 0x7E;
  const unsigned char end = 0xEF;
  unsigned int checksum = calculateChecksum(cmd, feedback, para1, para2);
  
  #ifdef DEBUG
    Serial.print("Command Sent: ");
    Serial.print(start, HEX);
    Serial.print(" ");
    Serial.print(cmd, HEX);
    Serial.print(" ");
    Serial.print(feedback, HEX);
    Serial.print(" ");
    Serial.print(para1, HEX);
    Serial.print(" ");
    Serial.print(para2, HEX);
    Serial.print(" ");
    Serial.print(checksum, HEX);
    Serial.print(" ");
    Serial.print(end, HEX);
    Serial.println(" ");
  #endif
  
}

unsigned int calculateChecksum(unsigned char cmd, unsigned char feedback, unsigned char para1, unsigned char para2){
  Serial.println(cmd, DEC);
  Serial.println(feedback, DEC);
  Serial.println(para1, DEC);
  Serial.println(para2, DEC);
  unsigned int calculation = 65535;
  Serial.println(calculation, DEC);
  calculation -= (cmd + feedback + para1 + para2);
  Serial.println(calculation, DEC);
  calculation += 1;
  Serial.println(calculation, HEX);
  
  //int calculation = ( 65535 - ((short) cmd + (short) feedback + (short) para1 + (short) para2) + 1 );
  
  Serial.println( 65535 - ((short) cmd + (short) feedback + (short) para1 + (short) para2) + 0x01 );
  
  //Serial.println(calculation, DEC);
  
  return calculation;
}

