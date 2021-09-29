//Knight Scanner v3
//By Ashrond
//https://www.programmingelectronics.com/arduino-sketch-with-millis-instead-of-delay/

#include <FastLED.h>
#define NUM_LEDS 72                                  // How many leds in your strip?
//to manage buffer for edge fade out/in, add or subtract a multiple of 2 to the NUM_LEDS
#define NUM_LEDS_WITH_MARGIN	80
#define DATA_PIN 7

#define series1 210                                  //190 2000 for 80's 218 3000 for 2008
#define series2 239                                  //190 2000 for 80's 218 3000 for 2008  
#define delay1 51                                    //Swipe fast out
#define delay2 142                                   //Swipe slow in
#define PulseFreq 15000

// Green, Red, Blue
#define Orange ( 70, 255, 0)
#define Red ( 0, 255, 0)
#define Green ( 255, 0, 0);
#define Blue ( 0, 0, 255)
#define color Red //type the name of the above predefined color you want the scanner to be


unsigned long counter; 
unsigned long pre_counter; 
//int nHoldDelay = 8000;
int nHoldOuterDelay = 1000;
int nHoldInnerDelay = 1000;

//0 = out, 1 = hold, 2 = in
bool bSwipeOut = true;
CRGB leds[NUM_LEDS];                                 // Define the array of leds

void setup() { 
	Serial.begin(57600);
	//Serial.println("resetting");
	LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
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
		//for(int i = 0; i < (NUM_LEDS / 2) + 38; i++) {
		//	leds[i] = CRGB color;                           // Set the color with variable "color" 
		//	blur1d(leds, NUM_LEDS, 15);                  // Apply blur effect to lead the pattern with a few dim LED's

		//	for (uint8_t i=0; i < (NUM_LEDS); i--) {           //shift the position of the first half by half the total length.
		//		leds[i + (NUM_LEDS) + 35] = leds[i];
		//	}
		//	for (uint8_t i = 0; i < NUM_LEDS; i++) {        //Mirror the second half BACK to the first half.
		//		leds[i] = leds[(NUM_LEDS) -1 -i];
		//	}
			
		//	FastLED.show();                                   // Show the leds
		//	fadeall();                                      // Apply fade effect
		//	FastLED.delay(delay1);                                // Speed of cycle, in one direction
		//}
		
		//swipe out
		//initial is 2 led lit up, with partial blur to 1 led on each of their sides
		//in each step of the loop, move the 2 lit leds up by 1 in both right and left direction
		
		//start the loop in the center. 72/2 = 36, but to keep this symmetric we want #36 and #37 lit. actual center is led #36.5
		//since we start with 2 leds instead of 1 lit up in middle, the blur1d will fade it to start as 4 total lit
		for (int j = (NUM_LEDS / 2); j < NUM_LEDS_WITH_MARGIN; j++) {
			int nDistanceFromCenter = j - (NUM_LEDS / 2);
			
			//this allows us to add iterations to the main loop.
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
			FastLED.show();                                 // Show the leds
			fadeall();                                      // Apply fade effect
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
		//for(int i = 0; i < (NUM_LEDS /2) + 38; i++) {
		//	leds[i] = CRGB color;                           // Set the color with variable "color" 
		//	blur1d(leds, NUM_LEDS, 15);                  // Apply blur effect to lead the pattern with a few dim LED's
		//	mirror();                                  // Mirror pattern to other side of the strip
		//	FastLED.show();                                  // Show the leds
		//	fadeall();                                    // Apply fade effect
		//	//set delay speed to same as way out
		//	FastLED.delay(delay1);                               // Speed of cycle, in one direction			
		//}
		
		//almost exact same loop, but instead we start from outer edge and decrement until we hit center
		//for (int k = NUM_LEDS_WITH_MARGIN; k >= (NUM_LEDS / 2); k = k - 1) {
		int k = NUM_LEDS_WITH_MARGIN;
		int nDistanceFromCenter2 = 0;
		while (k >= (NUM_LEDS / 2)) {
			nDistanceFromCenter2 = k - (NUM_LEDS / 2);
			
			//this allows us to add iterations to the main loop.
			//we only set leds as max color when we fully get in from the edges.
			//once that happens, we let this iterate so 
			//the blur1d and fadeall functions can eventually
			//fade everything in from black
			if (nDistanceFromCenter2 >= 0) {							
				if (nDistanceFromCenter2 == (NUM_LEDS / 2)) {
					leds[k] = CRGB color;
					leds[0] = CRGB color;
				} else if (nDistanceFromCenter2 < (NUM_LEDS / 2)) {
					//set both sides, as blur1d can handle this. both indexes guaranteed to be valid leds
					leds[(NUM_LEDS / 2) + nDistanceFromCenter2] = CRGB color;
					leds[(NUM_LEDS / 2) - nDistanceFromCenter2] = CRGB color;
				} else if (nDistanceFromCenter2 > (NUM_LEDS / 2)) {
					//set only right edge, as mirror will be necessary to carry fade inward on left side					
					leds[(NUM_LEDS / 2) + nDistanceFromCenter2] = CRGB color;
					//this avoids attempting to set a full brightness color on an LED with index < 0
					//leds[(NUM_LEDS / 2) - nDistanceFromCenter] = CRGB color;
				}
			}
			
			blur1d(leds, NUM_LEDS, 15);
			//mirror();
			FastLED.show();                                 // Show the leds
			fadeall();                                      // Apply fade effect
			FastLED.delay(delay1);                          // Speed of cycle, in one direction
			k = k - 1;
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

void mirror() {                                     // mirror data to the other half
  for (uint8_t i = 0; i < NUM_LEDS / 2; i++) {
    leds[NUM_LEDS - 1 - i] = leds[i];
  }
}
