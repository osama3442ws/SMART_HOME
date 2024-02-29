
The Smart Home System utilizes two AVR microcontrollers and employs USART and I2C communication protocols. Its components include two Atmega32 microcontrollers, EEPROM, two LCD16x2 displays, keypad, servo motor, buzzer, LM35 temperature sensor, two DC motors, MPX4115 smoke sensor, LEDs, and LDR sensor.

The software architecture is structured into four layers:

Application Layer (APP) consists of main.c
Library Layer (LIB) includes STD_TYPES.h and BIT_MATH.h
Hardware Abstraction Layer (HAL) encompasses LED, LCD, keypad (KPAD), EEPROM, and servo motor (SERVO_MOTOR)
Microcontroller Abstraction Layer (MCAL) covers DIO, ACD, GIE, timers, PWM, USART, and I2C functionalities.
The system is divided into two main parts:

User Interface:

Handles interaction with users, prompting for username and password. Users have three attempts to enter the correct password. Upon successful authentication, the system initializes and opens the door using the servo motor. An incorrect password triggers an alarm and system shutdown.
Control Home:

Offers two modes of operation:

Manual Mode:
Users control home systems via the master AVR, selecting devices and toggling them on/off using the keypad. Commands are sent to the slave AVR for execution, and the status of home systems is displayed on the LCD.
Automatic Mode:
Home systems are autonomously controlled by the AVR based on sensor readings:
Air conditioner: Adjusts based on temperature sensor readings. If the temperature exceeds 25°C, the system activates the air conditioner and closes the window using the servo motor at 0° angle. Conversely, if the temperature is below 25°C, the system deactivates the air conditioner and opens the window at a 180° angle.
Light: Regulates based on LDR sensor readings. Lights turn off when sunlight is high and turn on when sunlight is low and We have the ability to control the lighting of each room individually
Water Pump: Activated by smoke sensor readings exceeding 50, indicating a fire alarm. The water pump and buzzer are activated, and "Fire!!" is displayed on the LCD.
TV: Automatically turns on when a user enters the home through the door and turns off when the user leaves.
Edit Password:

Allows users to change their passwords. The master AVR handles this by storing user passwords in external EEPROM memory using the I2C communication protocol, ensuring password retention during power loss.
