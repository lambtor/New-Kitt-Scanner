//Knight Scanner v3
//By Ashrond & Lambtor
//https://www.programmingelectronics.com/arduino-sketch-with-millis-instead-of-delay/

#include <FastLED.h>
#define NUM_LEDS 72                                  // How many leds in your strip?
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

bool bSwipeOut = true;
unsigned long nLastUpdateOut = 0;
unsigned long nLastUpdateIn = 0;
unsigned long counter; 
unsigned long pre_counter; 
uint8_t nSwipeOutIndex = 0;
uint8_t nSwipeInIndex = 0;

CRGB leds[NUM_LEDS];                                 // Define the array of leds

void setup() { 
	Serial.begin(57600);
	//Serial.println("resetting");
	LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
	LEDS.setBrightness(200);	
	//counter=0; 
	//set all LED colors here, as they aren't changing
	for (uint8_t i = 0; i < (NUM_LEDS -1); i++ ) {
		leds[i] = CRGB color;
	}
}

//.nscale8 effects the length of the light trail 190 is 2000 series, 220 appears similar to 3000 series.
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
		
	//if(counter=PulseFreq) {
		//int i = 0;
		
		if (bSwipeOut) {
			//------------------------Swipe Out-----------------------
			Serial.print(" Swipe out");
			//for(int i = 0; i < (NUM_LEDS / 2) + 38; i++) {			
			while (nSwipeOutIndex < (NUM_LEDS / 2) + 38) {
				if (millis() - nLastUpdateOut > delay1) {
					//leds[i] = CRGB color;                           // Set the color with variable "color" 
					blur1d(leds, NUM_LEDS, 15);                  // Apply blur effect to lead the pattern with a few dim LED's

					for (uint8_t i=0; i < (NUM_LEDS); i--) {           //shift the position of the first half by half the total length.
						leds[i + (NUM_LEDS) + 35] = leds[i];
					}
					for (uint8_t i = 0; i < NUM_LEDS; i++) {        //Mirror the second half BACK to the first half.
						leds[i] = leds[(NUM_LEDS) -1 -i];
					}
					FastLED.show();                                   // Show the leds
					fadeall();                                      // Apply fade effect
					nLastUpdateOut = millis();
					nSwipeOutIndex++;
				}
				//FastLED.delay(delay1);                                // Speed of cycle, in one direction
			}
			if (nSwipeOutIndex >= (NUM_LEDS / 2) + 38) {
				bSwipeOut = false;
				nSwipeOutIndex = 0;
			}
		} else {
			
			//------------------------Swipe In------------------------
			Serial.print(" Swipe in");
			while (nSwipeInIndex < (NUM_LEDS / 2) + 38) {
			//for(int i = 0; i < (NUM_LEDS / 2) + 38; i++) {
				if (millis() - nLastUpdateIn > delay2) {
					//leds[i] = CRGB color;                           // Set the color with variable "color" 
					blur1d(leds, NUM_LEDS, 15);                  // Apply blur effect to lead the pattern with a few dim LED's
					mirror();                                  // Mirror pattern to other side of the strip
					FastLED.show();                                  // Show the leds
					fadeall2();                                    // Apply fade effect
					nLastUpdateIn = millis();
					//FastLED.delay(delay2);                               // Speed of cycle, in one direction
					nSwipeInIndex++;
				}			
			}
			if (nSwipeInIndex >= (NUM_LEDS / 2) + 38) {
				bSwipeOut = true;
				nSwipeInIndex = 0;
			}
		}
	//}

	/*
	//------------------------Swipe In------------------------
	if(counter=PulseFreq) { 
	 
		Serial.print(" Swipe in");
		for(int i = 0; i < (NUM_LEDS /2) + 38; i++) {
			leds[i] = CRGB color;                           // Set the color with variable "color" 
			blur1d(leds, NUM_LEDS, 15);                  // Apply blur effect to lead the pattern with a few dim LED's
			mirror();                                  // Mirror pattern to other side of the strip
			FastLED.show();                                  // Show the leds
			fadeall2();                                    // Apply fade effect
			FastLED.delay(delay2);                               // Speed of cycle, in one direction
		}
		
		counter=0; 
	}
	*/
	//------------------------Mirror---------------------------
	Serial.print(" End cycle");
	Serial.print(counter);
}

void mirror() {                                     // mirror data to the other half
	for (uint8_t i = 0; i < NUM_LEDS / 2; i++) {
		leds[NUM_LEDS - 1 - i] = leds[i];
	}
}
