#include "Player.h"

void Player::CreatePlayer()
{
	initialLife = 20;
	initialGold = 300;
	currentLife = initialLife;
	currentGold = initialGold;
}
int Player::GetInitialLife() { return initialLife; }
void Player::SetGold(int gold) { this->currentGold = gold; }
int Player::GetGold() { return currentGold; }
void Player::SetLife(int life) { this->currentLife = life; }
int Player::GetLife() { return currentLife; }
void Player::Lost() { win = false; }
void Player::Won() { win = true; };
