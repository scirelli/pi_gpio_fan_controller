pi_fan:
	gcc -O3 -o pi_fan pi_fan.c -lwiringPi

debug:
	gcc -g -o pi_fan pi_fan.c -lwiringPi

all: pi_fan

run: pi_fan
	/opt/vc/bin/vcgencmd measure_temp | sudo ./pi_fan
