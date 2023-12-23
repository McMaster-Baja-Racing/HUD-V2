#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

//create a list of lvgl objects to be labels for the TPMS
lv_obj_t *tpmsLabel[4];

String knownAddresses[] = {
  "38:64:00:00:e0:10",
  "38:5b:00:00:5e:40",
  "38:5d:00:00:b6:68",
  "38:5a:00:00:10:03"
};


String addressToName[] = {
  "FR",
  "FL",
  "RL",
  "RR"
};

struct blePacket{
  uint8_t status;
  uint8_t batteryVoltage;
  uint8_t temperature;
  uint16_t absPressure;
  uint16_t checksum;
};

blePacket packets[4];
uint16_t lastChecksum[4];


int scanTime = 5; //In seconds
BLEScan* pBLEScan;
BLEUtils utils;

char convertCharToHex(char ch)
{
  char returnType;
  switch(ch)
  {
    case '0':
    returnType = 0;
    break;
    case  '1' :
    returnType = 1;
    break;
    case  '2':
    returnType = 2;
    break;
    case  '3':
    returnType = 3;
    break;
    case  '4' :
    returnType = 4;
    break;
    case  '5':
    returnType = 5;
    break;
    case  '6':
    returnType = 6;
    break;
    case  '7':
    returnType = 7;
    break;
    case  '8':
    returnType = 8;
    break;
    case  '9':
    returnType = 9;
    break;
    case  'a':
    returnType = 10;
    break;
    case  'b':
    returnType = 11;
    break;
    case  'c':
    returnType = 12;
    break;
    case  'd':
    returnType = 13;
    break;
    case  'e':
    returnType = 14;
    break;
    case  'f' :
    returnType = 15;
    break;
    default:
    returnType = 0;
    break;
  }
  return returnType;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      String address = advertisedDevice.getAddress().toString().c_str();
      uint8_t index = 4;
      for(int i = 0; i < 4; i++){
        if(address == knownAddresses[i]){
            index = i;
            break;
        }
      }
      if (index < 4){
        char *pPL = utils.buildHexData(nullptr, (uint8_t*)advertisedDevice.getPayload(), advertisedDevice.getPayloadLength());
        String sPL = (String) pPL;
        byte plByte[16];
        byte plNib[31];
        sPL.getBytes(plNib,31);
        for (int i=0; i<30; i=i+2) {
            plByte[i/2] = convertCharToHex(plNib[i])*16 + convertCharToHex(plNib[i+1]);
        }

            //Serial.print("  ADDR: "); Serial.print(addr.substring(12));
        char *pHex = utils.buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
        //Serial.print("  MFG DATA: "); Serial.print(pHex);
        String sHex = (String) pHex;
        byte nib[16];
        sHex.getBytes(nib,15);
        for (int i=0; i<14; i++) {
            nib[i]=convertCharToHex(nib[i]);
        }
        blePacket tempPacket;
        tempPacket.status=nib[1]*2+nib[0];
        tempPacket.batteryVoltage=nib[3]*2+nib[2];
        tempPacket.temperature=nib[5]*2+nib[4];
        tempPacket.absPressure=nib[9]*8+nib[8]*4+nib[7]*2+nib[6];
        tempPacket.checksum=nib[13]*8+nib[12]*4+nib[11]*2+nib[10];

        packets[index]=tempPacket;

        //If the label has been created
        if(tpmsLabel[index] != NULL){
          //If the checksum has changed
          if(lastChecksum[index] != tempPacket.checksum){
            //Update the label
            lv_label_set_text_fmt(tpmsLabel[index], "%s\n%u PSI", addressToName[index].c_str(), tempPacket.absPressure);
            //Update the checksum
            lastChecksum[index] = tempPacket.checksum;
          }
        }
      }


    }
};