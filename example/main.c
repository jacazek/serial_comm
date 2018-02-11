#include <serialcomm.h>
#define F_CPU 16000000	// 16 MHz oscillator.
#define BaudRate 9600
#define MYUBRR ((F_CPU / 16 / BaudRate ) - 1)

//01101000
int main() {
	struct SerialSettings settings;
	settings.baudrate = MYUBRR;
	settings.TX = 1;
	configureSerial(settings);
	transmitComplete();
	while(1) {
		
	}
}

void transmitComplete() {
	transmitSerialAsync("Hello!\n", &transmitComplete);
}