#ifndef _RFID_H
#define _RFID_H

#include "rdm630.h"
#include <maybe.h>

int16_t initRFID(size_t, uint8_t *);
maybe readTag(void *);
maybe readTagAt(void *);
void writeTag(Tag *);
void writeTagAt(uint8_t, Tag *);
bool cmpTag(Tag, Tag);
Tag *makeTag(UID);

#endif
