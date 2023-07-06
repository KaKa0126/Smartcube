void setup() {
    // 시리얼 통신 설정
    Serial.begin(9600);
}

void loop() {
    // A0를 핀을 통한 아날로그 값 받기 (0~1023 사이 정수)
    int sensorValue = analogRead(A8);

    // 0~1023 사이 정수 값을 (0~5V의 전압 값으로 다시 변환
    float voltage = sensorValue * (5.0 / 1023.0);
  
    // 아두이노 IDE 시리얼 모니터로 출력
    Serial.println(voltage);
}
