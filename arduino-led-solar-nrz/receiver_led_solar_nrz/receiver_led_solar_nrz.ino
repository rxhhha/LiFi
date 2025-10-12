// ARDUINO LIFI RECEIVER

#define LDR_PIN A3
#define SAMPLING_TIME 4   // ms per bit
#define THESHOLD 150

bool previous_state = true;
bool current_state = true;

void setup() {
  Serial.begin(9600);
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
  // Serial.println(analogRead(LDR_PIN));
  return analogRead(LDR_PIN) > THESHOLD ? true : false;
}

char get_byte() {
  char data_byte = 0;
  delay(SAMPLING_TIME * 1.5);

  for (int i = 0; i < 8; i++) {
    data_byte |= (char)get_ldr() << i;
    delay(SAMPLING_TIME);
  }

  return data_byte;
}
