#ifndef _RFID_TYPES_H
#define _RFID_TYPES_H

typedef uint8_t * RawData;
typedef uint8_t * UID;
typedef uint8_t * TagData;

typedef struct {
	RawData rawData;
	UID uid;
	TagData tagData;
} Tag;

#endif
