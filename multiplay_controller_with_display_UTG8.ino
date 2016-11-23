/*
 * Multiplay controller 1.1 by HBT
 * Arduino controller for the Windows Multiplay software
 * for cues controls on live shows.
 * Multiplay is available at http://www.da-share.com/software/multiplay/ .
 * 
 */
 
#include "U8glib.h"
#include <HID.h>
#include <Keyboard.h>

#define stop_button 2       // Pin for the STOP button
#define fade_button 3       // Pin for the FADE ALL button
#define play_button 4       // Pin for the PLAY button
#define previous_button 5   // Pin for the PREVIOUS button
#define next_button 6       // Pin for the NEXT button
#define secure_button 7     // Pin for the SECURE button
#define secure_led 13       // Pin for SECURE led
int waiting = 500;          // Delay in ms for keyboard presses after touch.
boolean firstboot = true;   // Flag to signal booting

U8GLIB_SH1106_128X64 u8g(12, 11, 10, 9);	// SW SPI Com: CLK = 12, MOSI = 11, CS = 10, A0 = 9

// Setting up the assignated key on MULTIPLAY to the buttons on the controller. 
int stop_key = 32;        //[space]
int fade_key = 48;        //[0]
int play_key = 46;        //[.]
int previous_key = 217;   //[up arrow]
int next_key = 218;       //[down arrow]

//Functions to draw icons on screen
// Draw SECURE text
void drawSecure(void) {
  u8g.setFont(u8g_font_osb18);
  u8g.drawStr( 9, 48, "SECURE");
}

//Draw Credits 
void drawCredits(void) {
  u8g.setFont(u8g_font_helvB12);
  u8g.drawStr( 21, 17, "MULTIPLAY");
  u8g.drawStr( 2, 35, "Controller");
  u8g.drawStr( 23, 63, "ver 1.1");    //Change version number here
}

//Draw NOT SECURE text
void drawUnsecure(void) {
  //u8g.setFont(u8g_font_unifont);
  u8g.setFont(u8g_font_osb18);
  u8g.drawStr( 34, 24, "NOT");
  u8g.drawStr( 8, 60, "SECURE");
}

//Draw STOP icon
void drawStop(void) {
  u8g.drawBox(35,3, 60,60);
}

//Draw FADE icon
void drawFade(void) {
  u8g.drawLine(30,3 , 64,3);
  u8g.drawLine(64,3 , 98, 61);
}

//Draw PLAY icon
void drawPlay(void) {
  u8g.drawTriangle(34,3 , 94,32, 34,61);
}

//Draw PREVIOUS icon
void drawPrevious(void) {
  u8g.drawTriangle(30,16 , 62,32, 30,48);
  u8g.drawTriangle(64,16 , 96,32, 64,48);
}

//Draw NEXT icon
void drawNext(void) {
  u8g.drawTriangle(62,16 , 30,32, 62,48);
  u8g.drawTriangle(96,16 , 64,32, 96,48);
}

//Draw WARNING icon - not implemented yet
void drawCircle(void) {
  u8g.drawDisc(64, 32, 30);
}

//Clear the screen
void clearScreen(void) {
}

//SETUP
void setup(void) {
  // PULL UP enabled on all inputs, to have them all HIGH except if connected to ground (button press)
  pinMode(stop_button, INPUT_PULLUP);
  pinMode(fade_button, INPUT_PULLUP);
  pinMode(play_button, INPUT_PULLUP);
  pinMode(previous_button, INPUT_PULLUP);
  pinMode(next_button, INPUT_PULLUP);
  pinMode(secure_button, INPUT_PULLUP);
  pinMode(secure_led, OUTPUT);

  Keyboard.begin();
  
  if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
}

//LOOP
void loop(void) {

//BOOT SCREEN
  if(firstboot == true){
    //Draw initial CREDITS on display
    u8g.firstPage();  
    do {
      drawCredits();
    } while( u8g.nextPage() );
    //Waits 3 seconds
    delay(3000);
    //Set firstboot variable to false to avoid looping all over credits again
    firstboot = false;
    //Clear the screen
    u8g.firstPage();  
    do {
      clearScreen();
    } while( u8g.nextPage() );
    delay(1000);
    }
    
//SECURE MODE
  //Check if the secure button is unpressed, to avoid sending keyboard stroke on accidental one button press.
  if(digitalRead(secure_button)==HIGH){
    //Turn secure led off 
    digitalWrite(secure_led, LOW);
    
    //Draw SECURE on display
    u8g.firstPage();  
    do {
      drawSecure();
    } while( u8g.nextPage() );    
  }
  
// UNSECURE MODE - Allows PLAY, STOP and FADE press
  //If secure button is pressed
  if(digitalRead(secure_button)==LOW){
     //Turn on secure led to warn the secure button is pressed.
    digitalWrite(secure_led, HIGH);
    
    //Draw NOT SECURE on display
    u8g.firstPage();  
    do {
      drawUnsecure();
    } while( u8g.nextPage() );
    
    /*//Draw WARNING circle on display
    u8g.firstPage();  
    do {
      drawCircle();
    } while( u8g.nextPage() );
    */ 
    
// STOP - Stops all cues
    //If STOP button is pressed
    while(digitalRead(stop_button)==LOW){
      //Draw STOP icon on display
      u8g.firstPage();  
      do {
        drawStop();
      } while( u8g.nextPage() );
      //Send stop_key keystroke
      keystroke(stop_key, waiting);
    }
    
// FADE - Fade out all cues
    //If FADE button is pressed
    while(digitalRead(fade_button)==LOW){
      //Draw FADE icon on display
      u8g.firstPage();  
      do {
        drawFade();
      } while( u8g.nextPage() );
      //Send fade_key keystroke
      keystroke(fade_key, waiting);
    } 
    
//PLAY - Play selected cue
    //If PLAY button is pressed
    while(digitalRead(play_button)==LOW){

      //Draw PLAY icon on display
      u8g.firstPage();  
      do {
        drawPlay();
      } while( u8g.nextPage() );
      //Send play_key keystroke
      keystroke(play_key, waiting);
    }
  }
  
// PREVIOUS - Move up on the cue list
  //If PREVIOUS button is pressed
  while(digitalRead(previous_button)==LOW){
  
    //Draw PREVIOUS icon on display
    u8g.firstPage();  
    do {
      drawPrevious();
    } while( u8g.nextPage() );
    //Send previous_key keystroke
    keystroke(previous_key, waiting);
  } 

//NEXT - Move down on the cue list
  //If NEXT button is pressed
  while(digitalRead(next_button)==LOW){
    
    //Draw NEXT icon on display 
    u8g.firstPage();  
    do {
      drawNext();
    } while( u8g.nextPage() );
    //Send next_key keystroke
    keystroke(next_key, waiting);
  } 
}

//KEYSTROKE
//Function to invoke key press with delay dly to debounce key press
void keystroke(int key, int dly){
    Keyboard.write(key);
    delay(dly);
}


