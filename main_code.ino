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

// 점멸 감지를 위한 변수
bool lastGreenState = false;
bool wasGreenBefore = false;

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

void resetBlinkDetection() {
  lastGreenState = false;
  wasGreenBefore = false;
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

  // 점멸 감지 로직
  if (state == GREEN) {
    if (greenSeen != lastGreenState && wasGreenBefore) {
      // 초록불 상태가 변경되었고, 이전에 초록불이 켜진 적이 있으면 점멸로 판단
      state = GREEN_BLINK;
      Serial.println("🟢→🟡  (GREEN→GREEN_BLINK)");
      playGreenToBlinkSound();
    }
    lastGreenState = greenSeen;
    if (greenSeen) {
      wasGreenBefore = true;
    }
  }

  switch (state) {
    case RED:
      if (greenSeen) {
        Serial.println("🔴→🟢  (RED→GREEN)");
        playRedToGreenSound();
        resetBlinkDetection();
        resetBlinkToneFlag();
        state = GREEN;
      }
      break;

    case GREEN:
      if (redSeen) {
        Serial.println("🟢→🔴  (GREEN→RED)");
        playGreenToRedSound();
        resetBlinkDetection();
        state = RED;
      }
      break;

    case GREEN_BLINK:
      if (redSeen) {
        Serial.println("🟡→🔴  (BLINK→RED)");
        playBlinkToRedSound();
        resetBlinkDetection();
        state = RED;
      }
      break;
  }

  delay(100);
}
