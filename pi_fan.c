/*
 * pin_fan.c:
 *	Turn pin 17 (3v) on and off based on the temperature of the Pi, reported by the command:
 *	      /opt/vc/bin/vcgencmd measure_temp
 *	This could be used to turn a fan on and off to keep the Pi cool. 
 *
 *	This program reads the stdout of 'vcgencmd measure_temp' from stdin and turns on pin 17
 *	if the temp is >= 30C
 *
 * Note:
 * 	Do not run a fan from this pin, use a relay or mosfet from a power supply that can run
 *      the fan you want.
 *
 * Example temp string:
 *  temp=37.9'C
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define PIN_17 0
#define INDEX_OF_START_OF_TEMP 6

int main(int argumentCount, char *argumentVector[])
{
    char line[BUFSIZ], c, *tempStart;
    float temperature = 0.f;
    int index = INDEX_OF_START_OF_TEMP,
        endIndex = 0;
    size_t length = 0;

    printf("Raspberry Pi: Turn on/off pin 17, based on vcgencmd measure_temp, input. \n");
    
    fgets(line, BUFSIZ, stdin);
    length = strlen(line);

    printf("vcgencmd temp reported as: %s\n", line);
    
    while( (c = line[index++]) != '\0' ){
        if(index >= length){
            return 1;
        }else if(c > '9' || c < '0' && c != '.'){
            line[--index] = '\0';
            break;
        }
    }
    tempStart = line + (INDEX_OF_START_OF_TEMP-1);
    temperature = atof(tempStart);

    printf("Parsed the temperature as '%s', %1f\n", tempStart, temperature);

    if( temperature <= 0 ){
        printf("Nothing to do.");
        return 0;
    }
    
    wiringPiSetup();
    pinMode(PIN_17, OUTPUT);

    if( temperature > 29.f ){
        digitalWrite (PIN_17, HIGH);// On
    }else{
        digitalWrite (PIN_17, LOW); // Off
    }

    return 0;
}
