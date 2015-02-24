#ifndef ELSHIELD_H
#define ELSHIELD_H 1

const int EL_NUM_CH = 3;

void
el_init(void);

void
el_set_all_off(void);

void
el_set_ch(uint8_t ch);

void
el_clear_ch(uint8_t ch);

void
el_toggle_ch(uint8_t ch);

#endif
