#include "Arduino.h"

/***********************************************************/

// Serial MP3 Player Catalex (YX5300 chip)
#include "mp3serial.h"


void setup()
{
	Serial.begin(9600);
	mp3.begin(9600);
	delay(500);

  sendCommand(CMD_SEL_DEV, DEV_TF);
	delay(200);
	sendCommand(CMD_PLAY_W_VOL, 0X0F02); // Playing a 15 (0x0F) Vol the song num 02 (0x02).
}


void loop()
{
 char c=' ';

  // If there a char on Serial call sendMP3Command to sendCommand
   if( Serial.available() )
    {
    c = Serial.read();
    sendMP3Command(c);
    }

  // Check for the answer.
  if (mp3.available())
  {
    Serial.println(decodeMP3Answer());
  }
  delay(100);
}
