# EVB 500 Throttle Control
###### This is a short summery of the inner workings of the throttle control and how to easily emulate and fix it if it breaks. 

###### This is not a guide on increasing the power or speed of the electric scooter. 



###### The throttle control of my friends EVB 500 electric scooter broke and I offered to fix it. The throttle sensors leads had broken of and needed replacement. 



### Throttle construction

###### The throttle consists of two parts, one moving (handle) and one static.  The handle has embedded magnets that rotate with the handle and the  static part has a linear hall effect sensor that measures the magnetic field and outputs a analog voltage proportional to the measured magnetic field. 

###### The EVB 500 uses the [D49E-757H](https://github.com/JoeyStrandnes/EVB_500-Throttle/blob/master/Datasheets/K1139513440.pdf), 49E-Series linear hall effect sensor by YANGZHOU POSITIONING TECH. The sensor is directly connected between VCC (~5V) red cable, GND (black cable) and outputs a analog voltage between 0.9V-4.2V on the green cable.  A control pin voltage equal to 4.2V will result in MAX throttle and <0.9V results in no power going to the motor. The voltage in-between the thresholds is linearly proportional to the speed. Example;  2.55V would result in 50% motor speed.

###### The main control unit of the electric scooter has some sort of sanity check and refuses an instant 4.2V on the throttle pin (green cable) so a switch between VCC and throttle pin will not work (not even with a voltage divider or regulated 4.2V). 

###### The control unit requires a ramp-up in voltage from 0V to desired voltage/speed in order to pass the sanity check, the speed of the ramp-up can very a lot without any consequences and does not seem to be particularly picky. 



### Fixing the throttle

#### Electronics

###### The obvious solution would be to replace the sensor with an identical one, I was unable to find one and did not want to wait for one to show up. Finding a similar one would also be an alternative but we decided to just get it working with standard components we already had at our disposal. A potentiometer can be used, it worked but was not comfortable to use as a throttle. The "cruse control" button would be used as the  throttle, press the button for full throttle and release it for zero throttle, it was already a part of the scooter and was comfortably mounted to the handle. 

###### An Arduino micro (ATMega32u4) was used to convert the binary ON/OFF signal from the button to a value that the main controller could read. Virtually any microcontroller could be used as long as it has a PWM output. The button is connected to a regular digital pin and the green wire (motor controller/ throttle cable) is connected to a microcontroller pin compatible with PWM. 

###### A low pass filter was firstly connected on the microcontroller output to get a cleaner analog signal but was not needed, the electric scooter was able to take the "raw" PWM signal without any problems so the low pass filter was removed.

![Electrical connections](https://i.imgur.com/B2swCKA.png)



#### Software

###### The microcontroller polls the pin that the button is connected, the microcontroller outputs a PWM value of ~0.9V  if the button is not pressed, running function "StopMotor". The microcontroller will output a PWM value matching ~4.2V if it detects a button press, running function (StartMotor).

###### No adjustability was necessary in this case so a simple MAX throttle was sufficient, NO refined throttle control worked pretty well, the throttle can be adjusted by simply varying the "MAX" variable value in the ramp function. The constants MIN/MAX are tuned to be as close to the motor controller thresholds as possible without passing them.

```c++
//Arduino code to adjust the throttle of the EVB 500 electric scooter

//Pins used for the project
const int OutputPin = 3;
const int ButtonPin = 4;

int ButtonState = 0;

//Values for the throttle adjustment
const int MIN = 40; 
const int MAX = 225;
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
```



### Images of the microcontroller output

#### No throttle

![No Throttle](https://i.imgur.com/nKJ84WR.png)

#### Max throttle

![Max Throttle](https://i.imgur.com/oMCIa8J.png)



###### I was unable to capture the ramp-up of the PWM signal, it was too fast... 



### Final thoughts and images of the completed mod

###### The electric scooter works great, no throttle glitches or other problems related to the throttle. It was more convenient to just use a button to accelerate instead of using a throttle grip. All electronics are connected to the old sensor cables and the power is cut when the power switch is flipped. A standard bicycle grip will be used instead of the old throttle grip.

![Final](https://i.imgur.com/koUKxbN.jpg)

![Back side](https://i.imgur.com/Vljg8pD.jpg)

![Outside of box](https://i.imgur.com/7wDQaM5.jpg)