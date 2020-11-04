#include "BuildingArea.h"

	void BuildingArea::CreateBuildingArea(Object2D_Static buildingArea, Object2D_Static buildingAreaOver, float buildingAreaPos[3], Shader* shader_Animation_NL, Shader* shader_Static_NL, Shader* shader_Animation, Shader* shader_Static, Texture& towersMenuTex, Text text, vector<GameObject*>* gameObjects)
	{
		towerLevel = 1;
		towerSold = false;
		buildingIsUnderMouse = false;
		towerIsUnderMouse = false;
		towerIsShown = false;
		towerOverIsShown = false;
		buildingAreaVisible = true;
		towerPlaced = false;
		towerMenuOpened = false;
		towerMenuUpgradeOpened = false;
		upg1 = false;
		upg2 = false;
		s.isOver = false;
		s.selected = false;
		this->buildingArea = buildingArea;
		this->buildingAreaOver = buildingAreaOver;
		this->towersMenuTex = towersMenuTex;
		this->shader_Animation_NL = shader_Animation_NL;
		this->shader_Animation = shader_Animation;
		this->shader_Static = shader_Static;
		this->shader_Static_NL = shader_Static_NL;
		this->buildingAreaPos[0] = buildingAreaPos[0];
		this->buildingAreaPos[1] = buildingAreaPos[1];
		this->buildingAreaPos[2] = buildingAreaPos[2];
		this->text = text;
		this->gameObjects = gameObjects;
		towersMenuPos[0] = buildingAreaPos[0];
		towersMenuPos[1] = buildingAreaPos[1];
		towersMenuColor[0] = 0.0f;
		towersMenuColor[1] = 0.0f;
		towersMenuColor[2] = 0.0f;
		towersMenuColor[3] = 1.0f;
		towersMenuUpgradePos[0] = buildingAreaPos[0];
		towersMenuUpgradePos[1] = buildingAreaPos[1] + 0.05f;
		towerIconPos[0] = towersMenuPos[0] - ((towersMenuSize[0] / 5.0f) * 3.5f);
		towerIconPos[1] = towersMenuPos[1] - ((towersMenuSize[1] / 5.0f) * 3.5f);
		towerPosPrice[0] = towerIconPos[0] - 0.02f;
		towerPosPrice[1] = towerIconPos[1] - 0.08f;
		towerUpg1IconPos[0] = { towersMenuUpgradePos[0] - ((towersMenuUpgradeSize[0] / 3.0f) * 2.0f) };
		towerUpg1IconPos[1] = { towersMenuUpgradePos[1] + ((towersMenuUpgradeSize[1] / 5.0f) * 4.0f) };
		towerUpg2IconPos[0] = { towersMenuUpgradePos[0] + ((towersMenuUpgradeSize[0] / 3.0f) * 2.0f) };
		towerUpg2IconPos[1] = { towersMenuUpgradePos[1] + ((towersMenuUpgradeSize[1] / 5.0f) * 4.0f) };
		towerUpg1PosPrice[0] = { towerUpg1IconPos[0] - 0.02f };
		towerUpg1PosPrice[1] = { towerUpg1IconPos[1] - 0.08f };
		towerUpg2PosPrice[0] = { towerUpg2IconPos[0] - 0.02f };
		towerUpg2PosPrice[1] = { towerUpg2IconPos[1] - 0.08f };
		sellTowerIconPos[0] = towersMenuUpgradePos[0];
		sellTowerIconPos[1] = { towersMenuUpgradePos[1] - ((towersMenuUpgradeSize[1] / 5.0f) * 4.0f) };
		towerSellValuePos[0] = sellTowerIconPos[0] - 0.02f;
		towerSellValuePos[1] = sellTowerIconPos[1] - 0.08f;
	};

	void BuildingArea::LoadArcherTower(vector<ArcherTower*>* archerTowers, Texture towersMenuUpgradeTex, Texture towersMenuUpgradeDTex, Texture& towerTextures, Texture& arrowTex, Texture& towerRangeTex, vector<Sound*>* towerShotSounds, int* totalArcherTowers, Object2D_Static tower_archer_over, Object2D_Animated particle)
	{
		this->towerShotSounds = towerShotSounds;
		towersMenu.CreateAnimatedObject2D(shader_Animation_NL, towersMenuTex, 5, 5, 0, 0);
		towersMenuUpgrade.CreateAnimatedObject2D(shader_Animation_NL, towersMenuUpgradeTex, 5, 5, 0, 0);
		towersMenuUpgradeD.CreateAnimatedObject2D(shader_Animation_NL, towersMenuUpgradeDTex, 5, 5, 0, 25);
		this->tower_archer_over = tower_archer_over;
		this->archerTowers = archerTowers;
		this->towerTextures = towerTextures;
		arrow.CreateProjectile(*shader_Animation, arrowTex);
		arrow.size[0] = 0.03f;
		arrow.size[1] = 0.008f;
		float arrowPos[3] = { 0.0f, 0.0f, -1.0f };
		arrow.SetPosition(arrowPos);
		this->towerRange = towerRange;
		towerRange.CreateRangeObject(*shader_Static_NL, towerRangeTex);
		this->totalArcherTowers = totalArcherTowers;
		this->particle = particle;
	}

	void BuildingArea::DrawBuildingArea(float mouseCoords[2], float viewXY[2], bool leftMousePressed, Player& player)
	{
		if (!towerPlaced) // Se não há torre colocada...
		{
			if (s.isUnderMouse(mouseCoords, buildingAreaPos, buildingAreaSize)) // Se o mouse está sobre a área de construção
			{
				// glow sobre a área
				buildingAreaOver.DrawStatic(buildingAreaOverSize, buildingAreaPos, viewXY, buildingAreaRotation, buildingAreaDegrees, buildingAreaColor);
			}
			if (buildingAreaVisible) // Se a área de construção está visível
			{
				// desenha a área
				buildingArea.DrawStatic(buildingAreaSize, buildingAreaPos, viewXY, buildingAreaRotation, buildingAreaDegrees, buildingAreaColor);
			}
			// se clicou na área de construção, menu se abre
			if (s.isSelected(mouseCoords, buildingAreaPos, buildingAreaSize, leftMousePressed))
			{
				towerMenuOpened = true;
			}
			else if (!s.isUnderMouse(mouseCoords, towerIconPos, towerIconSize)) towerMenuOpened = false;

			if (towerMenuOpened || towerSold) // e se o menu está aberto
			{

				towersMenu.DrawAnimated(towersMenuSize, towersMenuPos, viewXY, buildingAreaRotation, 0, towersMenuColor, false, 40, false);

				if (towersMenu.animationFinished)
					text.RenderText(to_string(archerTowerTemp.price), towerPosPrice[0], towerPosPrice[1], 0.4f, glm::vec3(1.0, 0.8f, 0.0f));
				// se o mouse estiver sobre o ícone da torre arqueira...
				if (s.isUnderMouse(mouseCoords, towerIconPos, towerIconSize) && player.GetGold() >= archerTowerTemp.price)
				{
					// e se o esquerdo for pressionado...coloco a torre e área da construção n é visível
					if (leftMousePressed)
					{
						towerPlaced = true;
						towerIsShown = true;
					}
					// se ainda não registrou que o mouse está em cima e a torre está sendo mostrada...registre e desenhe a torre.
					if (!buildingIsUnderMouse && !towerIsShown)
					{
						archerTower = new ArcherTower;
						archerTowers->push_back(archerTower);
						towerPos[0] = buildingAreaPos[0];
						towerPos[1] = buildingAreaPos[1] + (archerTowers->at(*totalArcherTowers)->size[1] / 2.0f);
						towerPos[2] = ((1.0f - towerPos[1]) / 2.0f) + (archerTowers->at(*totalArcherTowers)->size[1] / 2.0f);
						archerTowers->at(*totalArcherTowers)->SetPosition(towerPos);

						archerTowers->at(*totalArcherTowers)->tempTower = true;
						archerTowers->at(*totalArcherTowers)->CreateArcherTower(*shader_Static, towerTextures, arrow, towerRange, towerShotSounds, *gameObjects);
						(*totalArcherTowers)++;
						buildingIsUnderMouse = true;
						towerIsShown = true;

						archerTowerTemp = *archerTower;
						archerTowerTemp.size[0] = archerTower->size[0] + 0.02;
						archerTowerTemp.size[1] = archerTower->size[1] + 0.02;
					}
					// se a torre for colocada, a torre deixa de ser temporária...
					if (towerPlaced) {
						archerTowers->at(*totalArcherTowers - 1)->tempTower = false;
						player.SetGold(player.GetGold() - archerTowerTemp.price);
					}
				}
				// mas se o mouse não está mais sobre o ícone da torre arqueira, mas a torre ainda está sendo mostrada...delete-a 
				else if (towerIsShown)
				{
					for (int i = 0; i < gameObjects->size(); i++)
					{
						for (int j = 0; j < archerTowers->size(); j++)
						{
							if (archerTowers->at(j)->internalObjID == gameObjects->at(i)->internalObjID)
							{
								if (archerTowers->at(j)->internalObjID == archerTower->internalObjID)
								{
									for (int k = 0; k < gameObjects->size(); k++)
									{
										if (archerTowers->at(j)->internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
										if (archerTowers->at(j)->projectile.internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
										if (archerTowers->at(j)->fire.internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
										if (archerTowers->at(j)->poison.internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
										if (archerTowers->at(j)->towerRange.internalObjID == gameObjects->at(k)->internalObjID) { gameObjects->erase(gameObjects->begin() + k); k = 0; }
									}

									delete archerTowers->at(j);

									archerTowers->erase(archerTowers->begin() + j);
									(*totalArcherTowers)--;
								}
							}
						}
					}
					// e registre que não está mais sob o mouse e não está sendo mostrada
					buildingIsUnderMouse = false;
					towerIsShown = false;
				}
				else // se não estiver mais com o mouse em cima e a torre já n é mostrada, mostre a área de construção visível...
				{
					buildingAreaVisible = true;
				}
			}
			if (towerSold) {
				buildingAreaVisible = true;
				towerSold = false;
				upg1 = false;
				upg2 = false;
			}
		}
		else if (towerPlaced)
		{
			if (s.isUnderMouse(mouseCoords, archerTowerTemp.position, archerTowerTemp.size)) // Se o mouse está sobre a área da torre
			{
				archerTower->SetRangeVisibility(true);
				// glow sobre a torre
				tower_archer_over.DrawStatic(archerTowerTemp.size, archerTowerTemp.position, viewXY, buildingAreaRotation, buildingAreaDegrees, buildingAreaColor);
			}
			else archerTower->SetRangeVisibility(false);
			if (s.isSelected(mouseCoords, archerTowerTemp.position, archerTowerTemp.size, leftMousePressed))
			{
				archerTower->SetRangeVisibility(true);
				towerMenuUpgradeOpened = true;
			}
			else if
				(!s.isUnderMouse(mouseCoords, towerUpg1IconPos, towerUpg1IconSize)
					&& !s.isUnderMouse(mouseCoords, towerUpg2IconPos, towerUpg2IconSize)
					&& !s.isUnderMouse(mouseCoords, sellTowerIconPos, sellTowerIconSize)
					) towerMenuUpgradeOpened = false;

			if (towerMenuUpgradeOpened)
			{
				if (towerLevel == 1 && player.GetGold() >= archerTowerTemp.upgradeToFirePrice)
				{
					towersMenuUpgrade.DrawAnimated(towersMenuUpgradeSize, towersMenuUpgradePos, viewXY, buildingAreaRotation, 0, buildingAreaColor, false, 40, false);
				}
				else {
					towersMenuUpgradeD.DrawAnimated(towersMenuUpgradeSize, towersMenuUpgradePos, viewXY, buildingAreaRotation, 0, buildingAreaColor, false, 0, false);
				}
				if (towersMenuUpgrade.animationFinished || towersMenuUpgradeD.animationFinished)
				{
					text.RenderText(to_string(archerTowerTemp.upgradeToFirePrice), towerUpg1PosPrice[0], towerUpg1PosPrice[1], 0.4f, glm::vec3(1.0, 0.8f, 0.0f));
					text.RenderText(to_string(archerTowerTemp.upgradeToPoisonPrice), towerUpg2PosPrice[0], towerUpg2PosPrice[1], 0.4f, glm::vec3(1.0, 0.8f, 0.0f));
					text.RenderText(to_string(archerTowerTemp.sellValue), towerSellValuePos[0], towerSellValuePos[1], 0.4f, glm::vec3(1.0, 0.8f, 0.0f));
				}

				if (s.isUnderMouse(mouseCoords, towerUpg1IconPos, towerUpg1IconSize) && player.GetGold() >= archerTowerTemp.upgradeToFirePrice)
				{
					archerTower->towerRange.size[0] = 0.28f * 1.33f;
					archerTower->towerRange.size[1] = 0.2f * 1.33f;
					if (leftMousePressed)
					{
						if (!upg1 && !upg2)
						{
							archerTower->towerRange.size[0] = 0.28f * 1.33f;
							archerTower->towerRange.size[1] = 0.2f * 1.33f;
							towerLevel = 2;
							player.SetGold(player.GetGold() - archerTowerTemp.upgradeToFirePrice);
							upg1 = true;
						}
					}
				}

				else if (s.isUnderMouse(mouseCoords, towerUpg2IconPos, towerUpg2IconSize) && player.GetGold() >= archerTowerTemp.upgradeToFirePrice)
				{
					archerTower->towerRange.size[0] = 0.28f * 1.33f;
					archerTower->towerRange.size[1] = 0.2f * 1.33f;
					if (leftMousePressed)
					{
						if (!upg1 && !upg2)
						{
							archerTower->towerRange.size[0] = 0.28f * 1.33f;
							archerTower->towerRange.size[1] = 0.2f * 1.33f;
							towerLevel = 2;
							player.SetGold(player.GetGold() - archerTowerTemp.upgradeToPoisonPrice);
							upg2 = true;
						}
					}
				}
				else if (s.isUnderMouse(mouseCoords, sellTowerIconPos, sellTowerIconSize))
				{
					if (leftMousePressed)
					{
						towerSold = true;
						towerPlaced = false;
						player.SetGold(player.GetGold() + archerTowerTemp.sellValue);
						towerLevel = 1;
					}
				}
				else
				{
					if (towerLevel == 1)
					{
						archerTower->towerRange.size[0] = 0.28f;
						archerTower->towerRange.size[1] = 0.2f;
					}
				}
			}
			if (upg1) archerTower->SetArrowOnFire(particle);
			else if (upg2) archerTower->SetArrowOnPoison(particle);
		}
	}