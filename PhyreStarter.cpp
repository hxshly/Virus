/* SCE CONFIDENTIAL
PhyreEngine(TM) Package 3.12.0.0
* Copyright (C) 2015 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/


#include <Phyre.h>
#include <Framework/PhyreFramework.h>
#include <Rendering/PhyreRendering.h>
#include <Geometry/PhyreGeometry.h>
#include <Scene/PhyreScene.h>
#include <Serialization/PhyreSerialization.h>
#include <Animation/PhyreAnimation.h>
#include <Gameplay/PhyreGameplay.h>
#include <Text/PhyreText.h>
#include <Sprite/PhyreSprite.h>

#include <../Samples/Common/PhyreSamplesCommon.h>
#include <../Samples/Common/PhyreSamplesCommonScene.h>
#include "PlayerManager.h"
#include "Player.h"
#include "PhyreStarter.h"
#include "UIManager.h"

using namespace Phyre;
using namespace PAnimation;
using namespace PFramework;
using namespace PRendering;
using namespace PSerialization;
using namespace PSamplesCommon;
using namespace Vectormath::Aos;
using namespace PText;


// Description:
// The static sample application instance.
static PhyreStarter s_phyreStarter;

// The maximum length for the dynamic text string.
#define PD_MAX_DYNAMIC_TEXT_LENGTH	(32)

// Description:
// The constructor for the PhyreStarter class.
PhyreStarter::PhyreStarter()
	: m_loadedCluster(NULL)
	, m_camera(NULL)
//	, m_bitmapFont(NULL)
{
	setWindowTitle("Basic Sample");

	//for (PUInt32 i = 0; i < c_totalTextStrings; i++)
	//{
	//	m_text[i] = NULL;
	//	m_textMaterial[i] = NULL;
	//}

	// Register the required utilities
	PUtility::RegisterUtility(PScene::s_utilityScene);

	// Register additional utilities to increase the range of files that can be loaded.
	PUtility::RegisterUtility(PAnimation::s_utilityAnimation);
	PUtility::RegisterUtility(PGameplay::s_utilityGameplay);
	
	// Register text and sprites
	PUtility::RegisterUtility(PText::s_utilityText);
//	PUtility::RegisterUtility(PSprite::s_utilitySprite);

}

// Description:
// Initialize the application.
// Arguments:
// argv - Array of strings representing command line arguments. This does not include program name.
// argc - Length of argv string array.
// Return Value List:
// Other - An error occurred initializing the application.
// PE_RESULT_NO_ERROR - The application was initialized successfully.
PResult PhyreStarter::initApplication(PChar **argv, PInt32 argc)
{
	(void)argv;
	(void)argc;

	// Load the asset file.
	PHYRE_TRY(PCluster::LoadAssetFile(m_loadedCluster, "Media/" PHYRE_PLATFORM_ID "/Basic/basic.phyre"));
	PHYRE_TRY(FixupClusters(&m_loadedCluster, 1));

	UIManager::GetInstance().InitUIText(m_loadedCluster);
//	UIManager::GetInstance().InitUISprites(m_loadedCluster);
	//UIManager::GetInstance().AddUISprite("scanner_normal", SPRITE_PING_PONG, -630, -380, 440, 220, 0, 0.05f);

	// Add the clusters to a world
	m_world.addCluster(*m_loadedCluster);

	pMan = new PlayerManager();

	// Initialize gui.
	PHYRE_TRY(PImGui::Initialize(getWidth(), getHeight()));

	return PE_RESULT_NO_ERROR;
}

// Description:
// Exit the application.
// Return Value List:
// Other - An error occurred exiting the application.
// PE_RESULT_NO_ERROR - The application was exited successfully.
PResult PhyreStarter::exitApplication()
{
	// Shutdown gui.
	PImGuiDebug::Terminate();
	PImGui::Terminate();

	UIManager::GetInstance().~UIManager();

	// Free the loaded clusters
	delete m_loadedCluster;

	return PApplication::exitApplication();
}

// Description:
// Initialize the scene in preparation for rendering.
// Return Value List:
// Other - The scene initialization failed.
// PE_RESULT_NO_ERROR - The scene initialization succeeded.
PResult PhyreStarter::initScene()
{

	//----------ATTEMPT AT FONT----------//
	// Search for the font object in the loaded cluster
	//m_bitmapFont = FindAssetRefObj<PBitmapFont>(NULL, "Fonts/Space.fgen");
	//if (!m_bitmapFont)
	//	return PHYRE_SET_LAST_ERROR(PE_RESULT_OBJECT_NOT_FOUND, "Unable to find bitmap font object in cluster");

	//const PMaterial *textShader = FindAssetRefObj<PMaterial>(NULL, "Shaders/PhyreText");
	//if (!textShader)
	//	return PHYRE_SET_LAST_ERROR(PE_RESULT_OBJECT_NOT_FOUND, "Unable to find text shader in cluster");

	//// Create the text and text materials
	//for (PUInt32 i = 0; i < c_totalTextStrings; i++)
	//	PHYRE_TRY(PUtilityText::CreateText(*m_bitmapFont, *m_loadedCluster, *textShader, m_text[i], m_textMaterial[i], PUtilityText::PE_TEXT_RENDER_TECHNIQUE_ALPHA_BLEND));

	//// Configure text 2
	//if (m_text[0])
	//{
	//	PMatrix4 matrix = PMatrix4::identity();
	//	float viewportScale = (float)getWidth() / (float)getHeight();

	//	// Since the text will be updated dynamically, we want to make sure that there should be no stalls due
	//	// to allocating memory or waiting for existing memory for geometry to become available.
	//	PHYRE_TRY(m_text[0]->setBufferCount(2));
	//	PHYRE_TRY(m_text[0]->setTextLength(PD_MAX_DYNAMIC_TEXT_LENGTH));

	//	// Position the text in the bottom left corner
	//	float scale = 1.0f / (12 * m_textMaterial[0]->getBitmapFontSize());

	//	matrix.setUpper3x3(Vectormath::Aos::Matrix3::scale(Vectormath::Aos::Vector3(scale, scale, 1.0f)));
	//	matrix.setTranslation(Vectormath::Aos::Vector3((-1.0f * viewportScale), -0.98f, 1.0f));
	//	m_text[0]->setMatrix(matrix);
	//}
	//----------END OF FONT ATTEMPT----------//

	// Look for a camera in the asset, otherwise refer to a default camera
	m_camera = FindFirstInstanceInCluster<PCameraPerspective>(*m_loadedCluster);
	if(!m_camera)
	{
		PHYRE_WARN("Unable to find perspective camera in asset file, using default camera\n");
		m_camera = &m_defaultCamera;
	}

	// The scene context encapsulates information about the current scene, so add the lights from the cluster in order to light the scene
	PUInt32 lightCount = PopulateSceneContextWithLights(m_sceneContext, *m_loadedCluster, 1);
	if(!lightCount)
		PHYRE_WARN("Unable to find any lights in the asset file, your asset will be unlit\n");
	
	PHYRE_TRY(m_cameraController.bind(*m_camera));

	// User INterface
	//if (m_text[0] && m_textMaterial[0])
	//{
		// Configure the text 2 string and its position

		//		if (!QueryPerformanceFrequency(&i))
		//		return false;
		/*frequencyPerSecond = (float)(i.QuadPart);
		QueryPerformanceCounter(&i);
		start = i.QuadPart;
		elapsedGameTime = 0;

		elapsedGameTime = (float)(i.QuadPart - start) / frequencyPerSecond;

		start = i.QuadPart;
		timeLeft -= elapsedGameTime;*/

		//timeLeft -= PTimer::GetTime();
		//PTimeController neo;
		//neo.getTime();

		PChar distanceString[PD_MAX_DYNAMIC_TEXT_LENGTH];
		PHYRE_SNPRINTF(distanceString, PHYRE_STATIC_ARRAY_SIZE(distanceString), "TimeLeft: %.0f", timeLeft);
		m_text[TEXT_TIME] = UIManager::GetInstance().AddScreenText(distanceString[0], 0.5, 0.1, Vector3(1.0f, 0.5f, 0.0f), 1.0f);

		
	//}
	//char message_text[32] = "Time Left: ";
	//m_text[TEXT_ACTION] = UIManager::GetInstance().AddScreenText(message_text[0], 0.45, 0.15, Vector3(1.0f, 0.5f, 0.0f), 1.0f);
	
	//attempt at timer
	


		pMan->initPlayers(m_loadedCluster, 4);


		//new
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				PChar scoreStringP1[PD_MAX_DYNAMIC_TEXT_LENGTH];
				PHYRE_SNPRINTF(scoreStringP1, PHYRE_STATIC_ARRAY_SIZE(scoreStringP1), "Score: %.0f", pMan->getScore(i));
				m_text[TEXT_SCORE_P1] = UIManager::GetInstance().AddScreenText(scoreStringP1[0], 0.1, 0.1, Vector3(1.0f, 0.0f, 0.0f), 1.0f);
				break;
			case 1:
				PChar scoreStringP2[PD_MAX_DYNAMIC_TEXT_LENGTH];
				PHYRE_SNPRINTF(scoreStringP2, PHYRE_STATIC_ARRAY_SIZE(scoreStringP2), "Score: %.0f", pMan->getScore(i));
				m_text[TEXT_SCORE_P2] = UIManager::GetInstance().AddScreenText(scoreStringP2[0], 0.1, 0.95, Vector3(0.0f, 0.0f, 1.0f), 1.0f);
				break;
			case 2:
				PChar scoreStringP3[PD_MAX_DYNAMIC_TEXT_LENGTH];
				PHYRE_SNPRINTF(scoreStringP3, PHYRE_STATIC_ARRAY_SIZE(scoreStringP3), "Score: %.0f", pMan->getScore(i));
				m_text[TEXT_SCORE_P3] = UIManager::GetInstance().AddScreenText(scoreStringP3[0], 0.90, 0.1, Vector3(0.0f, 1.0f, 0.0f), 1.0f);
				break;
			case 3:
				PChar scoreStringP4[PD_MAX_DYNAMIC_TEXT_LENGTH];
				PHYRE_SNPRINTF(scoreStringP4, PHYRE_STATIC_ARRAY_SIZE(scoreStringP4), "Score: %.0f", pMan->getScore(i));
				m_text[TEXT_SCORE_P4] = UIManager::GetInstance().AddScreenText(scoreStringP4[0], 0.90, 0.95, Vector3(1.8f, 2.0f, 0.0f), 1.0f);
				break;
			}
		}//end of new/**/

	return PApplication::initScene();
}

// Description:
// Exit the scene in preparation for exiting the application.
// Return Value List:
// Other - The scene exit failed.
// PE_RESULT_NO_ERROR - The scene exit succeeded.
PResult PhyreStarter::exitScene()
{
	// Free up the scene context containing the lights
	PHYRE_TRY(m_sceneContext.m_lights.resize(0));

	if(m_camera)
		m_cameraController.unbind(*m_camera);

	// Free the text objects and text materials
	//for (PUInt32 i = 0; i < c_totalTextStrings; i++)
	//{
	//	delete m_textMaterial[i];
	//	m_textMaterial[i] = NULL;
	//	delete m_text[i];
	//	m_text[i] = NULL;
	//}

	delete pMan;

	return PApplication::exitScene();
}

// Description:
// Setup call for the sample gui.
// Return Value List:
// Other -  The setup failed.
// PE_RESULT_NO_ERROR - The setup succeeded.
PResult PhyreStarter::setupGui()
{
	for(PWorld::PConstObjectIteratorOfType<PMaterial> it(m_world) ; it ; ++it)
	{
		const PMaterial &mat = *it;
		const PAssetReference *ar = PAssetReference::GetAssetReferenceByAsset(mat);
		const PChar *name = ar ? ar->getID().c_str() : NULL;
		if(name)
			PImGuiDebug::DisplayObject(mat, strrchr(name, '#'));
	}

	// Readme
	if (ImGui::CollapsingHeader("Readme"))
	{
#ifdef PHYRE_PLATFORM_WIN32
		ImGui::Text("Y - Toggle sample gui.");
#else //! PHYRE_PLATFORM_WIN32
		ImGui::Text("Start/Options - Toggle sample gui.");
#endif //! PHYRE_PLATFORM_WIN32
	}
	return PE_RESULT_NO_ERROR;
}

// Description:
// Render method.
// Return Value list:
// PE_RESULT_NO_ERROR - The render operation completed successfully.
// Other - An error occurred whilst rendering the scene.
PResult PhyreStarter::render()
{
	m_camera->setAspect((float)(getWidth()) / (float)(getHeight()));
	m_camera->updateViewMatrices();
	m_renderer.setCamera(*m_camera);

	PWorldRendering::PWorldRendererFrame wrFrame(m_worldRendererView, *m_camera, m_renderer, m_rendererGroup, m_sceneContext, (float)m_elapsedTime);
	wrFrame.startFrame(m_world);
	m_worldRenderer.renderWorld(wrFrame, m_world);


	m_renderer.beginScene(0); 
	m_renderer.setSceneRenderPassType(PHYRE_GET_SCENE_RENDER_PASS_TYPE(Transparent));
	UIManager::GetInstance().Render(m_renderer, *m_camera); 
	m_renderer.endScene();

	
	// Render the text objects
	//for (PUInt32 i = 0; i < c_totalTextStrings; i++)
	//{
	//	if (m_text[i])
	//		PHYRE_TRY(m_text[i]->renderText(m_renderer));
	//}
	//float timeLeft = 90;
	//timeLeft = timeLeft - (float)m_elapsedTime;


	// Render sample gui.
    PImGui::RenderGui(m_renderer, *this);
	
	return PE_RESULT_NO_ERROR;
}

// Description:
// Handle user inputs.
// Return Value List:
// PE_RESULT_NO_ERROR - The inputs were handled successfully.
// Other - An error occurred whilst handling inputs.
#define MAX_CAM_DIST	-50.0f
#define	MIN_CAM_DIST	-25.0f
PResult PhyreStarter::handleInputs()
{
	// Update Players
	
	pMan->updatePlayers();

	// Handle gui.
	if (checkAndClearKey(PInputBase::InputChannel_Key_Y) || checkAndClearJoypadButton(PInputBase::InputChannel_Button_Start))
		m_showImGui = !m_showImGui;

	PImGui::Update((float)m_elapsedTime, getInputMapper(), getWidth(), getHeight());
	updateCamera(m_cameraController, (float)m_elapsedTime);

	// ****************************************
	//  Simple Camera
	// ****************************************
	Vector3 camPos = m_camera->getLocalToWorldMatrix()->getMatrix().getTranslation();
	static Vector3 actualFocusPos = Vector3( 0.0f, 0.0f, 0.0f );
	Vector3 targetCamPos = camPos;
	/*Vector3 targetFocusPos = m_pPlayer1->GetPosition();
	Vector3 focusPosDiff = targetFocusPos - actualFocusPos;
	actualFocusPos += focusPosDiff/16;*/
	//targetCamPos.setZ( -40.0f );
	//targetCamPos.setX( actualFocusPos.getX()/-10.0f );
	//targetCamPos.setY( actualFocusPos.getY()/-10.0f );
	m_cameraController.lookAt( actualFocusPos/100.0f );
	
	m_camera->getLocalToWorldMatrix()->getMatrix().setTranslation( targetCamPos );
	m_camera->updateViewMatrices();

	// ****************************************
	// More Advanced Camera
	// ****************************************
	//Vector3 camPos = m_camera->getLocalToWorldMatrix()->getMatrix().getTranslation();
	//static Vector3 camFocusPos = Vector3(0.0f, 0.0f, 0.0f);
	//Vector3 targetCamPos = camPos;
	//Vector3 targetFocusPos = m_pCube->GetPosition() / 10.0f;
	//targetCamPos.setZ(MIN_CAM_DIST);
	//targetCamPos.setX(m_pCube->GetPosition().getX() - 100.0f);
	//targetCamPos.setY(m_pCube->GetPosition().getY() - 100.0f);
	//m_cameraController.lookAt(targetFocusPos);

	//m_camera->getLocalToWorldMatrix()->getMatrix().setTranslation(targetCamPos);


	//// Reduce the field of view so that camera moves before ship reaches edge of view
	//float fov = m_camera->getFOV();
	//m_camera->setFOV(fov - 0.9f);
	//m_camera->updateViewMatrices();

	//PMeshInstanceBounds *bounds = m_pCube->GetInstance()->getBounds();

	//if (!bounds->isVisibleForViewProjection(m_camera->getViewProjectionMatrix()) || m_pCube->GetSpeed() > 0.05f)
	//	targetCamPos.setZ(MAX_CAM_DIST);

	//// This does a nice "easy in" camera movement
	//Vector3 diff = targetCamPos - camPos;
	//camPos += diff / 30;

	//diff = targetFocusPos - camFocusPos;
	//camFocusPos += diff / 30;

	// Set everything back to how it was
	//m_camera->setFOV(fov);
	//m_camera->getLocalToWorldMatrix()->getMatrix().setTranslation(camPos);
	//m_cameraController.lookAt(camFocusPos);
	//m_camera->updateViewMatrices();

/*	if (m_text[0] && m_textMaterial[0])
	{
		// Configure the text 2 string and its position

//		if (!QueryPerformanceFrequency(&i))
	//		return false;
		frequencyPerSecond = (float)(i.QuadPart);
		QueryPerformanceCounter(&i);
		start = i.QuadPart;
		elapsedGameTime = 0;

		elapsedGameTime = (float)(i.QuadPart - start) / frequencyPerSecond;

		start = i.QuadPart;
		timeLeft -= elapsedGameTime;

		//float currentDistance = m_cameraController.getDistance();
		//if (currentDistance != m_previousDistance)
		//{
			PChar distanceString[PD_MAX_DYNAMIC_TEXT_LENGTH];
			PHYRE_SNPRINTF(distanceString, PHYRE_STATIC_ARRAY_SIZE(distanceString), "TimeLeft: %.0f", timeLeft);
			PHYRE_TRY(m_text[0]->setText(distanceString));

			// Update the previous distance
			//m_previousDistance = currentDistance;
		//}
	}
*/

	//PTimeController neo;
	//neo.getTime();
	
//	PHYRE_TRY(PTimeController::GlobalTick(elapsedGameTime));
//	elapsedGameTime;
//	timeLeft -= elapsedGameTime;

	elapsedGameTime = PTimer::GetTime();//new

	timeLeft -= elapsedGameTime/temp;//new
	//temp = temp + 1;//new
	PChar distanceString[PD_MAX_DYNAMIC_TEXT_LENGTH];//new
	PHYRE_SNPRINTF(distanceString, PHYRE_STATIC_ARRAY_SIZE(distanceString), "TimeLeft: %.0f", timeLeft);//new
	m_text[TEXT_TIME] = UIManager::GetInstance().UpdateText(0, *distanceString);//new
	//m_text[TEXT_TIME] = UIManager::GetInstance().AddScreenText(distanceString[0], 0.45, 0.15, Vector3(1.0f, 0.5f, 0.0f), 1.0f);
/*	*/

	//----------displaying score----------//new
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			PChar scoreStringP1[PD_MAX_DYNAMIC_TEXT_LENGTH];
			PHYRE_SNPRINTF(scoreStringP1, PHYRE_STATIC_ARRAY_SIZE(scoreStringP1), "Score: %.0f", pMan->getScore(i));
			m_text[TEXT_SCORE_P1] = UIManager::GetInstance().UpdateText(1, *scoreStringP1);
			break;
		case 1:
			PChar scoreStringP2[PD_MAX_DYNAMIC_TEXT_LENGTH];
			PHYRE_SNPRINTF(scoreStringP2, PHYRE_STATIC_ARRAY_SIZE(scoreStringP2), "Score: %.0f", pMan->getScore(i));
			m_text[TEXT_SCORE_P2] = UIManager::GetInstance().UpdateText(2, *scoreStringP2);
			break;
		case 2:
			PChar scoreStringP3[PD_MAX_DYNAMIC_TEXT_LENGTH];
			PHYRE_SNPRINTF(scoreStringP3, PHYRE_STATIC_ARRAY_SIZE(scoreStringP3), "Score: %.0f", pMan->getScore(i));
			m_text[TEXT_SCORE_P3] = UIManager::GetInstance().UpdateText(3, *scoreStringP3);
			break;
		case 3:
			PChar scoreStringP4[PD_MAX_DYNAMIC_TEXT_LENGTH];
			PHYRE_SNPRINTF(scoreStringP4, PHYRE_STATIC_ARRAY_SIZE(scoreStringP4), "Score: %.0f", pMan->getScore(i));
			m_text[TEXT_SCORE_P4] = UIManager::GetInstance().UpdateText(4, *scoreStringP4);
			break;
		}
	}/**/
	//----------end of score----------//end of NEW
	return PApplication::handleInputs();
}
PResult PhyreStarter::animate() 
{ 
	UIManager::GetInstance().Update(m_elapsedTime); 
	return PApplication::animate(); 
}
