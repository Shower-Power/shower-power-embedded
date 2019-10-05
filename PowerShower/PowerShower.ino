void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT); // Pin 0 is heartbeat
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(0, HIGH);  // heartbeat on
  delay(500);
  digitalWrite(0, LOW);  // heartbeat off
  delay(500);

  
}
