#include <Wire.h> // <-- remove spaces
#include <IRremote.h>

int IR_Recv = 12;   //IR Receiver Pin 12
IRrecv irrecv(IR_Recv);
decode_results results;

#define blueLED 9
#define yellowLED 10
#define greenLED 11

int arduino_1_Address = 9;
int switchValue = 1;
int doorOldState = 0;
int bulbOldState = 0;
int fanOldState = 0;

unsigned long previousMillisDoor = 0; 
const long intervalDoor = 3000;  // interval at which to blink (milliseconds)
unsigned long currentMillisDoor;

unsigned long previousMillisBulb = 0; 
const long intervalBulb = 60000;  // interval at which to blink (milliseconds)
unsigned long currentMillisBulb;

int fanState = 0;
int doorState = 0;
int bulbState = 0;

int arduino_2Pin = 6;
int tempMin = 30 ;
int distance ;
int temperature ;
int LDR_Reading ;
int PIR_Reading;

int distanceOld = 0;
int temperatureOld = 0;
int LDR_ReadingOld = 0;
int PIR_ReadingOld = 0;

//*************************************************************
void setup()
{
  pinMode(arduino_2Pin, INPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  Wire.begin(9); // join i2c bus 
  Wire.onRequest(requestEvent); // register event
  
  irrecv.enableIRIn(); // Starts the receiver
  
  //Begin Serial communication at a baudrate of 9600:
  Serial.begin(9600);
}
//*************************************************************
void requestEvent() {
  Wire.write(switchValue);
  Wire.write(fanState);
  Wire.write(doorState);
  Wire.write(bulbState);
}
//*************************************************************
void loop()
{ 
  //---------------------------------------------------------------------------  
    ////----------- Sending and Receving values -----------////
  //---------------------------------------------------------------------------   

  // read the switch value and send it:
  switchValue = digitalRead(arduino_2Pin);
  Wire.beginTransmission( arduino_1_Address ); // transmit to device #9
  Wire.write(switchValue); // sends switch value
  Wire.endTransmission(); // stop transmitting
  
  // receive the values 
  Wire.requestFrom(8, 7);
  doorOldState = Wire.read();// read one character from the I2C
  bulbOldState = Wire.read();
  fanOldState = Wire.read();
  distance = Wire.read();
  temperature = Wire.read();
  LDR_Reading = Wire.read();
  PIR_Reading = Wire.read();

  //********** Arduino #2 **********//
  // check the state of the switch. If it is HIGH, we are in Manual Mode:
  if (switchValue == HIGH ){ // Go to Arduino #2
    //decodes the infrared input
    if (irrecv.decode(&results)){
      long int decCode = results.value;
      //switch case to use the selected remote control button
      switch (results.value){
      case 0xFD08F7: //when you press the 1 button ( turn on the Fan )
          fanState = 1;
          Serial.println("Fan opened");
          if (fanState != fanOldState){
            analogWrite(greenLED, LOW);
          }
          break;
      case 0xFD8877: //when you press the 2 button ( turn off the fan)
          fanState = 0;  
          Serial.println("Fan closed");
          if (fanState != fanOldState) {
            analogWrite(greenLED, LOW);
          }
          break;
      case 0xFD48B7: //when you press the 3 button (door open)
          doorState = 90 ;
          Serial.println("door opened");
          if (doorState != doorOldState){
            analogWrite(blueLED, LOW);
          }
          break;
      case 0xFD28D7: //when you press the 4 button
          doorState = 0 ;
          Serial.println("door closed");
          if (doorState != doorOldState){
            analogWrite(blueLED, LOW);
          }
          break;
      case 0xFDA857: //when you press the 5 button (tuen the bulb on)
          bulbState = 1 ;
          Serial.println("Bulb opened");
          if (bulbState != bulbOldState){
            analogWrite(yellowLED, LOW);
          }
          break;
      case 0xFD6897: //when you press the 6 button
          bulbState = 0 ;
          Serial.println("Bulb closed");
          if (bulbState != bulbOldState){
            analogWrite(yellowLED, LOW);
          }
          break;
    }
   irrecv.resume(); // Receives the next value from the button you press
  }
    
    
    //---------------------------------------------------------------------------  
      ////----------- Arduino #2 Door -----------////
    //---------------------------------------------------------------------------   
    currentMillisDoor = millis();
    if (currentMillisDoor - previousMillisDoor >= intervalDoor) { 
      // save the last of the blink
      previousMillisDoor = currentMillisDoor;

      Serial.print("Distance = ");
      Serial.print(distance);
      Serial.print(" cm ");
      
      if(doorState != doorOldState || distance != distanceOld){
        analogWrite(blueLED , 350);
        if(distance <= 50){
          Serial.println(";  || The Door should be Opened!");
        }
        else{
          Serial.println(";  || The Door should be Closed!");
        }
        doorOldState = doorState ;
        distanceOld = distance ;
      }
      else{
        //analogWrite(blueLED , LOW);
        Serial.println("   NoChange in Distance");
      }    
      
    }
    //---------------------------------------------------------------------------  
      ////----------- Arduino #2 Bulb -----------////
    //---------------------------------------------------------------------------   
    currentMillisBulb = millis();
    if (currentMillisBulb - previousMillisBulb >= intervalBulb) {
      // save the last of the blink
      previousMillisBulb = currentMillisBulb ;
      
      Serial.print("LDR reading = ");
      Serial.print(LDR_Reading); 
      Serial.print(" ; PIR reading = ");
      Serial.print(PIR_Reading);
        
      if(bulbState != bulbOldState || ((LDR_Reading != LDR_ReadingOld)||(PIR_Reading != PIR_ReadingOld) )){
          analogWrite(yellowLED, 500);
          if (LDR_Reading < 500 && PIR_Reading == HIGH) {
            Serial.print(" ; Dark ; Motion detected!");
            Serial.println(" ; Bulb should be turned ON");
          } 
          else	{
            Serial.print(" ; No need for Light");
            Serial.println(" ; Bulb should be turned OFF");
          }
          bulbOldState = bulbState ;
          LDR_ReadingOld = LDR_Reading ;
          PIR_ReadingOld = PIR_Reading ;
        }
        else{
          //analogWrite(yellowLED, LOW);
          Serial.println("   NoChange in Motion and light");
        }
        
      
    } 
    //---------------------------------------------------------------------------  
      ////----------- Arduino #2 FAN -----------////
    //---------------------------------------------------------------------------   
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.print(" degree ");
      
    if(fanState!= fanOldState || temperature != temperatureOld){
      analogWrite(greenLED, 200);
      if(temperature > tempMin){
        Serial.print(" ; Fan speed should be increased\n");
      }
      else{
        Serial.print(" ; Fan speed should be zero\n");
      }
      fanOldState = fanState ;
      temperatureOld = temperature ;
    }
    else{
      //digitalWrite(greenLED, 0);
      Serial.println("   NoChange in temperature");
    }
    
  }
  //---------------------------------------------------------------------------  
    ////----------- Arduino #1 -----------////
  //---------------------------------------------------------------------------   
  else{  // Go to Arduino #1
    digitalWrite(blueLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    Serial.println("\n\n----------------------------------------------");
    Serial.println("Arduino #1 ;  Automatic Mode");
    Serial.println("----------------------------------------------\n\n");
  }
  delay(1000); // Delay a little bit to improve simulation performance 
}