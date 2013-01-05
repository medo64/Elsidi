#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

void communication_init();
unsigned char readByte();

bit readNothing();
bit readPercent(unsigned char *value, unsigned char *charCount);

void writePercent(unsigned char value);

void processByte(unsigned char data);

#endif
