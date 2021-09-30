//Knight Scanner v3
//By Ashrond
//https://www.programmingelectronics.com/arduino-sketch-with-millis-instead-of-delay/

#include <FastLED.h>
#define NUM_LEDS 72                                  // How many leds in your strip?
//to manage buffer for edge fade out/in, add or subtract a multiple of 2 to the NUM_LEDS
#define NUM_LEDS_WITH_MARGIN	136
#define DATA_PIN 7

#define series1 210                                  //190 2000 for 80's 218 3000 for 2008
#define series2 239                                  //190 2000 for 80's 218 3000 for 2008  
#define delay1 51                                    //Swipe fast out
#define delay2 142                                   //Swipe slow in
#define PulseFreq 15000

// Green, Red, Blue
#define Orange (70, 255, 0)
#define Red (0, 255, 0)
#define Green (255, 0, 0)
#define Blue (0, 0, 255)
#define color Red //type the name of the above predefined color you want the scanner to be


unsigned long counter; 
unsigned long pre_counter; 
//int nHoldDelay = 8000;
int nHoldOuterDelay = 1000;
int nHoldInnerDelay = 1000;
//need this 
int nAnimationSwipeFramesCount = NUM_LEDS_WITH_MARGIN - (NUM_LEDS / 2);
int nAnimationStartupFramesCount = NUM_LEDS_WITH_MARGIN - NUM_LEDS;
//int nAnimationTotalFramesCount = nAnimationSwipeFramesCount + nAnimationStartupFramesCount;

//0 = out, 1 = hold, 2 = in
bool bSwipeOut = true;

CRGB leds[NUM_LEDS];                                 // Define the array of leds


void setup() { 

	Serial.begin(57600);
	//Serial.println("resetting");
	LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds, NUM_LEDS);
	LEDS.setBrightness(200);
	//ensure all LEDS are off at start
	FastLED.clear(true);
	
  //counter=0; 
}

//.nscale8 effects the lenght of the light trail 190 is 2000 series, 220 appears similar to 3000 series.
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(series1); } }   //Swipe in
void fadeall2() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(series2); } }  //Swipe out

void loop() { 
	//delay(PulseFreq);                              // Scan pattern delay
	//static uint8_t hue = 255;
	system_tick();
	Serial.print(" Begin cycle");
}
 
void system_tick() { 
    //counter=millis();
	
    if (bSwipeOut) {	  
		//------------------------Swipe Out-----------------------
		Serial.print(" Swipe out");
		
		//startup is now 2 leds fading in, with partial blur to 1 led on each of their sides
		//fade these in	by just repeatedly calling fade function a decreasing number of times between show()
		for (int nStartupIndex = 0; nStartupIndex < nAnimationStartupFramesCount; nStartupIndex++) {
			//if (nStartupIndex == 0) {
			leds[NUM_LEDS / 2] = CRGB color;
			leds[(NUM_LEDS / 2) + 1] = CRGB color;
			blur1d(leds, NUM_LEDS, 15);
			//}
				
			//fade and blur the 2 lit LEDs gradually
			//this is done by having each frame blur
			for (int nTemp = 0; nTemp < (nAnimationStartupFramesCount - nStartupIndex); nTemp++) {				
				fadeall();                                      // Apply fade effect
			}
			
			FastLED.show();                                 // Show the leds
			FastLED.delay(delay1);                          // Speed of cycle, in one direction
		}
		
		//in each step of the loop, move the 2 lit leds up by 1 in both right and left direction
		
		//start the loop in the center. 72/2 = 36, but to keep this symmetric we want #36 and #37 lit. actual center is led #36.5
		//since we start with 2 leds instead of 1 lit up in middle, the blur1d will fade it to start as 4 total lit
		//this margin value allows us to add iterations to the main loop, without having any leds set at max brightness
		for (int j = (NUM_LEDS / 2); j <= NUM_LEDS_WITH_MARGIN; j++) {
			int nDistanceFromCenter = j - (NUM_LEDS / 2);
			
			//we only set leds as max color until we get to the edges.
			//once that happens, we let this iterate so 
			//the blur1d and fadeall functions can eventually
			//fade everything to black - effectively spreading the swipe effect beyond the LED edges
			if (nDistanceFromCenter <= (NUM_LEDS / 2)) {							
				if (nDistanceFromCenter == 0) {
					leds[j] = CRGB color;
					leds[j+1] = CRGB color;
				} else {
					//mirror the outer right edge to outer left
					leds[(NUM_LEDS / 2) + nDistanceFromCenter] = CRGB color;
					leds[(NUM_LEDS / 2) - nDistanceFromCenter] = CRGB color;
				}				
			}
			
			blur1d(leds, NUM_LEDS, 15);				
			fadeall();                                      // Apply fade effect
			FastLED.show();                                 // Show the leds
			
			FastLED.delay(delay1);                          // Speed of cycle, in one direction
		}
		
		bSwipeOut = false;
		//ensure all lights off before next swipe cycle
		FastLED.clear(true);
		//this is edge delay before cycling back toward center
		//this, if uncommented, lets you add time between cycle halves
		//FastLED.delay(nHoldOuterDelay);
	} else { 
		 
		Serial.print(" Swipe in");
		
		//major bug in fastLED library doesn't allow fading with a decrementing loop?? wtf		
		for (int i = 0; i < nAnimationSwipeFramesCount; i++) {
			//we can only assign to the right side.
			//otherwise fastled will error out when we try to blur out from a negative led index (before beginning)
			leds[(NUM_LEDS / 2) + (nAnimationSwipeFramesCount - i)] = CRGB color;
			
			//reverse order = max size - current frame# 
			//this allows the i index in the loop to go up while visible LED display goes "backwards"
			
			blur1d(leds, NUM_LEDS, 15);				
			fadeall();                                      // Apply fade effect
			mirrorToLeft();
			FastLED.show();                                 // Show the leds
			FastLED.delay(delay1);                          // Speed of cycle, in one direction
		}	
		
		//leds have hit center. fade them out now.
		for (int nStartupIndex = 0; nStartupIndex < nAnimationStartupFramesCount; nStartupIndex++) {							
			//fade down the 2 center lit LEDs gradually		
			fadeall();                                      // Apply fade effect
			
			FastLED.show();                                 // Show the leds
			FastLED.delay(delay1);                          // Speed of cycle, in one direction
		}
		
		bSwipeOut = true;
		//ensure all lights off before next swipe cycle
		FastLED.clear(true);
		//FastLED.delay(nHoldInnerDelay);	
	}
	//------------------------Mirror---------------------------
	Serial.print(" End cycle");
	//Serial.print(counter);
}

void mirrorToRight() {                                     // mirror data to the other half
	for (uint8_t i = 0; i < NUM_LEDS / 2; i++) {
		leds[NUM_LEDS - 1 - i] = leds[i];
	}
}

void mirrorToLeft() {
	for (uint8_t i = 0; i < NUM_LEDS / 2; i++) {
		leds[i] = leds[NUM_LEDS - 1 - i] ;
	}
}
