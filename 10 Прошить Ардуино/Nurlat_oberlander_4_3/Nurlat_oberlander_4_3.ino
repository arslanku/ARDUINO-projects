// Управление гидравликой
#define DOOR_LEFT_OPEN 18
#define DOOR_LEFT_CLOSE 14
#define DOOR_RIGHT_OPEN 19
#define DOOR_RIGHT_CLOSE 17
#define PUSHER_FORWARD 11
#define PUSHER_BACK 15
#define SHEAR_DOWN 9
#define SHEAR_UP 8
#define PUMP_2 6
#define SHEAR_CUT 16
#define PUMP_1 10


// Концевые выключатели
#define LIMIT_DOOR_LEFT_OPEN 23
#define LIMIT_DOOR_LEFT_CLOSE 24
#define LIMIT_DOOR_RIGHT_OPEN 21
#define LIMIT_DOOR_RIGHT_CLOSE 22
#define LIMIT_PUSHER_FORWARD 29
#define LIMIT_PUSHER_PULSE 16
#define LIMIT_PUSHER_BACK 18
#define LIMIT_SHEAR_DOWN 26
#define LIMIT_SHEAR_UP 25       

// Концевые выключатели по давлению
//#define PRESSURE_DOOR_LEFT 95
//#define PRESSURE_DOOR_RIGHT 96
#define PRESSURE_PUSHER 30
#define PRESSURE_SHEAR 13
#define PRESSURE_SHEAR_CUT 17

//триггеры
#define TRIG_PUSHER_FORWARD 81
#define TRIG_SHEAR_CUT 82
#define TRIG_SHEAR_PRESSURE 83
#define TRIG_AUTO 84

// Управление кабина
#define JOYSTICK_DOOR_LEFT_OPEN 11
#define JOYSTICK_DOOR_LEFT_CLOSE 7
#define JOYSTICK_DOOR_RIGHT_OPEN 6
#define JOYSTICK_DOOR_RIGHT_CLOSE 5
#define JOYSTICK_PUSHER_BACK 3
#define JOYSTICK_PUSHER_FORWARD 4
#define JOYSTICK_SHEAR_DOWN 8
#define JOYSTICK_SHEAR_UP 0
#define BUTTON_AUTO 29

int QLeft=3;
int QRight=12;
int CLK=4;
int SHLD=7;

int pin_number;
bool pin_state;
bool input_pins[100];
bool control_pins[100];
int joystick_pins[8];
String inputString = ""; 
String inString="";
bool stringComplete = false;
int counter=-1;
int count_max=5;
int registr_length=16;
int check_count=0;
int repeat=0;
int command_1=-1;
int command_check=-1;
long previousMillis = 0;   
long interval = 1000; 

void setup(){
  int i;
  pin_number=8;
  Serial.begin(9600);
  inputString.reserve(200);
  pinMode(QLeft, INPUT);
  pinMode(QRight, INPUT);
  pinMode(CLK, OUTPUT);
  pinMode(SHLD, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, OUTPUT);
  for (i=8; i<12; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  for (i=14; i<22; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  } 
  digitalWrite(SHLD, HIGH);
  for (i=0; i<100; i++){
    input_pins[i]=false;
    control_pins[i]=false;  
  }
  outputs_reset();
  outputs_turn();
  Serial.println("OBERLANDER CONTROL UNIT NANO SOFTWARE v4.3 autoshear");

  joystick_pins[0]=JOYSTICK_DOOR_LEFT_OPEN;
  joystick_pins[1]=JOYSTICK_DOOR_LEFT_CLOSE;
  joystick_pins[2]=JOYSTICK_DOOR_RIGHT_OPEN;
  joystick_pins[3]=JOYSTICK_DOOR_RIGHT_CLOSE;
  joystick_pins[4]=JOYSTICK_PUSHER_BACK;
  joystick_pins[5]=JOYSTICK_PUSHER_FORWARD;
  joystick_pins[6]=JOYSTICK_SHEAR_DOWN;
  joystick_pins[7]=JOYSTICK_SHEAR_UP;

  for (i=0; i<8; i++){
   Serial.print("joystick pin ");
   Serial.print(i, DEC);
   Serial.print(" : ");
   Serial.print(joystick_pins[i], DEC);
   Serial.println("-------------------");
  }
}

void outputs_turn()
{
   
  digitalWrite(DOOR_LEFT_OPEN,control_pins[DOOR_LEFT_OPEN]);
  digitalWrite(DOOR_LEFT_CLOSE,control_pins[DOOR_LEFT_CLOSE]);
  digitalWrite(DOOR_RIGHT_OPEN,control_pins[DOOR_RIGHT_OPEN]);
  digitalWrite(DOOR_RIGHT_CLOSE,control_pins[DOOR_RIGHT_CLOSE]);
  digitalWrite(PUSHER_BACK,control_pins[PUSHER_BACK]);
  digitalWrite(PUSHER_FORWARD,control_pins[PUSHER_FORWARD]);
  digitalWrite(SHEAR_DOWN,control_pins[SHEAR_DOWN]);
  digitalWrite(SHEAR_UP,control_pins[SHEAR_UP]);
  digitalWrite(SHEAR_CUT,control_pins[SHEAR_CUT]); 

  digitalWrite(PUMP_1, control_pins[PUMP_1]);   
  digitalWrite(PUMP_2, control_pins[PUMP_2]); 
}

void outputs_reset()
{
  control_pins[DOOR_LEFT_OPEN]=LOW;
  control_pins[DOOR_LEFT_CLOSE]=LOW;
  control_pins[DOOR_RIGHT_OPEN]=LOW;
  control_pins[DOOR_RIGHT_CLOSE]=LOW;
  control_pins[PUSHER_BACK]=LOW;
  control_pins[PUSHER_FORWARD]=LOW;
  control_pins[SHEAR_DOWN]=LOW;
  control_pins[SHEAR_UP]=LOW;  
  control_pins[SHEAR_CUT]=LOW;
  
  control_pins[PUMP_1]=LOW;
  control_pins[PUMP_2]=LOW;
}

void show_pins(){
  inputString=""; 
  String state="";
  for (int j=0; j<registr_length*2; j++){
    if (input_pins[j]){state="+++";} else {state="";}
    inputString=inputString+" "+String(j)+"("+state+")"; 
  }
  Serial.println(inputString);
}

void setpin(int number,bool state){
  digitalWrite(number, state);
}

void serialEvent() {
  bool stringComplite=false;
  while (Serial.available()) {
  char inChar = (char)Serial.read();
  inString += inChar;
  if (int(inChar) == 10) {
    stringComplete = true;
    int i;
    i=inString.toInt();
    inString="";
    if (i>=6){
      if (i!=QRight && i!=LED_BUILTIN && i!=SHLD){
          setpin(i, HIGH);
          delay(1000);
          setpin(i, LOW);
          }
      }
    }
  }
}  

void pollpins(){
  digitalWrite(SHLD, LOW);
  delay(5);
  digitalWrite(SHLD, HIGH);
  delay(5);
  for (int i=0; i<registr_length; i++){
    digitalWrite(CLK, HIGH);
    delay(1);
    digitalWrite(CLK, LOW);
    delay(1);
    input_pins[i]=digitalRead(QLeft);
    input_pins[i+registr_length]=digitalRead(QRight);  
  }
}

// УПРАВЛЕНИЕ ГИДРАВЛИКОЙ
void hydraulic_control(int hydro_command){
  if (hydro_command>=0){
    Serial.print("hydro_command ");       
    Serial.println(hydro_command, DEC);  
  }
 
 //основная комманда
 switch (hydro_command)
 {
    case JOYSTICK_DOOR_LEFT_OPEN:{
     control_pins[PUMP_1]= HIGH;
     control_pins[DOOR_LEFT_OPEN]= HIGH;
     if (input_pins[LIMIT_DOOR_LEFT_OPEN]==HIGH){ 
         control_pins[DOOR_LEFT_OPEN]=LOW;
         control_pins[PUMP_1]= LOW;
       }
     break;
    }      
    case JOYSTICK_DOOR_LEFT_CLOSE:{
       control_pins[PUMP_1]= HIGH;
       control_pins[DOOR_LEFT_CLOSE]= HIGH;
       if (input_pins[LIMIT_DOOR_LEFT_CLOSE]==HIGH){
           control_pins[DOOR_LEFT_CLOSE]=LOW;
          control_pins[PUMP_1]= LOW;
          } 
       break;
    }  
    case JOYSTICK_DOOR_RIGHT_OPEN:{
       control_pins[PUMP_1]= HIGH;
       control_pins[DOOR_RIGHT_OPEN]= HIGH;
        if (input_pins[LIMIT_DOOR_RIGHT_OPEN]==HIGH){
           control_pins[DOOR_RIGHT_OPEN]=LOW;
           control_pins[PUMP_1]= LOW;
         }
       break;
    }     
    case JOYSTICK_DOOR_RIGHT_CLOSE:{
       control_pins[PUMP_1]= HIGH;
       control_pins[DOOR_RIGHT_CLOSE]= HIGH;
       if (input_pins[LIMIT_DOOR_RIGHT_CLOSE]==HIGH){
            control_pins[DOOR_RIGHT_CLOSE]=LOW;
            control_pins[PUMP_1]= LOW;
         }
      break;
    }     
    case JOYSTICK_PUSHER_BACK:{
       control_pins[PUMP_1]= HIGH;
       control_pins[PUSHER_BACK]= HIGH;      
       counter = -1;
       if (input_pins[LIMIT_PUSHER_BACK]==HIGH){
           control_pins[PUSHER_BACK]=LOW;
           control_pins[PUMP_1]= LOW;
           counter=0;
           input_pins[TRIG_PUSHER_FORWARD]=LOW;
       }
       break;
    }      
    case JOYSTICK_PUSHER_FORWARD:{
       if ((input_pins[LIMIT_PUSHER_PULSE]==HIGH) && (input_pins[TRIG_PUSHER_FORWARD]!=HIGH)){
         input_pins[TRIG_PUSHER_FORWARD]=HIGH;
         counter++;
       }      
       if ((counter>=0) && (counter<=count_max)){ 
        control_pins[PUMP_1]= HIGH;  
        control_pins[PUSHER_FORWARD]= HIGH;
       }
       if ((input_pins[TRIG_PUSHER_FORWARD]==HIGH))
          { 
          control_pins[PUMP_1]= LOW;
          control_pins[PUSHER_FORWARD]=LOW;
       } 
       if (input_pins[LIMIT_PUSHER_FORWARD]==HIGH){ 
          control_pins[PUMP_1]= LOW;
          control_pins[PUSHER_FORWARD]=LOW;
       } 
       break;
    }      
    case JOYSTICK_SHEAR_DOWN:{
       control_pins[PUMP_1]= HIGH;  
       control_pins[PUMP_2]= HIGH;
       control_pins[SHEAR_DOWN]= HIGH;
       control_pins[SHEAR_CUT]= LOW;
       
       if (input_pins[PRESSURE_SHEAR_CUT]){ 
        control_pins[TRIG_SHEAR_CUT]=HIGH;
       }
       if (control_pins[TRIG_SHEAR_CUT]){
         control_pins[SHEAR_CUT]= HIGH;
         control_pins[PUMP_2]= LOW;
       }
       if (input_pins[LIMIT_SHEAR_DOWN]==HIGH){
           control_pins[PUMP_1]= LOW;  
           control_pins[PUMP_2]= LOW;
           control_pins[SHEAR_DOWN]= LOW;
           control_pins[SHEAR_CUT]= LOW;
           control_pins[TRIG_SHEAR_CUT]= LOW;
         }      
       break;
      }       
    case JOYSTICK_SHEAR_UP:{
       control_pins[PUMP_1]= HIGH;
       control_pins[PUMP_2]= HIGH;  
       control_pins[SHEAR_UP]=HIGH;
       control_pins[SHEAR_CUT]= LOW;
       control_pins[TRIG_SHEAR_CUT]= LOW;
       
       if (input_pins[LIMIT_SHEAR_UP]==HIGH){
            control_pins[PUMP_1]= LOW;
            control_pins[PUMP_2]= LOW;
            control_pins[SHEAR_UP]= LOW;
            control_pins[SHEAR_CUT]= LOW;
          }
       break;
    }
    case -1:{
      if (counter<=count_max){
      input_pins[TRIG_PUSHER_FORWARD]=false;  
      }
    }        
   }
}

void select_command(){
  String state="";
  int pin_n;
  command_check=-1;
  for (int k=0; k<8; k++){
    pin_n=joystick_pins[k];
    if (input_pins[pin_n]){
      command_check=pin_n;
    }
  }
  if (command_check==-1){
    repeat=0;
    command_1=-1;
  }
  if (command_1!=command_check){
    repeat++;
    if (repeat>3){
      repeat=0;
      command_1=command_check;
    }
  }
}

void auto_shear_command(){


}

void loop() {
  outputs_reset();
  
  pollpins();
   
  if (millis() - previousMillis > interval){ 
    previousMillis = millis(); 
    show_pins();  
  }
  
  select_command(); 
  
  if (control_pins[TRIG_AUTO]){ 
    auto_shear_command();
  }
  hydraulic_control(command_1);   
  
  outputs_turn();
}
