# Arduino LPG Gas Detector with Line Notification(เครื่องตรวจจับเเก๊สLPGเเจ้งเตือนผ่านไล)

![Project Image](project_image.jpg)

## Description
This project is aimed at creating an LPG gas detector using Arduino, capable of notifying users via Line when gas levels exceed predefined thresholds.

## Features
- Detects LPG gas in the environment
- Can set customizable notification thresholds
- Notifies users via Line through the Line Notify API

## Materials Used
- Arduino Uno
- MQ-5 Sensor
- Buzzer 100 db
- Jumper Wires
- Line Developer Account

## Installation and Usage
1. Install the ArduinoJson library via the Arduino IDE Library Manager.
2. Download the attached code.
3. Upload the code to the Arduino Uno board.
4. Connect the MQ-6 sensor to the Arduino as per the code.
5. Set your Line Notify Token and UserID in the code.
6. Upload the code to the Arduino board.
7. Start using the program.

## Setting up Line Notify
1. Generate a Line Notify Token from [Line Notify](https://notify-bot.line.me/th/).
2. Use the generated token in the code.

