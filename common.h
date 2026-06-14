#ifndef __COMMON_H__
#define __COMMON_H__

typedef unsigned char byte;
typedef unsigned short word;
typedef signed char sbyte;
typedef signed short sword;

// VDP Registers
#define VDP_CONTROL 0xbe
#define VDP_DATA 0xbe
#define VRAM_ADDR(addr) ((addr) & 0x3fff) | (((addr) >> 8) & 0xc0)

// Input
#define INPUT_UP 0x01
#define INPUT_DOWN 0x02
#define INPUT_LEFT 0x04
#define INPUT_RIGHT 0x08
#define INPUT_BUTTON1 0x10
#define INPUT_BUTTON2 0x20
#define INPUT_START 0x80

// Functions from common.c
void init_vdp(void);
void clear_screen(void);
void draw_sprite(byte x, byte y, byte sprite_id);
void draw_text(byte x, byte y, const char* text);
void draw_char(byte x, byte y, char c);
byte read_input(void);
void load_sprites(void);

#endif
