#pragma once
#include "PlayerManager.h"
//#include "PhyreStarter.h"

using namespace Vectormath::Aos;


class Player
{
public:
	Player(int pNumber, bool isVirus, Phyre::PCluster* destCluster);
	~Player();

	void  updateWorldMatrix(float zRotation, Vector3 Translation);

	//Setter functions
	void setVirusStatus(bool status) { isVirus = status; };

	void incremenetScore()	{ score += incVal; };
	void decrementScore()	{ score -= decVal; };

	void setPosition(Vector3 newPos) { position = newPos; };
	void setRotation(Vector3 newRot) { rotation = newRot; };

	void setXPos(float newX) { position.setX(newX); };
	void setYPos(float newY) { position.setY(newY); };
	void setZPos(float newZ) { position.setZ(newZ); };

	void incXPos(float newX) {position.setX(position.getX() + newX); };
	void incYPos(float newY) { position.setY(position.getY() + newY); };
	void incZPos(float newZ) { position.setZ(position.getZ() + newZ); };

	void decXPos(float newX) { position.setX(position.getX() - newX); };
	void decYPos(float newY) { position.setY(position.getY() - newY); };
	void decZPos(float newZ) { position.setZ(position.getZ() - newZ); };


	//Getter functions
	std::string getName()	{ return playerName; };
	bool checkVirus()		{ return isVirus; };
	int getScore()			{ return score; };
	int getPNumber()		{ return playerNumber; };
	float getXPos()			{ return position.getX(); };
	float getYPos()			{ return position.getY(); };
	float getZPos()			{ return position.getZ(); };
	float getThePos(int pos);
	float minX()			{ return (getXPos() - 50.0f); };
	float maxX()			{ return (getXPos() + 50.0f); };
	float minY()			{ return (getYPos() - 50.0f); };
	float maxY()			{ return (getYPos() + 50.0f); };
	float minZ()			{ return (getZPos() - 50.0f); };
	float maxZ()			{ return (getZPos() + 50.0f); };

	Vector3 getPosition()	{ return position; };
	Vector3 getRotation()	{ return rotation; };

private:
	//Fixed attributes
	std::string playerName;
	int playerNumber;

	//Mutable attributes
	bool isVirus;
	int score;
	Vector3 position;
	Vector3 rotation;

	Phyre::PWorldMatrix* pWorldMat;
	Phyre::PRendering::PMeshInstance* pMeshInstance;

	int incVal = 5;
	int decVal = 5;
};

