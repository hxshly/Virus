#ifndef MESHINSTANCES_H
#define MESHINSTANCES_H

using namespace Phyre;
using namespace PRendering;

// These are defined in PhyreAssets.xml, but we're extern-ing them here so they can be used in oter files without redefining them
extern const Phyre::PUInt32 g_fileNamesCount;
extern const Phyre::PChar *g_fileNamesPrefix;
extern const Phyre::PChar *g_fileNames[];

// This is defined in PhyreStarter.cpp and extern-ed here for the same reason as above
extern PCluster *g_clusterArray[];	// An array of loaded clusters.

void SetVisibility( PMeshInstance* meshInstance, bool isVisible );
PMeshInstance* CreateRigidObjectInstance( PMeshInstance* pSourceInstance, PCluster* pDestCluster, PWorldMatrix* &worldMatrix );
PMeshInstance* CreateSkinnedObjectInstance( PMeshInstance* pSourceInstance, PCluster* pDestCluster, PWorldMatrix* &worldMatrix);
PMeshInstance* FindMeshInstanceWithPose(PCluster &cluster);


#endif //MESHINSTANCES_H