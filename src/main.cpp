/* 
 * Filename: main.cpp
 * File Created: Tuesday, May 22nd 2018, 18:25:05 
 * ------
 * Author: Davide Sordi, Daniele Rizzieri, Martina Cane
 * GitHub: @AmI-2018
 * ------
 * Last Modified: Thursday, May 24th 2018, 16:52:27 
 * ------
 * This is the arduino sketch for detecting gas leak and water flow. 
 * In case of danger arduino will send a warning using pins 2 and 3
 */

#include <Arduino.h>

//Integrated led on Arduino board
const int led = 13;
// Interrupt pin for communcating problem to raspberry
const int interruptFaucet = 2;
const int interruptGas = 3;

// Gas Sensor

//Faucet Sensor (Ultrasonic sensor HCSR04)
const int trigPin = 9;
const int echoPin = 10;

// Define variables
long duration; // Used to measure the travel time of ultrasonic wave
int distance;  // Used to get the distance (in our case we only need to know if we detect something near)

// Function prototypes
bool checkFlow();
bool checkGas();

void setup()
{
    // Arduino Setup function
    pinMode(trigPin, OUTPUT);         // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);          // Sets the echoPin as an Input
    pinMode(led, OUTPUT);             //Sets the integrated led as Output
    pinMode(interruptFaucet, OUTPUT); // Sets the interrupt faucet pin as Output
    pinMode(interruptGas, OUTPUT);    // Sets the interrupt gas pin as Output
    Serial.begin(9600);               // Starts the serial communication
}

void loop()
{
    // check if faucet left open
    if (checkFlow())
    {
        //We detect open water
        delay(5000); // 5 seconds delay to try to detect if water is still open
        if (checkFlow())
        {
            // ok maybe we have a problem
            digitalWrite(interruptFaucet, HIGH); // turn HIGH interrupt pin
            delay(2000);                         // wait 2 seconds for rasp interrupt management
            digitalWrite(interruptFaucet, LOW);  //restore at low faucet interrupt pin
        }
    }

    // check for a gas leak
    if (checkGas())
    {
        // Gas leak detected
        digitalWrite(interruptGas, HIGH); // turn HIGH interrupt pin
        delay(2000);
        digitalWrite(interruptGas, LOW); // restore
    }
}

/* Function to detect water flow
 * Return True if water is detect, false if not
*/
bool checkFlow()
{

    // Clears the trigPin
    digitalWrite(trigPin, LOW);

    delayMicroseconds(5);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads from the echoPin the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance
    distance = duration * 0.034 / 2;

    // Distance measured above 5 cm ---> water flow
    if (distance < 5)
    {
        digitalWrite(led, HIGH);
        return true;
    }
    else
    {
        digitalWrite(led, LOW);
        return false;
    }
}

bool checkGas()
{

    // todo
    return false;
}