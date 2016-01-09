int buttonPin = 2;
int sensorPin = 0;
int dataPin = 4;
int clockPin = 8;
int latchPin = 7;
int debugPin = 13;

String command;


void setLeds(int value) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  digitalWrite(latchPin, HIGH);
}

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(debugPin, OUTPUT);
  
  digitalWrite(latchPin, HIGH);

  command.reserve(200);
  Serial.begin(9600);
  setLeds(0);
  Serial.println("init");
}

void loop() {
}

void ProcessCommand(String command) {
  String instr = command.substring(0, 4);
  String args = command.substring(5);
  if (instr == "rdst") {
    bool buttonState = (digitalRead(buttonPin) == LOW);
    int sensorValue = analogRead(sensorPin);
    Serial.print("btst:");
    Serial.print(buttonState);
    Serial.print(",");
    Serial.println(sensorValue);
  } else if (instr == "wled") {
    int val = args.toInt();
    setLeds(val);
    Serial.println("anwr:done");
  }
}

void serialEvent() {
  while (Serial.available()) {
    char ch = (char)Serial.read();
    if (ch == '\n') {
      ProcessCommand(command);
      command = "";
    } else {
      command += ch;
    }
  }
}

