#ifndef EEPROM_H
#define EEPROM_H

#define SLAVE_READ		0xA1
#define SLAVE_WRITE		0xA0


void write_external_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_external_eeprom(unsigned char address1);

#endif