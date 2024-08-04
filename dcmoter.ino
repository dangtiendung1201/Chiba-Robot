// 定義
const int in1Pin = 2;
const int in2Pin = 4;
const int enablePin = 7;

void setup() {
  // ピンモードの設定
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // モータを最初はオフにする
  digitalWrite(enablePin, LOW);
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
}

void loop() {
  // モータをオンにする
  digitalWrite(enablePin, HIGH);
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  delay(2000);  

  // モータをオフにする
  digitalWrite(enablePin, LOW);
  delay(2000);  
}
