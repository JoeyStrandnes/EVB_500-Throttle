//Arduino code to adjust the throttle of the EVB 500 electric scooter

//Pins used for the project
const int OutputPin = 3;
const int ButtonPin = 4;

int ButtonState = 0;

//Values for the throttle adjustment
int MIN = 40; 
int MAX = 225;
int Counter = MIN;  

void setup() {
    
  pinMode(OutputPin, OUTPUT);
  pinMode(ButtonPin, INPUT);
}

void RunMotor(){
  
  while(ButtonState == HIGH){
    
    if( Counter <= MAX){
      Counter++;
    }
    
    analogWrite(OutputPin, Counter);
    ButtonState = digitalRead(ButtonPin);
  }
  
  Counter = MIN;
  return;
}

void StopMotor(){
  Counter = MIN;
  analogWrite(OutputPin, Counter);
  return;
}


void loop() {
  
  ButtonState = digitalRead(ButtonPin);


  if(ButtonState == HIGH){
    RunMotor();
  }
  
  else{
    StopMotor();
  }
}