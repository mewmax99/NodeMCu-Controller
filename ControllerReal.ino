#include <DHT.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>
const char* ssid     = "barry";
const char* password = "mewmax99";
#define APPID   "siaotest1"
#define KEY     "5XXtGafIHBAzPU8"
#define SECRET  "kx1tILAU925Oc65anBQaGXXUb"
#define ALIAS   "pieled"


int RmotorUp = 5;
int RmotorDn = 4;
int LmotorUp = 0;
int LmotorDn = 2;

WiFiClient client;
char state = 0;
int num1 = 0;
int num2 = 0;
int num = 0;
char stateOutdated = 0;
char buff[16];
int timer = 0;
char str[32];
#define DHTTYPE DHT11
#define DHTPIN D1
DHT dht(DHTPIN, DHTTYPE, 15);
int humid;
int temp;


MicroGear microgear(client);

void MoveF()
{
  Serial.println("<Robot Name> Forward");
  digitalWrite(LmotorUp,HIGH); //As said in video we are writing a Digital HIGH to the upper left pin of the motor driver
  digitalWrite(LmotorDn,LOW); //As said in video we are writing a Digital LOW to the lower left pin of the motor driver
  digitalWrite(RmotorUp,HIGH); //As said in video we are writing a Digital HIGH to the upper right pin of the motor driver
  digitalWrite(RmotorDn,LOW); //As said in video we are writing a Digital LOW to the lower right pin of the motor driver
   // As a result the robot will move forward
}

void MoveB()
{
  Serial.println("<Robot Name> Backward");
  digitalWrite(LmotorUp,LOW); //Reverse logic of above
  digitalWrite(LmotorDn,HIGH);
  digitalWrite(RmotorUp,LOW);
  digitalWrite(RmotorDn,HIGH);
}

void MoveL()
{
  Serial.println("<Robot Name> Left");
  digitalWrite(LmotorUp,LOW); //we are completely stopping the left motor and allowing the right motor to move forward
  digitalWrite(LmotorDn,LOW); //this will cause the robot to move left
  digitalWrite(RmotorUp,HIGH);
  digitalWrite(RmotorDn,LOW);
}

void MoveR()
{
  Serial.println("<Robot Name> Right");
  digitalWrite(LmotorUp,HIGH);//we are completely stopping the right motor and allowing the left motor to move forward
  digitalWrite(LmotorDn,LOW);//this will cause the robot to move right
  digitalWrite(RmotorUp,LOW);
  digitalWrite(RmotorDn,LOW);
}

void Stop()
{
  Serial.println("<Robot Name> Stop");
  digitalWrite(LmotorUp,LOW);//Writing a digital LOW to all pins associated with movement of motor driver causes all motors to stop
  digitalWrite(LmotorDn,LOW);
  digitalWrite(RmotorUp,LOW);
  digitalWrite(RmotorDn,LOW);
}


//void sendState(){
  /*if (state==0){
    microgear.publish("/pieled1/state","0");
    num1=1;
    //Serial.println(state); 
  }else if (state==1){
    microgear.publish("/pieled1/state","1"); 
    //Serial.println("send state..");
    //Serial.println(state); 
    //stateOutdated = 0;
  }
  
  if (state==3){
    microgear.publish("/pieled2/state","3");
    //Serial.println("gg");
  }else if (state==2){
    microgear.publish("/pieled2/state","2");
    num2=2;
    //Serial.println("send state..");
    //stateOutdated = 0;
  }

  if (state==5){
    microgear.publish("/pieled/state","5");
    microgear.publish("/pieled1/state","1"); 
    microgear.publish("/pieled2/state","3");
    //Serial.println("gg");
  }else if (state==4){
    microgear.publish("/pieled/state","4");
    microgear.publish("/pieled1/state","0");
    microgear.publish("/pieled2/state","2"); 
    //Serial.println("send state..");
    //stateOutdated = 0;
  }

  if (num1 == 1 && num2 == 2) {
    microgear.publish("/pieled/state","4"); 
    num1=0;
    num2=0;
  }*/
  
//}
void updateIO(){
  if (state == 1) {
    state == 0;
    MoveF();
    delay(1000);
    Stop();
  } else if (state == 2){
    state == 0;
    MoveR();
    delay(1000);
    Stop();
  } else if (state == 3) {
    state == 0;
    MoveL();
    delay(1000);
    Stop();
  } else if (state == 4) {
    state == 0;
    MoveB();
    delay(1000);
    Stop();
  }

  

  

}
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) { 
  char m = *(char *)msg;
  Serial.print("Incoming message -->"); 
  msg[msglen] = '\0'; 
  Serial.println((char *)msg);
  if (m == '1' || m == '2') {
    state = m=='1'?1:2;
    updateIO();
  }
  if ( m == '3' || m == '4') {
    state = m=='3'?3:4;
    updateIO();
  }

  
  
}
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) { 
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
  stateOutdated = 1;
}
void setup(){
    Serial.begin(115200);
    Serial.println("Starting...");
    pinMode(RmotorUp, OUTPUT);
    pinMode(RmotorDn, OUTPUT);
    pinMode(LmotorUp, OUTPUT);
    pinMode(LmotorDn, OUTPUT);
    if (WiFi.begin(ssid, password)) {
      while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
}
microgear.on(MESSAGE,onMsghandler); 
microgear.on(CONNECTED,onConnected); 
microgear.init(KEY,SECRET,ALIAS); 
microgear.connect(APPID);
}
void loop(){
  if (microgear.connected()) {
    if (stateOutdated) 
    //sendState();
    microgear.loop();
    //humid = dht.readHumidity();
        //temp = dht.readTemperature();
        timer = 0;
        if (timer >= 1000) {
         } else timer += 100;
    
  } else {
    Serial.println("connection lost, reconnect..."); 
    if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0; 
        } else timer += 100;
  }
  delay(100); 
}
