### Required components
- ESP32 Dev Module
- 2 potentiometers
- 1 button
- Contact plate
- Connection wires


### Connection diagram


#### Potentiometers
1 **Potentiometer 1**:
- Connect the middle pin of the potentiometer to GPIO36 (ADC1_CH0) of the ESP32.
- Connect one of the other potentiometer pins to 3.3V.
- Connect the other of the remaining potentiometer pins to GND.


2 **Potentiometer 2**:
- Connect the middle pin of the potentiometer to GPIO39 (ADC1_CH3) of the ESP32.
- Connect one of the other potentiometer pins to 3.3V.
- Connect the other of the remaining potentiometer pins to GND.


#### Button
1. connect one pin of the button to GPIO15 of the ESP32.
2. connect the other button pin to GND.
3. use the built-in pull-up resistor in your code, setting the GPIO15 pin as `INPUT_PULLUP`.
