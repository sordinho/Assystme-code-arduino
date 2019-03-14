# Assystme-code-arduino

This is the code running on Arduino for Sensor and and digital-pin Communication

[Assystme-code](https://github.com/AmI-2018/Assystme-code)

## Interrupt pin (from Arduino toRaspberry)
Arduino Pin | Purpose
------- | -------
2| Faucet Interrupt
3| Gas Interrupt
## Used sensors:
#### Gas: MQ-2 Module Sensor
###### Pin Connection:
Sensor | Arduino
------- | --------
 A0 | A0
 D0 | Not Used
 Vcc | 5V
 GND | GND
#### Faucet: Ultrasonic sensor (HCSR04) to detect water flow
###### Pin Connection:
Sensor | Arduino
------- | --------
Vcc | 5V
Echo | 10
Trig | 9
GND | GND
