#include "PlayerManager.h"


PlayerManager::PlayerManager()
{
	maxPlayers = 0;

}

void PlayerManager::initPlayers(Phyre::PCluster* destCluster, int playerCount)
{
	maxPlayers = playerCount;

		for (int i = 0; i < playerCount; i++)
	{
		int playerNum = i;
		Player* newP = new Player(playerNum, false, destCluster);
		playerList.push_back(newP);
		playerList.at(i)->updateWorldMatrix(playerList.at(i)->getRotation().getZ(), playerList.at(i)->getPosition());
	}
}


void PlayerManager::updatePlayers()
{

	Phyre::PFramework::PApplication* pApp = Phyre::PFramework::PApplication::GetApplication();


	for (int i = 0; i < playerList.size(); i++)
	{
		Vector3 oldPosition = playerList.at(i)->getPosition();

		if (pApp->isJoypadButtonDown(Phyre::PInputBase::InputChannel_Button_Left, i) || (i == 0 && (pApp->isKeyDown(Phyre::PInputBase::InputChannel_Key_A))))
			{
				playerList.at(i)->incXPos(20);
			}


		if (pApp->isJoypadButtonDown(Phyre::PInputBase::InputChannel_Button_Right, i) || (i == 0 && (pApp->isKeyDown(Phyre::PInputBase::InputChannel_Key_D))))
			{
				playerList.at(i)->decXPos(20);
			}


		if (pApp->isJoypadButtonDown(Phyre::PInputBase::InputChannel_Button_Up, i) || (i == 0 && (pApp->isKeyDown(Phyre::PInputBase::InputChannel_Key_W))))
			{
				playerList.at(i)->incZPos(20);
			}


		if (pApp->isJoypadButtonDown(Phyre::PInputBase::InputChannel_Button_Down, i) || (i == 0 && (pApp->isKeyDown(Phyre::PInputBase::InputChannel_Key_S))))
			{
				playerList.at(i)->decZPos(20);
			}
	

		if (checkCollision(*playerList.at(i)))
			{
				playerList.at(i)->setPosition(oldPosition);
			}




		playerList.at(i)->updateWorldMatrix(playerList.at(i)->getRotation().getZ(), playerList.at(i)->getPosition());
	}



}

bool PlayerManager::checkCollision(Player &player)
{
	bool collided = false;
	float distanceBetween;
	Vector3 vectorBetween;

	for (int i = 0; i < playerList.size(); i++)
	{
		if (playerList.at(i)->getPNumber() != player.getPNumber())   
		{
			vectorBetween = playerList.at(i)->getPosition() - player.getPosition();
			distanceBetween = getDistanceBetween(vectorBetween);

			if (distanceBetween < 200)
			{
				return (player.minX() <= playerList.at(i)->maxX() && player.maxX() >= playerList.at(i)->minX()) &&
					   (player.minY() <= playerList.at(i)->maxY() && player.maxY() >= playerList.at(i)->minY()) &&
					   (player.minZ() <= playerList.at(i)->maxZ() && player.maxZ() >= playerList.at(i)->minZ());
			}
			
			
		}
	}
	return collided;

}

float PlayerManager::getDistanceBetween(Vector3 vectorBetween)
{
	return length(vectorBetween);
}


PlayerManager::~PlayerManager()
{
	for (int i = 0; i < playerList.size(); i++)
	{
		delete playerList.at(i);
	}
}
