// ARDUINO LIFI TRANSMITTER

#define LED_PIN 9
#define SAMPLING_TIME 2    // ms per bit

// 4B/5B Control Symbols
#define CTRL_IDLE  0b11111  // idle
#define CTRL_SOS1  0b11000  // Start of Stream 1
#define CTRL_SOS2  0b10001  // Start of Stream 2
#define CTRL_EOS1  0b01101  // End of Stream 1
#define CTRL_EOS2  0b00111  // End of Stream 2
#define CTRL_ERR   0b00100  // Transmission Error

String text = "";          // Default message is empty
bool currentLevel = HIGH;

// 4b5b table
const byte trans4b5b[16] = {
  0b11110, 0b01001, 0b10100, 0b10101,
  0b01010, 0b01011, 0b01110, 0b01111,
  0b10010, 0b10011, 0b10110, 0b10111,
  0b11010, 0b11011, 0b11100, 0b11101
};

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // idle state
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

// Transmit one byte using 4B/5B
void transmit_byte(char data) {
  digitalWrite(LED_PIN, LOW);
  delay(SAMPLING_TIME);

  byte highNibble = (data >> 4) & 0x0F;
  byte lowNibble  = data & 0x0F;

  transmit_5bits(trans4b5b[highNibble]);
  transmit_5bits(trans4b5b[lowNibble]);

  digitalWrite(LED_PIN, HIGH);
  delay(SAMPLING_TIME);
}

void transmit_5bits(byte code) {
  for (int i = 4; i >= 0; i--) {
    bool bitVal = (code >> i) & 0x01;
    digitalWrite(LED_PIN, bitVal ? HIGH : LOW);
    delay(SAMPLING_TIME);
  }
}