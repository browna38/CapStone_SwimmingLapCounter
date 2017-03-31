//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
// Timer Code, Start Stop Button,  
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>

LiquidCrystal lcd(32,30,28,26,24,22);

// For Sensor Code:
#define TRIGGER_PIN  33                 
#define ECHO_PIN     2              // PWM Pin
#define Buzzer       13             //Buzzer PWM Pin
#define CS_PIN       53

// For Timer Code, Start/Stop:
int buttonPin = 34;                 // button on pin 34

int buttonState;                    // variable to store button state
int lastButtonState;                // variable to store last button state
int state;                          // variable to differ from button conditions

int frameRate = 100;                // the frame rate (frames per second) at which the stopwatch runs
long interval = (1000/frameRate);   // blink interval

long startTime ;                    // start time for stop watch
long elapsedTime ;                  // elapsed time for stop watch

int fractional;                     // variable used to store fractional part of Frames
int fractionalSecs;                 // variable used to store fractional part of Seconds
int fractionalMins;                 // variable used to store fractional part of Minutes

int elapsedFrames;                  // elapsed frames for stop watch
int elapsedSeconds;                 // elapsed seconds for stop watch
int elapsedMinutes;                 // elapsed Minutes for stop watch

char buf[10];                       // string buffer for itoa function

//For Buzzer Code:
int sound = 250;                        //Initializing tone sound

//For SD Card Reader Code
const int chipSelect = 53;
int lapDelay = true;

void setup(){
  lcd.begin(16, 2);                // Intialise the LCD.
  
  pinMode(TRIGGER_PIN, OUTPUT);    // Set Trigger Pin as Output
  pinMode(ECHO_PIN, INPUT);        // Set Echo Pin as Input
  pinMode(buttonPin, INPUT);       // Set Pin as Input
  pinMode(Buzzer, OUTPUT);         // Buzzer Pin as Output
  pinMode(CS_PIN, OUTPUT);         // Set SD card chip select as output

  digitalWrite(CS_PIN, HIGH);
  
  digitalWrite(buttonPin, HIGH);   // Turn on pullup resistors. Wire button so that press shorts pin to ground.

  if (!SD.begin(chipSelect)) {     // See if the SD card is present and can be initialized
    return;
  }
  
  lcd.setCursor(0,0);
  lcd.print("      Timer     ");
  lcd.setCursor(0,1);
  lcd.print("       G02      ");
}

void loop(){
  float Duration, Distance;
  buttonState = digitalRead(buttonPin);                   // Check for button press, read the button state and store

  // Initialize for Sensor:
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  Duration = pulseIn(ECHO_PIN, HIGH);     //Duration of the sensor pulse
  Distance = (Duration/2)/29.1;           //Distance in cm
  
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
//  Timer Code - Start Stop function with running time
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
   if (buttonState == LOW && lastButtonState == HIGH  &&  state == false){
      lapDelay = true;                                    // Makes it not count a lap as you begin
      //Starting Tones To Inform Swimmer Of Beginning Of Session:
      sound = 800;                                        // Tone of buzzer that will be played
      tone(Buzzer, sound);                                // Execute buzzer sound
      delay(300);                                         // Delay 0.3s for tone sound then delay for no tone
      noTone(Buzzer);
      delay(300);
      sound = 800;                                        
      tone(Buzzer, sound);                                
      delay(300);
      noTone(Buzzer);
      delay(300);
      sound = 800;                                         
      tone(Buzzer, sound);                                
      delay(300);
      noTone(Buzzer);
      delay(300);
      sound = 1000;                                        
      tone(Buzzer, sound);                                
      delay(1000);
      noTone(Buzzer);
   
      startTime = millis();                               // store the start time (millis() is the time in milliseconds from which the program started running)
      state = true;                                       // turn on blinking while timing   
      delay(10);                                          // short delay to debounce switch
      lastButtonState = buttonState;                      // store buttonState in lastButtonState, to compare next time 
      
      delay(5000);                                        // Waits 5s for you to get away from the wall
      lapDelay = false;                                   // Allows the next incoming lap to be counted
      
   }else if (buttonState == LOW && lastButtonState == HIGH && state == true){
      state = false;                                      // turn off blinking, all done timing
      lastButtonState = buttonState;                      // store buttonState in lastButtonState, to compare next time
      // Elapsed Time Routine            
      elapsedTime = millis() - startTime;                 // store elapsed time
      elapsedMinutes = (elapsedTime / 60000L);
      elapsedSeconds = (elapsedTime / 1000L);             // divide by 1000 to convert to seconds - then cast to an int to print
      elapsedFrames = (elapsedTime / interval);           // divide by 100 to convert to 1/100 of a second - then cast to an int to print
      fractional = (int)(elapsedFrames % frameRate);      // use modulo operator to get fractional part of 100 Seconds
      fractionalSecs = (int)(elapsedSeconds % 60L);       // use modulo operator to get fractional part of 60 Seconds
      fractionalMins = (int)(elapsedMinutes % 60L);       // use modulo operator to get fractional part of 60 Minutes
      lcd.clear();                                        // clear the LDC
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      if (fractionalMins < 10){                           // pad in leading zeros (For Minutes)
            lcd.print("Time: 0");                         // add a zero
      }
      lcd.print(itoa(fractionalMins, buf, 10));           // convert the int to a string and print a fractional part of 60 Minutes to the LCD
      lcd.print(":");                                     // print a colan. 
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      if (fractionalSecs < 10){                           // Same as above for seconds 
           lcd.print("0");                                 
      }
      lcd.print(itoa(fractionalSecs, buf, 10));          
      lcd.print(":");                                    
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      if (fractional < 10){                               // Same as above for ms 
            lcd.print("0");                                
      }     
      lcd.print(itoa(fractional, buf, 10));              
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   
   }else{
      lastButtonState = buttonState;                       // store buttonState in lastButtonState, to compare next time
   }

   // When timer is running clear the LCD, only print time when stopped
   if (state == true){
      lcd.clear();                                    // clear the LCD
      if(Distance <= 30 && Distance >= 20  && lapDelay == false){   //If we are between 30-20cm and haven't recorded a lap in the last 10s we want the buzzer to go off
          lapDelay = true;                                    // Stops it from counting another lap immediately
          elapsedTime =   millis() - startTime;               // store elapsed time
          elapsedMinutes = (elapsedTime / 60000L);
          elapsedSeconds = (elapsedTime / 1000L);             // divide by 1000 to convert to seconds - then cast to an int to print
          elapsedFrames = (elapsedTime / interval);           // divide by 100 to convert to 1/100 of a second - then cast to an int to print
          fractional = (int)(elapsedFrames % frameRate);      // use modulo operator to get fractional part of 100 Seconds
          fractionalSecs = (int)(elapsedSeconds % 60L);       // use modulo operator to get fractional part of 60 Seconds
          fractionalMins = (int)(elapsedMinutes % 60L);       // use modulo operator to get fractional part of 60 Minutes
          lcd.clear();                                        // clear the LCD
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
          if (fractionalMins < 10){                           // pad in leading zeros (For Minutes)
              lcd.print("Time: 0");                         // add a zero
          }
          lcd.print(itoa(fractionalMins, buf, 10));           // convert the int to a string and print a fractional part of 60 Minutes to the LCD
          lcd.print(":");                                     // print a colon. 
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
          if (fractionalSecs < 10){                           // Same as above for seconds 
              lcd.print("0");                                 
          }
          lcd.print(itoa(fractionalSecs, buf, 10));          
          lcd.print(":");                                    
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
          if (fractional < 10){                               // Same as above for ms 
              lcd.print("0");                                
          }     
          lcd.print(itoa(fractional, buf, 10));
//-------------------------------------------------------------------------------------------------------------------------------------------------------
          
          String dataString = "";                             // Creating a 00:00:00 form string with the lap time
          if (fractionalMins < 10){                           //
            dataString += String(0);                          //
          }                                                   //
          dataString += String(fractionalMins);               //
          dataString += ":";                                  //
          if (fractionalSecs < 10){                           //
            dataString += String(0);                          //
          }                                                   //
          dataString += String(fractionalSecs);               //
          dataString += ":";                                  //
          if (fractional < 10){                               //
            dataString += String(0);                          //
          }                                                   //
          dataString += String(fractional);                   //
                                                              
          File dataFile = SD.open("datalog.csv", FILE_WRITE); // open the file and call it "datalog.csv"

          if (dataFile) {                                     // if the file is available, write to it:
            dataFile.println(dataString);
            dataFile.close();
          }
          sound = 800;                                        // Tone of buzzer that will be played
          tone(Buzzer, sound);                                // Execute buzzer sound
          delay(1000);                                        // Delay 1s for tone sound
          noTone(Buzzer);                                     // Turn sound off
          delay(10000);                                       // Delay for 10s to let swimmer get away from the wall
          lapDelay = false;                                   // Allow the next lap to be counted 10s later
      }
      
      else{                                                   // If not in range, then display the distance for now
        lcd.setCursor(0,0);
        lcd.print("Distance:");
        lcd.setCursor(0,1);
        lcd.print(Distance);
        lcd.print("cm");
      }
      delay(100);                         //Delay between readings
   }
   
} //End Of Loop Function

