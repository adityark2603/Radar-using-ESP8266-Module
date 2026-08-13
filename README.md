# Radar-using-ESP8266-Module
A simple radar-style object detection system built using an ESP8266 NodeMCU, HC-SR04 ultrasonic sensor, and a servo motor.

This project demonstrates how an ESP8266 can be used to build a low-cost radar-style sensing system.

The ultrasonic sensor is mounted on a servo motor and continuously rotates through a specified angle. At each position, the sensor measures the distance to an object. The ESP8266 collects these measurements and sends the data through serial communication for visualization.

⚠️ <ins>NOTE</ins>: This is an ultrasonic object-detection system, not a true electromagnetic radar.

### <ins>Components Required</ins>:
| Component | Quantity |
| :--- | :---: |
| ESP8266 NodeMCU | 1 |
| HC-SR04 Ultrasonic Sensor | 1 |
| SG90 Servo Motor | 1 |
| Jumper Wires | Several |
| Breadboard | 1 |
| 5V Power Supply | 1 |

### <ins>Wiring Diagram</ins>:
<img width="2043" height="1512" alt="Wifi_Radar_Circuit_Diagram" src="https://github.com/user-attachments/assets/f82f1980-88ac-4cca-8cb0-c6e951124701" />

### <ins>Working Principle</ins>:
The system works in four main stages: <br>
1. The servo motor rotates the ultrasonic sensor to a specific angle.
2. The HC-SR04 sends an ultrasonic pulse.
3. The sensor receives the reflected echo from an object.
4. The ESP8266 calculates the distance using the echo time.

The approximate distance is calculated using:
```bash
Distance = (Time × Speed of Sound) / 2
```

The division by 2 is required because the ultrasonic pulse travels to the object and then returns to the sensor.
The ESP8266 then sends the angle and measured distance through the serial interface.

Example output:

Angle: 0°   Distance: 125 cm
Angle: 10°  Distance: 118 cm
Angle: 20°  Distance: 96 cm
Angle: 30°  Distance: 72 cm
Angle: 40°  Distance: 55 cm
