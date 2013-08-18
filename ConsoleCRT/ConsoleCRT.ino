// NOP = 1 ciclo = 1/16 us = 62.5ns
#define NOP __asm __volatile ("nop")

// Delay = 0.375 us
void NOPF()
{
  NOP;NOP;NOP;NOP;NOP;NOP;
}
void DrawBars();

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
// pal video line loop
byte line;

//video pins
#define SYNC_PIN  8
#define R_PIN    11
#define G_PIN    10
#define B_PIN     9


/****************************************/


void setup() {
  cli();
  // put your setup code here, to run once:
  //Serial.begin(9600);
  //Serial.println("SETUP STARTS!");
 
  pinMode (SYNC_PIN, OUTPUT);
  pinMode (R_PIN, OUTPUT);
  pinMode (G_PIN, OUTPUT);
  pinMode (B_PIN, OUTPUT);
  
  digitalWrite (SYNC_PIN, HIGH);
  digitalWrite (R_PIN, HIGH);
  digitalWrite (G_PIN, HIGH);
  digitalWrite (B_PIN, HIGH);
  line = 0;

  //DrawBars();
  Serial.println("SETUP END!");
}

void loop() {
  line++;
  // HSync
  // front porch (1.5 us)
  PORTB = _BLACK;
  delayMicroseconds(1.5);

  //sync (4.7 us)
  PORTB = _SYNC;
  delayMicroseconds(4.7);

  // breezeway (.6us) + burst (2.5us) + colour back borch (1.6 us)
  PORTB = _BLACK;
  delayMicroseconds(0.6+2.5+1.6);
    
    delayMicroseconds(1); 
    PORTB = _RED; delayMicroseconds(1); PORTB = _RED; delayMicroseconds(1); PORTB = _RED; delayMicroseconds(1);
    PORTB = _RED; delayMicroseconds(1); PORTB = _RED; delayMicroseconds(1); PORTB = _RED; delayMicroseconds(1);
    PORTB = _RED; delayMicroseconds(1); PORTB = _RED; delayMicroseconds(1); PORTB = _RED; delayMicroseconds(1);

    PORTB = _BLUE; delayMicroseconds(1); PORTB = _BLUE; delayMicroseconds(1); PORTB = _BLUE; delayMicroseconds(1);
    PORTB = _BLUE; delayMicroseconds(1); PORTB = _BLUE; delayMicroseconds(1); PORTB = _BLUE; delayMicroseconds(1);
    PORTB = _BLUE; delayMicroseconds(1); PORTB = _BLUE; delayMicroseconds(1); PORTB = _BLUE; delayMicroseconds(1);

    PORTB = _GREEN; delayMicroseconds(1); PORTB = _GREEN; delayMicroseconds(1); PORTB = _GREEN; delayMicroseconds(1);
    PORTB = _GREEN; delayMicroseconds(1); PORTB = _GREEN; delayMicroseconds(1); PORTB = _GREEN; delayMicroseconds(1);
    PORTB = _GREEN; delayMicroseconds(1); PORTB = _GREEN; delayMicroseconds(1); PORTB = _GREEN; delayMicroseconds(1);

  delayMicroseconds(10);

  if(line >= DISPLAY_LINES){
    //vsync
    PORTB = _SYNC;
    //delayMicroseconds(1);
    //delayMicroseconds(analogRead(6));
    //delayMicroseconds(305);
    line = 0;
  }
}



/****************************/


void paint2(){
}

void setPixel(byte x, byte y, byte color)
{
  frameBuffer[x] = color;
}

// Dibuja las barras de color
void DrawBars()
{
  byte color;
  int x,y;
  for (x=0; x<WIDTH; x++)
  {
    switch(x/10)
    {
    case 0:
      color = _WHITE;
      break;
    case 1:
      color = _YELLOW;
      break;
    case 2:
      color = _CYAN;
      break;
    case 3:
      color = _GREEN;
      break;
    case 4:
      color = _MAGENTA;
      break;
    case 5:
      color = _RED;
      break;
    case 6:
      color = _BLUE;
      break;
    default:
      color = _BLACK;
      break;
    }
      setPixel(x, 0, color);
  }
}


void paint(){

    PORTB = frameBuffer[0];NOPF();
    PORTB = frameBuffer[1];NOPF();
    PORTB = frameBuffer[2];NOPF();
    PORTB = frameBuffer[3];NOPF();
    PORTB = frameBuffer[4];NOPF();
    PORTB = frameBuffer[5];NOPF();
    PORTB = frameBuffer[6];NOPF();
    PORTB = frameBuffer[7];NOPF();
    PORTB = frameBuffer[8];NOPF();
    PORTB = frameBuffer[9];NOPF();
    PORTB = frameBuffer[10];NOPF();
    PORTB = frameBuffer[11];NOPF();
    PORTB = frameBuffer[12];NOPF();
    PORTB = frameBuffer[13];NOPF();
    PORTB = frameBuffer[14];NOPF();
    PORTB = frameBuffer[15];NOPF();
    PORTB = frameBuffer[16];NOPF();
    PORTB = frameBuffer[17];NOPF();
    PORTB = frameBuffer[18];NOPF();
    PORTB = frameBuffer[19];NOPF();
    PORTB = frameBuffer[20];NOPF();
    PORTB = frameBuffer[21];NOPF();
    PORTB = frameBuffer[22];NOPF();
    PORTB = frameBuffer[23];NOPF();
    PORTB = frameBuffer[24];NOPF();
    PORTB = frameBuffer[25];NOPF();
    PORTB = frameBuffer[26];NOPF();
    PORTB = frameBuffer[27];NOPF();
    PORTB = frameBuffer[28];NOPF();
    PORTB = frameBuffer[29];NOPF();
    PORTB = frameBuffer[30];NOPF();
    PORTB = frameBuffer[31];NOPF();
    PORTB = frameBuffer[32];NOPF();
    PORTB = frameBuffer[33];NOPF();
    PORTB = frameBuffer[34];NOPF();
    PORTB = frameBuffer[35];NOPF();
    PORTB = frameBuffer[36];NOPF();
    PORTB = frameBuffer[37];NOPF();
    PORTB = frameBuffer[38];NOPF();
    PORTB = frameBuffer[39];NOPF();
    PORTB = frameBuffer[40];NOPF();
    PORTB = frameBuffer[41];NOPF();
    PORTB = frameBuffer[42];NOPF();
    PORTB = frameBuffer[43];NOPF();
    PORTB = frameBuffer[44];NOPF();
    PORTB = frameBuffer[45];NOPF();
    PORTB = frameBuffer[46];NOPF();
    PORTB = frameBuffer[47];NOPF();
    PORTB = frameBuffer[48];NOPF();
    PORTB = frameBuffer[49];NOPF();
    PORTB = frameBuffer[50];NOPF();
    PORTB = frameBuffer[51];NOPF();
    PORTB = frameBuffer[52];NOPF();
    PORTB = frameBuffer[53];NOPF();
    PORTB = frameBuffer[54];NOPF();
    PORTB = frameBuffer[55];NOPF();
    PORTB = frameBuffer[56];NOPF();
    PORTB = frameBuffer[57];NOPF();
    PORTB = frameBuffer[58];NOPF();
    PORTB = frameBuffer[59];NOPF();
    PORTB = frameBuffer[60];NOPF();
    PORTB = frameBuffer[61];NOPF();
    PORTB = frameBuffer[62];NOPF();
    PORTB = frameBuffer[63];NOPF();
    PORTB = frameBuffer[64];NOPF();
    PORTB = frameBuffer[65];NOPF();
    PORTB = frameBuffer[66];NOPF();
    PORTB = frameBuffer[67];NOPF();
    PORTB = frameBuffer[68];NOPF();
    PORTB = frameBuffer[69];NOPF();
    PORTB = frameBuffer[70];NOPF();
    PORTB = frameBuffer[71];NOPF();
    PORTB = frameBuffer[72];NOPF();
    PORTB = frameBuffer[73];NOPF();
    PORTB = frameBuffer[74];NOPF();
    PORTB = frameBuffer[75];NOPF();
    PORTB = frameBuffer[76];NOPF();
    PORTB = frameBuffer[77];NOPF();
    PORTB = frameBuffer[78];NOPF();
    PORTB = frameBuffer[79];NOPF();
    PORTB = frameBuffer[80];NOPF();
    PORTB = frameBuffer[81];NOPF();
    PORTB = frameBuffer[82];NOPF();
    PORTB = frameBuffer[83];NOPF();
}



