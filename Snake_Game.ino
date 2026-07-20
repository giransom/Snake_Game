#include "LedControl.h"
#define joyX A0   //Joystick x richtung auf A0
#define joyY A1   //Joystick y richtung auf A1




int movewt = 500; //speed (lower = faster , higher = slower) milliseconds to move
int lengthtowin = 20; //determine the number of apples you need to win


int joybut = 9; //joystick button pin 9


int maxlength = 3; //length of the snake


int yValue;
int xValue;
int Punkte;
char direction;
int row = 4;
int col = 4;
bool neustart = true;
LedControl lc=LedControl(12,11,10,1);
bool dead = false;
int matrix [8][8];


int apfelPosition [8][8];
bool needapple = true;
int ax; //apple X position
int ay; //apple Y position
int atime = 200; //apple blink speed (lower = faster , higher = slower)1
bool leuchtet = false;
unsigned long lastappletime;


bool won = false;
unsigned long lasttime;
byte lachen[8]={B11111111, B11000011, B10101001, B10000101, B10000101, B10101001, B11000011, B11111111};
byte traurig[8]={B11111111, B11000011, B10100101, B10001001, B10001001, B10100101, B11000011, B11111111};
void setup() {
 delay(2000);
 pinMode(joybut, INPUT_PULLUP);
 // put your setup code here, to run once:
 Serial.begin (9600);
 Serial.println("Guten Tag");
 lc.shutdown(0,false);
 lc.setIntensity(0,8);
 lc.clearDisplay(0);
 lc.setLed(0,row,col,true);
 matrix [row][col] = 1;
}


void loop() {
 if (won == false){
 if (dead == false){
 xValue = analogRead(joyX);
 yValue = analogRead(joyY); 
 richtung();
 apfeltocollect();
 weiterfahren();
 }
  if (dead == true)
 {
   col = 4;
   row = 4;
   for(int x = 0; x <8 ; x ++){   
       for(int y = 0 ; y <8 ; y++){
        
           matrix[x][y] = 0;
          
        
       }
   }
   for(int x = 0; x <8 ; x ++){   
       for(int y = 0 ; y <8 ; y++){
        
           apfelPosition[x][y] = 0;
          
        
       }
   }
   maxlength = 3;
   for (int i=0; i<8; i++){
    lc.setRow(0,i,traurig[i]);
   }
   if(digitalRead(joybut)== 0){
      
       dead = false;
       lc.clearDisplay(0);
       lc.setLed(0,row,col,true);
       matrix [row][col] = 1;
     }
 }
 if (lengthtowin<=maxlength){
   won = true;
 }
 }
 if (won == true)
 {
   neustart = true;
   col = 4;
   row = 4;
   for(int x = 0; x <8 ; x ++){   
       for(int y = 0 ; y <8 ; y++){
        
           matrix[x][y] = 0;
          
        
       }
   }
   for(int x = 0; x <8 ; x ++){   
       for(int y = 0 ; y <8 ; y++){
        
           apfelPosition[x][y] = 0;
          
        
       }
   }
   maxlength = 3;
   for (int i=0; i<8; i++){
    lc.setRow(0,i,lachen[i]);
   }
   if(digitalRead(joybut)== 0){
      
       won = false;
       lc.clearDisplay(0);
       lc.setLed(0,row,col,true);
       matrix [row][col] = 1;
     }
 // put your main code here, to run repeatedly:


}}
void apfeltocollect(){
 if (needapple == true){ //generate new apple
   needapple = false;
   ay = random(0 , 7);
   ax = random(0 , 7);
   while(matrix[ax][ay] != 0){
     ay = random(0 , 7);
     ax = random(0 , 7);
   }
   apfelPosition[ax][ay] = 1;
 }
 if (ax == row && ay == col && needapple == false){ // collect apple
   needapple = true;
   for(int x = 0; x <8 ; x ++){   
       for(int y = 0 ; y <8 ; y++){
        
           apfelPosition[x][y] = 0;
          
        
       }
   }
   maxlength ++;


 }
 if (needapple == false){ // blinken
   if(leuchtet == false && millis()-lastappletime > atime){
     lc.setLed(0,ax,ay,true);
     lastappletime = millis();
     leuchtet = true;
     }
   else if(leuchtet == true && millis()-lastappletime > atime){
     lc.setLed(0,ax,ay,false);
     lastappletime = millis();
     leuchtet = false;
   }
 }




}
void weiterfahren(){
  if (millis()- lasttime > movewt){
   lasttime = millis();
 switch (direction){
   case 'N':
     col --;
     Serial.println("Nord");
     break;
   case 'O':


     Serial.println("Ost");
     row --;
     break;
   case 'S':
     Serial.println("South");
      col ++;
     break;
   
  
   case 'W':
     Serial.println("West");
     row ++;
     break;
   default:
     Serial.println("MoveJoystick");
     break;
  }
 if (direction == 'W' ||direction == 'O' ||direction == 'S' ||direction == 'N' ){
 if (row <0 || row >7 || col < 0 || col > 7) //ob es in den Rand gefahren ist
 {
   Serial.println("DED because of wall");
   neustart = true;
   dead = true;


 }
 else if(matrix [row][col] > 0 && matrix [row][col] != maxlength ) //if touches snake //(
  {
   Serial.println("DED because of snake");
   neustart = true;
   dead = true;
  
 }
 else{
     for(int x = 0; x <8 ; x ++){    //remove last part of snake
       for(int y = 0 ; y <8 ; y++){
         if (matrix[x][y] == maxlength){
           matrix[x][y] = 0;
           lc.setLed(0,x,y,false);
         }
       }


     }


    // move numbers back
    for(int l = maxlength - 1; l > 0 ; l --){
      for(int x = 0; x <8 ; x ++){   
       for(int y = 0 ; y <8 ; y++){
         if (matrix[x][y] == l){
           matrix[x][y] = l + 1;
          
         }
       }
    
    }
    //set new point
    matrix [row][col] = 1;
    lc.setLed(0,row,col,true);


   }
   }
  }
 }
 }
void richtung(){
 if (xValue > 700){
   if(direction != 'N'){
     direction = 'S';
 }}


 else if(xValue < 250){
   if(direction != 'S'){
     direction = 'N';
   }
 }
  else if (yValue > 620){
   if (direction != 'O'){
   direction = 'W';
   }
 }
 else if (yValue < 400)
 {
   if (direction != 'W'){
   direction = 'O';
 }}
 else if(neustart == true){
   neustart = false;
   direction= 'p';
 }
// Serial.println(direction);
}


//#    #   ##       #  ######   ######   #########  #
//#   #    # #      #  #       #             #      #
//#  #     #  #     #  #      #              #      #
//# #      #   #    #  #       ##            #      #
//##       #    #   #  ######   #####        #      #
//# #      #     #  #  #            ##       #      #
//#  #     #      # #  #             #       #      #
//#   #    #       ##  #            #        #      #
//#    #   #        #  ######  #####         #      #
