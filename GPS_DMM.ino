/***********************************************************************/
/* GPS_DMM.ino by fabelec (Geocaching account: fab_seeker)             */
/* GPS with coordinates for geocaching                                 */
/*                                                                     */
/* Purpose: get GPS coordinates at DMM format and display on 16*2 lcd  */
/*                                                                     */
/* Requires:                                                           */
/* - a UBlox NEO 6M gps module                                         */
/* - a LCD screen 1602                                                 */
/* - Arduino uno R3                                                    */
/* - a 10K Potentiometer                                               */
/*                                                                     */
/* Note:                                                               */
/* When the program is downloaded and everything connected, adjust the */
/* contrast of the screen with the potentiometer. You should be able   */
/* to go geocaching !! :D                                              */
/*                                                                     */
/* Example besides a Garmin Oregon 600:                                */
/* http://chercheetcache.e-monsite.com/medias/images/montage.jpg       */
/*                                                                     */
/***********************************************************************/

/* Libraries used: */
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <math.h>
#include <LiquidCrystal.h>

/* Connections for the LCD screen:
   -------------------------------
- LCD RS connected to the pin 7 of arduino uno R3
- LCD Enable connected to the pin 8 of arduino uno R3
- LCD D4 connected to the pin 9 of arduino uno R3
- LCD D5 connected to the pin 10 of arduino uno R3
- LCD D6 connected to the pin 11 of arduino uno R3
- LCD D7 connected to the pin 12 of arduino uno R3
- LCD R/W connected to the Ground GND
- LCD VDD to the Ground GND
- LCD K to the Ground GND
- LCD VSS to +5V
- LCD A to +5V
- Potentiometer 10K : a pin at +5V the other to the Ground GND
- the middle pin of the Potentiometer connected to LCD VO
*/
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

/* Variables: */
long lat, lon;
long latD, lonD, latM, lonM, latMM, lonMM;
char NS, EW;
bool SatOk = false;

/* Connections of ublox neo 6m:
   ----------------------------
Power supply:
- VCC of ublox neo 6m connected to 3,3V, and the GND to GND of arduino uno R3
RX and TX:
- TX of ublox neo 6m connected to the pin 2 of Arduino uno R3
- RX of ublox neo 6m connected to the pin 3 of Arduino uno R3
*/
SoftwareSerial gpsSerial(2,3);

/* Variable based on TinyGPS: */
TinyGPS gps;

void setup()
{
    Serial.begin(19200);
    gpsSerial.begin(9600);
    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    SatOk = false;
}

void loop()
{
    while(gpsSerial.available() == false and SatOk == false){
            lcd.setCursor(0,0);
            lcd.print("Searching...    ");
            lcd.setCursor(0,1);
            lcd.print("                ");
    }
    while(gpsSerial.available()){
       
        if(gps.encode(gpsSerial.read())){
            /* If it is the first time that gps coordinates are being displayed */
            if (SatOk == false){
                /* clear the screen (removes "Searching..." display) */
                lcd.clear();
            }
            /* Set at true the variable memorizing that satellites conditions are Ok */
            SatOk = true;
           
            /* Get the Position read at XY format: */
            gps.get_position(&lat,&lon);

            /* Convert GPS Position Data to DMM format: */
            if(lat>=0){NS = 'N';}
            else{NS = 'S';}
            if(lon>=0){EW = 'W';}
            else{EW = 'E';}
            lat = abs(lat);
            lon = abs(lon);
            latD = floor(lat/1000000);
            lonD = floor(lon/1000000);
            latM = floor(((lat-(latD*1000000))*60)/1000000);
            lonM = floor(((lon-(lonD*1000000))*60)/1000000);
            latMM = floor((((((lat-(latD*1000000))*60)-(latM*1000000))/1000)));
            lonMM = floor((((((lon-(lonD*1000000))*60)-(lonM*1000000))/1000)));
           
            /* Print latitude at the DMM format on lcd */
            /* NS latD ° latM . latMM                  */
            lcd.setCursor(0,0);
            lcd.print(NS);
            if(latD<10){lcd.print('0');}
            lcd.print(latD);
            lcd.print("  ");
            if(latM<10){lcd.print('0');}
            lcd.print(latM);
            lcd.print('.');
            if(latMM<100){lcd.print('0');}
            if(latMM<10){lcd.print('0');}
            lcd.print(latMM);
           
            /* Print longitude at the DMM format on lcd */
            /* EW lonD ° lonM . lonMM                   */
            lcd.setCursor(0,1);
            lcd.print(EW);
            if(lonD<100){lcd.print('0');}
            if(lonD<10){lcd.print('0');}
            lcd.print(lonD);
            lcd.print(" ");
            if(lonM<10){lcd.print('0');}
            lcd.print(lonM);
            lcd.print('.');
            if(lonMM<100){lcd.print('0');}
            if(lonMM<10){lcd.print('0');}
            lcd.print(lonMM);
            delay(200);
        }
    }
}
