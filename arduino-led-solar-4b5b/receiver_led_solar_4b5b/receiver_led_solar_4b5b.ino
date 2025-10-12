// ARDUINO LIFI RECEIVER

#define LDR_PIN A3
#define SAMPLING_TIME 2   // ms per bit
#define NUM_SAMPLES 8

int threshold = 0;
bool current_state = true;
bool previous_state = true;

// 4B/5B Control Symbols
#define CTRL_IDLE  0b11111  // idle
#define CTRL_SOS1  0b11000  // Start of Stream 1
#define CTRL_SOS2  0b10001  // Start of Stream 2
#define CTRL_EOS1  0b01101  // End of Stream 1
#define CTRL_EOS2  0b00111  // End of Stream 2
#define CTRL_ERR   0b00100  // Transmission Error

// 4b5b table
const byte recv4b5b[32] = {
  0xFF, 0xFF, 0xFF, 0xFF, CTRL_ERR, 0xFF, 0xFF, CTRL_EOS2,  // 00000–00111
  0xFF, 0x01, 0x04, 0x05, 0xFF, CTRL_EOS1, 0x06, 0x07,      // 01000–01111
  0xFF, CTRL_SOS2, 0x08, 0x09, 0x02, 0x03, 0x0A, 0x0B,      // 10000–10111
  CTRL_SOS1, 0xFF, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, CTRL_IDLE  // 11000–11111
};

void setup() {
  Serial.begin(9600);
  delay(1000);

  int sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(LDR_PIN);
    delay(5);
  }
  threshold = int(0.6 * (sum / NUM_SAMPLES));
  Serial.print("Threshold: ");
  Serial.println(threshold);
}

void loop() {
  current_state = get_ldr();

  if (!current_state && previous_state) {
    char c = get_byte();
    Serial.print(c);
  }

  previous_state = current_state;
}

bool get_ldr() {
  return analogRead(LDR_PIN) > threshold ? true : false;
}

char get_byte() {
  delay(SAMPLING_TIME * 1);
  byte highNibble = receive_5bits();
  byte lowNibble  = receive_5bits();

  if (highNibble > 15 || lowNibble > 15) return '?';
  return (highNibble << 4) | lowNibble;
}

byte receive_5bits() {
  byte code = 0;
  for (int i = 0; i < 5; i++) {
    bool bitVal = get_ldr();
    code = (code << 1) | bitVal;
    delay(SAMPLING_TIME);
  }
  return recv4b5b[code];
}