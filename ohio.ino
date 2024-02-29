#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>
#include <Buzzer.h>

int MQ5 = A0;       // กำหนดหมายเลขขาแอนะล็อก A0 ให้กับตัวแปร MQ5
int buzzerPin = D4; // ขา Buzzer

Buzzer buzzer(buzzerPin); // ประกาศอ็อบเจ็กต์ของ Buzzer ในระดับทั่วไป

float Rs;
unsigned long lastNotificationTime = 0; // เวลาล่าสุดที่ส่งการแจ้งเตือนไปยัง LINE

void setup() {
  Serial.begin(9600); // เปิดการสื่อสารทาง Serial
  buzzer.begin(0);
  connectWiFi(); // เชื่อมต่อ WiFi
}

void loop() {
  unsigned long currentTime = millis(); // เวลาปัจจุบันใน millis()

  if (currentTime - lastNotificationTime >= 10000) { // 10,000 millis = 10 วินาที
    lastNotificationTime = currentTime;               // อัปเดตเวลาล่าสุดที่ส่งการแจ้งเตือน
    int sensorValue = analogRead(MQ5);
    Serial.print("ค่า adc : ");
    Serial.print(sensorValue);
    Serial.print("\t");

    float lpg_concentration = analysis(sensorValue);

    Serial.print("ความเข้มข้นของแอลพีจี: ");
    Serial.print(lpg_concentration, 3);
    Serial.println(" ppm");

    float THRESHOLD = 100.000; // ppm

    if (lpg_concentration > THRESHOLD) {
      // ความ-concentration เกินค่าทศนิยม, ส่งการแจ้งเตือน LINE
      String message = "ตรวจพบก๊าซ! ความเข้มข้นของแอลพีจี: " + String(lpg_concentration) + " ppm";
      sendLineNotification(message);

      // ทำให้ Buzzer ดัง
      buzzer.distortion(NOTE_B3, NOTE_D4);
      delay(2000); // รอ 2 วินาที
      buzzer.end(0); // ปิด Buzzer
    } else {
      // ไม่พบก๊าซ, ส่งการแจ้งเตือน LINE
      String noGasMessage = "MQ-5 ไม่พบก๊าซ."
                            " ค่า adc: " + String(sensorValue) +
                            " ความเข้มข้นของแอลพีจี: " + String(lpg_concentration, 3) + " ppm" +
                            " ความต้านทานของเซ็นเซอร์: " + String(Rs);
      sendLineNotification(noGasMessage);

      // ตรวจสอบว่า Buzzer ปิด
      buzzer.end(0);
    }

    Serial.print("ความต้านทานของเซ็นเซอร์ : ");
    Serial.println(Rs);
  }

  delay(100); // รอสักครู่เพื่อลดการใช้งาน CPU
}

void sendLineNotification(String message) {
  Serial.println(message);
  LINE.setToken(""); // แทนที่ด้วย Token LINE Notify ของคุณ
  LINE.notify(message);
}

void connectWiFi() {
  // เชื่อมต่อ WiFi
  WiFi.begin("Name", "Password"); // แทนที่ด้วยชื่อและรหัส WiFi ของคุณ
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("กำลังเชื่อมต่อ WiFi...");
  }
  Serial.println("เชื่อมต่อ WiFi แล้ว");
}

float analysis(int adc) {
  float slope = -0.347708839;
  float A = 4.41760121;
  float Rseries = 1000;
  float V_Rseries = ((float)adc * 5) / 1023;
  Rs = ((5 - V_Rseries) / V_Rseries) * Rseries;
  float R0 = 1809.52;
  /*
   * R0 คือ ค่าความต้านทานเซ็นเซอร์ขณะที่แก๊ส H2 เท่ากับ 1000 ppm
   * ในอากาศที่สะอาด
   * ควรปรับจูนด้วยการสอบเทียบกับเซ็นเซอร์มาตรฐาน
   */
  float Y = Rs / R0;
  float LPG_gas = pow(10, (log10(Y / A) / slope));
  return LPG_gas;
}