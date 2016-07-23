pi_fan:
	gcc -g -o pi_fan pi_fan.c -lwiringPi

all: pi_fan

run:
	/opt/vc/bin/vcgencmd measure_temp | ./pi_fan
