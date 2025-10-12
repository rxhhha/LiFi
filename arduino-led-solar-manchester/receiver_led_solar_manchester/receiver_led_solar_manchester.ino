// ARDUINO LIFI RECEIVER

#define LDR_PIN A3
#define SAMPLING_TIME 4   // ms per bit
#define NUM_SAMPLES 8

int threshold = 0;
bool previous_state = true;
bool current_state = true;

void setup() {
  Serial.begin(9600);
  delay(1000);  // ignore noise while setting up

  // set threshold dynamically
  int sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(LDR_PIN);
    delay(5);
  }
  int ldr_idle = sum / NUM_SAMPLES;
  threshold = int(0.6 * ldr_idle);
  Serial.print("Threshold set to: ");
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
  // Serial.println(analogRead(LDR_PIN));
  return analogRead(LDR_PIN) > threshold ? true : false;
}

char get_byte() {
  char data_byte = 0;
  delay(SAMPLING_TIME * 1.1);

  for (int i = 0; i < 8; i++) {
    bool first_half = get_ldr();
    delay(SAMPLING_TIME / 2);
    bool second_half = get_ldr();
    delay(SAMPLING_TIME / 2);
    bool bitVal;
    if (first_half == LOW && second_half == HIGH)
      bitVal = 0;
    else if (first_half == HIGH && second_half == LOW)
      bitVal = 1;
    else
      bitVal = 0; // fallback

    data_byte |= (bitVal << i);
  }
  

  return data_byte;
}
