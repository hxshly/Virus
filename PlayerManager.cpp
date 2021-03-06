#include "PlayerManager.h"
#include <time.h>
#include <stdlib.h>


PlayerManager::PlayerManager()
	: maxPlayers (0)
{
	
}

void PlayerManager::initPlayers(Phyre::PCluster* destCluster, int playerCount)
{
	maxPlayers = playerCount;
	srand(time(NULL));
	int ranVir = rand() % (playerCount);
	

	for (int i = 0; i < playerCount; i++)
		{
			bool isVirus = false;
			if (i == ranVir)
			{
				isVirus = true;
			}
			int playerNum = i;
			Player* newP = new Player(playerNum, isVirus, destCluster);
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

		playerList.at(i)->countdownInvuln();
	
		//Check collision and if colided move the player to the position it was before collision
		if (checkCollision(*playerList.at(i)))
			{
				playerList.at(i)->setPosition(oldPosition);
			}
		

		/////////////////////////////////////////////////////////////////////////////
		//--------------Swap meshes if colliding with virus------------------------//
		/////////////////////////////////////////////////////////////////////////////

		//if the player is the virus, change the mesh to virus.
		if (playerList.at(i)->checkVirus())
		{
			playerList.at(i)->changeMeshToVirusMesh();
		}
		//else if it isnt the virus, change the mesh to the player mesh
		else if (!playerList.at(i)->checkVirus())
		{
			playerList.at(i)->changeMeshToPlayerMesh();
		}
		

		//----------score----------//new
		if (!playerList.at(i)->checkVirus())
		{
			playerList.at(i)->incremenetScore();
		}

		else if ((playerList.at(i)->checkVirus()) && (playerList.at(i)->getScore() > 1))
		{
			playerList.at(i)->decrementScore();
		}/*	*/
		//----------end of score----------//new

		//update the world matrix of player
		playerList.at(i)->updateWorldMatrix(playerList.at(i)->getRotation().getZ(), playerList.at(i)->getPosition());
	}



}

bool PlayerManager::checkCollision(Player &player)
{
	//Description:
	//Takes in a pointer to a player.
	//Checks against every player other than itself
	//and if the distance between them is less than 200 units, it checks if they have collided via AABB

	float distanceBetween;
	Vector3 vectorBetween;

	for (int i = 0; i < playerList.size(); i++)
	{
		//checks all players other than itself
		if (playerList.at(i)->getPNumber() != player.getPNumber())   
		{
			vectorBetween = playerList.at(i)->getPosition() - player.getPosition();
			distanceBetween = getDistanceBetween(vectorBetween);
			
			//Only checks to see if collided if they are closer than 200 units
			if (distanceBetween < 200)	
			{
				//checks to see if they have collided, if they have flip the virus status and return true
				if ((player.minX() <= playerList.at(i)->maxX() && player.maxX() >= playerList.at(i)->minX()) &&
					(player.minY() <= playerList.at(i)->maxY() && player.maxY() >= playerList.at(i)->minY()) &&
					(player.minZ() <= playerList.at(i)->maxZ() && player.maxZ() >= playerList.at(i)->minZ()))
						{
							//If the virus status is not the same (both not virus) flip the virus status
							if (!player.checkVirus() == playerList.at(i)->checkVirus())
								{	
									if (!player.checkVirus() && player.getInvuln() <= 0)
									{
									player.flipVirus();
									playerList.at(i)->flipVirus();
									playerList.at(i)->resetInvuln();
									}
									else if (player.checkVirus() && playerList.at(i)->getInvuln() <= 0)
									{
										player.flipVirus();
										player.resetInvuln();
										playerList.at(i)->flipVirus();
									}

								}
							return true;
						} ;
			}
				
		}
	}
	return false;

}

float PlayerManager::getDistanceBetween(Vector3 vectorBetween)
{
	return length(vectorBetween);
}

float PlayerManager::getScore(int pNumber)
{
	float score = playerList.at(pNumber)->getScore();
	return score;
}


PlayerManager::~PlayerManager()
{
	for (int i = 0; i < playerList.size(); i++)
	{
		delete playerList.at(i);
	}
}
