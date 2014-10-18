#include "main.h"
#include "movement.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

int next_char(WINDOW *w, struct position p, int direction){
  int next_ch;
  switch (direction) {
    case UP_DIRECTION:
      next_ch = mvwinch(w, p.y - 1, p.x );
      break;
    case DOWN_DIRECTION:
      next_ch = mvwinch(w, p.y + 1, p.x );
      break;
    case RIGHT_DIRECTION:
      next_ch = mvwinch(w, p.y, p.x + 1);
      break;
    case LEFT_DIRECTION:
      next_ch = mvwinch(w, p.y, p.x - 1 );
      break;
  }
  return next_ch;
}

//verifica se existe uma parede no caminho ou eh fim do mapa
int can_go_to_direction(WINDOW *w, sprite sp, int direction){
  int can_go = 0;
  switch (direction) {
    case UP_DIRECTION:
      if (sp.position.y > 0) {
        can_go = 1;
      }
      break;
    case RIGHT_DIRECTION:
      if (sp.position.x < MAX_X - 1) {
        can_go = 1;
      }
      break;
    case DOWN_DIRECTION:
      if (sp.position.y < MAX_Y - 1) {
        can_go = 1;
      }
      break;
    case LEFT_DIRECTION:
      if (sp.position.x > 0) {
        can_go = 1;
      }
  }
  int next_ch = next_char(w, sp.position, direction);
  if (sp.representation == CH_MR_DO){
    return (next_ch != CH_ROCK && next_ch != -1 && can_go);
  }
  else{
    return (next_ch != CH_WALL && next_ch != CH_ROCK && next_ch != -1 && can_go);
  }
}

void move_sprite(WINDOW *w, sprite* sprite, int direction){
  sprite->position.last_x = sprite->position.x;
  sprite->position.last_y = sprite->position.y;
  if (can_go_to_direction(w, *sprite, direction)) {
    switch (direction) {
      case UP_DIRECTION:
        sprite->position.y--;
        break;
      case RIGHT_DIRECTION:
        sprite->position.x++;
        break;
      case DOWN_DIRECTION:
        sprite->position.y++;
        break;
      case LEFT_DIRECTION:
        sprite->position.x--;
        break;
    }
  }
  if (direction) {
    sprite->direction = direction;
  }
}

int can_fall(WINDOW *w, struct position* p, int direction){
  int next_ch = next_char(w, *p, DOWN_DIRECTION);
  return (next_ch == ' ');
}

void move_ghost(WINDOW *w, sprite *gh){
  if (!can_go_to_direction(w, *gh, gh->direction)){
    int d = rand() % 4 + 1;
    while(!can_go_to_direction(w, *gh, d)){
      d = rand() % 4 + 1;
    }

    move_sprite(w, gh, d);
  }
  else{
    move_sprite(w, gh, gh->direction);
  }
}

void move_ghosts(WINDOW *w, sprite *ghosts){
  sprite *current = ghosts;
  while(current != NULL){
    move_ghost(w, current);
    current = current->next;
  }
}

void move_shot(WINDOW *w, sprite* s){
  move_if_possible(w, s);
}

//@TODO mover isso para a funcao de colisao
void move_if_possible(WINDOW *w, sprite* s){
  if (can_go_to_direction(w, *s, s->direction)) {
    move_sprite(w, s, s->direction);
  }
  else{
    s->alive = 0;
  }
}

/*
void move_rocks(WINDOW *w, sprite *rocks){
  for (int i = 0; i < MAX_ROCKS; i++) {
    if(rocks[i].alive){
      move_rock(w, &rocks[i]);
    }
  }
}
void move_rock(WINDOW *w, sprite *rk){
  if (can_fall(w, &rk->position, rk->direction)){
    if(rk->falling == 10){
      move_sprite(w, rk, rk->direction);
    }
    else{
      rk->falling++;
    }
  }
  else{
    if(rk->falling == 10){
      rk->alive = 0;
      print_char(w, rk);
    }
    else{
      print_char(w, rk);
    }
  }
}
*/
