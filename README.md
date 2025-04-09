# IOT-and-Web-based-Ambulance-Route-Clearance-and-Location-Tracking-System-Using-Raspberry-Pi-Pico-W
A real-time ambulance tracking and route clearance system using Raspberry Pi Pico W, GPS, GSM, and Wi-Fi. Sends live location to web servers and SMS alerts to authorities for faster response
Overview
This project enables emergency tracking and route clearance for ambulances using GPS and GSM modules integrated with a Raspberry Pi Pico W.

It includes:
- Real-time GPS location tracking.
- Uploading location data to a custom server and ThingSpeak IoT platform.
- Sending SMS alerts with location URLs to traffic officers or hospitals.
- LCD display feedback for status and selections.
- Hardware switch control for route options
  
 Hardware Components
- Raspberry Pi Pico W
- TinyGPS module (connected to Serial2)
- GSM module (connected to Serial1)
- I2C 16x2 LCD
- Two push buttons (connected to GPIO 18 and 19)
- Wi-Fi connection
  
 How It Works
Startup Sequence
1. Connects to the configured Wi-Fi network (`ambulance`).
2. Initializes GPS and GSM modules.
3. Displays initial messages on the LCD.

 Location Tracking
- Continuously reads GPS coordinates via TinyGPS.
- Displays latitude and longitude on LCD.
- Sends coordinates to both:
  - Custom web server
  - ThingSpeak IoT platform

 Route Alert System
- If **Button 1** is pressed, route 1 is selected:
  - Sends SMS to traffic officer #1 with live tracking URL.
- If **Button 2** is pressed, route 2 is selected:
  - Sends SMS to contact #2.
