/*
  SPACE CORE FRIMWARE
  March 2015
  Nick McComb / Dylan Thrush
  
  @desc
  This file is the magical consciousness of the PortalCore project
  
*/

#include <SoftwareSerial.h>

#define DEBUG


SoftwareSerial SOMOComm(11, 10); //Rx, Tx

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
    Serial.println("[Board Init]");
  #endif
  
  SOMOComm.begin(9600);
  
//  initDevice();
  
  //sendPacket(PLAY, FEEDBACK, 0x00, 0x00);
  
  byte readFromSDCard[8] = { 0x7E, 0x09, 0x00, 0x00, 0x02, 0xFF, 0xF5, 0xEF }; //Tells the SOMO to read from SD Card
  
  byte play[8] = { 0x7E, 0x0D, 0x00, 0x00, 0x00, 0xFF, 0xF3, 0xEF };  //Tells the SOMO to play the latest copied file
  
  SOMOComm.write(readFromSDCard, 9);
  delay(20);
  SOMOComm.write(play, 8);
  
  
}

//Main loop
void loop(){
  
  if(SOMOComm.available()){
    Serial.print(SOMOComm.read() , HEX);
    digitalWrite(13, HIGH);
  }
  
  //while(1);
  
}

void initDevice(void){
  sendPacket(VOLUME_NUMBER, NO_FEEDBACK, 0, 0x10);  //Sets the volume to 16
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
  
  SOMOComm.write(start);
  SOMOComm.write(cmd);
  SOMOComm.write(feedback);
  SOMOComm.write(para1);
  SOMOComm.write(para2);
  SOMOComm.write(checksum);
  SOMOComm.write(end);
  
  
}

unsigned int calculateChecksum(unsigned char cmd, unsigned char feedback, unsigned char para1, unsigned char para2){
  /*
  Serial.println(cmd, DEC);
  Serial.println(feedback, DEC);
  Serial.println(para1, DEC);
  Serial.println(para2, DEC); 
  */
  unsigned int calculation = 65535;
  //Serial.println(calculation, DEC);
  calculation -= (cmd + feedback + para1 + para2);
  //Serial.println(calculation, DEC);
  calculation += 1;
  //Serial.println(calculation, HEX);
  
  //int calculation = ( 65535 - ((short) cmd + (short) feedback + (short) para1 + (short) para2) + 1 );
  
  //Serial.println( 65535 - ((short) cmd + (short) feedback + (short) para1 + (short) para2) + 0x01 );
  
  //Serial.println(calculation, DEC);
  
  return calculation;
}

