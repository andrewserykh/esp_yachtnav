#include "Arduino.h"
#include "nmea.h"

#define NMEA_END_CHAR_1 '\n'
#define NMEA_MAX_LENGTH 70


bool nmea::GPZDA (int hour,int minute,int second,int day,int month,int year){
  //$GPZDA, hhmmss.s, xx, xx, xxxx, xx, xx  *hh <CR><LF> 
  
  message = "$GPZDA,";
  message = message + _zerohead(hour);
  message = message + _zerohead(minute);
  message = message + _zerohead(second);
  message = message + ".00,";
  message = message + _zerohead(day);
  message = message + ",";
  message = message + _zerohead(month);
  message = message + ",";
  message = message + _zerohead(year);
  message = message + ",00,00*";
  message = message + String(_crc (message),HEX);
  message = message + "\n";
  
  return (true);
}

String nmea::_zerohead (int number){
  String value="00";
  value = String (number);
  if (number < 10 && number >= 0) value = "0" + String(number);
  return value;
}

int nmea::_crc( String message )
{
    char Character;
    int Checksum = 0;


    //foreach(char Character in sentence)
    for (int i=0;i<message.length();i++)
    {
        Character = message[i];
        switch(Character)
        {
            case '$':
                // Ignore the dollar sign
                break;
            case '*':
                // Stop processing before the asterisk
                i = message.length();
                continue;
            default:
                // Is this the first value for the checksum?
                if (Checksum == 0)
                {
                    // Yes. Set the checksum to the value
                    Checksum = Character;
                }
                else
                {
                    // No. XOR the checksum with this character value
                    Checksum = Checksum ^ Character;
                }
                break;
        }
    }

    // Return the checksum
    return (Checksum);

} // _crc
