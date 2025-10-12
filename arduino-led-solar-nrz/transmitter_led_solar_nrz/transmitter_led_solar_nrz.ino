// ARDUINO LIFI TRANSMITTER

#define LED_PIN 9
#define SAMPLING_TIME 4   // ms per bit

String text = "";          // Default message is empty

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);   // Idle state = HIGH
}

void loop() {
  if (Serial.available()) {
    text = Serial.readString();
    Serial.print(text);
  }

  if (text.length() > 0) {
    for (int i = 0; i < text.length(); i++) {
      transmit_byte(text[i]);
    }
    text = "";
    delay(1000);
  }
}

void transmit_byte(char data_byte) {
  digitalWrite(LED_PIN, LOW);
  delay(SAMPLING_TIME);

  for (int i = 0; i < 8; i++) {
    digitalWrite(LED_PIN, (data_byte >> i) & 0x01);
    delay(SAMPLING_TIME);
  }

  digitalWrite(LED_PIN, HIGH);
  delay(SAMPLING_TIME);
}
