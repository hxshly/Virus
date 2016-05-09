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

	int Xdir = 0;
	int Ydir = 1;
	int Zdir = 2;


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
			int leftA, leftB;
			int rightA, rightB;
			int topA, topB;
			int bottomA, bottomB;

			//Calculate the sides of player 1 
			leftA = player.getXPos() - 50;
			rightA = player.getXPos() + 50;
			topA = player.getZPos() + 50;
			bottomA = player.getZPos() - 50;


			//Calculate the sides of player 2 
			leftB = playerList.at(i)->getXPos() - 50;
			rightB = playerList.at(i)->getXPos() + 50;
			topB = playerList.at(i)->getZPos() + 50;
			bottomB = playerList.at(i)->getZPos() - 50;


			//If any of the sides from 1 are outside of 2
			if (bottomA <= topB)
			{
				return false;
			}

			if (topA >= bottomB)
			{
				return false;
			}

			if (rightA <= leftB)
			{
				return false;
			}

			if (leftA >= rightB)
			{
				return false;
			}

			//If none of the sides from A are outside B
			return true;
		}




			//vectorBetween = playerList.at(i)->getPosition() - player.getPosition();
			//distanceBetween = getDistanceBetween(vectorBetween);

			//if (distanceBetween <= 125)
			//{
			//	collided = true;
			//	return collided;
			//}
		
	}

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
