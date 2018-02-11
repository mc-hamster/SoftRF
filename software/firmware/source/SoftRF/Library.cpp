/*
 * Library.cpp
 * Copyright (C) 2016-2018 Linar Yusupov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "RFHelper.h"
#include "SoCHelper.h"
#include "WiFiHelper.h"
#include "WebHelper.h"

#include "SoftRF.h"

#if LOGGER_IS_ENABLED
#include "LogHelper.h"
#endif /* LOGGER_IS_ENABLED */

char UDPpacketBuffer[256]; //buffer to hold incoming and outgoing packets

void WiFi_forward_to_argus(void);
void WiFi_forward_to_cloud(void);
void *WiFi_relay_from_android(void);

ufo_t fo, Container[MAX_TRACKING_OBJECTS], EmptyFO;
extern ufo_t ThisAircraft;

#if DEBUG
String TxDataTemplate = "0282dd204901f981798a85b69764bdf99ed77fd3c2300000";
#endif

//const float txrx_test_positions[][2] PROGMEM = { { 56.0092, 38.3710 } };
#if 1 /* 2017 */
const float txrx_test_positions[90][2] PROGMEM = {
         { 56.014028944788 , 38.353422195622 } ,
         { 56.01443675978 , 38.354237587162 } ,
         { 56.014484737731 , 38.355439216801 } ,
         { 56.014172879982 , 38.356168777653 } ,
         { 56.013669104455 , 38.356383354374 } ,
         { 56.013117342767 , 38.356168777653 } ,
         { 56.012805473978 , 38.355353386113 } ,
         { 56.012469612467 , 38.354409248539 } ,
         { 56.012133748035 , 38.353851349064 } ,
         { 56.011845861913 , 38.353422195622 } ,
         { 56.011773890047 , 38.354065925786 } ,
         { 56.011809875996 , 38.354387790865 } ,
         { 56.011917833645 , 38.354881317326 } ,
         { 56.011893843082 , 38.355696708866 } ,
         { 56.011917833645 , 38.356383354374 } ,
         { 56.012277690292 , 38.356984169193 } ,
         { 56.012613553472 , 38.357370407291 } ,
         { 56.013141332569 , 38.357542068668 } ,
         { 56.013765062205 , 38.357584984013 } ,
         { 56.01419686913 , 38.357542068668 } ,
         { 56.014532715622 , 38.357971222111 } ,
         { 56.014700637774 , 38.358700782962 } ,
         { 56.014772604187 , 38.359430343815 } ,
         { 56.014388781769 , 38.359902412601 } ,
         { 56.014004955537 , 38.359945327946 } ,
         { 56.013621125491 , 38.359988243289 } ,
         { 56.013141332569 , 38.359945327946 } ,
         { 56.012685523774 , 38.359773666568 } ,
         { 56.012325670926 , 38.35968783588 } ,
         { 56.012013795745 , 38.358743698307 } ,
         { 56.012277690292 , 38.357971222111 } ,
         { 56.013309260769 , 38.357842476078 } ,
         { 56.013861019717 , 38.358014137454 } ,
         { 56.014340803698 , 38.358185798832 } ,
         { 56.014484737731 , 38.358743698307 } ,
         { 56.014412770782 , 38.359344513126 } ,
         { 56.01443675978 , 38.360331566044 } ,
         { 56.014580693455 , 38.360975296207 } ,
         { 56.014820581721 , 38.361747772403 } ,
         { 56.014340803698 , 38.361833603091 } ,
         { 56.013765062205 , 38.361833603091 } ,
         { 56.013261281358 , 38.361790687747 } ,
         { 56.0127335039 , 38.361833603091 } ,
         { 56.012397641763 , 38.361876518436 } ,
         { 56.012061776706 , 38.361833603091 } ,
         { 56.012085767164 , 38.362219841189 } ,
         { 56.012685523774 , 38.362305671878 } ,
         { 56.013693093915 , 38.362348587222 } ,
         { 56.01443675978 , 38.362219841189 } ,
         { 56.01489254791 , 38.363249809451 } ,
         { 56.014868559196 , 38.36445143909 } ,
         { 56.014964513965 , 38.36526683063 } ,
         { 56.01489254791 , 38.366082222171 } ,
         { 56.014484737731 , 38.365481407351 } ,
         { 56.014220858262 , 38.365095169253 } ,
         { 56.013789051605 , 38.364494354434 } ,
         { 56.013381229775 , 38.364193947024 } ,
         { 56.013093352949 , 38.364236862369 } ,
         { 56.012637543587 , 38.364065200991 } ,
         { 56.0122297096 , 38.364279777712 } ,
         { 56.012661533688 , 38.364623100467 } ,
         { 56.01318931213 , 38.364623100467 } ,
         { 56.01364511498 , 38.364880592532 } ,
         { 56.01419686913 , 38.365695984073 } ,
         { 56.01467664894 , 38.366210968203 } ,
         { 56.014670651729 , 38.36485913486 } ,
         { 56.014634668445 , 38.363560945697 } ,
         { 56.014436759779 , 38.362809927172 } ,
         { 56.014736620997 , 38.362058908649 } ,
         { 56.014952519632 , 38.361179144092 } ,
         { 56.014652660091 , 38.360106260486 } ,
         { 56.013513172602 , 38.360127718157 } ,
         { 56.012589563341 , 38.360063345141 } ,
         { 56.012091764775 , 38.359215767091 } ,
         { 56.01207976955 , 38.358250171846 } ,
         { 56.012565573195 , 38.357863933748 } ,
         { 56.012061776705 , 38.35687688083 } ,
         { 56.012013795744 , 38.356061489291 } ,
         { 56.011797880682 , 38.353679687685 } ,
         { 56.012241704777 , 38.354366333193 } ,
         { 56.01248160757 , 38.354752571291 } ,
         { 56.012649538638 , 38.355310470766 } ,
         { 56.012829463973 , 38.355739624209 } ,
         { 56.013333250451 , 38.356061489291 } ,
         { 56.013777056906 , 38.356190235323 } ,
         { 56.014196869128 , 38.355846912569 } ,
         { 56.01435279822 , 38.355203182405 } ,
         { 56.014388781768 , 38.354495079226 } ,
         { 56.014184874557 , 38.354001552767 } ,
         { 56.014016950163 , 38.353615314669 }
      };
#endif

void ParseData()
{

#if DEBUG
    Hex2Bin(TxDataTemplate, RxBuffer);
#endif

    fo.raw = Bin2Hex(RxBuffer);

    if (settings->nmea_p) {
      StdOut.print(F("$PSRFI,")); StdOut.print(now()); StdOut.print(F(",")); StdOut.println(fo.raw);
    }

    if (protocol_decode && (*protocol_decode)((void *) RxBuffer, &ThisAircraft, &fo)) {
      for (int i=0; i < MAX_TRACKING_OBJECTS; i++) {
        int max_dist_ndx = 0;

        if (Container[i].addr == fo.addr) {
          Container[i] = fo;
           break;
        } else {
          if (now() - Container[i].timestamp > ENTRY_EXPIRATION_TIME) {
            Container[i] = fo;
            break;
          }
#if 0
          if  (Container[i].distance > Container[max_dist_ndx].distance)  {
            max_dist_ndx = i;
          }
          if ((i == (MAX_TRACKING_OBJECTS-1)) && (Container[max_dist_ndx].distance > fo.distance) ) {
            Container[max_dist_ndx] = fo; 
          }
#endif
        }
      }
    }
}

void Misc_info()
{
  Serial.println("\r\n");
  Serial.print(F("Chip ID: 0x"));
  Serial.println(SoC->getChipId(), HEX);

  uint32_t realSize = SoC->getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

  Serial.printf("Flash real id:   %08X\n", SoC->getFlashChipId());
  Serial.printf("Flash real size: %u\n\n", realSize);

  Serial.printf("Flash ide  size: %u\n", ideSize);
  Serial.printf("Flash ide speed: %u\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  if (ideSize != realSize) {
    Serial.println(F("Flash Chip configuration wrong!\n"));
  } else {
    Serial.println(F("Flash Chip configuration ok.\n"));
  }
}

void ClearExpired()
{
  for (int i=0; i < MAX_TRACKING_OBJECTS; i++) {
    if (Container[i].addr && (now() - Container[i].timestamp) > LED_EXPIRATION_TIME) {
      Container[i] = EmptyFO;
    }
  }
}

#define take_degrees(x) ( (int) x )
#define take_minutes(x) ( fabs(x - (float) take_degrees(x)) * 60.00)

char * dtostrf_workaround(double number, signed char width, unsigned char prec, char *s) {
  char * rval = dtostrf(number, width, prec, s);
  if (number < 10.0) {
    s[0] = '0';
  }
  return rval;
}

size_t Raw_Receive_UDP(uint8_t *buf)
{
  int noBytes = Uni_Udp.parsePacket();
  if ( noBytes ) {

    if (noBytes > MAX_PKT_SIZE) {
      noBytes = MAX_PKT_SIZE;
    }

    // We've received a packet, read the data from it
    Uni_Udp.read(buf,noBytes); // read the packet into the buffer

    return (size_t) noBytes;
  } else {
    return 0;
  }
}

void Raw_Transmit_UDP()
{
    size_t num = fo.raw.length();
    // ASSERT(sizeof(UDPpacketBuffer) > 2 * PKT_SIZE + 1)
    fo.raw.toCharArray(UDPpacketBuffer, sizeof(UDPpacketBuffer));
    UDPpacketBuffer[num] = '\n';
    SoC->WiFi_transmit_UDP(RELAY_DST_PORT, (byte *)UDPpacketBuffer, num + 1);
}