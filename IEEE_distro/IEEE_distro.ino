/* Code Created by Christopher Jones
   For use of BYU IEEE student branch
   Last revised 11/15/2016 */


/*
 * We are doing things a little differently.
 *
 * uint16_t leds stores which leds will be on with the bits representing these LEDs:
 * 0  1  2  3
 * 4  5  6  7
 * 8  9  10 11
 * 12 13 14 15
 *
 * uint8_t level stores which level of LEDs will be lit up
 * 0 = lowest
 * 3 = highest. The 4 MSB will be ignored.
 *
 * void light_leds(uint16_t leds) will take the leds variable and map them to the
 * correct leds on the board
 *
 * void light_level(uint8_t level) will take the level variable and map it to the 
 * correct level of the board.
 */
void light_leds(uint16_t leds);
void light_level(uint8_t level);
void my_frame(uint16_t the_leds[], int duration);
void box_up(int speed);

///////////////////////////////////////////////////////////////////////////////////
                              //INITIAL SETUP//
///////////////////////////////////////////////////////////////////////////////////

//Set all of the pins we will be using to output mode to get maximum current
void setup() 
{
  // Every single pin of the board is an output
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A0,OUTPUT);
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
                          //MAIN LOOP FOR ANIMATIONS//
///////////////////////////////////////////////////////////////////////////////////
void loop_through_leds(){
  int i = 0;

  while(i<8){
    PORTB = 0x1 << i++;
    delay(400);
  }
  PORTB = 0;
  i = 0;
  while(i<8){
    PORTD = 0x1 << i++;
    delay(800);
  }
  i = 0;
}

void loop() 
{
  light_leds(0xFFFF);
  
  light_level(0b00000001);
  delay(400);
  light_level(0b00000010);
  delay(400);
  light_level(0b00000100);
  delay(400);
  light_level(0b00001000);
  delay(400);
/*
// top row on
  PORTC = 0x1C;
  loop_through_leds();
  PORTC = 0x2C;
  loop_through_leds();
  PORTC = 0x34;
  loop_through_leds();
  PORTC = 0x38;
  loop_through_leds();

//  PORTB = 0xFF;
//  PORTD = 0xFF;
  */
  //uint16_t superleds[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
  //my_frame(superleds, 20);


  uint16_t a = 0;
  uint16_t b = 0;
  uint16_t c = 0;
  uint16_t d = 1;

  do{
    uint16_t why[4] = {a, b, c, d};
    my_frame(why,30);
    d = d << 1;
  } while(d != 0);

  c = 1;
  do{
    uint16_t why[4] = {a, b, c, d};
    my_frame(why,30);
    c = c << 1;
  } while(c != 0);

  b = 1;
  do{
    uint16_t why[4] = {a, b, c, d};
    my_frame(why,30);
    b = b << 1;
  } while(b != 0);

  a = 1;
  do{
    uint16_t why[4] = {a, b, c, d};
    my_frame(why,30);
    a = a << 1;
  } while(a != 0);
  
//  box_up(200);



}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
                      //HELPER FUNCTION DEFINITIONS//
///////////////////////////////////////////////////////////////////////////////////
void my_frame(uint16_t the_leds[], int duration){
  for(int i=0; i<duration; i++){
    for(int j=0; j<4; j++){
      light_level(0b00000001 << j);
      light_leds(the_leds[j]);
      delay(1);
    }
  }  
}

// level is high-asserted on the lower 4 bits.
// PORTC is low-asserted on bits 2-5
void light_level(uint8_t level){
  PORTC = (PORTC | 0b11111100) & ~(level << 2);
}

// leds is high asserted, as well as the actual
// led signals, but we have a mess on our hands now
void light_leds(uint16_t leds){
  PORTB = (PORTB & 0b11011111) | ((leds << 5) & 0b00100000); // PORTB[5] = leds[0]
  PORTB = (PORTB & 0b11110111) | ((leds << 2) & 0b00001000); // PORTB[3] = leds[1]
  PORTD = (PORTD & 0b11111011) | ((leds << 0) & 0b00000100); // PORTD[2] = leds[2]
  PORTD = (PORTD & 0b11111110) | ((leds >> 3) & 0b00000001); // PORTD[0] = leds[3]

  PORTB = (PORTB & 0b11101111) | ((leds << 0) & 0b00010000); // PORTB[4] = leds[4]
  PORTB = (PORTB & 0b11111011) | ((leds >> 3) & 0b00000100); // PORTB[2] = leds[5]
  PORTD = (PORTD & 0b11101111) | ((leds >> 2) & 0b00010000); // PORTD[4] = leds[6]
  PORTD = (PORTD & 0b11111101) | ((leds >> 6) & 0b00000010); // PORTD[1] = leds[7]

  PORTB = (PORTB & 0b11111101) | ((leds >> 7) & 0b00000010); // PORTB[1] = leds[8]
  PORTD = (PORTD & 0b10111111) | ((leds >> 3) & 0b01000000); // PORTD[6] = leds[9]
  PORTD = (PORTD & 0b11011111) | ((leds >> 5) & 0b00100000); // PORTD[5] = leds[10]
  PORTD = (PORTD & 0b11110111) | ((leds >> 8) & 0b00001000); // PORTD[3] = leds[11]

  PORTB = (PORTB & 0b11111110) | ((leds >>12) & 0b00000001); // PORTB[0] = leds[12]
  PORTD = (PORTD & 0b01111111) | ((leds >> 6) & 0b10000000); // PORTD[7] = leds[13]
  PORTC = (PORTC & 0b11111110) | ((leds >>14) & 0b00000001); // PORTC[0] = leds[14]
  PORTC = (PORTC & 0b11111101) | ((leds >>14) & 0b00000010); // PORTC[1] = leds[15]
}

// This is to make a box go up and down
void box_up(int speed){
  
  uint16_t why[4] = {0x0000, 0x0000, 0x0000, 0xFFFF};
  my_frame(why,speed);
  delay(speed);
  uint16_t why2[4] = {0x0000,0x0000,0xFFFF,0x0000};
  my_frame(why, speed);
  delay(speed);
  uint16_t why3[4] = {0x0000,0xFFFF,0x0000,0x0000};
  my_frame(why, speed);
  delay(speed);
  uint16_t why4[4] = {0xFFFF,0x0000,0x0000,0x0000};
  my_frame(why, speed);
  delay(speed);
}
///////////////////////////////////////////////////////////////////////////////////

