/********************************************************************************************/
/*   ARDUINO NANO - TV 8 COLORES - PAL50HZ                                                  */
/********************************************************************************************/

/********************************************************************************************/
/*                                                                                          */
/*   ARDUINO TO SCART CONNECTOR                                                             */
/*                                                                                          */
/*   ARDUINO               SCART                                                            */
/*   SYNC_PIN ------------ 20                                                               */
/*   R_PIN    ---[470R]--- 15                                                               */
/*   G_PIN    ---[470R]--- 11                                                               */
/*   B_PIN    ---[470R]--- 5                                                                */
/*   GND      ------------ 17                                                               */
/*                                                                                          */
/*   This code is GPLv3 :D                                                                  */
/*                                                                                          */
/*   Author: David Colmenero (D_Skywalk) - http://david.dantoine.org                        */
/*                                                                                          */
/*   Thanks to Raul Navarro (raulnd@gmail.com)                                              */
/*          and Javier Valcarce Personal Website at:                                        */
/*              http://www.javiervalcarce.eu/wiki/TV_Video_Signal_Generator_with_Arduino    */
/*                                                                                          */
/*   This code is tested on Amstrad CM14 Colour Monitor (Pal50Hz - 15.625Khz)               */
/*    * Not tested on regular TV sets yet *                                                 */
/*                                                                                          */
/********************************************************************************************/

// -- use debug to check time spend, adjust syncro, etc...
// -- on debug mode we use interrups that made the video do not work correctly. No worry, measure times and comment again.
//#define DEBUG 1

// NOP = 1 cicle = 1/16 us = 62.5ns = 6.25ms
#define NOP __asm __volatile ("nop")

// Delay: decimal us
#define DELAY_03  __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 0.25 - 1 ~ 4
#define DELAY_07  __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 0.625 - 1 ~ 10

// Video out voltage levels
//                 RGBS
#define _SYNC    0b0000
#define _BLACK   0b0001 // used like HSYNC too.
#define _BLUE    0b0011
#define _GREEN   0b0101
#define _RED     0b1001
#define _CYAN    0b0111
#define _MAGENTA 0b1011
#define _YELLOW  0b1101
#define _WHITE   0b1111

// dimensions of the screen
//number of lines to display
#define DISPLAY_LINES 305
#define WIDTH  84
#define HEIGHT 60

// the video frameBuffer
byte frameBuffer[WIDTH];
int val;

//video pins
#define SYNC_PIN  8
#define R_PIN    11
#define G_PIN    10
#define B_PIN     9


/****************************************/


void setup() {
  #ifndef DEBUG
  cli(); // interrupt mode off -- we need all cpu for the demo.
  #else
  Serial.begin(9600);
  Serial.println("SETUP STARTS!");
  #endif
  
  pinMode (SYNC_PIN, OUTPUT);
  pinMode (R_PIN, OUTPUT);
  pinMode (G_PIN, OUTPUT);
  pinMode (B_PIN, OUTPUT);
  
  digitalWrite (SYNC_PIN, HIGH);
  digitalWrite (R_PIN, HIGH);
  digitalWrite (G_PIN, HIGH);
  digitalWrite (B_PIN, HIGH);

}


void loop() {
 //register unsigned -- how many registers can i use on the nano device?
  
 register unsigned time;
 register int line = 0;
 
 while(1){
    CRTSyncTop(); // lines 1-5
    // 6-310 (305 lines):
    for( line = 0; line < DISPLAY_LINES;)
    {
      #ifdef DEBUG
        time = micros(); //get time
      #endif
      
      hsync_pulse();
      delayMicroseconds(3); NOP; // spare 3.5ms
      
      RenderBars();
      line++;
 
      delayMicroseconds(3); NOP; // spare 3.5ms
 
      #ifdef DEBUG
        time = micros() - time;
        Serial.println(time, DEC); // print time spend
        delay(1000);
      #endif
    }
    CRTSyncFooter(); // lines 311-312
  }
}

inline void RenderBars() { // 64ms available
      PORTB = _RED; delayMicroseconds(6);
      PORTB = _BLUE; delayMicroseconds(6);
      PORTB = _GREEN; delayMicroseconds(6);
      PORTB = _WHITE; delayMicroseconds(6);
      PORTB = _BLACK; delayMicroseconds(6);
      PORTB = _MAGENTA; delayMicroseconds(6);
      PORTB = _CYAN; delayMicroseconds(6);
      PORTB = _YELLOW; delayMicroseconds(6);
}

inline void CRTSyncTop(){
  // line 1:
  vsync_pulse(); vsync_pulse();
  // line 2:
  vsync_pulse(); vsync_pulse();
  // line 3:
  vsync_pulse(); equal_pulse();
  // line 4:
  equal_pulse(); equal_pulse();
  // line 5:
  equal_pulse(); equal_pulse();
}


inline void CRTSyncFooter(){
  // line 311:
  equal_pulse(); equal_pulse();
  // line 312:
  equal_pulse(); equal_pulse();
}

///////////////////////////////////////////////////////

inline void vsync_pulse()
{
  // Sync goes low and delay 27.3 microseconds
  PORTB = _SYNC;
  delayMicroseconds(27); DELAY_03;
       
  // Sync goes high and delay 4.7 microseconds
  PORTB = _BLACK; 
  delayMicroseconds(4); DELAY_07;
  // 512 cliclos
}

inline void equal_pulse()
{
  // Sync pulse goes low and delay 2.3 microseconds
  PORTB = _SYNC; 
  delayMicroseconds(2); DELAY_03;
     
  // Sync pulse goes high and delay 29.7 microseconds
  PORTB = _BLACK;
  delayMicroseconds(29); DELAY_07;

}

inline void hsync_pulse()
{
  // Front Porch - high for 1.65 microseconds -- cambiado a 1.7 y ok!
  PORTB = _BLACK;
  delayMicroseconds(1); DELAY_07;
      
  // Generate sync pulse 4.7 microseconds
  PORTB = _SYNC;
  delayMicroseconds(4); DELAY_07;
     
  // Back Porch 5.6 microseconds. -- changed to 5.7 and ok
  PORTB = _BLACK;
  delayMicroseconds(5); DELAY_07;
}

