int pin_out_camera=5; // experimential!
int pin_out_magnet=6;
int pin_out_flash=7;

int log_level=50;  // for future use, could make the program less chatty

int delay_before_magnet=1000,delay_after_magnet=0;
int flash_duration=10;
int magnet_duration=50;
int camera_duration=50;

#define INLENGTH 20
char inString[INLENGTH+1];
int inCount;

#define INTERMINATOR 13

void setup()  
{  
  
  Serial.begin(38400); 
  Serial.println("PhotoMagnet V1.01"); 
  Serial.println(); 
   
  pinMode(pin_out_magnet,OUTPUT);
  pinMode(pin_out_flash,OUTPUT);
  pinMode(pin_out_camera,OUTPUT);
  
}  

void waitForEnter() {
  Serial.println("---Press Enter to Continue>");
  // read string until EOF
  inCount=0;
  do {
    while (Serial.available()==0);
    inString[inCount]=Serial.read();
    if (inString[inCount]==INTERMINATOR) 
      break;
  } while (++inCount<INLENGTH);

}


void readInt(char *what, int old_value, int *p_new_value) {
  Serial.print("Enter "); 
  Serial.print(what); 
  Serial.print("(old value is "); 
  Serial.print(old_value);  
  Serial.print(") >"); 
  
  // read string until EOF
  inCount=0;
  do {
    while (Serial.available()==0);
    inString[inCount]=Serial.read();
    if (inString[inCount]==INTERMINATOR) 
      break;
  } while (++inCount<INLENGTH);
  // if empty string, return old value
  if (inCount==0) {
    *p_new_value=old_value;
  } else {
    // convert string to number
    *p_new_value=atoi(inString);
  }
  Serial.println();
  
} // readInt

void myDelay(int ms) {
  Serial.print("waiting for ");
  Serial.print(ms);
  Serial.println();
  delay(ms);
} // myDelay
  
void delayAfterFiring(int firing_duration, int delay_duration) {
  int d=delay_duration-firing_duration;
  if (d>0) {
    myDelay(d);
  } else {
    Serial.println("*** couldn't delay after firing, duration larger than delay");
  }
}

void fireRelay(int pin, char *what, int duration) {
  Serial.print("firing ");
  Serial.print(what);
  Serial.println("...");
  digitalWrite(pin,HIGH);
  myDelay(duration);  
  digitalWrite(pin,LOW);  
}
  
void readValues() {
  readInt("camera_duration    ",camera_duration,&camera_duration);
  readInt("delay_before_magnet",delay_before_magnet,&delay_before_magnet);
  readInt("delay_after_magnet ",delay_after_magnet,&delay_after_magnet);
  readInt("flash_duration     ",flash_duration,&flash_duration);
  readInt("magnet_duration    ",magnet_duration,&magnet_duration);
}
  
void loop()  
{ 
  
  // Action!
  Serial.println("here we go again...");

  // read values  
  readValues();
  
  // wait for Start
  waitForEnter();
  
  // fire camera - not activated
  // fireRelay(pin_out_camera,"camera",camera_duration);
  
  // wait until magnet
  delayAfterFiring(camera_duration,delay_before_magnet);
  
  // fire magnet
  fireRelay(pin_out_magnet,"magnet",magnet_duration);
  
  // wait until flash
  delayAfterFiring(magnet_duration,delay_after_magnet);

  // fire flash
  fireRelay(pin_out_flash,"flash",flash_duration);
  
}                            
