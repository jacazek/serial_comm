
# UART Serial Communications library
This library enables UART serial communication from atmega microcontroller

# Command line commands
to read data from atmega device on the command line

	sudo stty -F /dev/[device] cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
	sudo cat /dev/[device]

or run the test.py script in the example directory (may need privileged access)
	
	python ./example/test.py /dev/[device]
