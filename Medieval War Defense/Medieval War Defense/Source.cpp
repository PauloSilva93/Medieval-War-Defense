#include <iostream>
#include <SDL/SDL.h>
#include <glad/glad.h>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TimeStep.h"
#include "SDLWindow.h"
#include "Vertices.h"
#include "Texture.h"
#include "Transform.h"
#include "Object2D_Animated.h"
#include "Object2D_Static.h"
#include "Object2D_Interface.h"
#include <vector>
#include <ctime>
#include "Graph.h"
#include "MathUtils.h"
#include "Light.h"
#include "LightSystem.h"
#include "ParticleEmitter.h"
#include "Selection.h"
#include "Music.h"
#include "Sound.h"
#include <array>
#include "Player.h"
#include "Enemy.h"
#include "Skeleton.h"
#include "Ogre.h"
#include "Projectile.h"
#include "Range.h"
#include "Tower.h"
#include "ArcherTower.h"
#include "MapObject.h"
#include "Map.h"
#include "BuildingArea.h"
#include "Text.h"
#include "Sorter.h"
#include <type_traits>
#include "ProjectileEffects.h"
#include "FireEffects.h"
#include "PoisonEffects.h"

#define PI 3.1415f

int GameObject::objID = 0;

using namespace std;

int main(int argc, char** argv) {

	srand((unsigned int)time(NULL));

	MathUtils mu;	
	Selection selector;
	Sorter sorter;
	int mouseX, mouseY;
	float viewX = 0.0f, viewY = 0.0f;
	float viewXY[2] = { viewX , viewY };

	// Cria janela
	int windowWidth = GetSystemMetrics(SM_CXSCREEN), windowHeight = GetSystemMetrics(SM_CYSCREEN);
	windowHeight -= 60;
	windowWidth = windowHeight;
	SDLWindow window(windowWidth, windowHeight, "Medieval War Defense");

	// Carrega arquivos de shader
	Shader shader_Interface("shaders/shader_Interface.vert", "shaders/shader_Interface.frag");
	Shader shader_Static("shaders/shader_Static.vert", "shaders/shader_Static.frag");
	Shader shader_Static_NL("shaders/shader_Static_NL.vert", "shaders/shader_Static_NL.frag");
	Shader shader_Animation("shaders/shader_Animation.vert", "shaders/shader_Animation.frag");
	Shader shader_Animation_NL("shaders/shader_Animation_NL.vert", "shaders/shader_Animation_NL.frag");
	Shader shader_Text("shaders/shader_Text.vert", "shaders/shader_Text.frag");
	Shader lighShaderList[] = { shader_Static , shader_Animation };

	Text text;
	text.CreateText(&shader_Text, &window, "fonts/RINGM___.TTF");

	// Carrega vertices
	Vertices vertices;
	vertices.LoadVertices();

	const char* leafTexPaths[1] = { "imgs/leaf.png" };
	const char* earthTexPaths[1] = { "imgs/rock.png" };
	const char* waterTexPaths[1] = { "imgs/particle.png" };
	const char* fireTexPaths[1] = { "imgs/particle.png" };
	const char* cloudsTexPaths[1] = { "imgs/clouds.png" };
	const char* you_lostTexPaths[1] = { "imgs/you_Lost.png" };
	const char* playerHeartTexPaths[1] = { "imgs/playerHeart.png" };
	const char* playerGoldTexPaths[1] = { "imgs/playerGold.png" };
	const char* menu_backGroundTexPaths[5] = { "imgs/menu_background.png", "imgs/menu_background.png", "imgs/menu_background.png", "imgs/menu_background.png", "imgs/noEmit.png" };
	const char* play_glow_TexPaths[5] = { "imgs/play_glow.png", "imgs/play_glow.png", "imgs/play_glow.png", "imgs/play_glow.png", "imgs/noEmit.png" };
	const char* exit_glow_TexPaths[5] = { "imgs/exit_glow.png", "imgs/exit_glow.png", "imgs/exit_glow.png", "imgs/exit_glow.png", "imgs/noEmit.png" };
	const char* credits_glow_TexPaths[5] = { "imgs/credits_glow.png", "imgs/credits_glow.png", "imgs/credits_glow.png", "imgs/credits_glow.png", "imgs/noEmit.png" };
	const char* loading_TexPaths[5] = { "imgs/king_anim.png", "imgs/king_anim.png" , "imgs/king_anim.png" , "imgs/king_anim.png" , "imgs/noEmit.png" };
	const char* skeleton_w_TexPaths[5] = { "imgs/walk_skeleton.png", "imgs/walk_skeleton_n.png", "imgs/walk_skeleton.png" , "imgs/walk_skeleton.png" , "imgs/walk_skeleton_emit.png" };
	const char* skeleton_d_TexPaths[5] = { "imgs/dying_skeleton.png", "imgs/dying_skeleton_n.png", "imgs/dying_skeleton.png" , "imgs/dying_skeleton.png" , "imgs/noEmit.png" };
	const char* ogre_w_TexPaths[5] = { "imgs/walk_ogre.png", "imgs/walk_ogre_n.png", "imgs/walk_ogre.png", "imgs/walk_ogre.png", "imgs/noEmit.png" };
	const char* ogre_d_TexPaths[5] = { "imgs/dying_ogre.png", "imgs/dying_ogre.png", "imgs/dying_ogre.png", "imgs/dying_ogre.png", "imgs/noEmit.png" };
	const char* particleTexPaths[1] = { "imgs/particle.png" };
	const char* arrowTexPaths[5] = { "imgs/Arrow.png", "imgs/Arrow_n.png", "imgs/Arrow.png", "imgs/Arrow.png", "imgs/noEmit.png" };
	const char* towerRangeTexPaths[1] = { "imgs/selection_image.png" };
	const char* towerArcherTexPaths[5] = { "imgs/tower_archer.png", "imgs/tower_archer_n.png", "imgs/tower_archer.png", "imgs/tower_archer.png", "imgs/noEmit.png" };
	const char* tree_1TexPaths[5] = { "imgs/tree_1.png", "imgs/tree_1_n.png", "imgs/tree_1.png", "imgs/tree_1.png", "imgs/noEmit.png" };
	const char* forestTexPaths[5] = { "imgs/map_forest.png", "imgs/map_forest_n.png", "imgs/map_forest.png", "imgs/map_forest.png", "imgs/noEmit.png" };
	const char* stone_1TexPaths[5] = { "imgs/stone_1.png", "imgs/stone_1_n.png", "imgs/stone_1.png", "imgs/stone_1.png", "imgs/noEmit.png" };
	const char* stone_2TexPaths[5] = { "imgs/stone_2.png", "imgs/stone_2_n.png", "imgs/stone_2.png", "imgs/stone_2.png", "imgs/noEmit.png" };
	const char* buildingAreaTexPaths[5] = { "imgs/building_area.png", "imgs/building_area_n.png", "imgs/building_area.png", "imgs/building_area.png", "imgs/noEmit.png" };
	const char* buildingAreaOverTexPaths[5] = { "imgs/building_area_over.png", "imgs/building_area_over.png", "imgs/building_area_over.png", "imgs/building_area_over.png", "imgs/noEmit.png" };
	const char* towersMenuTexPaths[5] = { "imgs/towers_menu_anim.png", "imgs/towers_menu_anim.png", "imgs/towers_menu_anim.png", "imgs/towers_menu_anim.png", "imgs/noEmit.png" };
	const char* towersUpgradeTexPaths[5] = { "imgs/tower_upgrade_anim.png", "imgs/tower_upgrade_anim.png", "imgs/tower_upgrade_anim.png", "imgs/tower_upgrade_anim.png", "imgs/noEmit.png" };
	const char* tower_archer_overTexPaths[5] = { "imgs/tower_archer_over.png", "imgs/tower_archer_over.png", "imgs/tower_archer_over.png", "imgs/tower_archer_over.png", "imgs/noEmit.png" };
	const char* towerUpgradeDTexPaths[5] = { "imgs/tower_upgrade_d_anim.png", "imgs/tower_upgrade_d_anim.png", "imgs/tower_upgrade_d_anim.png", "imgs/tower_upgrade_d_anim.png", "imgs/noEmit.png" };
	const char* bush_1TexPaths[5] = { "imgs/bush_object_1.png", "imgs/bush_object_1_n.png", "imgs/bush_object_1.png" , "imgs/bush_object_1.png" , "imgs/noEmit.png" };
	const char* stump_1TexPaths[5] = { "imgs/stump_1.png", "imgs/stump_1_n.png", "imgs/stump_1.png" , "imgs/stump_1.png" , "imgs/noEmit.png" };
	const char* tree_2TexPaths[5] = { "imgs/tree_2.png", "imgs/tree_2_n.png", "imgs/tree_2.png" , "imgs/tree_2.png" , "imgs/noEmit.png" };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Texture loadingTex;
	loadingTex.LoadTextures(loading_TexPaths);
	Object2D_Animated loading;
	loading.CreateAnimatedObject2D(&shader_Animation_NL, loadingTex, 3, 4, 0, 0);
	float loadingPos[3] = { -0.4, 0, 0.1 };
	float loadingSize[2] = { 0.2, 0.2 };
	float loadingRotation[3] = { 0, 0, 1 };
	float loadingColor[4] = {0, 0, 0, 1 };
	while (!loading.animationFinished)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		loading.DrawAnimated(loadingSize, loadingPos, viewXY, loadingRotation, 0, loadingColor, false, 20, false);
		if (loading.animationFinished) text.RenderText("Loading...", -0.2, 0, 1, glm::vec3(1.0, 0.8f, 0.4f));
		window.SDL_SwapWindow();
	}

	Texture leafTex;
	Texture earthTex;
	Texture waterTex;
	Texture fireTex;
	Texture cloudsTex;
	Texture you_lostTex;
	Texture playerHeartTex;
	Texture playerGoldTex;
	Texture menu_backGroundTex;
	Texture play_glow_Tex;
	Texture exit_glow_Tex;
	Texture credits_glow_Tex;
	Texture skeleton_w_Tex;
	Texture skeleton_d_Tex;
	Texture ogre_w_Tex;
	Texture ogre_d_Tex;
	Texture backGroundTex;
	Texture particleTex;
	Texture arrowTex;
	Texture rangeTex;
	Texture towerArcherTex;
	Texture tree_1Tex;
	Texture forestTex;
	Texture stone_1Tex;
	Texture stone_2Tex;
	Texture buildingAreaTex;
	Texture buildingAreaOverTex;
	Texture towersMenuTex;
	Texture towersUpgradeTex;
	Texture tower_archer_overTex;
	Texture towersUpgradeDTex;
	Texture bush_1Tex;
	Texture stump_1Tex;
	Texture tree_2Tex;

	leafTex.LoadTextures(leafTexPaths);
	earthTex.LoadTextures(earthTexPaths);
	waterTex.LoadTextures(waterTexPaths);
	fireTex.LoadTextures(fireTexPaths);
	cloudsTex.LoadTextures(cloudsTexPaths);
	you_lostTex.LoadTextures(you_lostTexPaths);
	playerHeartTex.LoadTextures(playerHeartTexPaths);
	playerGoldTex.LoadTextures(playerGoldTexPaths);
	menu_backGroundTex.LoadTextures(menu_backGroundTexPaths);
	play_glow_Tex.LoadTextures(play_glow_TexPaths);
	exit_glow_Tex.LoadTextures(exit_glow_TexPaths);
	credits_glow_Tex.LoadTextures(credits_glow_TexPaths);
	skeleton_w_Tex.LoadTextures(skeleton_w_TexPaths);
	skeleton_d_Tex.LoadTextures(skeleton_d_TexPaths);
	ogre_w_Tex.LoadTextures(ogre_w_TexPaths);
	ogre_d_Tex.LoadTextures(ogre_d_TexPaths);
	particleTex.LoadTextures(particleTexPaths);
	arrowTex.LoadTextures(arrowTexPaths);
	rangeTex.LoadTextures(towerRangeTexPaths);
	towerArcherTex.LoadTextures(towerArcherTexPaths);
	tree_1Tex.LoadTextures(tree_1TexPaths);
	forestTex.LoadTextures(forestTexPaths);
	stone_1Tex.LoadTextures(stone_1TexPaths);
	stone_2Tex.LoadTextures(stone_2TexPaths);
	buildingAreaTex.LoadTextures(buildingAreaTexPaths);
	buildingAreaOverTex.LoadTextures(buildingAreaOverTexPaths);
	towersMenuTex.LoadTextures(towersMenuTexPaths);
	towersUpgradeTex.LoadTextures(towersUpgradeTexPaths);
	tower_archer_overTex.LoadTextures(tower_archer_overTexPaths);
	towersUpgradeDTex.LoadTextures(towerUpgradeDTexPaths);
	bush_1Tex.LoadTextures(bush_1TexPaths);
	stump_1Tex.LoadTextures(stump_1TexPaths);
	tree_2Tex.LoadTextures(tree_2TexPaths);

	// CARREGA AUDIO
	Music mainTheme;
	Music battleMusic;
	mainTheme.LoadAudio("audio/main_theme.mp3");
	mainTheme.SetVolume(25);
	battleMusic.LoadAudio("audio/battleMusic.mp3");
	bool firstBattleMusicPlay;

	Sound skeletonVoice;
	skeletonVoice.LoadAudio("audio/zombie_voice_brains_09.wav");
	Sound skeletonDying_1;
	skeletonDying_1.LoadAudio("audio/skeletonDying_1.wav");
	Sound skeletonDying_2;
	skeletonDying_2.LoadAudio("audio/skeletonDying_2.wav");
	Sound skeletonDying_3;
	skeletonDying_3.LoadAudio("audio/skeletonDying_3.wav");
	Sound skeletonDying_4;
	skeletonDying_4.LoadAudio("audio/skeletonDying_4.wav");
	Sound skeletonDying_5;
	skeletonDying_5.LoadAudio("audio/skeletonDying_5.wav");
	vector<Sound*> skeletonDying_Sounds = { &skeletonDying_1 , &skeletonDying_2 , &skeletonDying_3, &skeletonDying_4, &skeletonDying_5 };

	Sound ogreDying_1;
	ogreDying_1.LoadAudio("audio/ogreDying_1.wav");
	Sound ogreDying_2;
	ogreDying_2.LoadAudio("audio/ogreDying_2.wav");
	Sound ogreDying_3;
	ogreDying_3.LoadAudio("audio/ogreDying_3.wav");
	Sound ogreDying_4;
	ogreDying_4.LoadAudio("audio/ogreDying_4.wav");
	Sound ogreDying_5;
	ogreDying_5.LoadAudio("audio/ogreDying_5.wav");
	vector<Sound*> ogreDying_Sounds = { &ogreDying_1 , &ogreDying_2 , &ogreDying_3, &ogreDying_4, &ogreDying_5 };

	Sound arrowShot_1;
	Sound arrowShot_2;
	Sound arrowShot_3;
	Sound arrowShot_4;
	Sound arrowShot_5;
	arrowShot_1.LoadAudio("audio/arrowShot_1.wav");
	arrowShot_2.LoadAudio("audio/arrowShot_2.wav");
	arrowShot_3.LoadAudio("audio/arrowShot_3.wav");
	arrowShot_4.LoadAudio("audio/arrowShot_4.wav");
	arrowShot_5.LoadAudio("audio/arrowShot_5.wav");
	vector<Sound*> arrowShot_Sounds = { &arrowShot_1 , &arrowShot_2, &arrowShot_3, &arrowShot_4, &arrowShot_5 };

	Object2D_Static buildingArea;
	Object2D_Static buildingAreaOver;
	Object2D_Static tower_archer_over;
	Object2D_Animated particle;
	Object2D_Animated arrow;

	Object2D_Animated clouds;
	vector<Particle> cloudParticles;
	ParticleEmitter cloudsEmitter;
	clouds.CreateAnimatedObject2D(&shader_Animation_NL, cloudsTex, 1, 1, 0, 0);
	float cloudsSize[2] = { 1.0, 0.1375 };
	float cloudsPos[3] = { -0.8, 0.7, 1 };
	float cloudsRot[3] = { 0, 0, 1 };
	float cloudsColor[4] = { 0, 0, 0, 0 };
	float cloudsPntList[2][2] = { {0.0, 0.6},{0.0, -0.9} };
	cloudsEmitter.SetPosition(cloudsPos);
	cloudsEmitter.SetEmissionTime(-1);
	cloudsEmitter.SetEmissionDelay(1);
	cloudsEmitter.RandPosBtw2Pts(cloudsPntList);
	cloudsEmitter.SetParticleList(cloudParticles);
	cloudsEmitter.SetParticleProperties(clouds, cloudsSize, 0, 200, 10, cloudsColor, 0);
	float cloudsChgColor[4] = { 1, 1, 1, 1.0 };
	float cloudsChgColorVel[4] = { 0.5, 0.5, 0.5, .05 };

	// LEAF EMITTER
	Object2D_Animated leaf;
	vector<Particle> leafParticles;
	ParticleEmitter leafEmitter;
	leaf.CreateAnimatedObject2D(&shader_Animation_NL, leafTex, 1, 1, 0, 0);
	float leafSize[2] = { 0.08, 0.08 };
	float leafPos[3] = { 0, 0, 1 };
	float leafRot[3] = { 0, 0, 1 };
	float leafColor[4] = { 0, 0, 0, 1 };
	leafEmitter.SetEmissionTime(-1);
	float leafPntList[2][2] = { {-1.0, 1.0},{-1.0, 0.5} };
	leafEmitter.RandPosBtw2Pts(leafPntList);
	leafEmitter.SetEmissionDelay(0.5);
	float leafChgColor[4] = { 0, 0, 0, 1 };
	float leafChgColorVel[4] = { 0, 0, 0, 0 };

	leafEmitter.SetParticleProperties(leaf, leafSize, 0, 60, 5000, leafColor, 0);

	// FIRE BASIS EMITTER
	Object2D_Animated fireBasis;
	vector<Particle> fireBasisParticles;
	ParticleEmitter fireBasisEmitter;
	fireBasis.CreateAnimatedObject2D(&shader_Animation_NL, fireTex, 1, 1, 0, 0);
	float fireBasisSize[2] = { 0.03f, 0.03f };
	float fireBasisPos[3] = { 0, 0, 1 };
	float fireBasisRot[3] = { 0, 0, 1 };
	float fireBasisColor[4] = { 1, 0.8, 0, 1 };
	fireBasisEmitter.SetPosition(fireBasisPos);
	fireBasisEmitter.SetEmissionTime(-1);
	fireBasisEmitter.SetEmissionDelay(0);
	fireBasisEmitter.SetParticleList(fireBasisParticles);
	fireBasisEmitter.SetParticleProperties(fireBasis, fireBasisSize, 0, 15, 10000, fireBasisColor, 0);
	vector<array<float, 2>> fireBasisEmitterPntList = { {-0.71754,-0.04595},{-0.71838,-0.03168},{-0.71838,-0.0155},{-0.71838,-0.00275},{-0.7174,0.01195},{-0.71691,0.02911},{-0.71691,0.04725},{-0.71593,0.06392},{-0.71495,0.08206},{-0.71446,0.10118},{-0.71397,0.11686},{-0.7125,0.135},{-0.7125,0.15461},{-0.70416,0.17226},{-0.6821,0.17275},{-0.6669,0.1703},{-0.65269,0.16442},{-0.64,0.16},{-0.62965,0.15069},{-0.61837,0.13696},{-0.61151,0.11981},{-0.612,0.1051},{-0.61837,0.09088},{-0.62867,0.07765},{-0.63798,0.06637},{-0.64828,0.05804},{-0.66543,0.05166},{-0.6772,0.04921},{-0.68798,0.04921},{-0.70269,0.04921},{-0.46934,0.07716},{-0.47865,0.08598},{-0.49434,0.08843},{-0.5115,0.08598},{-0.52522,0.08157},{-0.53895,0.07323},{-0.54974,0.06294},{-0.55611,0.05117},{-0.56297,0.03597},{-0.5664,0.02176},{-0.56738,0.00411},{-0.56591,-0.01207},{-0.55856,-0.02727},{-0.5463,-0.03854},{-0.53013,-0.04344},{-0.50954,-0.04246},{-0.49189,-0.03658},{-0.47669,-0.02874},{-0.46394,-0.02482},{-0.45953,-0.03658},{-0.45365,-0.04933},{-0.44875,-0.05521},{-0.46688,-0.01158},{-0.46639,0.00509},{-0.46836,0.01833},{-0.46836,0.03352},{-0.46836,0.04676},{-0.46836,0.05902},{-0.46836,0.06931},{-0.40315,0.1002},{-0.40413,0.08696},{-0.40462,0.07274},{-0.40658,0.05804},{-0.40609,0.04382},{-0.40609,0.03009},{-0.40413,0.01538},{-0.4,0},{-0.39972,-0.01501},{-0.39531,-0.02776},{-0.38894,-0.03609},{-0.37521,-0.04197},{-0.35756,-0.04295},{-0.34,-0.04},{-0.32226,-0.0356},{-0.30756,-0.03364},{-0.30756,-0.04688},{-0.30756,-0.05668},{-0.30805,-0.0204},{-0.30854,-0.00521},{-0.30854,0.01146},{-0.30903,0.02911},{-0.3105,0.04578},{-0.3105,0.06049},{-0.3105,0.07666},{-0.30903,0.09186},{-0.30903,0.10559},{-0.23494,0.18125},{-0.23494,0.16469},{-0.23494,0.15011},{-0.23626,0.13554},{-0.23692,0.12163},{-0.23759,0.10639},{-0.23759,0.08983},{-0.23759,0.07526},{-0.23825,0.05737},{-0.23825,0.04346},{-0.23891,0.02955},{-0.23957,0.01298},{-0.23957,-0.00623},{-0.24024,-0.02676},{-0.24024,-0.04001},{-0.24024,-0.05127},{-0.11309,0.08934},{-0.13074,0.08737},{-0.14545,0.07945},{-0.1562,0.06983},{-0.16525,0.05569},{-0.16978,0.04041},{-0.17374,0.02627},{-0.17374,0.0093},{-0.16865,-0.0105},{-0.15903,-0.02465},{-0.14658,-0.03596},{-0.13018,-0.04275},{-0.1132,-0.04332},{-0.09623,-0.0354},{-0.08039,-0.02125},{-0.06964,-0.00145},{-0.06681,0.01835},{-0.06794,0.03815},{-0.0719,0.05682},{-0.07926,0.06983},{-0.08831,0.08115},{-0.09849,0.08454},{0.22286,0.14282},{0.2,0.15},{0.18552,0.15357},{0.17025,0.15357},{0.15,0.15},{0.1346,0.14282},{0.12046,0.13207},{0.11084,0.11792},{0.1,0.1},{0.11027,0.08228},{0.12498,0.07323},{0.14252,0.0687},{0.16402,0.06531},{0.18156,0.06078},{0.2,0.05},{0.21381,0.03985},{0.2223,0.0257},{0.22343,0.01099},{0.2189,-0.00768},{0.20872,-0.02069},{0.19401,-0.02917},{0.18043,-0.0354},{0.16515,-0.03992},{0.14196,-0.04219},{0.12329,-0.03879},{0.10745,-0.0337},{0.09047,-0.02578},{0.07916,-0.01503},{0.07689,-0.00202},{0.20985,0.14565},{0.10179,0.0902},{0.10179,0.1117},{0.3,0.1},{0.30037,0.08285},{0.29981,0.06531},{0.3,0.05},{0.29981,0.03136},{0.29924,0.01892},{0.3,0},{0.29924,-0.01446},{0.29981,-0.02748},{0.29981,-0.03936},{0.3,-0.05},{0.30546,0.14338},{0.30546,0.15696},{0.30546,0.16715},{0.3,0.15},{0.32074,0.15583},{0.29415,0.16036},{0.38637,0.18242},{0.38637,0.16884},{0.38524,0.1513},{0.38467,0.1332},{0.3841,0.12019},{0.3841,0.10548},{0.38297,0.08907},{0.3841,0.07549},{0.3841,0.06191},{0.3841,0.04664},{0.38354,0.03023},{0.38297,0.01778},{0.38297,0.00251},{0.38297,-0.01333},{0.38241,-0.02861},{0.38241,-0.04162},{0.38241,-0.0552},{0.45,0.1},{0.45143,0.08454},{0.45652,0.07097},{0.46161,0.05739},{0.46614,0.03985},{0.4718,0.02457},{0.47915,0.0059},{0.48311,-0.0105},{0.48877,-0.02408},{0.49273,-0.03823},{0.5,-0.05},{0.50914,-0.03257},{0.51401,-0.01815},{0.51897,-0.00451},{0.52434,0.01037},{0.53012,0.02607},{0.53508,0.04343},{0.54087,0.05665},{0.54665,0.07194},{0.54996,0.08558},{0.55699,0.10129},{0.68923,0.08434},{0.67849,0.09013},{0.66444,0.09302},{0.65038,0.09261},{0.63303,0.08682},{0.62,0.08},{0.61319,0.07236},{0.60617,0.06368},{0.59955,0.05335},{0.59377,0.03971},{0.59294,0.02731},{0.5917,0.01119},{0.5917,0.00086},{0.59211,-0.01112},{0.59749,-0.02228},{0.60617,-0.03013},{0.61484,-0.03798},{0.62931,-0.04336},{0.64419,-0.0446},{0.65617,-0.04253},{0.66774,-0.03674},{0.67973,-0.02972},{0.69419,-0.02021},{0.69832,-0.03055},{0.7,-0.04},{0.70494,-0.04956},{0.70907,-0.05617},{0.68745,-0.02496},{0.69371,-0.01068},{0.69371,0.00125},{0.69332,0.01083},{0.69234,0.02257},{0.69234,0.03411},{0.69195,0.04447},{0.69156,0.05445},{0.69039,0.06442},{0.69078,0.07635},{0.69,0.07} };
	fireBasisEmitter.RandBtwList(fireBasisEmitterPntList);
	float fireBasisChgColor[4] = { 1, 0, 0, 0 };
	float fireBasisChgColorVel[4] = { 0.05, 0.05, 0.05, 0.05 };

	// FIRE EMITTER 
	vector<Particle> fireParticles;
	ParticleEmitter fireEmitter;
	fireEmitter.SetPosition(fireBasisPos);
	fireEmitter.SetEmissionTime(-1);
	fireEmitter.SetEmissionDelay(0);
	fireEmitter.SetParticleList(fireParticles);
	fireEmitter.SetParticleProperties(fireBasis, fireBasisSize, 0, 15, 10000, fireBasisColor, 0);
	fireEmitter.RandBtwList(fireBasisEmitterPntList);
	float fireEmitterNewSize[2] = { 0.0, 0.0 };
	float fireChgColor[4] = { 1, 0, 0, 0 };
	float fireChgColorVel[4] = { 0.2, 0.2, 0.2, 0.2 };

	// --- especifica a unidade de textura que cada sampler pertence (só precisa ser feito uma vez)
	Texture texture;
	texture.AssignTextureUnits(shader_Interface, "texture0", 0);
	texture.AssignTextureUnits(shader_Animation_NL, "texture0", 0);
	texture.AssignTextureUnits(shader_Static_NL, "texture0", 0);
	texture.AssignTextureUnits(shader_Static, "dif", 0);
	texture.AssignTextureUnits(shader_Static, "norm", 1);
	texture.AssignTextureUnits(shader_Static, "spec", 2);
	texture.AssignTextureUnits(shader_Static, "amb", 3);
	texture.AssignTextureUnits(shader_Static, "emit", 4);
	texture.AssignTextureUnits(shader_Animation, "dif", 0);
	texture.AssignTextureUnits(shader_Animation, "norm", 1);
	texture.AssignTextureUnits(shader_Animation, "spec", 2);
	texture.AssignTextureUnits(shader_Animation, "amb", 3);
	texture.AssignTextureUnits(shader_Animation, "emit", 4);

	Map forest;

	Object2D_Animated you_lost;
	Object2D_Interface playerHeart;
	Object2D_Interface playerGold;
	Object2D_Static menu_backGround;
	Object2D_Static play_glow;
	Object2D_Static exit_glow;
	Object2D_Static credits_glow;
	you_lost.CreateAnimatedObject2D(&shader_Animation_NL, you_lostTex, 5, 5, 0, 0);
	playerHeart.CreateInterfaceObject2D(&shader_Interface, playerHeartTex);
	playerGold.CreateInterfaceObject2D(&shader_Interface, playerGoldTex);
	menu_backGround.CreateStaticObject2D(&shader_Static_NL, menu_backGroundTex);
	play_glow.CreateStaticObject2D(&shader_Static_NL, play_glow_Tex);
	exit_glow.CreateStaticObject2D(&shader_Static_NL, exit_glow_Tex);
	credits_glow.CreateStaticObject2D(&shader_Static_NL, credits_glow_Tex);
	float you_lostPos[3] = { 0, 0, 1 };
	float you_lostSize[2] = { 1, 1 };
	float you_lostColor[4] = { 0, 0, 0, 1 };
	float yout_lostRot[3] = { 0, 0, 1 };
	float playerHeartPos[3] = { -0.9, 0.9, 1.0 };
	float playerHeartSize[2] = { 0.08, 0.08 };
	float playerGoldPos[3] = { -0.5, 0.9, 1.0 };
	float playerGoldSize[2] = { 0.08, 0.08 };
	float menu_backGroundPos[3] = { 0, 0, 1.0 };
	float menu_backGroundSize[2] = { 1, 1 };
	float menu_backGroundRot[3] = { 0, 0, 1 };
	float menu_backGroundColor[4] = { 0, 0, 0, 1 };
	float playButtonPos[2] = { forest.position[0] - 0.5f , forest.position[1] - 0.53f };
	float playButtonSize[2] = { forest.size[0] / 6.4f, forest.size[1] / 16.0f };
	float exitButtonPos[2] = { forest.position[0] - 0.45f , forest.position[1] - 0.73f };
	float exitButtonSize[2] = { forest.size[0] / 5.81f, forest.size[1] / 21.3f };
	float creditsButtonPos[2] = { 0.77f, -0.73f };
	float creditsButtonSize[2] = { 0.14, 0.05 };
	float buttonRot[3] = { 0, 0, 1 };
	float buttonColor[4] = { 0, 0, 0, 1 };

	Player player;

	Graph forestPath;
	vector<GameObject*> gameObjects;

	vector<Enemy*> enemies;
	int outwardEnemies = 0;
	int totalOutwardEnemies = 0;

	TimeStep skeletonTime;
	float skeletonTimer = 0.0f;
	const int totalSkeletons = 1000;
	Skeleton *skeletons = new Skeleton[totalSkeletons];
	TimeStep skeletonReleaseTime;
	float skeletonReleaseTimer = 0.0f;
	float skeletonReleaseSeconds = 0.0f;
	int totalAliveSkeletons = 0;
	int skeletonsIterator = 0;
	TimeStep skeletonVoiceTime;
	float skeletonVoiceTimer = 0.0f;

	TimeStep ogreTime;
	float ogreTimer = 0.0f;
	const int totalOgres = 1000;
	Ogre *ogres = new Ogre[totalOgres];
	TimeStep ogreReleaseTime;
	float ogreReleaseTimer = 0.0f;
	float ogreReleaseSeconds = 0.0f;
	int totalAliveOgres = 0;
	int ogresIterator = 0;

	int totalArcherTowers = 0;
	
	vector<ArcherTower*> archerTowers;

	MapObject *stump_1 = new MapObject[5];
	MapObject *trees_1 = new MapObject[10];
	MapObject *trees_2 = new MapObject[10];
	MapObject *stones_1 = new MapObject[10];
	MapObject *stones_2 = new MapObject[10];
	MapObject *bushes = new MapObject[20];

	vector<BuildingArea> buildingAreas;
	float buildingAreasPos[8][3] = { { -0.2, -0.14, 1.0 }, { 0.2, 0.21, 1.0 }, { -0.2, 0.21, 1.0 }, { 0.2, -0.14, 1.0 }, { -0.2, -0.7, 1.0 }, { 0.2, 0.7, 1.0 }, { 0.7, -0.14, 1.0 }, { -0.7, 0.21, 1.0 } };
	for (int i = 0; i < 8; i++)
	{
		BuildingArea *ba = new BuildingArea;
		buildingAreas.push_back(*ba);
		delete ba;
	}

	TimeStep lightTime;
	float lightTimer;
	float dayDurationSeconds = 60.0f;
	float lightLevel = 0.2f;

	bool leftMousePressed = false;
	bool rightMousePressed = false;

	bool inMenu = true;
	bool inCredits = false;
	bool firstLoad = true;
	bool menuMusicIsPlaying = false;
	bool quit = 0;

	while (quit == 0) {

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		float viewXY[2] = { viewX, viewY };
		SDL_GetMouseState(&mouseX, &mouseY);
		int mouseXY[2] = { mouseX, mouseY };
		float mouseCoords[2] = { mu.ScreenToVertexCoords(window, mouseXY)[0],   mu.ScreenToVertexCoords(window, mouseXY)[1] };
		
		if (inMenu)
		{
			if (firstLoad)
			{
				firstLoad = false;
				delete[] stump_1;
				delete[] trees_1;
				delete[] trees_2;
				delete[] stones_1;
				delete[] bushes;
				delete[] skeletons;
				delete[] ogres;

				player.CreatePlayer();
				skeletonTimer = 0.0f;
				skeletonReleaseSeconds = 1.0f;
				ogreTimer = 0.0f;
				ogreReleaseSeconds = 10.0f;

				firstBattleMusicPlay = true;

				if (!menuMusicIsPlaying) {
					mainTheme.PlayAudio();
					menuMusicIsPlaying = true;
				}

				selector.isOver = false;
				selector.selected = false;
				enemies.clear();
				gameObjects.clear();
				archerTowers.clear();

				lightTimer = 0.0f;
				totalArcherTowers = 0;
				totalAliveSkeletons = 0;
				skeletonsIterator = 0;
				totalAliveOgres = 0;
				ogresIterator = 0;
				outwardEnemies = 0;
				totalOutwardEnemies = 0;

				skeletons = new Skeleton[totalSkeletons];
				ogres = new Ogre[totalOgres];

				stump_1 = new MapObject[5];

				for (int i = 0; i < 5; i++)
				{
					stump_1[i].size[1] = mu.GenRandFloat(0.04f, 0.06f);
					stump_1[i].size[0] = mu.GenRandFloat(0.05f, 0.08f);
					float stump_1_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - stump_1_Pos[1]) / 2.0f) + (stump_1[i].size[1] / 2.0f) };
					stump_1[i].SetPosition(stump_1_Pos);
					stump_1[i].CreateMapObject(shader_Static, stump_1Tex, gameObjects);
				}

				trees_1 = new MapObject[10];

				for (int i = 0; i < 5; i++)
				{
					trees_1[i].size[1] = mu.GenRandFloat(0.15f, 0.2f);
					trees_1[i].size[0] = mu.GenRandFloat(0.05f, 0.10f);
					float tree_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - tree_Pos[1]) / 2.0f) + (trees_1[i].size[1] / 2.0f) };
					trees_1[i].SetPosition(tree_Pos);
					trees_1[i].CreateMapObject(shader_Static, tree_1Tex, gameObjects);
				}

				trees_2 = new MapObject[10];

				for (int i = 0; i < 5; i++)
				{
					trees_2[i].size[1] = mu.GenRandFloat(0.12f, 0.15f);
					trees_2[i].size[0] = mu.GenRandFloat(0.04f, 0.08f);
					float tree_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - tree_Pos[1]) / 2.0f) + (trees_2[i].size[1] / 2.0f) };
					trees_2[i].SetPosition(tree_Pos);
					trees_2[i].CreateMapObject(shader_Static, tree_2Tex, gameObjects);
				}

				stones_1 = new MapObject[10];

				for (int i = 0; i < 10; i++)
				{
					stones_1[i].size[1] = mu.GenRandFloat(0.05f, 0.06f);
					stones_1[i].size[0] = mu.GenRandFloat(0.05f, 0.1f);
					float stone_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - stone_Pos[1]) / 2.0f) + (stones_1[i].size[1] / 2.0f) };
					stones_1[i].SetPosition(stone_Pos);
					stones_1[i].CreateMapObject(shader_Static, stone_1Tex, gameObjects);
				}

				stones_2 = new MapObject[10];

				for (int i = 0; i < 10; i++)
				{
					stones_2[i].size[1] = mu.GenRandFloat(0.03f, 0.04f);
					stones_2[i].size[0] = mu.GenRandFloat(0.03f, 0.04f);
					float stone_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - stone_Pos[1]) / 2.0f) + (stones_2[i].size[1] / 2.0f) };
					stones_2[i].SetPosition(stone_Pos);
					stones_2[i].CreateMapObject(shader_Static, stone_2Tex, gameObjects);
				}

				bushes = new MapObject[20];
				for (int i = 0; i < 20; i++)
				{
					bushes[i].size[1] = mu.GenRandFloat(0.035f, 0.045f);
					bushes[i].size[0] = mu.GenRandFloat(0.025f, 0.05f);
					float bushes_Pos[3] = { mu.GenRandFloat(-0.9f, 1.0f), mu.GenRandFloat(-0.9f, 1.0f), ((1.0f - bushes_Pos[1]) / 2.0f) + (bushes[i].size[1] / 2.0f) };
					bushes[i].SetPosition(bushes_Pos);
					bushes[i].CreateMapObject(shader_Static, bush_1Tex, gameObjects);
				}

				forest.CreateMap(shader_Static, forestTex);
				particle.CreateAnimatedObject2D(&shader_Animation_NL, particleTex, 1, 1, 0, 0);
				arrow.CreateAnimatedObject2D(&shader_Animation, arrowTex, 1, 1, 0, 0);
				buildingArea.CreateStaticObject2D(&shader_Static, buildingAreaTex);
				buildingAreaOver.CreateStaticObject2D(&shader_Static_NL, buildingAreaOverTex);
				tower_archer_over.CreateStaticObject2D(&shader_Static_NL, tower_archer_overTex);

				for (size_t i = 0; i < buildingAreas.size(); i++)
				{
					buildingAreas[i].CreateBuildingArea(buildingArea, buildingAreaOver, buildingAreasPos[i], &shader_Animation_NL, &shader_Static_NL, &shader_Animation, &shader_Static, towersMenuTex, text, &gameObjects);
					buildingAreas[i].LoadArcherTower(&archerTowers, towersUpgradeTex, towersUpgradeDTex, towerArcherTex, arrowTex, rangeTex, &arrowShot_Sounds, &totalArcherTowers, tower_archer_over, particle);
				}

			}

			menu_backGround.DrawStatic(menu_backGroundSize, menu_backGroundPos, viewXY, menu_backGroundRot, 0, menu_backGroundColor);
			if (selector.isUnderMouse(mouseCoords, playButtonPos, playButtonSize))
			{
				play_glow.DrawStatic(menu_backGroundSize, menu_backGroundPos, viewXY, buttonRot, 0, buttonColor);
				if (selector.isSelected(mouseCoords, playButtonPos, playButtonSize, leftMousePressed)) inMenu = false;
			}
			if (selector.isUnderMouse(mouseCoords, exitButtonPos, exitButtonSize))
			{
				exit_glow.DrawStatic(menu_backGroundSize, menu_backGroundPos, viewXY, buttonRot, 0, buttonColor);
				if (selector.isSelected(mouseCoords, exitButtonPos, exitButtonSize, leftMousePressed)) quit = 1;
			}
			if (selector.isUnderMouse(mouseCoords, creditsButtonPos, creditsButtonSize))
			{
				credits_glow.DrawStatic(menu_backGroundSize, menu_backGroundPos, viewXY, buttonRot, 0, buttonColor);
				if (selector.isSelected(mouseCoords, creditsButtonPos, creditsButtonSize, leftMousePressed))
				{
					inCredits = true;
					inMenu = false;
				}
			}

			float cloudsVel[2] = { mu.GenRandFloat(0.01, 0.02), mu.GenRandFloat(-0.0005, 0.005) };
			cloudsEmitter.SetVelocity(cloudsVel);
			cloudsEmitter.ChangeColor(cloudsChgColor, cloudsChgColorVel);
			cloudsEmitter.Emit();

			float leafVel[2] = { mu.GenRandFloat(0.001, 0.2), mu.GenRandFloat(-0.1, -0.05) };
			leafEmitter.SetVelocity(leafVel);
			leafEmitter.ChangeColor(leafChgColor, leafChgColorVel);
			leafEmitter.Rotate(30);
			leafEmitter.Emit();
		}
		else if (inCredits)
		{
			text.RenderText("Programmer:", -0.8, 0.4, 0.001 * window.getWindowHeight(), glm::vec3(1.0, 1.0f, 0.0f));
			text.RenderText("Music by Matthew Pablo", -0.8, -0.3, 0.001 * window.getWindowHeight(), glm::vec3(1.0, 1.0f, 0.0f));
			text.RenderText("http://www.matthewpablo.com", -0.8, -0.5, 0.001 * window.getWindowHeight(), glm::vec3(1.0, 1.0f, 0.0f));

			fireBasisEmitter.ChangeColor(fireBasisChgColor, fireBasisChgColorVel);
			fireBasisEmitter.Emit();
			float fireVel[2] = { mu.GenRandFloat(-0.02, 0.02), mu.GenRandFloat(0.01, 0.05) };
			fireEmitter.SetVelocity(fireVel);
			fireEmitter.ChangeColor(fireChgColor, fireChgColorVel);
			fireEmitter.ChangeSize(fireEmitterNewSize, 0.05);
			fireEmitter.Emit();
		}
		else
		{
			if (firstBattleMusicPlay)
			{
				mainTheme.ResumeOrPauseMusic();
				menuMusicIsPlaying = false;
				battleMusic.PlayAudio();
				firstBattleMusicPlay = false;
			}

			skeletonTimer += skeletonTime.step(1);
			if (skeletonTimer > 10.0f)
			{
				if (skeletonReleaseSeconds > 10.0f)
				skeletonReleaseSeconds -= 0.1f;
				skeletonTimer = 0.0f;
			}

			ogreTimer += ogreTime.step(1);
			if (ogreTimer > 10.0f)
			{
				if (ogreReleaseSeconds > 10.0f)
				ogreReleaseSeconds -= 0.1f;
				ogreTimer = 0.0f;
			}

			// ILUMINAÇÃO DIA E NOITE
			if (lightTimer <= 2.0f * PI)
			{
				lightTimer += lightTime.step(1) * (2.0f * PI) / dayDurationSeconds;
			}
			else lightTimer = 0.0f;
			float lightPosTemp[2] = { cos(lightTimer) , sin(lightTimer) };
			float lightposition_day[3] = { mu.TextureToVertexCoords(lightPosTemp)[0], mu.TextureToVertexCoords(lightPosTemp)[1], lightLevel };
			float lightIntensity_day = (lightposition_day[1] + 1.0f) / 2.0f;
			float difuseLightColor[3] = { lightIntensity_day * 0.7f + abs(cos(lightTimer)) * 0.1f, lightIntensity_day * 0.7f, lightIntensity_day * 1.0f };
			float specularLightColor[3] = { lightIntensity_day * 0.7f + abs(cos(lightTimer)) * 0.1f, lightIntensity_day * 0.7f, lightIntensity_day * 1.0f };
			float ambientLightColor[3] = { lightIntensity_day * 0.5f + abs(cos(lightTimer)) * 0.08f, lightIntensity_day * 0.5f, lightIntensity_day * 0.5f };
			float lightposition_night[3] = { mu.TextureToVertexCoords(lightPosTemp)[0], mu.TextureToVertexCoords(lightPosTemp)[1], lightLevel };
			float lightIntensity_night = (-lightposition_night[1] + 1.0f) / 2.0f;
			float difuseLightColor2[3] = { lightIntensity_night * 0.3f + abs(cos(lightTimer)) * 0.1f, lightIntensity_night * 0.3f, lightIntensity_night * 0.8f };
			float specularLightColor2[3] = { lightIntensity_night * 0.3f + abs(cos(lightTimer)) * 0.1f, lightIntensity_night * 0.3f, lightIntensity_night * 0.8f };
			float ambientLightColor2[3] = { lightIntensity_day * 0.1f + abs(cos(lightTimer)) * 0.08f, lightIntensity_night * 0.1f, lightIntensity_night * 0.2f };
			float ambientLightColor3[3] = { 0.0f , 0.0f , 0.0f };
			float shininess = 1.0f;
			float falloff[3] = { .4f, 3.0f, 2.0f };
			Light light(lighShaderList, lightposition_day, difuseLightColor, specularLightColor, ambientLightColor, shininess, falloff);
			Light light2(lighShaderList, lightposition_night, difuseLightColor2, specularLightColor2, ambientLightColor2, shininess, falloff);
			Light light3(lighShaderList, lightposition_day, difuseLightColor, specularLightColor, ambientLightColor3, shininess, falloff);
			Light light4(lighShaderList, lightposition_night, difuseLightColor2, specularLightColor2, ambientLightColor3, shininess, falloff);
			LightSystem lightSys;
			light.EnableDirectionalLight();
			light2.EnableDirectionalLight();
			lightSys.AddLight(light);
			lightSys.AddLight(light2);
			lightSys.AddLight(light3);
			lightSys.AddLight(light4);
			lightSys.DrawLights(lighShaderList);

			//OBS: Quanto maior o Z do obj, mais proximo ele está da câmera. SEMPRE desenhar na ordem do Z.
			forest.DrawMapObject(viewXY);

			skeletonReleaseTimer += skeletonReleaseTime.step(1);
			if (skeletonReleaseTimer >= skeletonReleaseSeconds && skeletonsIterator < totalSkeletons)
			{
				skeletons[skeletonsIterator].CreateSkeleton(shader_Animation, skeleton_w_Tex, skeleton_d_Tex, forestPath, &skeletonDying_Sounds, gameObjects, enemies, player);
				skeletonReleaseTimer = 0.0f;
				skeletonsIterator++;
			}

			for (int i = 0; i < totalSkeletons; i++)
			{
				float skeletonsAlive = 0;
				if (skeletons[i].alive) skeletonsAlive++;
				totalAliveSkeletons = skeletonsAlive;
			}

			ogreReleaseTimer += ogreReleaseTime.step(1);
			if (ogreReleaseTimer >= ogreReleaseSeconds && ogresIterator < totalOgres)
			{
				ogres[ogresIterator].CreateOgre(shader_Animation, ogre_w_Tex, ogre_d_Tex, forestPath, &ogreDying_Sounds, gameObjects, enemies, player);
				ogreReleaseTimer = 0.0f;
				ogresIterator++;
			}

			for (int i = 0; i < totalOgres; i++)
			{
				float ogresAlive = 0;
				if (ogres[i].alive) ogresAlive++;
				totalAliveOgres = ogresAlive;
			}

			skeletonVoiceTimer += skeletonVoiceTime.step(1);
			if (totalAliveSkeletons > 0 && skeletonVoiceTimer > 30.0f)
			{
				skeletonVoice.PlayAudio();
				skeletonVoiceTimer = 0.0f;
			}

			bool enemyOutward = false;
			for (int i = 0; i < enemies.size(); i++)
			{
				if (enemies[i]->outward) {
					outwardEnemies++;
					enemyOutward = true;	
				} 
			}
			totalOutwardEnemies = outwardEnemies;
			outwardEnemies = 0;
			if (enemyOutward) {
				if (player.GetLife() > 0) player.SetLife(player.GetInitialLife() - totalOutwardEnemies);
				enemyOutward = false;
			}

			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				if (!gameObjects[i]->exists)
				{
					gameObjects.erase(gameObjects.begin() + i);
				}
			}

			sorter.QuickSort(gameObjects, 0, gameObjects.size() - 1);

			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->Draw(viewXY);
			}

			for (int i = 0; i < archerTowers.size(); i++)
			{
				archerTowers[i]->SetTarget(enemies);
			}

			for (size_t i = 0; i < buildingAreas.size(); i++)
			{
				buildingAreas[i].DrawBuildingArea(mouseCoords, viewXY, leftMousePressed, player);
			}

			//Objetos de interface aqui
			playerHeart.DrawInterface(playerHeartSize, playerHeartPos, viewXY);
			text.RenderText(to_string(player.GetLife()), playerHeartPos[0] + 0.075, playerHeartPos[1] - 0.01, 0.9, glm::vec3(1.0, 0.0f, 0.0f));
			playerGold.DrawInterface(playerGoldSize, playerGoldPos, viewXY);
			text.RenderText(to_string(player.GetGold()), playerGoldPos[0] + 0.07, playerGoldPos[1] - 0.01, 0.9, glm::vec3(1.0, 0.0f, 0.0f));

			if (player.GetLife() <= 0)
			{
				player.Lost();
				you_lost.DrawAnimated(you_lostSize, you_lostPos, viewXY, yout_lostRot, 0, you_lostColor, false, 15, false);
			}
		}

		//PROCESSA INPUTS
		leftMousePressed = false;
		rightMousePressed = false;

		switch (window.SDL_ManageEvent())
		{
		case SDL_QUIT:
			quit = 1;
			SDL_Quit();
			Mix_Quit();
			glDeleteVertexArrays(1, vertices.GetVao());
			glDeleteBuffers(1, vertices.GetVbo());
			glDeleteBuffers(1, vertices.GetEbo());
			break;

		case SDL_MOUSEWHEEL:
			if (window.getEvnt().wheel.y > 0)
			{
				//if (lightLevel >= 0.0f)
				//lightLevel -= 0.04f;
			}
			else if (window.getEvnt().wheel.y < 0)
			{
				//if (lightLevel <= 5.0f)
				//lightLevel += 0.04f;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (window.getEvnt().button.button == (SDL_BUTTON_LEFT))
			{
				if (leftMousePressed) leftMousePressed = false;
				else leftMousePressed = true;
				rightMousePressed = false;
				//std::cout << "esquerdo pressionado!" << std::endl;
			}
			else if (window.getEvnt().button.button == (SDL_BUTTON_RIGHT))
			{
				if (rightMousePressed) rightMousePressed = false;
				else rightMousePressed = true;
				leftMousePressed = false;
				//std::cout << "direito pressionado!" << std::endl;
			}
			break;

		case SDL_KEYDOWN:
			int keyPressed = window.getEvnt().key.keysym.sym;
			switch (keyPressed) {
			case SDLK_ESCAPE:
				inMenu = true;
				firstLoad = true;
				inCredits = false;
				break;
			}
		}

		window.SDL_SwapWindow();
	}

	return 0;
}

