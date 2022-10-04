# Ready to use Dustation web monitoring & WiFi management system

ระบบ Web Monitoring & WiFi management สำหรับ Dustation Dev Kit V.2 ใช้ในการแสดงผลค่าต่างที่วัดได้ และการตั้งค่าต่างๆ ผ่านเว็บบราวเซอร์

## ฟีเจอร์การแสดงผล
- แสดงผลค่าฝุ่นละอองขนาดเล็ก PM1.0 , PM2.5 และ PM10
- แสดงผลค่าอุณหภูมิ และความชื้น
- แสดงสถานะการควบคุมสั่งงานผ่าน RELAY
- แสดงสถานะเงื่อนไขสั่งงานที่ได้ตั้งค่าไว้ล่วงหน้า

## ฟีเจอร์การตั้งค่า
- ตั้งค่าการเชื่อมต่อ WiFi
- ตั้งค่าเงื่อนไขจุดสั่งงาน
- ตั้งค่าชื่อและไอดีอุปกรณ์
- การตั้งค่าผ่าน WiFi AP Mode

## Compatible Hardware
บอร์ดพัฒนา Dustation Dev Kit V.2 [สั่งซื้อ]

## Hardware setup
- บนบอร์ด Dustation Dev Kit V.2 เลื่อน dip switch ของ SET ไปที่ ON (Set mode)
- ติดตั้งอุปกรณ์ทั้งหมดให้ถูกต้องครบถ้วนได้แก่
1. ESP32
2. PMS7003
3. AM2302
4. OLED


## Software setup
1. ดาวน์โหลดและติดตั้ง Arduino IDE https://www.arduino.cc/en/software
2. เพิ่มบอร์ด ESP32 board package สำหรับ Arduino IDE
- เข้าเมนู File > Preferences, ใส่ค่าใน "Additional Boards Manager URLs" ด้วย: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
- เข้าเมนู Tools > Board > Boards Manager..., พิมพ์ "esp32" ในช่องค้นหา, เลือกติดตั้งบอร์ด ****esp32**** รุ่นล่าสุด หากไม่เจอให้รีสาตาร์ทโปรแกรม Arduino IDE
- เข้าเมนู Tools -> Manage Libraries ติดตั้ง Library ต่อไปนี้
1. DHT sensor library by Adafruit Version 1.4.4 https://github.com/adafruit/DHT-sensor-library
2. PMS Library by Mariusz Kacki Version 1.1.0 https://github.com/fu-hsi/pms
3. U8g2lib by oliver Version 2.33.15 https://github.com/olikraus/u8g2
4. ezButton by ArduinoGetStarted.com Version 1.0.4 https://arduinogetstarted.com/tutorials/arduino-button-library

## Flash โปรแกรมลง ESP32
1. เข้าเมนู Tools เลือกบอร์ด DOIT ESP32 DEVKIT V1
2. เลือกพอร์ต
3. ดาวน์โหลดโค๊ดโปรแกรม ready2u-dustation-web-man-main.zip
4. เปิดไฟล์ readyu2-dustation-web-man.ino
5. อัพโหลดไฟล์


## การใช้งานตั้งค่า 'SET MODE' 
- กด Switch ค้างไว้… แล้ว กด EN หรือ reset บน ESP32 1 ครั้ง จากนั้นรอ 2 วินาที จึงปล่อยปุ่ม Switch บน OLED Display จะแสดงผล IP address , SSID และ Password สำหรับการเข้าถึงการตั้งค่าผ่าน WiFi

SET MODE ประกอบไปด้วย

1. ส่วนของการตั้งค่า WiFi Router เชื่อมต่อ WiFi เข้ากับ Router ที่บ้านหรือสำนักงาน
2. ส่วนของการตั้งค่า Set Max Values เงื่อนไขในการตั้งค่าจุดควบคุมสั่งงานผ่าน Relay
3. ส่วนของการตั้งค่า System Setting ใช้สำหรับ ตั้งค่าชื่อและไอดีอุปกรณ์
4. ส่วนของการตั้งค่า WiFi Access Point [AP Mode] เพื่อใช้ในการเข้าถึง/การจัดการ Dustation Dev Kit V.2

## Factory Reset เพื่อกลับสู่ค่าเริ่มต้น 'RESET MODE'

- กด Switch ค้างไว้… แล้ว กด EN หรือ reset บน ESP32 1 ครั้ง จากนั้นรอ 10 วินาที จึงปล่อยปุ่ม Switch บน OLED Display จะแสดงผล Factory Resettings
- จากนั้นกด กด EN หรือ reset บน ESP32 อีก 1 ครั้ง โปรแกรมจะเข้าสู่ค่าเริ่มต้น