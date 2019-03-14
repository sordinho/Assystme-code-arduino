/* 
 * Filename: main.cpp
 * File Created: Tuesday, May 22nd 2018, 18:25:05 
 * ------
 * Author: Davide Sordi, Daniele Rizzieri, Martina Cane
 * GitHub: @AmI-2018
 * ------
 * Last Modified: Thursday, June 1st 2018, 18:59:00
 * ------
 * This is the arduino sketch for detecting gas leak and water flow. 
 * In case of danger arduino will send a warning using pins 2 and 3.
 */

#include <Arduino.h>

//Integrated led on Arduino board
const int led = 13;
// Interrupt pin for communcating problem to raspberry
const int interruptFaucet = 2;
const int interruptGas = 3;

// Gas Sensor
const int gasSensor = A0;
const int gasThreshold = 200; // This is the gas threshold for the sensor: if above we have a leak

//Faucet Sensor (Ultrasonic sensor HCSR04)
const int trigPin = 9;
const int echoPin = 10;
const int flowThreshold = 5;

// Define variables
long duration;            // Used to measure the travel time of ultrasonic wave
int distance;             // Used to get the distance (in our case we only need to know if we detect something near)
int gasConcentration = 0; // Used to get the gas concentration in the air

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
            Serial.print("Faucet\n");
            // ok maybe we have a problem
            digitalWrite(interruptFaucet, HIGH); // turn HIGH interrupt pin
            delay(2000);                         // wait 2 seconds for rasp interrupt management
            digitalWrite(interruptFaucet, LOW);  //restore at low faucet interrupt pin
        }
    }
    Serial.println("monitoring");
    delay(2000);

    // check for a gas leak
    if (checkGas())
    {
        Serial.print("Gas\n");
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
    Serial.println(distance);
    
    // Distance measured above 5 cm ---> water flow
    if (distance < flowThreshold)
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

/* Function to check gas presence in the environment
 * Return true if the gas presence is over the treshold for more than 2 secodns
*/
bool checkGas()
{
    // Read the analog value from sensor
    gasConcentration = analogRead(gasSensor);
    if (gasConcentration > gasThreshold)
    {
        //Probably we have a leak
        int total = 0;
        // Get an average of 10 measurements in 2 seconds
        for (int i = 0; i < 10; i++)
        {
            total += analogRead(gasSensor);
            delay(200);
        }
        total = total / 10;
        // If average above the threshold we think we have a leak
        if (total > gasThreshold)
        {
            //Gas leak
            Serial.println(total);
            Serial.println("  Gas Leak detected");
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
