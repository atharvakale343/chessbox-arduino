#define MUX_PIN0 15
#define MUX_PIN1 13
#define MUX_PIN2 12

void select_pins_on_demux(int reed_switch_number) {
  byte pin0_signal = (reed_switch_number & 0b001) >> 0;
  byte pin1_signal = (reed_switch_number & 0b010) >> 1;
  byte pin2_signal = (reed_switch_number & 0b100) >> 2;
  Serial.printf("(%d, %d, %d)\n", pin2_signal, pin1_signal, pin0_signal);
  digitalWrite(MUX_PIN0, pin0_signal);
  digitalWrite(MUX_PIN1, pin1_signal);
  digitalWrite(MUX_PIN2, pin2_signal);
}

void setup() {
  Serial.begin(9600);
  // sets the de-mux select pins to output mode
  pinMode(MUX_PIN0, OUTPUT);
  pinMode(MUX_PIN1, OUTPUT);
  pinMode(MUX_PIN2, OUTPUT);
}

void loop() {
  // Serial.println();
  select_pins_on_demux(3);
  delay(500);
}