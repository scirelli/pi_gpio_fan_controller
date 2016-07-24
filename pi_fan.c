/*
 * pin_fan.c:
 *	Turn pin 17 (3v) on and off based on the temperature of the Pi, reported by the command:
 *	      /opt/vc/bin/vcgencmd measure_temp
 *	This could be used to turn a fan on and off to keep the Pi cool. 
 *
 *	This program reads the stdout of 'vcgencmd measure_temp' from stdin and turns on pin 17
 *	if the temp is >= 30C
 * 
 * To configure the temperature at which the pin is turned on or off create a config file with a single number.
 * That number will be read in as the max temperature. By default the program will look for a file called config.conf
 * in the working directory. Or you can pass the file path as the first paramater.
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
#define INDEX_OF_START_OF_TEMP 5
#define MAX_TEMP 29.f

float readMaxTempFromConfigFile(const char *filePath){
    FILE *filePointer;
    char line[BUFSIZ];
    float temperature = 0;
    
    if(filePath == NULL) {
        filePath = "./config.conf";
    }

    filePointer = fopen(filePath, "r");
    if( filePointer == NULL ){
        printf("Could not find config file.");
        return 0;
    }

    fgets(line, BUFSIZ, filePointer);
    temperature = atof(line);
    printf("Reading max temp from config file '%s'.\n", line);
    printf("Parsed as '%f'.\n", temperature);

    if(temperature <= 0){
        temperature = MAX_TEMP;
        printf("Using '%f' instead.", temperature);
    }

    return temperature;
}

int main(int argumentCount, char *argumentVector[])
{
    char line[BUFSIZ], c, *tempStart;
    float currentTemperature = 0.f,
          maxTemperature = 0.f;
    int index = INDEX_OF_START_OF_TEMP,
        endIndex = 0;
    size_t length = 0;

    printf("---------------------------------------------------------\nTurn on/off pin 17, based on vcgencmd measure_temp, input.\n---------------------------------------------------------\n\n");
    
    fgets(line, BUFSIZ, stdin);
    length = strlen(line);

    printf("vcgencmd temp reported as: %s\n", line);
    
    while( (c = line[index++]) != '\0' ){
        if(index >= length){
            printf("Could not parse vcgencmd output. 1");
            return EXIT_FAILURE;
        }else if(c > '9' || c < '0' && c != '.'){
            line[--index] = '\0';
            break;
        }
    }

    if( INDEX_OF_START_OF_TEMP >= length ){
        printf("Could not parse vcgencmd output. 2");
        return EXIT_FAILURE;
    }

    tempStart = line + (INDEX_OF_START_OF_TEMP);
    currentTemperature = atof(tempStart);

    printf("Parsed the temperature as '%s', %1f\n", tempStart, currentTemperature);

    if( currentTemperature <= 0 ){
        printf("Nothing to do.");
        return EXIT_SUCCESS;
    }
    
    if(argumentCount > 1){
        maxTemperature = readMaxTempFromConfigFile(argumentVector[1]);
    }else{
        maxTemperature = readMaxTempFromConfigFile(NULL);
    }

    wiringPiSetup();
    pinMode(PIN_17, OUTPUT);
    
    if(currentTemperature > maxTemperature ){
        printf("Turning pin on.\n");
        digitalWrite (PIN_17, HIGH);// On
    }else{
        printf("Turning pin off.\n");
        digitalWrite (PIN_17, LOW); // Off
    }
    printf("=========================================================\n");

    return EXIT_SUCCESS;
}
