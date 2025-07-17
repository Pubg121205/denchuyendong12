#define PIR_PIN 2      // Cảm biến PIR
#define RELAY_PIN 3    // Relay

bool relayState = false;
bool motionDetected = false;

unsigned long motionStartTime = 0;
unsigned long noMotionStartTime = 0;
unsigned long lastCheckTime = 0;

const unsigned long RELAY_ON_DURATION = 3000;   // 3 giây bật relay
const unsigned long RELAY_OFF_DELAY = 500;      // 500ms sau không có chuyển động thì tắt

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  unsigned long currentTime = millis();

  // Kiểm tra trạng thái cảm biến
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    if (!motionDetected) {
      Serial.println("📡 Phát hiện chuyển động");
      motionDetected = true;
      motionStartTime = currentTime;
    }

    // Bật relay nếu chưa bật
    if (!relayState) {
      digitalWrite(RELAY_PIN, HIGH);
      relayState = true;
      Serial.println("⚡ Relay BẬT (3 giây)");
    }
  } else {
    if (motionDetected) {
      motionDetected = false;
      noMotionStartTime = currentTime;
    }

    // Đủ 3 giây bật + 500ms không có chuyển động thì tắt
    if (relayState &&
        currentTime - motionStartTime >= RELAY_ON_DURATION &&
        currentTime - noMotionStartTime >= RELAY_OFF_DELAY) {
      digitalWrite(RELAY_PIN, LOW);
      relayState = false;
      Serial.println("❌ Tắt Relay");
    }
  }

  // Không có delay, xử lý liên tục
}
