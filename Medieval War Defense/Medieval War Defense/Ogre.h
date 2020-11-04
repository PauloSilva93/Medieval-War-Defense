#pragma once

#include "Enemy.h"

class Ogre : public Enemy
{
public:

	string name() const;

	void CreateOgre(Shader& shader_animation, Texture& textures_walk, Texture& textures_die, Graph mapPath, vector<Sound*>* enemyDyingSounds, vector<GameObject*>& gameObjects, vector<Enemy*>& enemies, Player& player);
};
