#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

File dataFile;
RTC_DS1307 rtc;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");
  
  // Open up the file we're going to log to!
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }
  pinMode(7, OUTPUT);
   digitalWrite(7, HIGH);
   delay(1000);
   digitalWrite(7, LOW);
   delay(1000);
   digitalWrite(7, HIGH);
   delay(1000);
   digitalWrite(7, LOW);
}

void loop()
{
  // make a string for assembling the data to log:
  String dataString = "";
  DateTime now = rtc.now();

  // read three sensors and append to the string:
   float sensor = analogRead(5);
   float sensor_output=5*sensor/1024;
   if(sensor_output > 3.6){
       digitalWrite(7, HIGH);
   }else{
       digitalWrite(7, LOW);
   }
   dataString += String(sensor_output);
   dataString += ","; 
   dataString += String(now.month());
   dataString += "/"; 
   dataString += String(now.day());
   dataString += " "; 
   dataString += String(now.hour());
   dataString += ":"; 
   dataString += String(now.minute());
   dataString += ","; 

  dataFile.println(dataString);

  // print to the serial port too:
  Serial.println(dataString);

  
  // The following line will 'save' the file to the SD card after every
  // line of data - this will use more power and slow down how much data
  // you can read but it's safer! 
  // If you want to speed up the system, remove the call to flush() and it
  // will save the file only every 512 bytes - every time a sector on the 
  // SD card is filled with data.
  dataFile.flush();
  
  // Take 1 measurement every 500 milliseconds
  delay(60000UL);
}
