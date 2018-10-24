#include "rfid.h"
#include "rdm630.h"

static uint8_t current_position;
static size_t num_antennas;
static uint8_t *antenna_pins;

static void selectAntenna(uint8_t pos) {
	uint8_t i;
	
	if (pos < 1 || pos > num_antennas) {
		return;
	}

	for (i = 0; i < pos - 1; i++) {
		digitalWrite(antenna_pins[i], LOW);
	}
	
	digitalWrite(antenna_pins[i], HIGH);
	
	for (++i; i < num_antennas; i++) {
		digitalWrite(antenna_pins[i], LOW);
	}

	delay(100);
}

int16_t initRFID(size_t num_ant, uint8_t *ant_pins) {
	num_antennas = num_ant;
	antenna_pins = ant_pins;
	current_position = 0;
	
	return RDM630init();
}

/* maybe Tag */
maybe readTag(void *_) {
	uint8_t n = 1;
	return readTagAt(&n);
}

/* uint8_t -> maybe Tag */
maybe readTagAt(void *n_) {
	uint8_t *n = (uint8_t *) n_;

	selectAntenna(*n);

	return maybe_do
		(RDM630readData,
		 RDM630parse);
}

void writeTag(Tag *tag) {
	return;
}

void writeTagAt(uint8_t n, Tag *tag) {
	return;
}

bool cmpTag(Tag *x, Tag *y) {
	return RDM630cmpTag(x, y);
}

Tag *makeTag(UID uid) {
	return RDM630makeTag(uid);
}
