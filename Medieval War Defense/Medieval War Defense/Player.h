#pragma once

class Player
{
private:
	int initialLife;
	int initialGold;
	int currentGold;
	int currentLife;
	bool win;
public:
	void CreatePlayer();
	int GetInitialLife();
	void SetGold(int gold);
	int GetGold();
	void SetLife(int life);
	int GetLife();
	void Lost();
	void Won();
};
