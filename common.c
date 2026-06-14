#include "common.h"

// VDP initialization for SG-1000
void init_vdp() {
  // Set up VDP registers for graphics mode
  // Register 0: Mode control 1
  volatile byte* vdp = (byte*)0xbf00;
  
  vdp[0] = 0x00;
  vdp[0] = 0x08;
  
  // Register 1: Mode control 2 (40x24 text, enable display)
  vdp[0] = 0xe0;
  vdp[0] = 0x81;
  
  // Clear VRAM
  clear_screen();
}

void clear_screen() {
  byte i, j;
  volatile byte* vram = (byte*)0x4000;
  
  for (i = 0; i < 24; i++) {
    for (j = 0; j < 40; j++) {
      vram[i * 40 + j] = 0x20; // Space character
    }
  }
}

void draw_sprite(byte x, byte y, byte sprite_id) {
  // SG-1000 sprite drawing
  // sprite_id 0 = geisha, sprite_id 1 = dog
  // Simplified - actual implementation would use hardware sprites
  volatile byte* vram = (byte*)0x4000;
  byte screen_y = y / 8;
  byte screen_x = x / 8;
  
  if (screen_y < 24 && screen_x < 40) {
    byte sprite_char = (sprite_id == 0) ? 0x01 : 0x02; // Different chars for geisha/dog
    vram[screen_y * 40 + screen_x] = sprite_char;
  }
}

void draw_text(byte x, byte y, const char* text) {
  volatile byte* vram = (byte*)0x4000;
  byte screen_x = x / 8;
  byte screen_y = y / 8;
  byte i = 0;
  
  while (text[i] && screen_x + i < 40) {
    vram[screen_y * 40 + screen_x + i] = text[i];
    i++;
  }
}

void draw_char(byte x, byte y, char c) {
  volatile byte* vram = (byte*)0x4000;
  byte screen_x = x / 8;
  byte screen_y = y / 8;
  
  if (screen_y < 24 && screen_x < 40) {
    vram[screen_y * 40 + screen_x] = c;
  }
}

byte read_input() {
  // Read input from SG-1000 controller
  volatile byte* input_port = (byte*)0xdc;
  byte result = 0;
  
  // Simplified input reading
  // Port DC: bit 0=up, 1=down, 2=left, 3=right, 4=button1, 5=button2
  byte port_val = input_port[0];
  
  if (port_val & 0x01) result |= INPUT_UP;
  if (port_val & 0x02) result |= INPUT_DOWN;
  if (port_val & 0x04) result |= INPUT_LEFT;
  if (port_val & 0x08) result |= INPUT_RIGHT;
  if (port_val & 0x10) result |= INPUT_BUTTON1;
  if (port_val & 0x20) result |= INPUT_BUTTON2;
  
  return result;
}

void load_sprites() {
  // Load sprite patterns into VRAM
  // For SG-1000, sprites are defined in character patterns
  volatile byte* vram = (byte*)0x4000;
  volatile byte* vdp_cmd = (byte*)0xbf00;
  
  // Geisha sprite pattern (character 0x01)
  // Simple 8x8 pattern
  byte geisha_pattern[] = {
    0x18, // ...XX...
    0x3C, // ..XXXX..
    0x3C, // ..XXXX..
    0x18, // ...XX...
    0x18, // ...XX...
    0x3C, // ..XXXX..
    0x66, // .XX..XX.
    0xC3  // XX....XX
  };
  
  // Dog sprite pattern (character 0x02)
  byte dog_pattern[] = {
    0x0C, // ....XX..
    0x1E, // ...XXXX.
    0x3F, // ..XXXXXX
    0x1E, // ...XXXX.
    0x1E, // ...XXXX.
    0x0C, // ....XX..
    0x08, // ....X...
    0x04  // .....X..
  };
}
