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
/********************************************************************************************/

// -- usalo para medir tiempos y ajustar la syncro
// -- en modo debug no se vera el video correctamente, ya que no puedes usar el modo cli.
//#define DEBUG 1

// NOP = 1 ciclo = 1/16 us = 62.5ns = 6.25ms
#define NOP __asm __volatile ("nop")

// Delay: decimal us
#define DELAY_03  __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 0.25 - 1 ~ 4
#define DELAY_035 __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 3.125 - 1 ~ 5
#define DELAY_065 __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 0.5625 - 1 ~ 9
#define DELAY_07  __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 0.625 - 1 ~ 10

// Video out voltage levels
//                 RGBS
#define _SYNC    0b0000
#define _BLACK   0b0001 // lo usamos tambien como HSYNC
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
  cli();
  #else
  // put your setup code here, to run once:
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
 //register unsigned -- cambiando a registro gano tiempo por si fuera necesario para animar.
  
 register unsigned time;
 register int line = 0;
 
 while(1){
    CRTSyncTop(); // lineas de 1-5
    // 6-310 (305 lineas):
    for( line = 0; line < DISPLAY_LINES;)
    {
      #ifdef DEBUG
        time = micros();
      #endif
      
      hsync_pulse();
      delayMicroseconds(3); NOP; // sobran 3.5ms
      
      RenderBars();
      line++;
 
      delayMicroseconds(3); NOP; // sobran 3.5ms
 
      #ifdef DEBUG
        time = micros() - time;
        Serial.println(time, DEC);
        delay(1000);
      #endif
    }
    CRTSyncFooter(); // lineas de 311-312
  }
}

inline void RenderBars() { // 64ms disponibles
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
  // linea 1:
  vsync_pulse(); vsync_pulse();
  // linea 2:
  vsync_pulse(); vsync_pulse();
  // linea 3:
  vsync_pulse(); equal_pulse();
  // linea 4:
  equal_pulse(); equal_pulse();
  // linea 5:
  equal_pulse(); equal_pulse();
}


inline void CRTSyncFooter(){
  // linea 311:
  equal_pulse(); equal_pulse();
  // linea 312:
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
     
  // Back Porch 5.6 microseconds. -- cambiado a 5.7 y ok -- subido el delay un microsegundo mas para tener mas rojo.
  PORTB = _BLACK;
  delayMicroseconds(5); DELAY_07;
}

