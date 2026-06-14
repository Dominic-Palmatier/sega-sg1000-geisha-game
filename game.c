#include "common.h"

// Game state
typedef struct {
  byte x, y;
  byte width, height;
  byte vx, vy;
  byte jumping;
} Player;

typedef struct {
  byte x, y;
  byte width, height;
  byte active;
} Dog;

typedef struct {
  byte world;
  byte level;
  byte score_low, score_high;
  byte lives;
  Player player;
  Dog dogs[4];
  byte dog_count;
  byte frame_count;
} GameState;

GameState game;

void init_game() {
  game.world = 1;
  game.level = 1;
  game.score_low = 0;
  game.score_high = 0;
  game.lives = 3;
  
  game.player.x = 120;
  game.player.y = 140;
  game.player.width = 8;
  game.player.height = 12;
  game.player.vx = 0;
  game.player.vy = 0;
  game.player.jumping = 0;
  
  game.frame_count = 0;
  spawn_dogs();
}

void spawn_dogs() {
  if (game.world == 1) {
    game.dog_count = 2;
    game.dogs[0].x = 40;
    game.dogs[0].y = 100;
    game.dogs[0].width = 10;
    game.dogs[0].height = 8;
    game.dogs[0].active = 1;
    
    game.dogs[1].x = 200;
    game.dogs[1].y = 70;
    game.dogs[1].width = 10;
    game.dogs[1].height = 8;
    game.dogs[1].active = 1;
  } else {
    game.dog_count = 4;
    game.dogs[0].x = 30;
    game.dogs[0].y = 120;
    game.dogs[0].width = 10;
    game.dogs[0].height = 8;
    game.dogs[0].active = 1;
    
    game.dogs[1].x = 180;
    game.dogs[1].y = 80;
    game.dogs[1].width = 10;
    game.dogs[1].height = 8;
    game.dogs[1].active = 1;
    
    game.dogs[2].x = 100;
    game.dogs[2].y = 50;
    game.dogs[2].width = 10;
    game.dogs[2].height = 8;
    game.dogs[2].active = 1;
    
    game.dogs[3].x = 220;
    game.dogs[3].y = 40;
    game.dogs[3].width = 10;
    game.dogs[3].height = 8;
    game.dogs[3].active = 1;
  }
}

void update_player() {
  byte input = read_input();
  
  // Left/Right movement
  if (input & INPUT_LEFT) {
    if (game.player.x > 0) game.player.x -= 2;
  }
  if (input & INPUT_RIGHT) {
    if (game.player.x < 240) game.player.x += 2;
  }
  
  // Jump
  if ((input & INPUT_UP) && !game.player.jumping) {
    game.player.vy = -8;
    game.player.jumping = 1;
  }
  
  // Gravity
  if (game.player.jumping) {
    game.player.vy += 1;
    game.player.y += game.player.vy;
    
    // Landing
    if (game.player.y >= 140) {
      game.player.y = 140;
      game.player.vy = 0;
      game.player.jumping = 0;
    }
  }
}

void update_dogs() {
  byte i;
  for (i = 0; i < game.dog_count; i++) {
    if (!game.dogs[i].active) continue;
    
    // Simple AI - patrol back and forth
    if (game.frame_count & 1) {
      game.dogs[i].x += (game.frame_count & 4) ? 1 : -1;
      
      if (game.dogs[i].x < 10 || game.dogs[i].x > 240) {
        game.dogs[i].x = (game.dogs[i].x < 10) ? 10 : 240;
      }
    }
  }
}

byte check_collision(byte x1, byte y1, byte w1, byte h1,
                     byte x2, byte y2, byte w2, byte h2) {
  return (x1 < x2 + w2) && (x1 + w1 > x2) &&
         (y1 < y2 + h2) && (y1 + h1 > y2);
}

void check_collisions() {
  byte i;
  for (i = 0; i < game.dog_count; i++) {
    if (!game.dogs[i].active) continue;
    
    if (check_collision(game.player.x, game.player.y, 
                       game.player.width, game.player.height,
                       game.dogs[i].x, game.dogs[i].y,
                       game.dogs[i].width, game.dogs[i].height)) {
      game.lives--;
      if (game.lives == 0) {
        game_over();
      } else {
        game.player.x = 120;
        game.player.y = 140;
      }
    }
  }
}

void draw_game() {
  byte i;
  
  clear_screen();
  
  // Draw player (geisha)
  draw_sprite(game.player.x, game.player.y, 0);
  
  // Draw dogs
  for (i = 0; i < game.dog_count; i++) {
    if (game.dogs[i].active) {
      draw_sprite(game.dogs[i].x, game.dogs[i].y, 1);
    }
  }
  
  // Draw HUD
  draw_hud();
}

void draw_hud() {
  draw_text(10, 10, "WORLD:");
  draw_char(70, 10, '0' + game.world);
  
  draw_text(100, 10, "LIVES:");
  draw_char(150, 10, '0' + game.lives);
  
  draw_text(200, 10, "SCORE:");
  draw_char(260, 10, '0' + game.score_high);
  draw_char(270, 10, '0' + game.score_low);
}

void game_over() {
  // Game over logic
  draw_text(100, 80, "GAME OVER");
  draw_text(80, 100, "PRESS START");
  
  while (!(read_input() & INPUT_START)) {
    // Wait
  }
  
  init_game();
}

void next_world() {
  if (game.world < 2) {
    game.world++;
    game.level = 1;
    spawn_dogs();
    game.player.x = 120;
    game.player.y = 140;
  } else {
    game_over();
  }
}

void main() {
  init_vdp();
  load_sprites();
  init_game();
  
  while (1) {
    update_player();
    update_dogs();
    check_collisions();
    draw_game();
    
    game.frame_count++;
    if (game.player.y < 20) {
      game.score_low += 10;
      next_world();
    }
  }
}
