#pragma once

#include <Phyre.h>
#include <Framework/PhyreFramework.h>
#include <Rendering/PhyreRendering.h>
#include <string>
#include <vector>
#include "Player.h"


using namespace std;
using namespace Vectormath::Aos;

class Player;

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

	void initPlayers(Phyre::PCluster* destCluster, int playerCount);
	void updatePlayers();
	bool checkCollision(Player &player);
	float getDistanceBetween(Vector3 vectorBetween);

private:
	
	std::vector<Player*> playerList;

	int maxPlayers;

};

