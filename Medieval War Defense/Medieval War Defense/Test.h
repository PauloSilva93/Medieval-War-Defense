#pragma once

// Cria janela
int windowWidth = 1200, windowHeight = 900;
SDLWindow window(windowWidth, windowHeight, "Medieval War Defense");

// Carrega arquivos de shader
Shader shader_Interface("shaders/shader_Interface.vs", "shaders/shader_Interface.fs");
Shader shader_Static("shaders/shader_Static.vs", "shaders/shader_Static.fs");
Shader shader_Static_NL("shaders/shader_Static_NL.vs", "shaders/shader_Static_NL.fs");
Shader shader_Animation("shaders/shader_Animation.vs", "shaders/shader_Animation.fs");
Shader shader_Animation_NL("shaders/shader_Animation_NL.vs", "shaders/shader_Animation_NL.fs");
Shader shader_Text("shaders/shader_Text.vs", "shaders/shader_Text.fs");

Text text;
Vertices vertices;

void loadText()
{
	
	text.CreateText(&shader_Text, &window, "fonts/RINGM___.TTF");
}

void loadVertices()
{
	vertices.LoadVertices();
}

const char* menu_backGroundTexPaths[5] = { "imgs/menu_background.png", "imgs/menu_background.png", "imgs/menu_background.png", "imgs/menu_background.png", "imgs/noEmit.png" };
const char* loading_TexPaths[5] = { "imgs/king_anim.png", "imgs/king_anim.png" , "imgs/king_anim.png" , "imgs/king_anim.png" , "imgs/noEmit.png" };
const char* skeleton_w_TexPaths[5] = { "imgs/walk_skeleton.png", "imgs/walk_skeleton_n.png", "imgs/walk_skeleton.png" , "imgs/walk_skeleton.png" , "imgs/walk_skeleton_emit.png" };
const char* skeleton_d_TexPaths[5] = { "imgs/dying_skeleton.png", "imgs/dying_skeleton_n.png", "imgs/dying_skeleton.png" , "imgs/dying_skeleton.png" , "imgs/noEmit.png" };
const char* particleTexPaths[1] = { "imgs/particle.png" };
const char* arrowTexPaths[5] = { "imgs/Arrow.png", "imgs/Arrow_n.png", "imgs/Arrow.png", "imgs/Arrow.png", "imgs/noEmit.png" };
const char* towerRangeTexPaths[1] = { "imgs/selection_image.png" };
const char* towerArcherTexPaths[5] = { "imgs/tower_archer.png", "imgs/tower_archer_n.png", "imgs/tower_archer.png", "imgs/tower_archer.png", "imgs/noEmit.png" };
const char* tree_1TexPaths[5] = { "imgs/tree_1.png", "imgs/tree_1_n.png", "imgs/tree_1.png", "imgs/tree_1.png", "imgs/noEmit.png" };
const char* forestTexPaths[5] = { "imgs/map_forest.png", "imgs/map_forest_n.png", "imgs/map_forest.png", "imgs/map_forest.png", "imgs/noEmit.png" };
const char* stone_1TexPaths[5] = { "imgs/stone_1.png", "imgs/stone_1_n.png", "imgs/stone_1.png", "imgs/stone_1.png", "imgs/noEmit.png" };
const char* buildingAreaTexPaths[5] = { "imgs/building_area.png", "imgs/building_area_n.png", "imgs/building_area.png", "imgs/building_area.png", "imgs/noEmit.png" };
const char* buildingAreaOverTexPaths[5] = { "imgs/building_area_over.png", "imgs/building_area_over.png", "imgs/building_area_over.png", "imgs/building_area_over.png", "imgs/noEmit.png" };
const char* towersMenuTexPaths[5] = { "imgs/towers_menu_anim.png", "imgs/towers_menu_anim.png", "imgs/towers_menu_anim.png", "imgs/towers_menu_anim.png", "imgs/noEmit.png" };
const char* towersUpgradeTexPaths[5] = { "imgs/tower_upgrade_anim.png", "imgs/tower_upgrade_anim.png", "imgs/tower_upgrade_anim.png", "imgs/tower_upgrade_anim.png", "imgs/noEmit.png" };
const char* tower_archer_overTexPaths[5] = { "imgs/tower_archer_over.png", "imgs/tower_archer_over.png", "imgs/tower_archer_over.png", "imgs/tower_archer_over.png", "imgs/noEmit.png" };
const char* towerUpgradeDTexPaths[5] = { "imgs/tower_upgrade_d_anim.png", "imgs/tower_upgrade_d_anim.png", "imgs/tower_upgrade_d_anim.png", "imgs/tower_upgrade_d_anim.png", "imgs/noEmit.png" };
const char* bush_1TexPaths[5] = { "imgs/bush_object_1.png", "imgs/bush_object_1_n.png", "imgs/bush_object_1.png" , "imgs/bush_object_1.png" , "imgs/noEmit.png" };
const char* stump_1TexPaths[5] = { "imgs/stump_1.png", "imgs/stump_1_n.png", "imgs/stump_1.png" , "imgs/stump_1.png" , "imgs/noEmit.png" };
const char* tree_2TexPaths[5] = { "imgs/tree_2.png", "imgs/tree_2_n.png", "imgs/tree_2.png" , "imgs/tree_2.png" , "imgs/noEmit.png" };
const char* mushroomsTexPaths[5] = { "imgs/mushrooms.png", "imgs/mushrooms_n.png", "imgs/mushrooms.png" , "imgs/mushrooms.png" , "imgs/noEmit.png" };

Texture menu_backGroundTex;
Texture skeleton_w_Tex;
Texture skeleton_d_Tex;
Texture backGroundTex;
Texture particleTex;
Texture arrowTex;
Texture rangeTex;
Texture towerArcherTex;
Texture tree_1Tex;
Texture forestTex;
Texture stone_1Tex;
Texture buildingAreaTex;
Texture buildingAreaOverTex;
Texture towersMenuTex;
Texture towersUpgradeTex;
Texture tower_archer_overTex;
Texture towersUpgradeDTex;
Texture bush_1Tex;
Texture stump_1Tex;
Texture tree_2Tex;
Texture mushroomsTex;

Music mainTheme;
Sound skeletonVoice;
Sound skeletonDying_1;
Sound skeletonDying_2;
Sound skeletonDying_3;
Sound skeletonDying_4;
Sound skeletonDying_5;
Sound arrowShot_1;
Sound arrowShot_2;
Sound arrowShot_3;
Sound arrowShot_4;
Sound arrowShot_5;

vector<Sound*> skeletonDying_Sounds = { &skeletonDying_1 , &skeletonDying_2 , &skeletonDying_3, &skeletonDying_4, &skeletonDying_5 };
vector<Sound*> arrowShot_Sounds = { &arrowShot_1 , &arrowShot_2, &arrowShot_3, &arrowShot_4, &arrowShot_5 };

void loadTextures()
{
	menu_backGroundTex.LoadTextures(menu_backGroundTexPaths);
	skeleton_w_Tex.LoadTextures(skeleton_w_TexPaths);
	skeleton_d_Tex.LoadTextures(skeleton_d_TexPaths);
	particleTex.LoadTextures(particleTexPaths);
	arrowTex.LoadTextures(arrowTexPaths);
	rangeTex.LoadTextures(towerRangeTexPaths);
	towerArcherTex.LoadTextures(towerArcherTexPaths);
	tree_1Tex.LoadTextures(tree_1TexPaths);
	forestTex.LoadTextures(forestTexPaths);
	stone_1Tex.LoadTextures(stone_1TexPaths);
	buildingAreaTex.LoadTextures(buildingAreaTexPaths);
	buildingAreaOverTex.LoadTextures(buildingAreaOverTexPaths);
	towersMenuTex.LoadTextures(towersMenuTexPaths);
	towersUpgradeTex.LoadTextures(towersUpgradeTexPaths);
	tower_archer_overTex.LoadTextures(tower_archer_overTexPaths);
	towersUpgradeDTex.LoadTextures(towerUpgradeDTexPaths);
	bush_1Tex.LoadTextures(bush_1TexPaths);
	stump_1Tex.LoadTextures(stump_1TexPaths);
	tree_2Tex.LoadTextures(tree_2TexPaths);
	mushroomsTex.LoadTextures(mushroomsTexPaths);
}

void loadAudio()
{
	mainTheme.LoadAudio("audio/332043_Vaifan90___To_Settle_The_S.mp3");
	skeletonVoice.LoadAudio("audio/zombie_voice_brains_09.wav");
	skeletonDying_1.LoadAudio("audio/skeletonDying_1.wav");
	skeletonDying_2.LoadAudio("audio/skeletonDying_2.wav");
	skeletonDying_3.LoadAudio("audio/skeletonDying_3.wav");
	skeletonDying_4.LoadAudio("audio/skeletonDying_4.wav");
	skeletonDying_5.LoadAudio("audio/skeletonDying_5.wav");
	arrowShot_1.LoadAudio("audio/arrowShot_1.wav");
	arrowShot_2.LoadAudio("audio/arrowShot_2.wav");
	arrowShot_3.LoadAudio("audio/arrowShot_3.wav");
	arrowShot_4.LoadAudio("audio/arrowShot_4.wav");
	arrowShot_5.LoadAudio("audio/arrowShot_5.wav");

}

