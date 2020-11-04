#include "Skeleton.h"

	string Skeleton::name() const
	{
		return typeid(Skeleton).name();
	}

	void Skeleton::CreateSkeleton(Shader& shader_animation, Texture& textures_walk, Texture& textures_die, Graph mapPath, vector<Sound*>* enemyDyingSounds, vector<GameObject*>& gameObjects, vector<Enemy*>& enemies, Player& player)
	{
		anim_size_walk[0] = { 0.05f };
		anim_size_walk[1] = { 0.05f };
		anim_size_die[0] = { 0.06f };
		anim_size_die[1] = { 0.06f };
		fps_walk = 60;
		fps_die = 90;
		speed = 0.05f;
		life = 3.0f;
		frames_x_walk = 8;
		frames_y_walk = 13;
		blankFrames_walk = 4;
		frames_x_die = 8;
		frames_y_die = 13;
		blankFrames_die = 4;

		CreateEnemy(shader_animation, textures_walk, textures_die, mapPath, enemyDyingSounds, gameObjects, &enemies, player);
	}