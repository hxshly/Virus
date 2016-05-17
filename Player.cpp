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

	playerNumber = pNumber;
	isVirus = isPVirus;
	score = 0;
	invuln = 180;
	incVal = 1;
	decVal = 1;

	switch (playerNumber)
	{
	case 0:
		position = Vector3(900.0f, 290.0f, 100.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		pMeshInstanceVirus = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "infectedCube.dae"), destCluster, pWorldMatVirus);
		pMeshInstancePlayer = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "cube.dae"), destCluster, pWorldMat);

		if (isVirus)
		{
			SetVisibility(pMeshInstancePlayer, false);
			SetVisibility(pMeshInstanceVirus, true);
		}
		else
		{
			SetVisibility(pMeshInstanceVirus, false);
			SetVisibility(pMeshInstancePlayer, true);
		}

		break;

	case 1:
		position = Vector3(-900.0f, 290.0f, 100.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		pMeshInstanceVirus = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "infectedCube.dae"), destCluster, pWorldMatVirus);
		pMeshInstancePlayer = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "cube.dae"), destCluster, pWorldMat);

		if (isVirus)
		{
			SetVisibility(pMeshInstancePlayer, false);
			SetVisibility(pMeshInstanceVirus, true);
		}
		else
		{
			SetVisibility(pMeshInstanceVirus, false);
			SetVisibility(pMeshInstancePlayer, true);
		}

		break;

	case 2:
		position = Vector3(900.0f, 290.0f, -900.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		pMeshInstanceVirus = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "infectedCube.dae"), destCluster, pWorldMatVirus);
		pMeshInstancePlayer = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "cube.dae"), destCluster, pWorldMat);

		if (isVirus)
		{
			SetVisibility(pMeshInstancePlayer, false);
			SetVisibility(pMeshInstanceVirus, true);
		}
		else
		{
			SetVisibility(pMeshInstanceVirus, false);
			SetVisibility(pMeshInstancePlayer, true);
		}

		break;

	case 3:
		position = Vector3(-900.0f, 290.0f, -900.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		pMeshInstanceVirus = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "infectedCube.dae"), destCluster, pWorldMatVirus);
		pMeshInstancePlayer = CreateRigidObjectInstance(FindAssetRefObj<PMeshInstance>(NULL, "cube.dae"), destCluster, pWorldMat);

		if (isVirus)
		{
			SetVisibility(pMeshInstancePlayer, false);
			SetVisibility(pMeshInstanceVirus, true);
		}
		else
		{
			SetVisibility(pMeshInstanceVirus, false);
			SetVisibility(pMeshInstancePlayer, true);
		}

		break;
	}

}

void Player::changeMeshToPlayerMesh()
{
	SetVisibility(pMeshInstanceVirus, false);
	SetVisibility(pMeshInstancePlayer, true);
}

void Player::changeMeshToVirusMesh()
{
	SetVisibility(pMeshInstancePlayer, false);
	SetVisibility(pMeshInstanceVirus, true);
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

	*pWorldMatVirus = *pWorldMat;

}