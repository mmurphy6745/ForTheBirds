//Make sure to use ver. 1.0.0 of the SD library!
//You may have to run the PCF8523 example .ino to initialize the RTC once.
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
 
#include "motor_trigger.h"
 
typedef  unsigned char uint_8t;
 
// Compile with the debug flag set only for testing purposes
#define DEBUG 1
#define TRIGGER_MOTOR 2

// When compiling the code with this flag enabled, every time the Arduino boots,
// the RTC clock time is set to the time at which the code was compiled.
#define SET_TIME 0

#define BUF_SIZE 20
 
#define VALID_TAGS_FILE "TAGS.TXT"
#define LOG_FILE "LOG.TXT"
 
// for the data logging shield cs line
#define SD_CARD_PIN 10

// reducing buffer size makes memory problems less likely to happen
#if DEBUG > 0
#define TAGS_BUFFER_SIZE 128
#else
#define TAGS_BUFFER_SIZE 256
#endif
 
RTC_PCF8523 rtc;
 
bool failed_flag_g;
 
File current_file_handler;
 
char accepted_tags[TAGS_BUFFER_SIZE];
unsigned long time;
 

// Show blinking code on LED light.
void fatal_error(int dd)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  while(1)
  {
    for (int n = 0; n < dd; n++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
    }
    delay(1000);
  }
}
 
void setup()
{
  failed_flag_g = false;
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
 
  //initialize accepted tags buffer
 
  for (int i = 0; i < TAGS_BUFFER_SIZE; i++)
  {
    accepted_tags[i] = '\0';
  }
 
  if (!Serial)
  {
    fatal_error(1);
  }
 
 
   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    fatal_error(2);
  }
  
  
  delay(1000);
 
  if (! rtc.initialized()) {
#if DEBUG
	
	// Just for convenience, wait a little bit to give user time to see debug output
	Serial.println(F("Waiting 10 seconds..."));
	delay(5000);
	Serial.println(F("Waiting 5 seconds..."));
	delay(5000);
	Serial.println(F("RTC is NOT running!"));
#if DEBUG
	
#if SET_TIME
	// Setting the time will initialize the RTC
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
#else
	fatal_error(3);
#endif

	
#if DEBUG
	delay(1000);
	// test if RTC has been initialized
	if (rtc.initialized()) {
		Serial.println(F("RTC is now initialized."));
	}
	else {
		Serial.println(F("RTC did not initialize after time was set."));
	}
	
	delay(1000);
	
	// Display current time
	DateTime now = rtc.now();

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
#endif
    
  }
 
#if TRIGGER_MOTOR
  motor_trigger_init();
#endif
 
 
 
 
#if DEBUG
  Serial.println(F("rtc OK"));
  DateTime now = rtc.now();
  Serial.println(now.unixtime());
#endif
 
  // INITIALIZE SD CARD
#if DEBUG
  Serial.print(F("Initializing SD card..."));
#endif
  pinMode(SD_CARD_PIN, OUTPUT);
 
  if (!SD.begin(SD_CARD_PIN)) {
#if DEBUG
    Serial.println(F("SD card failed, or not present"));
#endif
    failed_flag_g = true;
    return;
  }
#if DEBUG
  Serial.println(F("SD card initialization OK"));
 
#endif
 
  // LOAD VALID TAGS INTO MEMORY
  current_file_handler =  SD.open(VALID_TAGS_FILE, FILE_READ);
 
  if (current_file_handler)
  {
#if DEBUG
    Serial.print(F(VALID_TAGS_FILE));
    Serial.println(F(" open successfully."));
    time = millis();
#endif
    int i = 0;
    char c;
 
    while (i < TAGS_BUFFER_SIZE - 1 && current_file_handler.available())
    {
      c = current_file_handler.read();
      if (c >= 48 && c <= 70)
      {
        accepted_tags[i] = c;
        i++;
      }
    }
    accepted_tags[i] = '\0';
 
#if DEBUG
    Serial.println(millis() - time);
    Serial.println(accepted_tags);
#endif
    current_file_handler.close();
  }
  else
  {
#if DEBUG
    Serial.print(F(VALID_TAGS_FILE));
    Serial.println(F(" failed to open."));
#endif
  }
 
  current_file_handler =  SD.open(LOG_FILE, FILE_WRITE);
 
  if (current_file_handler)
  {
#if DEBUG
    Serial.print(F(LOG_FILE));
    Serial.println(F(" open successfully."));
#endif
  }
}
 
 
void process_tag(uint8_t * buffer, int n)
{
 
#if DEBUG
  time = millis();
#endif
  // terminate string to 10 characters
  buffer[10] = 0;
 
  // determine if tag belongs to the list of accepted tags
  bool tag_accepted = (strstr(accepted_tags, (char *) buffer) != NULL) ? true : false;
 
  DateTime now = rtc.now();
 
  if (tag_accepted)
  {
#if TRIGGER_MOTOR
    motor_trigger();
#endif
  }
 
  String date_str = String(now.year(), DEC) + "/" +  String(now.month(), DEC) + "/" + String(now.day(), DEC);
  String time_str = String(now.hour(), DEC) + ":" +  String(now.minute(), DEC) + ":" + String(now.second(), DEC);
  String out_str = date_str + "," + time_str + "," + String((char *) buffer) + ",";
 
  if (tag_accepted)
  {
    out_str += "T";
  }
  else
  {
    out_str += "F";
  }
 
  current_file_handler.println(out_str);
  current_file_handler.flush();
#if DEBUG
  Serial.println("Took " + String(millis() - time, DEC) + " ms.");
  Serial.println(out_str);
 
#endif
}
 
 
void loop()
{
  uint_8t buffer[BUF_SIZE];
  uint_8t buf_idx = 0;
  uint_8t c;
  for (int i = 0; i < BUF_SIZE; i++)
  {
    buffer[i] = 0;
  }
 
  while (1)
  {
    if (Serial.available())
    {
      // read incoming byte
      c = (uint_8t) Serial.read();
 
      // BEGINNING OF TRANSMISSION byte (do nothing)
      if (c == 2)
      {
        break;
      }
      // END OF TRANSMISSION byte
      else if (c == 3)
      {
        buf_idx++;
        if (buf_idx >= 10)
        {
          process_tag(buffer, buf_idx);
        }
        break;
 
      }
      // Store byte in buffer
      else
      {
        buffer[buf_idx] = c;
        buf_idx++;
        if (buf_idx >= BUF_SIZE)
        {
          // ERROR, this should not happen
          break;
        }
      }
    }
  }
}

