#include <Servo.h>
#include <Wire.h> // <-- remove spaces

Servo servo_2;

// Define Trig and Echo pin:
#define trigPin 8
#define echoPin 7
long duration;
int distance;
unsigned long previousMillisDoor = 0;
const long intervalDoor = 3000; // interval at which to blink (milliseconds)
unsigned long currentMillisDoor;

#define LDR 0 // the cell and 10K pulldown are connected to a0
#define PIR 5
#define bulb 4
int LDR_Reading; // the analog reading from the sensor divider
int PIR_Reading; // variable for reading the pin status of PIR
unsigned long previousMillisBulb = 0;
const long intervalBulb = 60000; // interval at which to blink (milliseconds)
unsigned long currentMillisBulb;

#define FAN 6
#define TEMP 1
int tempMin = 30;
int fanSpeed;
double temperature; // variable for storing the analog readong from the Temperature sensor
int tempSend = 0;

int automaticMode = 1; //variable to make it possible to read the state, if we gonna be in the automatic mode or not
int fanStateHere = 0;      //Blue led will turn on to alarm about the situation of the servo motor(Door)
int doorStateHere = 0;         //Yellow led will turn on to alarm about the situation of the Bulb
int bulbStateHere = 0;             //Green led will turn on to alarm about the situation of the DC motor(FAN)

int fanState = 0;
int doorState = 0;
int bulbState = 0;
//*************************************************************
void setup()
{
    // Define inputs and outputs:
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    servo_2.attach(2, 500, 2500); //servo motor

    pinMode(LDR, INPUT);
    pinMode(PIR, INPUT);
    pinMode(bulb, OUTPUT);

    pinMode(TEMP, INPUT);
    pinMode(FAN, OUTPUT);

    servo_2.write(0);
    digitalWrite(bulb, LOW);

    /// For sending
    Wire.begin(8);                // join i2c bus with address #2
    Wire.onRequest(requestEvent); // register event

    //Begin Serial communication at a baudrate of 9600:
    Serial.begin(9600);
}
//*************************************************************
void requestEvent()
{
    Wire.write(doorStateHere);
    Wire.write(bulbStateHere);
    Wire.write(fanStateHere);
    Wire.write(distance);
    Wire.write(tempSend);
    Wire.write(LDR_Reading);
    Wire.write(PIR_Reading);
}
//*************************************************************
void loop()
{
    //---------------------------------------------------------------------------
    ////----------- readUltrasonicDistance -----------////
    //---------------------------------------------------------------------------

    currentMillisDoor = millis();
    if (currentMillisDoor - previousMillisDoor >= intervalDoor)
    {

        // save the last of the blink
        previousMillisDoor = currentMillisDoor;

        // Clear the trigPin by setting it LOW:
        digitalWrite(trigPin, LOW);
        // Trigger the sensor by setting the trigPin high for 3 seconds:
        digitalWrite(trigPin, HIGH);
        digitalWrite(trigPin, LOW);
        // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
        duration = pulseIn(echoPin, HIGH);
        // Calculate the distance:
        distance = duration * 0.01723;
        Serial.print("Distance = ");
        Serial.print(distance);
        Serial.print(" cm ");

        if (automaticMode == 0)
        {   /// Auto mode
            // Print the distance on the Serial Monitor (Ctrl+Shift+M):
            if (distance <= 50)
            {
                servo_2.write(90);
                Serial.println(";  || Door Open!");
                doorStateHere = 90 ;
            }
            else
            {
                servo_2.write(0);
                Serial.println("; || Door Closed!");
                doorStateHere = 0 ;
            }
        }
        else
        { ///// send alarm to arduino #2 about the door
            servo_2.write(doorState);
        }
    }

    //---------------------------------------------------------------------------
    ////----------- LDR (to detect Light) and PIR (to detect movement) -----------////
    //---------------------------------------------------------------------------

    currentMillisBulb = millis();
    if (currentMillisBulb - previousMillisBulb >= intervalBulb)
    {
        // save the last of the blink
        previousMillisBulb = currentMillisBulb;

        LDR_Reading = analogRead(LDR);
        Serial.print("LDR reading = ");
        Serial.print(LDR_Reading); // the raw analog reading

        PIR_Reading = digitalRead(PIR);
        Serial.print(" ; PIR reading = ");
        Serial.print(PIR_Reading); // the raw digital reading

        if (automaticMode == 0)
        { /// Auto mode
            if (LDR_Reading < 500 && PIR_Reading == HIGH)
            {
                digitalWrite(bulb, HIGH);
                Serial.print(" ; Dark ; Motion detected!");
                Serial.println(" ; Bulb ON");
                bulbStateHere = HIGH ;
            }
            else
            {
                digitalWrite(bulb, LOW);
                Serial.print(" ; No need for Light");
                Serial.println(" ; Bulb OFF");
                bulbStateHere = LOW ;
            }
        }
        else
        { //sending alarm
            digitalWrite(bulb, bulbState);
        }
    }

    //---------------------------------------------------------------------------
    ////----------- Temperature Sensor and fan (DC Motor) -----------////
    //---------------------------------------------------------------------------

    temperature = analogRead(TEMP);
    temperature = (temperature * 5) / 1024;
    temperature = (temperature - 0.5) * 100;
    tempSend = temperature;
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.print(" degree ");

    if (automaticMode == 0)
    { /// Auto mode
        if (temperature > tempMin)
        {
            // the actual speed of fan//map(temp, tempMin, tempMax, 32, 255);
            fanSpeed = 1.7 * temperature;
            analogWrite(FAN, fanSpeed);
            Serial.print(" ; Fan speed = ");
            Serial.println(fanSpeed);
            Serial.print("\n");
            fanStateHere = fanSpeed ;
        }
        else
        {
            fanSpeed = 0;
            analogWrite(FAN, fanSpeed);
            Serial.print(" ; Fan speed = ");
            Serial.println(fanSpeed);
            Serial.print("\n");
            fanStateHere = fanSpeed ;
        }
    }
    else
    { //sending alarm
        analogWrite(FAN, fanState);
    }
    // receive the values
    Wire.requestFrom(9, 4);
    automaticMode = Wire.read(); // read one character from the I2C
    fanState = Wire.read();
    doorState = Wire.read();
    bulbState = Wire.read();

    delay(1000); // Delay a little bit to improve simulation performance
}