// ARDUINO LIFI TRANSMITTER

#define TRANSMIT_LED 9
#define SAMPLING_TIME 3   // ms per bit

String text = "";          // Default message is empty

bool transmit_data = true;
int bytes_counter = 0;
int total_bytes = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TRANSMIT_LED, OUTPUT);
  digitalWrite(TRANSMIT_LED, HIGH);   // Idle state = HIGH
}

void loop() {
  if (Serial.available()) {
    text = Serial.readString();
    text.trim();
    Serial.println(text);
    text.concat("#");
  }

  total_bytes = text.length();
  if (total_bytes > 0) {
    bytes_counter = total_bytes;
    while (transmit_data) {
      transmit_byte(text[total_bytes - bytes_counter]);
      bytes_counter--;
      if (bytes_counter == 0) {
        transmit_data = false;
      }
    }

    transmit_data = true;
    text = "";
    delay(1000);
  }
}

void transmit_byte(char data_byte) {
  digitalWrite(TRANSMIT_LED, LOW);
  delay(SAMPLING_TIME);

  for (int i = 0; i < 8; i++) {
    digitalWrite(TRANSMIT_LED, (data_byte >> i) & 0x01);
    delay(SAMPLING_TIME);
  }

  digitalWrite(TRANSMIT_LED, HIGH);
  delay(SAMPLING_TIME);
}
