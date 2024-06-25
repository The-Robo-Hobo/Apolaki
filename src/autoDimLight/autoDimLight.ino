/* 
Library: Rtc by Makuna
Install this library from Arduino Library Manager first
*/

#include <RtcDS1302.h>
#include <ThreeWire.h>

#define PIN_DAT = 4     // blue wire
#define PIN_CLK = 3     // purple wire
#define PIN_RST = 2     // gray wire

ThreeWire myWire(4, 3, 2);        // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

// add pins here
const int PIN_FLASH_LED;

void setup () 
{
    Serial.begin(9600);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();
    setTime();

    // pin setups here
}

void loop () {
    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    if (!now.IsValid()) {
        // Common Causes:
        //    1) the battery on the device is low or even missing and the power line was disconnected
        Serial.println("RTC lost confidence in the DateTime!");
    }

    int time = getTime(now);
    Serial.print("int time: ");
    Serial.println(time);

    int lux = calculateLux(time);
    setLux(lux);
    bool sunrise = isSunRise(time);
    setBuzzer(sunrise);

    delay(5000);        // five seconds
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}

void setTime() {
    // for setting the correct time
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected()) {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning()) {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    } else if (now > compiled) {
        Serial.println("RTC is newer than compile time. (this is expected)");
    } else if (now == compiled) {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
}

int getTime(const RtcDateTime& dt) {
    // for getting the current time
    char time[8];

    snprintf_P(time, 
            countof(time),
            PSTR("%02u%02u%02u"),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print("char time: ");
    Serial.println(time);

    int intTime = atoi(time);
    return intTime;
}

/**
 * Calculate light intensity (lux) value from time
 * 
 * Examples: 
 *  time 50000 == starting to glow
 *  time 120000 == highest lux
 *  time 190000 == complete darkness
 * 
 * @param time value y
 * @return lux percentage value (light intensity) from 0 to 100
 */
int calculateLux(int time) {
    int lux;
    return lux;
}

/**
 * Set light intensity (lux) of flash LED 
 * 
 * @param lux
 */
void setLux(int lux) {

}

/**
 * Determine if sun is risen or not
 * 
 * Example:
 *  time 53000 == true
 *  else false
 * 
 * @param time
 * @return boolean value
 */
bool isSunRise(int time) {

}

/**
 * Activates the buzzer if the sun is risen
 */
void setBuzzer(bool isSunRise) {
    
}