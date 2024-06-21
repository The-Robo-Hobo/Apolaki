#define PWM_PIN 3
#define CONTROL_PIN 2
#define RELAY_PIN 6
#define ANALOG_PIN A4

void smooth_blink(int induced_delay=0);
void smart_light(bool start=true);
volatile byte state = LOW;

int i, val;

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(CONTROL_PIN, INPUT_PULLUP);
  // pinMode(ANALOG_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CONTROL_PIN), control_mode, RISING);

  Serial.begin(9600);
}


void loop() {
  // if(digitalRead(CONTROL_PIN))
  //   manual_relay();
  // else
  //   smooth_blink();
  smooth_blink(10);
  // manual_relay();
  
  
}


void smooth_blink(int induced_delay=0){
  /*
    255 is the max value that can be taken by analogWrite.
    13 is used for incrementing the loops so that there are 
      20 states of brightness.
  */
  for(i=0 ; i<=255 ; i=i+13){
    analogWrite(PWM_PIN, i);
    // Serial.print("PWM input value: ");
    // Serial.println(i);
    delay(induced_delay);
  }
  for(i=255 ; i>=0 ; i=i-13){
    analogWrite(PWM_PIN, i);
    // Serial.print("PWM input value: ");
    // Serial.println(i);
    delay(induced_delay);
  }
}


void smart_light(bool start=true){
  // check time_now()
  // if time now 
}


void manual_relay(){
  val = analogRead(ANALOG_PIN);
  analogWrite(PWM_PIN, val/4);
  
  Serial.print("val: ");
  Serial.println(val/4);
}


void control_mode(){
  state = !state;
}
