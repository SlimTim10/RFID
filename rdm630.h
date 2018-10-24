#ifndef _RFID_RDM630_H
#define _RFID_RDM630_H

#include <Arduino.h>
#include <maybe.h>
#include "types.h"

int16_t RDM630init(void);
maybe RDM630readData(void *);
maybe RDM630parse(void *);
bool RDM630cmpTag(Tag *, Tag *);
Tag *RDM630makeTag(UID);

#endif
