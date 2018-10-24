#include "rdm630.h"
#include <maybe.h>

#define	EINVAL	1	/* Invalid argument */

enum raw_data_bytes {
	START_BYTE = 0x02,
	END_BYTE = 0x03,
};

enum raw_data_structure {
	RAW_DATA_TOTAL_LEN = 14,
	RAW_DATA_UID_IDX = 1,
	RAW_DATA_UID_LEN = 10,
	RAW_DATA_CHK_IDX = 11,
	RAW_DATA_CHK_LEN = 2,
};

enum uid_info {
	UID_LEN = 5,
};

static uint8_t rawDataBuf[RAW_DATA_TOTAL_LEN];
static uint8_t uidBuf[UID_LEN];
static uint8_t tagDataBuf[0];

static Tag ctx;

static uint8_t chrToHex(uint8_t c) {
	if (c >= 0x30 && c <= 0x39) {
		return c - 0x30;
	} else if (c >= 0x41 && c <= 0x46) {
		return c - 0x41 + 0x0A;
	} else {
		return -EINVAL;
	}
}

/* Validate incoming data */
static bool validate(RawData rawData) {
	uint8_t hd = rawData[0];
	uint8_t lst = rawData[RAW_DATA_TOTAL_LEN - 1];

	return (hd == START_BYTE && lst == END_BYTE);
}

/* Checksum of UID is XOR of all bytes */
static uint8_t checksum(UID uid) {
	uint8_t r = 0x00;
	for (size_t i = 0; i < UID_LEN; i++) {
		r ^= uid[i];
	}
	return r;
}

static uint8_t *hexstrToNum(uint8_t *num, uint8_t *str, size_t num_len) {
	for (size_t i = 0, j = 0; i < num_len; i++) {
		uint8_t x = str[j++];
		uint8_t y = str[j++];
		uint8_t b = (chrToHex(x) << 4) + chrToHex(y);
		num[i] = b;
	}
	return num;
}

int16_t RDM630init(void) {
	Serial.begin(9600);

	ctx.rawData = rawDataBuf;
	ctx.uid = uidBuf;
	ctx.tagData = tagDataBuf;

	return 0;
}

/* Parse raw tag data into its UID */
/* RawData -> maybe Tag */
maybe RDM630parse(void *rawData_) {
	RawData rawData = (RawData) rawData_;

	if (!validate(rawData)) return nothing();

	hexstrToNum(ctx.uid, &rawData[RAW_DATA_UID_IDX], UID_LEN);

	uint8_t chk;
	hexstrToNum(&chk, &rawData[RAW_DATA_CHK_IDX], sizeof(ctx.rawData[RAW_DATA_CHK_IDX]));

	if (chk != checksum(ctx.uid)) return nothing();

	return mreturn(&ctx);
}

/* Get tag data from RFID reader */
/* maybe RawData */
maybe RDM630readData(void *_) {
	if (!Serial.available()) return nothing();

	for (size_t i = 0; i < RAW_DATA_TOTAL_LEN; i++) {
		ctx.rawData[i] = Serial.read();
	}

	// Throw away any extra
	while (Serial.available()) Serial.read();

	return mreturn(ctx.rawData);
}

static bool cmpUID(UID x, UID y) {
	return memcmp(x, y, UID_LEN) == 0;
}

bool RDM630cmpTag(Tag *x, Tag *y) {
	return cmpUID(x->uid, y->uid);
}

Tag *RDM630makeTag(UID uid) {
	ctx.uid = uid;
	ctx.rawData = NULL;
	ctx.tagData = NULL;
	return &ctx;
}
