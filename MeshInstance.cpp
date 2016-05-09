#include <Phyre.h>
#include <Rendering/PhyreRendering.h>

#include <../Samples/Common/PhyreSamplesCommon.h>
#include <../Samples/Common/PhyreSamplesCommonScene.h>

#include "MeshInstance.h"

using namespace Phyre;
using namespace PRendering;

// Controls the visibility of a PMeshInstance
void SetVisibility( PMeshInstance* meshInstance, bool isVisible )
{
	PMeshInstanceBounds *bounds = meshInstance->getBounds();
	if( bounds )
		bounds->setMeshInstance( isVisible ? meshInstance : NULL );
}



// Description:
// Finds the first mesh instance in a cluster that has a set of pose matrices which implies that it may be skinned.
// Arguments:
// cluster : The cluster in which to search.
// Returns:
// The the mesh instance if found.
PMeshInstance* FindMeshInstanceWithPose(PCluster &cluster)
{
	PMeshInstance *meshInstance = NULL;
	for(PCluster::PObjectIteratorOfType<PMeshInstance> fellaMeshInstanceIt(cluster); fellaMeshInstanceIt; ++fellaMeshInstanceIt)
	{
		Phyre::PRendering::PMeshInstance &meshInstanceCandidate = *fellaMeshInstanceIt;
		if(meshInstanceCandidate.getCurrentPoseMatrices())
			meshInstance = &meshInstanceCandidate;
	}
	return meshInstance;
}

// Description:
// Creates a new mesh instance based on an existing mesh instance.
// Arguments:
// cluster : The cluster in which to allocate objects.
// sourceInstance : The source mesh instanceto share the mesh of and copy the bounds from.
// worldMatrix : The world matrix for the mesh instance
// Note:
// This sample only creates the minimum to create a new instance of a skinned animated mesh
PMeshInstance* CreateSkinnedObjectInstance( PMeshInstance *sourceInstance, PCluster* destCluster, PWorldMatrix* &worldMatrix)
{
	worldMatrix = destCluster->create<PWorldMatrix>(1);

	// Original code from CloneMeshInstanceSimple
	PGeometry::PMesh	&mesh		= sourceInstance->getMesh();
	PMeshInstance		*instance	= destCluster->allocateInstanceAndConstruct<PMeshInstance>(mesh);
	if(!instance)
	{
		PHYRE_SET_LAST_ERROR(PE_RESULT_OUT_OF_MEMORY, "Unable to allocate new mesh instance");
		return NULL;
	}
	instance->setLocalToWorldMatrix(worldMatrix);

	const PMeshInstanceBounds *bounds = sourceInstance->getBounds();
	if(bounds)
	{
		PMeshInstanceBounds *newBounds = destCluster->create<PMeshInstanceBounds>(1);
		if(!newBounds)
		{
			PHYRE_SET_LAST_ERROR(PE_RESULT_OUT_OF_MEMORY, "Unable to allocate new mesh bounds");
			return NULL;
		}
		newBounds->setMinAndSize(bounds->getMin(), bounds->getSize());
		newBounds->setWorldMatrix(worldMatrix);
		newBounds->setMeshInstance(instance);
		instance->setBounds( newBounds );
	}

	PDynamicMeshInstance *dynamicMeshInstance = sourceInstance->getDynamicMeshInstance();
    
	if(dynamicMeshInstance)
        instance->setDynamicMeshInstance(dynamicMeshInstance->getDynamicMesh().createInstance(*destCluster, dynamicMeshInstance));

	return instance;
}

// Description:
// Creates a new mesh instance based on an existing mesh instance.
// Arguments:
// cluster : The cluster in which to allocate objects.
// sourceInstance : The source mesh instanceto share the mesh of and copy the bounds from.
// worldMatrix : The world matrix for the mesh instance
// Note:
// This sample only creates the minimum to create a new instance of a rigid animated mesh
PMeshInstance* CreateRigidObjectInstance(PMeshInstance	*sourceInstance, PCluster* destCluster, PWorldMatrix* &worldMatrix)
{
	worldMatrix = destCluster->create<PWorldMatrix>(1);

	// Original code from CloneMeshInstanceSimple
	PGeometry::PMesh	&mesh		= sourceInstance->getMesh();
	PMeshInstance		*instance	= destCluster->allocateInstanceAndConstruct<PMeshInstance>(mesh);
	if(!instance)
	{
		PHYRE_SET_LAST_ERROR(PE_RESULT_OUT_OF_MEMORY, "Unable to allocate new mesh instance");
		return NULL;
	}
	instance->setLocalToWorldMatrix(worldMatrix);

	const PMeshInstanceBounds *bounds = sourceInstance->getBounds();
	if(bounds)
	{
		PMeshInstanceBounds *newBounds = destCluster->create<PMeshInstanceBounds>(1);
		if(!newBounds)
		{
			PHYRE_SET_LAST_ERROR(PE_RESULT_OUT_OF_MEMORY, "Unable to allocate new mesh bounds");
			return NULL;
		}
		newBounds->setMinAndSize(bounds->getMin(), bounds->getSize());
		newBounds->setWorldMatrix(worldMatrix);
		newBounds->setMeshInstance(instance);
		instance->setBounds( newBounds );
	}

	PDynamicMeshInstance *dynamicMeshInstance = sourceInstance->getDynamicMeshInstance();
    
	if(dynamicMeshInstance)
        instance->setDynamicMeshInstance(dynamicMeshInstance->getDynamicMesh().createInstance(*destCluster, dynamicMeshInstance));

	return instance;
}
