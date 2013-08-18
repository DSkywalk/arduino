//#define DEBUG 1

// NOP = 1 ciclo = 1/16 us = 62.5ns
#define NOP __asm __volatile ("nop")

// Delay: decimal us
#define DELAY_03  __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 4.8 - 1 ~ 4
#define DELAY_035 __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 5.6 - 1 ~ 5
#define DELAY_065 __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 10.4 - 1 ~ 9
#define DELAY_07  __asm __volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // 11.2 - 1 ~ 10

// Video out voltage levels
//                 RGBS
#define _SYNC    0b0000
#define _BLACK   0b0001
#define _BLUE    0b0011
#define _GREEN   0b0101
#define _RED     0b1001
#define _CYAN    0b0111
#define _MAGENTA 0b1011
#define _YELLOW  0b1101
#define _WHITE   0b1111

// dimensions of the screen
//number of lines to display
#define DISPLAY_LINES 240
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

  //Serial.println( val);
}


int i = 0;

void loop() {
 int line = 0;
 
 while(1){
    CRTSyncTop(); // lineas de 1-5
    // 6-310 (305 lines):
    for( line = 0; line < 305; line++)
    {
      hsync_pulse();
      
      PORTB = _RED; delayMicroseconds(9);
      PORTB = _BLUE; delayMicroseconds(6);
      PORTB = _GREEN; delayMicroseconds(6);
      PORTB = _WHITE; delayMicroseconds(6);
      PORTB = _BLACK; delayMicroseconds(6);
      PORTB = _MAGENTA; delayMicroseconds(6);
      PORTB = _CYAN; delayMicroseconds(7);
      PORTB = _YELLOW; delayMicroseconds(7);
    }
    CRTSyncFooter(); // lineas de 311-312
  }
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
     
  // Back Porch 5.6 microseconds. -- cambiado a 5.7 y ok
  PORTB = _BLACK;
  delayMicroseconds(5); DELAY_07;
}

