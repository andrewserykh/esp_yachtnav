#include "Arduino.h"
#include "modbusrtu.h"

bool modbusrtu::make(byte adr,byte func, int start, int len){
  union { char bytes[2]; int  ival; } type_int;  
  //char pkt[8]; //Стандартный пакет запроса 01 04 00 00 00 02 30 F3
  packetout[0]=adr;
  packetout[1]=func;
  type_int.ival=start;
  packetout[2] = (byte)type_int.bytes[1];
  packetout[3] = (byte)type_int.bytes[0];
  type_int.ival=len;
  packetout[4] = (byte)type_int.bytes[1];
  packetout[5] = (byte)type_int.bytes[0];
  type_int.ival=_crc(packetout,6);
  packetout[6] = type_int.bytes[1];
  packetout[7] = type_int.bytes[0];
  //String packet="";
  //for (int i=0; i<8; i++) packet+=(char)pkt[i];
  packetout_len=8;
  txdelay = 10;
  return (true);
}

float modbusrtu::getfloat(int start){
      union {
        double u_v;
        float u_f;
        byte u_b[4];
      };
      u_b[1] = packet[3+start];
      u_b[0] = packet[3+start+1];
      u_b[3] = packet[3+start+2];
      u_b[2] = packet[3+start+3];
      return u_f;
}

int modbusrtu::getint(int start){
 return (packet[((start)+3)] * 256 + packet[((start)+4)]);
}


int modbusrtu::length(){
  return (int)packet[2];
}

byte modbusrtu::adr(){
 return packet[0];
}

byte modbusrtu::fn(){
 return packet[1];
}


int modbusrtu::getint_reg(int start){
 return (packet[((start*2)+3)] * 256 + packet[((start*2)+4)]);
}

bool modbusrtu::ispacket() {
  if (packet_length<6 && packet_length>95) return false;
  //byte pkt[96];
  //for (int i=0; i<packet_length; i++) pkt[i]=(byte)packet[i];
  union { char bytes[2]; int  ival; } type_int;  
  type_int.ival=_crc(packet,packet_length-2);
  if ((byte)type_int.bytes[1]==packet[packet_length-2] && (byte)type_int.bytes[0]==packet[packet_length-1]) return true; 
  return false;
}

unsigned int modbusrtu::_crc (byte pkt[],int len){
    unsigned int temp, temp2, flag;
    temp = 0xFFFF;
    for (unsigned char i = 0; i < len; i++)
    {
        temp = temp ^ pkt[i];
        for (unsigned char j = 1; j <= 8; j++)
        {
            flag = temp & 0x0001;
            temp >>=1;
            if (flag)
                temp ^= 0xA001;
        }
    }
    temp2 = temp >> 8;
    temp = (temp << 8) | temp2;
    temp &= 0xFFFF;
    return temp;
}
