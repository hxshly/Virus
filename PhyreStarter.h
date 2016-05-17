/* SCE CONFIDENTIAL
PhyreEngine(TM) Package 3.12.0.0
* Copyright (C) 2015 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef BASIC_H
#define BASIC_H

#include "PlayerManager.h"


enum TextID
{
	TEXT_ACTION = 0,
	TEXT_TIME,
	TEXT_SCORE_P1,
	TEXT_SCORE_P2,
	TEXT_SCORE_P3,
	TEXT_SCORE_P4,
	TEXT_TOTAL
};

class PhyreStarter : public Phyre::PFramework::PApplication
{
protected:
	// Phyre Framework overrides
	virtual Phyre::PResult initApplication(Phyre::PChar **argv, Phyre::PInt32 argc);
	virtual Phyre::PResult handleInputs();
	virtual Phyre::PResult exitApplication();
	virtual Phyre::PResult initScene();
	virtual Phyre::PResult exitScene();
	virtual Phyre::PResult render();
	virtual Phyre::PResult setupGui();

	virtual Phyre::PResult animate();

	Phyre::PCluster						*m_loadedCluster;	// The cluster to hold the contents of the loaded asset file.
	Phyre::PCameraPerspective			*m_camera;			// The camera used for rendering the scene. This is the camera found in the asset file.
	Phyre::PCameraPerspective			m_defaultCamera;	// The default camera to use if no camera can be found in the asset file.

	Phyre::PWorldMatrixOrbitController	m_cameraController; // The camera controller for this sample.
	
	LARGE_INTEGER i;
	float frequencyPerSecond = 0;
	float elapsedGameTime;
	LONGLONG start;
	float m_text[TEXT_TOTAL];
	float timeLeft = 60;
	PlayerManager* pMan;
	int temp = 60; // new

public:
	PhyreStarter();
};

#endif //! BASIC_H
