/* 
Library: Rtc by Makuna
Install this library from Arduino Library Manager first
*/

#include <stdlib.h>
#include <RtcDS1302.h>
#include <ThreeWire.h>

#define PIN_DAT 4     // blue wire
#define PIN_CLK 3     // purple wire
#define PIN_RST 2     // gray wire

ThreeWire myWire(PIN_DAT, PIN_CLK, PIN_RST);        // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

// add pins here
#define PIN_FLASH_LED 5
#define ANALOG_PIN A0

// global variables
long time;
unsigned long prevMillis = 0;
const long INTERVAL = 5000; // 5 seconds async delay


void setup () {
    Serial.begin(9600);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();
    setTime();

    // pin setups here
}

void loop () {
    unsigned long currentMillis = millis();

    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    if (!now.IsValid()) {
        // Common Causes:
        //    1) the battery on the device is low or even missing and the power line was disconnected
        Serial.println("RTC lost confidence in the DateTime!");
    }

    // for testing purposes
    /*
    Serial.println("enter time: ");
    while (!Serial.available()) {}
    long time = Serial.parseInt();
    Serial.print("entered: ");
    Serial.println(time, DEC);
    */
    
    // updates time every 5 seconds
    if (currentMillis - prevMillis >= INTERVAL){
        prevMillis = currentMillis;

        // get current time
        time = getTime(now);
    }

    // maps time to analogWrite range (0-255)
    long lux = calculateLux(time);

    // lux brightness based on time
    float luxMultiplier = (analogRead(ANALOG_PIN) / 1023.0);
    setLux(lux, luxMultiplier);

    bool sunrise = isSunRise(time);
    setBuzzer(sunrise);

    // delay(5000);        // five seconds
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

long getTime(const RtcDateTime& dt) {
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

    long longTime = atol(time);
    Serial.print("long time: ");
    Serial.println(longTime);
    
    return longTime;
}

/**
 * Calculate light intensity (lux) value from time
 * 
 * Examples: 
 *  time 50000 == starting to glow
 *  time 120000 == highest lux
 *  time 190000 == complete darkness
 * 
 * @param time long value
 * @return lux percentage value (light intensity) from 0 to 255
 */
long calculateLux(long time) {
    long sunrise = 50000;
    long noon = 120000;
    long sunset = 190000;
    long lux;
    long minLux = 1;
    long maxLux = 255;

    if ((sunrise < time) && (time < noon)) {        // morning
        lux = map(time, sunrise, noon, minLux, maxLux);
    } else if (time == noon) {      // noon
        lux = maxLux;
    } else if ((noon < time) && (time < sunset)) {      // afternoon
        lux = map(time, noon, sunset, maxLux, minLux);
    } else {
        lux = 0;
    }

    Serial.print("lux: ");
    Serial.println(lux);
    
    return lux;
}

/**
 * Set light intensity (lux) of flash LED 
 * 
 * @param lux
 * @return None
 */
void setLux(long lux, float multiplier) {
    float luxValFloat = (lux * multiplier) + 0.5;   // 0.5 is used for rounding operations
    int luxVal = int(luxValFloat);                  // using int() to float variable floors it
    analogWrite(PIN_FLASH_LED, luxVal);
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

/**
 * Potentiometer multiplier for max lux.
 * 
 * @param analogVal
 * @return value 0.01 to 1.00
 */
float potentionMultiplier(int analogVal){
    float multiplier = (analogVal / 1023);
    Serial.print("analog readings from function: ");
    Serial.println(multiplier);
    return (multiplier);
}
