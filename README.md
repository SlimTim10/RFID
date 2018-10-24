# RFID

## Description

General purpose RFID.

## Installation
To use this as an Arduino library, download it as a ZIP and extract the folder to your [Arduino libraries](https://www.arduino.cc/en/hacking/libraries) directory, then rename the folder from `RFID-master` to `RFID`.

## Usage

```
enum { NUM_ANTENNAS = 5 };
uint8_t antennas[] = {ANT1, ANT2, ANT3, ANT4, ANT5};

/* Tag -> maybe void */
maybe printUID(void *x_) {
	Tag *tag = (Tag *) x_;
	
	dbg_print("UID:");
	for (size_t i = 0; i < 5; i++) {
		if (tag->uid[i] < 0x10) {
			dbg_print(" 0");
		} else {
			dbg_print(' ');
		}
		dbg_print(tag->uid[i], HEX);
	}
	dbg_println();

	return nothing();
}

initRFID(NUM_ANTENNAS, antennas);

uint8_t n = 1;
maybe t = readTagAt(&n);
bind(printUID, t);
```

## Dependencies

- [MaybeC](https://github.com/SlimTim10/MaybeC)

## RFID Module Support

- RDM630