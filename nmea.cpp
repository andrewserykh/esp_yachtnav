#include "Arduino.h"
#include "nmea.h"

#define NMEA_END_CHAR_1 '\n'
#define NMEA_MAX_LENGTH 70

uint8_t nmea::queryCount (){
  //01234567890123456789012
  //$PWPEQ,ZDA,GGA,POVER*30
  uint8_t count=0;
  String temp;

  temp = message.substring(0, message.indexOf(",")); //выделяем заголовок
  if (temp.indexOf('Q') < 0) count = 0; //если нет символа Q - запрос, то выход

  if (message.indexOf(',') < 0) return (0); //если нет символа "," то нет запросов

  temp = message.substring( 1, message.indexOf('*') ); //убираем * в конце пакета

  while ( temp.indexOf(',') > 0 ){
      temp = temp.substring( temp.indexOf(',')+1, temp.length() ); 
      count ++;   
  }

  return count;
}

String nmea::query(uint8_t number){
  String res = "";

  if (number > queryCount()) return res;

  String temp = message.substring( 1, message.indexOf('*') ); //убираем * в конце пакета

  uint8_t count = 0;

  while ( temp.indexOf(',') > 0 ){
      temp = temp.substring( temp.indexOf(',')+1, temp.length() );

      if (number == count) {
        if (temp.indexOf(',')!=-1) { res = temp.substring(0, temp.indexOf(',')); }
        else { res = temp; }
        return res;
      }
      
      count ++;   
  }
  
  
  return res;
}

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
  message = message + NMEA_END_CHAR_1;
  
  return (true);
}

String nmea::_zerohead (int number){
  String value="00";
  value = String (number);
  if (number < 10 && number >= 0) value = "0" + String(number);
  return value;
}

bool nmea::crcValid (){
  String temp = message.substring( 1, message.indexOf('*') ); //убираем * в конце пакета
  String tcrc = String(_crc (temp),HEX);
  String scrc = message.substring( message.length()-2, message.length() );

  if ( tcrc.equalsIgnoreCase(scrc) ) {
    return true;
  } else {
    return false; 
  }

}

int nmea::_crc( String msg )
{
    char Character;
    int Checksum = 0;


    //foreach(char Character in sentence)
    for (int i=0;i<msg.length();i++)
    {
        Character = msg[i];
        switch(Character)
        {
            case '$':
                // Ignore the dollar sign
                break;
            case '*':
                // Stop processing before the asterisk
                i = msg.length();
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
