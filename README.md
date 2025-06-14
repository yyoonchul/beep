# Pixy2 신호등 경고음 시스템

## 개요
Pixy2 카메라를 사용하여 신호등 색상을 감지하고, 상태 변화에 따라 적절한 경고음을 출력하는 Arduino 프로젝트입니다.

## 프로젝트명 의미
BEEP (Blind pedestrian Environmental Early Protection)
- 일상적 의미: 자동차, 경고음 등에서 익숙한 "삐- 소리"
- 시각장애인의 보행 환경을 조기에 보호하는 시스템이라는 의미를 담고 있으며, 친숙한 경고음의 이미지와 완벽하게 부합합니다.

## 하드웨어 요구사항
- Arduino Uno/Nano
- Pixy2 카메라
- 부저 2개
  - 메인 부저 (빨간불, 점멸용): 디지털 핀 2번
  - 보조 부저 (초록불 전용): 디지털 핀 3번

## 기능
1. 신호등 상태 감지
   - 초록불: 시그니처 1, 2, 3
   - 빨간불: 시그니처 4, 5, 6

2. 상태 전환 알림
   - 빨간불 → 초록불: 1초 경고음
   - 초록불 → 빨간불: 1초 경고음
   - 초록불 → 점멸: 빠른 반복음 (80ms × 8회)
   - 점멸 → 빨간불: 1초 경고음

3. 점멸 상태 감지
   - 초록불이 한 번이라도 꺼졌다 켜지면 즉시 점멸 상태로 인식
   - 실시간으로 신호등의 점멸을 감지하여 즉각적인 상태 전환

## 주요 특징
- 시리얼 모니터링 (115200 baud)
- 상태 전환 이모지 표시
- 초록불 점멸 시 빠른 반복음으로 긴급 상황 알림 