/* Code Created by Christopher Jones
   For use of BYU IEEE student branch
   Last revised 11/15/2016 */

///////////////////////////////////////////////////////////////////////////////////
                                //PREFACE//
///////////////////////////////////////////////////////////////////////////////////

/*
Please note that the operation of this code is dependent upon the physical construction
of the LED cube. There are multiple ways to build and wire your cube but assuming that
you are using common anodes for columns and common cathodes for layers, the cube can be 
wired to work with this code. The simple image below shows a top view of how my cube was 
connected and how yours must be if you would like to use this code as is. The pins on the
arduino that should be connected to the columns are shown in the image below. The other
image shows a side view and shows the ground pin connections.

 **Top view of cube**       **Side view of cube**
  O    O    O    O                  0
   3    7    11   A5                  A3
  O    O    O    O                  0
   2    6    10   A4                  A2
  O    O    O    O                  0
   1    5    9    13                  A1
  O    O    O    O                  0
   0    4    8    12                  A0
                   
  **Front of cube**         **Bottom of the cube**                 

Two bytes are used to specify which columns recieve power at any given time. Byte 1 will
specify the columns in the left half of the cube, and byte 2 will specify the right half
of the cube. If I wanted to turn on just the LED in the upper left corner I would specify 
the bytes B00001000 and B00000000 where a 1 corresponds to on or "high" and 0 is off or "low."
This can be accomplished using the function: ledSelect(byte1, byte2)

Now in order to actually turn on the LED we also have to ground the layer that the LED is 
located in. To do this, we simply pull the layer low. This is specified with a single byte
where the lower 4 bits are the layers and the upper 4 bits are always high or 1.So if we 
wanted to turn on the LED in the top left corner in the first layer we would specify the
byte b11111110 to ground layer 1. This can be accomplished using the function: layerSelect(byte)

So overall if we wanted to turn on the upper left corner led in the 1st layer, we would type
the following code:

ledSelect(B00001000,B00000000);
layerSelect(B11111110);

Of course these functions can turn on as many LED's in a layer as desired, for example to turn 
on all the LED's in the SECOND layer, we would type the code:

ledSelect(B11111111,B11111111);
layerSelect(B11111101);


To greatly simplify the process of coding animations, we will code an entire frame at a time using
the function: 

frame(byte L1a, byte L1b, byte L2a, byte L2b, byte L3a, byte L3b, byte L4a, byte L4b, int duration);

This takes in 8 Bytes to specify all the LED's in the cube and a relative duration for the frame 
using an integer. Note that byte L1a specifies layer 1, half "a" (left half in the image), L1b specifes
layer 1, half "b" (right half in the image) and so forth. This function uses the ledSelect and layerSelect
functions so you shouldn't need to use those functios directly. In this manner we can write animation 
functions which simply have a collection of sequential "frames" 
*/
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
                              //INITIAL SETUP//
///////////////////////////////////////////////////////////////////////////////////

//Set all of the pins we will be using to output mode to get maximum current
void setup() 
{
  //Columns of the LED cube
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
  
  //Layers of the LED cube
  pinMode(A3,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A0,OUTPUT);
}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
                    //HELPER FUNCTIONS (DECLARATIONS)//
///////////////////////////////////////////////////////////////////////////////////

//This designates which of the power columns to turn on
void ledSelect(byte D, byte B );

//This will pull the specified layer down to 0 (i.e B11111110 will ground layer 1)
void layerSelect(byte x);

//Function to draw a single frame for a specified duration (The bytes specify which LED's to turn on)
//Example: L2a specifies which LED's in layer 2, half a, to turn on
void frame(byte L1a, byte L1b, byte L2a, byte L2b, byte L3a, byte L3b, byte L4a, byte L4b, int duration);

///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
                         //ANIMATIONS (DECLARATIONS)//
///////////////////////////////////////////////////////////////////////////////////

//Makes boxes that shrink and grow into and out of corners of the cube (rate default:40)
void squareAnimation(int rate); 

//Lights up the entire cube one LED at a time in a sequential line (rate default:20)
void line(int rate);

//Simply flashed the entire cube once (duration default:50)
void flashing(int duration);   

//Fade in the whole cube from corners
void fadeIn(int rate);


///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
                          //MAIN LOOP FOR ANIMATIONS//
///////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  PORTC = 0x1F;
  PORTB = 0xFF;
  PORTD = 0xFF;
  
//  //First Animation//
//  for (int i = 0; i <1; i++)
//  {
//    line(20);
//  }
//  delay(300);
  
//  //Second Animation//
//  for (int i = 0; i <4; i++)
//  {
//     flashing(40);
//     delay(100);
//  }
//  delay(300);
  
//  //Third Animation//
//  for (int i = 0; i <4; i++)
//  {
//     squareAnimation(40);
//  }
//  delay(300);
  
//  //Fourth Animation//
//  for (int i = 0; i <1; i++)
//  {
//    fadeIn(20);
//  }
//  delay(300);

}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
                      //HELPER FUNCTION DEFINITIONS//
///////////////////////////////////////////////////////////////////////////////////

//These bit masks help set the ports in ledSelect and layerSelect
byte mask1 = B00111111;  //"and-ing" with this mask will preserve the lower 6 bits of a byte and zero the rest
byte mask2 = B00110000;  //"and-ing" with this mask will preserve only the 2 MSB and zero the rest
byte mask3 = B00001111;  //"or-ing" with this mask will set the lower 4 bits to 1 and leave the rest untouched
byte mask4 = B11000000;  //"and-ing" with this mask will preserve the uper 2 bits

//This designates which of the power columns to turn on
void ledSelect(byte D, byte B )
{
  PORTC = ((B>>2 & mask2)>>4)| 0x3C ;
//  PORTC = ((B>>2 & mask2))|mask3;
  PORTD = D;
  PORTB = (PORTB & mask4) | (B & mask1);
}

//This will pull the specified layer down to 0
void layerSelect(byte x)
{
// for the first layer: layerSelect(B11111101);
// for the second layer: layerSelect(B11111101);
// etc
  PORTC = (PORTC | mask3) & ((x >> 2) | 0xF0);
//  PORTC = (PORTC | mask3) & x;
}

//This function specifies the LED's to turn on in the entire cube as a "frame" for a specified, relative duration
  void frame(byte L1a, byte L1b, byte L2a, byte L2b, byte L3a, byte L3b, byte L4a, byte L4b, int duration)
{
  //The loop helps to give a specified period for which the "frame" is displayed on the cube
  for (int i = 0; i <duration; i++)
  {
    ledSelect(L1a,L1b);      //Power the columns of the LED's we want to turn on in layer 1
    layerSelect(B11111110);  //Ground layer 1
    delay(1);
    ledSelect(L2a,L2b);      //Power the columns of the LED's we want to turn on in layer 2   
    layerSelect(B11111101);  //Ground layer 2
    delay(1);
    ledSelect(L3a,L3b);      //Power the columns of the LED's we want to turn on in layer 3
    layerSelect(B11111011);  //Ground layer 3
    delay(1);
    ledSelect(L4a,L4b);      //Power the columns of the LED's we want to turn on in layer 4
    layerSelect(B11110111);  //Ground layer 4
    delay(1);
  } 
  //turn off all LED's
  layerSelect(B11111111);
}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
                           //ANIMATION FRAMES//
///////////////////////////////////////////////////////////////////////////////////


//Template
//  frame(B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);


void flashing(int duration)
{
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,duration);
  delay(200);
}

void fadeIn(int rate)
{
  int frame_duration = rate;
  frame(B00000001,B00000000,B00000001,B00000000,B00000001,B00000000,B00000001,B00000000,frame_duration);
  frame(B00010011,B00000000,B00010011,B00000000,B00010011,B00000000,B00010011,B00000000,frame_duration);
  frame(B00110111,B00000001,B00110111,B00000001,B00110111,B00000001,B00110111,B00000001,frame_duration);
  frame(B01111111,B00010011,B01111111,B00010011,B01111111,B00010011,B01111111,B00010011,frame_duration);
  frame(B11111111,B00110111,B11111111,B00110111,B11111111,B00110111,B11111111,B00110111,frame_duration);
  frame(B11111111,B01111111,B11111111,B01111111,B11111111,B01111111,B11111111,B01111111,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,2*frame_duration);
 
  frame(~B00000001,~B00000000,~B00000001,~B00000000,~B00000001,~B00000000,~B00000001,~B00000000,frame_duration);
  frame(~B00010011,~B00000000,~B00010011,~B00000000,~B00010011,~B00000000,~B00010011,~B00000000,frame_duration);
  frame(~B00110111,~B00000001,~B00110111,~B00000001,~B00110111,~B00000001,~B00110111,~B00000001,frame_duration);
  frame(~B01111111,~B00010011,~B01111111,~B00010011,~B01111111,~B00010011,~B01111111,~B00010011,frame_duration);
  frame(~B11111111,~B00110111,~B11111111,~B00110111,~B11111111,~B00110111,~B11111111,~B00110111,frame_duration);
  frame(~B11111111,~B01111111,~B11111111,~B01111111,~B11111111,~B01111111,~B11111111,~B01111111,frame_duration);
  frame(~B11111111,~B11111111,~B11111111,~B11111111,~B11111111,~B11111111,~B11111111,~B11111111,2*frame_duration);
  
  frame(~B11111111,~B01111111,~B11111111,~B01111111,~B11111111,~B01111111,~B11111111,~B01111111,frame_duration);
  frame(~B11111111,~B00110111,~B11111111,~B00110111,~B11111111,~B00110111,~B11111111,~B00110111,frame_duration);
  frame(~B01111111,~B00010011,~B01111111,~B00010011,~B01111111,~B00010011,~B01111111,~B00010011,frame_duration);
  frame(~B00110111,~B00000001,~B00110111,~B00000001,~B00110111,~B00000001,~B00110111,~B00000001,frame_duration);
  frame(~B00010011,~B00000000,~B00010011,~B00000000,~B00010011,~B00000000,~B00010011,~B00000000,frame_duration);
  frame(~B00000001,~B00000000,~B00000001,~B00000000,~B00000001,~B00000000,~B00000001,~B00000000,frame_duration);
  frame(~B00000000,~B00000000,~B00000000,~B00000000,~B00000000,~B00000000,~B00000000,~B00000000,2*frame_duration);

  frame(B11111111,B01111111,B11111111,B01111111,B11111111,B01111111,B11111111,B01111111,frame_duration);
  frame(B11111111,B00110111,B11111111,B00110111,B11111111,B00110111,B11111111,B00110111,frame_duration);
  frame(B01111111,B00010011,B01111111,B00010011,B01111111,B00010011,B01111111,B00010011,frame_duration);
  frame(B00110111,B00000001,B00110111,B00000001,B00110111,B00000001,B00110111,B00000001,frame_duration);
  frame(B00010011,B00000000,B00010011,B00000000,B00010011,B00000000,B00010011,B00000000,frame_duration);
  frame(B00000001,B00000000,B00000001,B00000000,B00000001,B00000000,B00000001,B00000000,frame_duration);
  frame(B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,2*frame_duration);
  
  frame(B00000000,B00010000,B00000000,B00010000,B00000000,B00010000,B00000000,B00010000,frame_duration);
  frame(B00000000,B00110001,B00000000,B00110001,B00000000,B00110001,B00000000,B00110001,frame_duration);
  frame(B00010000,B01110011,B00010000,B01110011,B00010000,B01110011,B00010000,B01110011,frame_duration);
  frame(B00110001,B11110111,B00110001,B11110111,B00110001,B11110111,B00110001,B11110111,frame_duration);
  frame(B01110011,B11111111,B01110011,B11111111,B01110011,B11111111,B01110011,B11111111,frame_duration);
  frame(B11110111,B11111111,B11110111,B11111111,B11110111,B11111111,B11110111,B11111111,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,2*frame_duration);
  
  frame(~B00000000,~B00010000,~B00000000,~B00010000,~B00000000,~B00010000,~B00000000,~B00010000,frame_duration);
  frame(~B00000000,~B00110001,~B00000000,~B00110001,~B00000000,~B00110001,~B00000000,~B00110001,frame_duration);
  frame(~B00010000,~B01110011,~B00010000,~B01110011,~B00010000,~B01110011,~B00010000,~B01110011,frame_duration);
  frame(~B00110001,~B11110111,~B00110001,~B11110111,~B00110001,~B11110111,~B00110001,~B11110111,frame_duration);
  frame(~B01110011,~B11111111,~B01110011,~B11111111,~B01110011,~B11111111,~B01110011,~B11111111,frame_duration);
  frame(~B11110111,~B11111111,~B11110111,~B11111111,~B11110111,~B11111111,~B11110111,~B11111111,frame_duration);
  frame(~B11111111,~B11111111,~B11111111,~B11111111,~B11111111,~B11111111,~B11111111,~B11111111,2*frame_duration);
  
  frame(~B11110111,~B11111111,~B11110111,~B11111111,~B11110111,~B11111111,~B11110111,~B11111111,frame_duration);  
  frame(~B01110011,~B11111111,~B01110011,~B11111111,~B01110011,~B11111111,~B01110011,~B11111111,frame_duration);
  frame(~B00110001,~B11110111,~B00110001,~B11110111,~B00110001,~B11110111,~B00110001,~B11110111,frame_duration);
  frame(~B00010000,~B01110011,~B00010000,~B01110011,~B00010000,~B01110011,~B00010000,~B01110011,frame_duration);
  frame(~B00000000,~B00110001,~B00000000,~B00110001,~B00000000,~B00110001,~B00000000,~B00110001,frame_duration);
  frame(~B00000000,~B00010000,~B00000000,~B00010000,~B00000000,~B00010000,~B00000000,~B00010000,frame_duration);
  frame(~B00000000,~B00000000,~B00000000,~B00000000,~B00000000,~B00000000,~B00000000,~B00000000,2*frame_duration);

  frame(B11110111,B11111111,B11110111,B11111111,B11110111,B11111111,B11110111,B11111111,frame_duration);  
  frame(B01110011,B11111111,B01110011,B11111111,B01110011,B11111111,B01110011,B11111111,frame_duration);
  frame(B00110001,B11110111,B00110001,B11110111,B00110001,B11110111,B00110001,B11110111,frame_duration);
  frame(B00010000,B01110011,B00010000,B01110011,B00010000,B01110011,B00010000,B01110011,frame_duration);
  frame(B00000000,B00110001,B00000000,B00110001,B00000000,B00110001,B00000000,B00110001,frame_duration);
  frame(B00000000,B00010000,B00000000,B00010000,B00000000,B00010000,B00000000,B00010000,frame_duration);
  frame(B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,2*frame_duration);
}

void squareAnimation(int rate)
{
  int frame_duration = rate;
  frame(B10011111,B11111001,B00001001,B10010000,B00001001,B10010000,B10011111,B11111001,frame_duration);
  frame(B11100000,B11101010,B10100000,B10100000,B11100000,B11101010,B00000000,B00000000,frame_duration);
  frame(B00000000,B11001100,B00000000,B11001100,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11100000,B11101010,B10100000,B10100000,B11100000,B11101010,B00000000,B00000000,frame_duration);
  frame(B10011111,B11111001,B00001001,B10010000,B00001001,B10010000,B10011111,B11111001,frame_duration);
  frame(B00000000,B00000000,B01010111,B00000111,B00000101,B00000101,B01010111,B00000111,frame_duration);
  frame(B00000000,B00000000,B00000000,B00000000,B00110011,B00000000,B00110011,B00000000,frame_duration);
  frame(B00000000,B00000000,B01010111,B00000111,B00000101,B00000101,B01010111,B00000111,frame_duration);
}

void line(int rate)
{
  int frame_duration = rate;
  frame(B00000001,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B00000011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B00000111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B00001111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B10001111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11001111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11101111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  
  frame(B11111111,B00000001,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B00000011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B00000111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B00001111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B10001111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11001111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11101111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  
  frame(B11111111,B11111111,B00000000,B00010000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B00000000,B00110000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B00000000,B01110000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B00000000,B11110000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B00000000,B11111000,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B00000000,B11111100,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B00000000,B11111110,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B00000000,B11111111,B00000000,B00000000,B00000000,B00000000,frame_duration);
  
  frame(B11111111,B11111111,B00010000,B11111111,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B00110000,B11111111,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B01110000,B11111111,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11110000,B11111111,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111000,B11111111,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111100,B11111111,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111110,B11111111,B00000000,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,frame_duration);
  
  frame(B11111111,B11111111,B11111111,B11111111,B00000001,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B00000011,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B00000111,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B00001111,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B10001111,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11001111,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11101111,B00000000,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B00000000,B00000000,frame_duration);
  
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B00000001,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B00000011,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B00000111,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B00001111,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B10001111,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11001111,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11101111,B00000000,B00000000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B00000000,frame_duration);
  
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B00010000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B00110000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B01110000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B11110000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B11111000,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B11111100,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B11111110,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000000,B11111111,frame_duration);
  
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00010000,B11111111,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00110000,B11111111,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B01110000,B11111111,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,B11111111,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111000,B11111111,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111100,B11111111,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B11111111,frame_duration);
  frame(B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,frame_duration*10);
}
