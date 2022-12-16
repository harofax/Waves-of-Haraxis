#include "SpriteManager.h"

SDL_Rect SpriteManager::sprite_table[16];
SDL_Texture* SpriteManager::atlas_texture {nullptr};
int SpriteManager::sprite_range_table[4];
