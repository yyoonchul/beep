#include <Pixy2.h>

Pixy2 pixy;

// 신호등 상태 정의
enum SignalState { RED, GREEN, GREEN_BLINK };
SignalState state = RED;

// 부저 핀
const int buzzerMain = 2;   // 빨간불, 점멸용
const int buzzerGreen = 3;  // 초록불 전용

// 사운드 톤
const int toneGreen = 1500;
const int toneRed = 1500;
const int toneBlink = 900;

// 사운드 유틸리티 설정
const int beepDurFast = 80;    // 한 삐 길이 (ms)
const int beepRepeatFast = 8;  // 반복 횟수
static bool blinkToneOn = false;

// 타이머
unsigned long greenStartTime = 0;
const unsigned long timeToBlink = 5000; // 초록불 → 점멸까지 5초

// 1회 경고음 (상태 전환용)
void playFastBeep(int pin, int toneHz) {
  tone(pin, toneHz);
  delay(1000);  // 1초 삐—
  noTone(pin);
}

// 빠른 반복음 (점멸 상태용)
void playBlinkToneOnce(int pin, int toneHz) {
  if (!blinkToneOn) {
    Serial.println("🟡 초록불 점멸음 (빠른 반복)");
    for (int i = 0; i < beepRepeatFast; i++) {
      tone(pin, toneHz);
      delay(beepDurFast);
      noTone(pin);
      delay(beepDurFast);
    }
    blinkToneOn = true;
  }
}

void resetBlinkToneFlag() {
  blinkToneOn = false;
}

void initializeSoundPins() {
  pinMode(buzzerMain, OUTPUT);
  pinMode(buzzerGreen, OUTPUT);
}

void playRedToGreenSound() {
  playFastBeep(buzzerGreen, toneGreen);
}

void playGreenToRedSound() {
  playFastBeep(buzzerMain, toneRed);
}

void playGreenToBlinkSound() {
  playBlinkToneOnce(buzzerMain, toneBlink);
}

void playBlinkToRedSound() {
  playFastBeep(buzzerMain, toneRed);
}

// 아두이노 초기화
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("=== Pixy2 State-Driven Buzzer (Max 1s each) ===");

  pixy.init();
  initializeSoundPins();
}

// 메인 루프
void loop() {
  int num = pixy.ccc.getBlocks(10, 255);
  bool redSeen = false;
  bool greenSeen = false;

  if (num > 0) {
    for (int i = 0; i < num; ++i) {
      uint16_t sig = pixy.ccc.blocks[i].m_signature;
      if (sig == 1 || sig == 2 || sig == 3) greenSeen = true;
      else if (sig == 4 || sig == 5 || sig == 6) redSeen = true;
    }
  }

  unsigned long now = millis();

  switch (state) {
    case RED:
      if (greenSeen) {
        Serial.println("🔴→🟢  (RED→GREEN)");
        playRedToGreenSound();
        greenStartTime = now;
        resetBlinkToneFlag();
        state = GREEN;
      }
      break;

    case GREEN:
      if (redSeen) {
        Serial.println("🟢→🔴  (GREEN→RED)");
        playGreenToRedSound();
        state = RED;
      } else if (now - greenStartTime >= timeToBlink) {
        Serial.println("🟢→🟡  (GREEN→GREEN_BLINK)");
        playGreenToBlinkSound();
        state = GREEN_BLINK;
      }
      break;

    case GREEN_BLINK:
      if (redSeen) {
        Serial.println("🟡→🔴  (BLINK→RED)");
        playBlinkToRedSound();  // 점멸에서 빨간불로 전환 시 소리 추가
        state = RED;
      }
      break;
  }

  delay(100);
}
