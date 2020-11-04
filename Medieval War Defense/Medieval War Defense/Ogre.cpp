#include "Ogre.h"

	string Ogre::name() const
	{
		return typeid(Ogre).name();
	}

	void Ogre::CreateOgre(Shader& shader_animation, Texture& textures_walk, Texture& textures_die, Graph mapPath, vector<Sound*>* enemyDyingSounds, vector<GameObject*>& gameObjects, vector<Enemy*>& enemies, Player& player)
	{
		anim_size_walk[0] = { 0.12f };
		anim_size_walk[1] = { 0.12f };
		anim_size_die[0] = { 0.12f };
		anim_size_die[1] = { 0.12f };
		fps_walk = 15;
		fps_die = 15;
		speed = 0.04f;
		life = 20.0f;
		frames_x_walk = 4;
		frames_y_walk = 5;
		blankFrames_walk = 2;
		frames_x_die = 4;
		frames_y_die = 4;
		blankFrames_die = 1;

		CreateEnemy(shader_animation, textures_walk, textures_die, mapPath, enemyDyingSounds, gameObjects, &enemies, player);
	}