void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initTFTouchScreen();
}

void loop() {
  // put your main code here, to run repeatedly:
  processTouch();
}
