#include <Arduino.h>
#include <U8g2lib.h>

#include <DFRobot_SHT20.h>

#include <RTClib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_KS0108_128X64_1 u8g2(U8G2_R1, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 16, /*dc=*/ 17, /*cs0=*/ 14, /*cs1=*/ 15, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  21); 	// Set R/W to low!
DFRobot_SHT20    sht20;

RTC_DS1307 rtc;
byte rtcState = 1;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 

#define PIN_PHOTO_SENSOR 21


void setup(void) {
    Serial.begin(57600);
    Serial.println("SHT20 Example!");
    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        rtcState = 0;
    }

    if (! rtc.isrunning()) {
        Serial.println("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    u8g2.begin();
    u8g2.setFlipMode(0);

    sht20.initSHT20();                                  // Init SHT20 Sensor
    delay(100);
    sht20.checkSHT20();                                 // Check SHT20 Sensor
}

void drawData(float humd, float temp)
{
    u8g2.setFont(u8g2_font_logisoso16_tn);
    u8g2.setCursor(1, 20);
    u8g2.print( temp, 1);
    u8g2.setCursor(1, 40);
    u8g2.print( humd, 1);
}

void printdata(float humd, float temp)
{

    Serial.print("Time:");
    Serial.print(millis());
    Serial.print(" Temperature:");
    Serial.print(temp, 1);
    Serial.print("C");
    Serial.print(" Humidity:");
    Serial.print(humd, 1);
    Serial.print("%");
    Serial.println();

}

void printDate(DateTime now)
{
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    Serial.println(); 
}

void drawAdc(int adc)
{
    u8g2.setFont(u8g2_font_crox4tb_tn);
    //u8g2.drawStr(1,60,"ADC:");
    u8g2.setCursor(1, 60);
    u8g2.print(adc);
}

void loop(void) {
    DateTime now = rtc.now(); 
    int val = analogRead(PIN_PHOTO_SENSOR);
    float humd = sht20.readHumidity();                  // Read Humidity
    float temp = sht20.readTemperature();               // Read Temperature
    u8g2.firstPage();
    do {
        drawData(humd, temp);
        drawAdc(val);
    } while ( u8g2.nextPage() );
    delay(4000);
    printdata(humd, temp);
    if(rtcState){
        printDate(now);
    }
}
