# Distance Measuring System – ATmega32 📏
👋 Hello, everyone!

This repository contains a mini project created as part of my Embedded Software Basics Diploma at EDGES for Training.

📝 Project Summary
This project is a simple distance measuring system that uses an ultrasonic sensor to calculate the distance between the sensor and an object. The measured value is then displayed on an LCD screen in real-time.

🎯 The objective was to apply embedded system principles to develop a functional and responsive measurement tool.

🧰 Technologies & Components Used
Component/Feature	Description
🧠 MCU	ATmega32 Microcontroller
📡 Sensor	Ultrasonic Sensor (HC-SR04 or equivalent)
📺 Display	LCD Display (for real-time distance output)
⚙️ Peripherals Used	GPIO, ICU (Input Capture Unit), LCD Interface

⚙️ How It Works
📡 The ultrasonic sensor sends a sound pulse and waits for the echo.

⏱️ The ICU measures the time it takes for the echo to return.

📐 The distance is calculated using the speed of sound formula.

📺 The result is displayed on the LCD screen in centimeters.

🧪 This real-time feedback system is ideal for basic range detection and proximity-based applications.

📄 Project Documentation
📎 You can find the full requirements and technical details in the attached documentation file.

✅ Project Status
🎉 Completed
All core functionalities have been implemented, tested, and verified on hardware.
