const byte set_mode = 2; // MODE PIN
const byte send_req = 4; // REQUEST PIN
const byte ready_flag = 3; // GETT READY
const byte valid_flag = 5; // GET VALID
const byte pin6 = 6;
const byte pin7 = 7;
const byte pin8 = 8;
const byte pin9 = 9;
const byte pin10 = 10;
const byte pin11 = 11;
const byte pin12 = 12;
const byte pin13 = 13;
int command_str;
bool PUT_mode = true;
bool flag = true;
byte PUF_code1[512];
byte PUF_code2[512];
byte RN;
int count = 0;
int file_num = 0;

void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  // initialize the LED pin as an output:
  pinMode(set_mode, OUTPUT);
  pinMode(send_req, OUTPUT);
  pinMode(ready_flag, INPUT);
  pinMode(valid_flag, INPUT);
  pinMode(pin6, INPUT);
  pinMode(pin7, INPUT);
  pinMode(pin8, INPUT);
  pinMode(pin9, INPUT);
  pinMode(pin10, INPUT);
  pinMode(pin11, INPUT);
  pinMode(pin12, INPUT);
  pinMode(pin13, INPUT);
  
  digitalWrite(send_req, LOW);
  digitalWrite(set_mode, LOW);
  // Serial.print("Set up finish\n");
}


void read_RN() {
  int state = 0;
  int ready_state = 0;
  int valid_state = 0;
  bool finish = false;
  
  RN &= 0x0; // init byte

  while (!finish){
    if (state == 0) { // Get ready and send request
      ready_state = digitalRead(ready_flag);
      if (ready_state == HIGH) {
        // Serial.print("Ready\n");
        state = 1;
        digitalWrite(send_req, HIGH);
        digitalWrite(send_req, LOW);
      }
    } else { // Get valid and read data
      valid_state = digitalRead(valid_flag);
      if (valid_state == HIGH) {
        // Serial.print("Valid\n");
        state = 0;
        
        RN |= digitalRead(pin6);
        RN |= digitalRead(pin7) << 1;
        RN |= digitalRead(pin8) << 2;
        RN |= digitalRead(pin9) << 3;
        RN |= digitalRead(pin10) << 4;
        RN |= digitalRead(pin11) << 5;
        RN |= digitalRead(pin12) << 6;
        RN |= digitalRead(pin13) << 7;
        finish = true;
      }
    }
  }
}



void loop() {
  digitalWrite(set_mode, LOW);
  delay(1000);
  digitalWrite(set_mode, HIGH);
  delay(1000);
  while (file_num < 87) {
    while (count < 13107200) {
    // while(count < 1024) {
        read_RN();
        Serial.write((RN & 0xFF));
        count = count + 1;
    }
    count = 0;
    file_num = file_num + 1;
  }
}
