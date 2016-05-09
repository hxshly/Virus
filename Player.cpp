#include "Player.h"
#include "MeshInstance.h"
#include <Framework/PhyreFramework.h>
#include <../Samples/Common/PhyreSamplesCommon.h>

using namespace Phyre;
using namespace PFramework;
using namespace PRendering;
using namespace PSamplesCommon;


Player::Player(int pNumber, bool isPVirus, Phyre::PCluster* destCluster)
{
	pWorldMat = NULL;
	pMeshInstance = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "cube.dae"), destCluster, pWorldMat);

	playerNumber = pNumber;
	isVirus = isPVirus;
	score = 0;

	switch (playerNumber)
	{
	case 0:
		position = Vector3(900.0f, 290.0f, 100.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		break;

	case 1:
		position = Vector3(-900.0f, 290.0f, 100.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		//pMeshInstance = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "MiddleConsoleUnit.dae"), destCluster, pWorldMat);
		break;

	case 2:
		position = Vector3(900.0f, 290.0f, -900.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		//pMeshInstance = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "MiddleConsoleUnit.dae"), destCluster, pWorldMat);
		break;

	case 3:
		position = Vector3(-900.0f, 290.0f, -900.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		//pMeshInstance = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "MiddleConsoleUnit.dae"), destCluster, pWorldMat);
		break;
	}

}


Player::~Player()
{


}

float Player::getThePos(int pos)
{
	if (pos == 0)
	{
		return getXPos();
	}

	if (pos == 1)
	{
		return getYPos();
	}

	if (pos == 2)
	{
		return getZPos();
	}
}


void Player::updateWorldMatrix(float zRotation, Vector3 Translation)
{
	pWorldMat->getMatrix() = Matrix4::rotationZ(zRotation);
	pWorldMat->getMatrix().setTranslation(Translation);
}