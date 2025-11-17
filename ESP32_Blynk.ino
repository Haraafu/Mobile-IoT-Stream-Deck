#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char* WIFI_SSID = "";
const char* WIFI_PASS = "";

#define LED_PIN 2

#define V_SPOT   V0
#define V_YT     V1
#define V_STATUS V4

enum MsgType : uint8_t { MSG_LED_BLINK, MSG_STATUS };
struct Msg {
  MsgType type;
  int i1;
  char* text;
};
QueueHandle_t qMain;

void enqueueStatus(const String& s) {
  Msg m{};
  m.type = MSG_STATUS;
  m.text = (char*)malloc(s.length() + 1);
  if (m.text) {
    strcpy(m.text, s.c_str());
    xQueueSend(qMain, &m, 0);
  }
}

void TaskActuator(void* pv) {
  pinMode(LED_PIN, OUTPUT);
  Msg msg;
  for (;;) {
    if (xQueueReceive(qMain, &msg, portMAX_DELAY) == pdTRUE) {
      if (msg.type == MSG_LED_BLINK) {
        for (int i = 0; i < msg.i1; i++) {
          digitalWrite(LED_PIN, HIGH);
          vTaskDelay(pdMS_TO_TICKS(120));
          digitalWrite(LED_PIN, LOW);
          vTaskDelay(pdMS_TO_TICKS(120));
        }
      } else if (msg.type == MSG_STATUS) {
        if (msg.text) {
          Blynk.virtualWrite(V_STATUS, msg.text);
          Serial.println(msg.text);
          free(msg.text);
        }
      }
    }
  }
}

BLYNK_WRITE(V_SPOT) {
  if (param.asInt() == 1) {
    Msg m{MSG_LED_BLINK, 2, nullptr};
    xQueueSend(qMain, &m, 0);
    enqueueStatus("Tombol Spotify ditekan");
  }
}
BLYNK_WRITE(V_YT) {
  if (param.asInt() == 1) {
    Msg m{MSG_LED_BLINK, 3, nullptr};
    xQueueSend(qMain, &m, 0);
    enqueueStatus("Tombol YouTube ditekan");
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  qMain = xQueueCreate(10, sizeof(Msg));

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Menghubungkan WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println(" tersambung!");

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  enqueueStatus("ESP32 siap menerima perintah");

  xTaskCreatePinnedToCore(TaskActuator, "Actuator", 4096, nullptr, 3, nullptr, APP_CPU_NUM);
}

void loop() { 
  Blynk.run(); 
}