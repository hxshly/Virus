#include "UIManager.h"

UIManager::UIManager( void )
	: m_spriteCollection( NULL )
	, m_bitmapFont(NULL)
	, m_textShader(NULL)
	, m_spriteTexture(NULL)
	, m_spriteShader(NULL)
{
	for( int i=0; i<c_totalTextStrings; ++i )
	{
		m_uiText[i].material = NULL;
		m_uiText[i].text = NULL;
		m_uiText[i].type = TEXT_NONE;
	}

	PHYRE_PRINTF("Creating UIManager\n");
}

UIManager::~UIManager()
{
	PHYRE_PRINTF("Destroying UIManager\n");
	for (int i = 0; i < c_totalTextStrings; ++i)
	{
		if (m_uiText[i].material != NULL)
		{
			delete m_uiText[i].material;
			m_uiText[i].material = NULL;
		}
		if (m_uiText[i].text != NULL)
		{
			delete m_uiText[i].text;
			m_uiText[i].text = NULL;
		}
	}

	// Terminate the quad mesh
	PSprite::s_utilitySprite.terminateQuadMesh();

	if(m_spriteCollection)
	{
		delete m_spriteCollection;
		m_spriteCollection = NULL;
	}
}

PResult UIManager::InitUIText(PCluster* pCluster)
{
	PResult ret = PE_RESULT_NO_ERROR;

	m_bitmapFont = FindAssetRefObj<PBitmapFont>(NULL, "Space.fgen");
	if(!m_bitmapFont)
		 PHYRE_ASSERTMSG( NULL , "Unable to find font in cluster" );

	m_textShader = FindAssetRefObj<PMaterial>(NULL, "Shaders/PhyreText");
	if (!m_textShader)
		return PHYRE_SET_LAST_ERROR(PE_RESULT_OBJECT_NOT_FOUND, "Unable to find text shader in cluster");

	// Create the text and text materials
	for (PUInt32 i = 0; i < c_totalTextStrings; i++)
	{
		ret = PUtilityText::CreateText(*m_bitmapFont, *pCluster, *m_textShader, m_uiText[i].text, m_uiText[i].material, PUtilityText::PE_TEXT_RENDER_TECHNIQUE_ALPHA_BLEND);
		if (ret != PE_RESULT_NO_ERROR)
			return ret;
	}

	return PE_RESULT_NO_ERROR;
}

PResult UIManager::InitUISprites( PCluster* pWorldCluster)
{
	PHYRE_ASSERTMSG( m_spriteCollection == NULL , "Reinitialisation of the UI Sprites is not supported! " );

	const PCluster* pCluster = NULL;
	m_spriteTexture = FindAssetRefObj<PTexture2D>(NULL, "sprite.agx");
	if (!m_spriteTexture)
		PHYRE_ASSERTMSG(NULL, "Unable to find sprite texture in cluster");

	m_spriteShader = FindAssetRefObj<PMaterial>(NULL, "Shaders/PhyreSprite");
	if (!m_spriteShader)
		PHYRE_ASSERTMSG(NULL, "Unable to find sprite texture in cluster");

	PMesh &mesh = PSprite::s_utilitySprite.createQuadMesh();
	m_spriteCollection = PHYRE_NEW PSprite::PSpriteCollection();
	if (!m_spriteCollection)
		return PE_RESULT_UNKNOWN_ERROR;

	// Create a sprite collection with maximum of sprites.
	PHYRE_TRY(m_spriteCollection->initialize(*pWorldCluster, c_totalSprites, *m_spriteShader, *m_spriteTexture, mesh));

	for( int i=0; i<c_totalSprites; ++i )
		m_uiSprite[i].type = SPRITE_NONE;

	return PE_RESULT_NO_ERROR;
}

PResult UIManager::AddUISprite( const char* animTag, SpriteType spriteType, int x, int y, int width, int height, int animIndex = 0, float frameStep = 0.0f )
{
	int s=0;

	// Find an unused slot
	for( s=0; s<c_totalSprites; ++s )
		if( m_uiSprite[s].type ==SPRITE_NONE ) break;

	PHYRE_ASSERTMSG( s < c_totalSprites , "Exceeded maximum number of UI sprites" );

	// The sprite collection needs to be mapped before sprites can be added/removed or be modified.
	PHYRE_TRY(m_spriteCollection->map());

	PSharray<PAssetReference *> spriteAnimationInfos;
	PAssetReference::Find(spriteAnimationInfos, NULL, "sprite.agx", &PHYRE_CLASS(PSprite::PSpriteAnimationInfo));
	for (PUInt32 i = 0; i<spriteAnimationInfos.getCount(); i++)
	{
		PSprite::PSpriteAnimationInfo &animInfo = (PSprite::PSpriteAnimationInfo &)(spriteAnimationInfos[i]->getAsset());

		if (strstr(animInfo.m_name.c_str(), animTag))
			m_uiSprite[s].animArray.add(&animInfo);
	}

	m_uiSprite[s].id = s;
	m_uiSprite[s].type = spriteType;
	m_uiSprite[s].timeInterval = frameStep;
	m_uiSprite[s].animIndex = animIndex;
	m_uiSprite[s].elapsedTime = 0.0f;
	m_uiSprite[s].frameIndex = 0;
	m_uiSprite[s].screenx = x;
	m_uiSprite[s].screeny = y;
	m_uiSprite[s].frameStep = 1;

	m_uiSprite[s].animArray[ animIndex ]->m_subTextureIDs[ m_uiSprite[s].frameIndex ];

	PUInt32 spriteIndex = m_uiSprite[s].animArray[ animIndex ]->m_subTextureIDs[ m_uiSprite[s].frameIndex ];
	PSprite::PTextureAtlasInfo *atlas=m_uiSprite[s].animArray[ animIndex ]->m_textureAtlasInfo;
	if (!atlas)
		return PE_RESULT_UNKNOWN_ERROR;
	PSprite::PSubTextureInfo &spriteInfo = atlas->m_subTextures[spriteIndex];

	float viewPortWidth = PApplication::GetApplication()->getWidth();
	float viewPortHeight = PApplication::GetApplication()->getHeight();

	PHYRE_TRY(m_spriteCollection->addSprite(m_uiSprite[s].id));
	PHYRE_TRY(m_spriteCollection->setPosition(s, x, y));
	PHYRE_TRY(m_spriteCollection->setRotation(s, 0.0f));
	PHYRE_TRY(m_spriteCollection->setDepth(s, -15.0f));
	PHYRE_TRY(m_spriteCollection->setSpriteSize(s, width, height));
	PHYRE_TRY(m_spriteCollection->setSpriteTextureOriginNormalized(s, spriteInfo.m_uOrigin, spriteInfo.m_vOrigin));
	PHYRE_TRY(m_spriteCollection->setSpriteTextureSizeNormalized(s, spriteInfo.m_textureSizeX, spriteInfo.m_textureSizeY));

	// Now unmap the sprite collection so it can be rendered.
	PHYRE_TRY(m_spriteCollection->unmap());

	return PE_RESULT_NO_ERROR;
}

void UIManager::Render( PRendering::PRenderer &renderer, PCamera &camera )
{
	float scale, width, height, viewportScale, dist;
	PMatrix4 matrix = PMatrix4::identity();
	Vector3 offset;

	float viewPortWidth = PApplication::GetApplication()->getWidth();
	float viewPortHeight = PApplication::GetApplication()->getHeight();
	viewportScale = viewPortWidth / viewPortHeight;

	// Use an ortho camera for rendering sprites
	m_orthoCamera.setHeight(viewPortHeight * 0.5f);
	m_orthoCamera.setAspect(viewportScale);
	m_orthoCamera.updateViewMatrices();

	// Tell the renderer which camera we want to use for rendering
	renderer.setCamera(m_orthoCamera);

	// Render the single PMeshInstance for all the sprites in the sprite collection.
	if (m_spriteCollection && m_spriteCollection->m_quadMeshInstance)
		renderer.renderMeshInstance(*m_spriteCollection->m_quadMeshInstance);

	// Restore Camera
	renderer.setCamera(camera);


	// Render the text objects
	for(PUInt32 i = 0; i < c_totalTextStrings; i++)
	{
		if( m_uiText[i].type != TEXT_NONE && m_uiText[i].hidden == false && m_uiText[i].text != NULL )
		{
			switch( m_uiText[i].type )
			{
				case TEXT_CENTRED_SCREEN_COORDS:

					scale  = m_uiText[i].scale * 1.0f / (8 * m_uiText[i].material->getBitmapFontSize());
					height = 0.5f * m_uiText[i].text->getTextHeight() * scale;
					width  = 0.5f * m_uiText[i].text->getTextWidth()  * scale;

					matrix.setUpper3x3(Matrix3::scale(Vector3(scale, scale, 1.0f)));
					matrix.setTranslation(Vector3(-width + (m_uiText[i].screenx*viewportScale*2)-viewportScale, -height + (-m_uiText[i].screeny*2)+1, 1.0f));
					
					break;

				case TEXT_CENTRED_WORLD_COORDS:

					Vector4 position = camera.getViewProjectionMatrix() * Point3(m_uiText[i].worldPos);
					Vector3 screenPosition = position.getXYZ() / position.getW();
					screenPosition.setZ(1.0f);

					offset = camera.getLocalToWorldMatrix()->getMatrix().getTranslation() - m_uiText[i].worldPos;
					dist = length( offset );

					scale  = m_uiText[i].scale * 1.0f / ((dist / 4) * m_uiText[i].material->getBitmapFontSize());
					width  = 0.5f * m_uiText[i].text->getTextWidth() * scale;
		
					matrix.setUpper3x3(Matrix3::scale(Vector3(scale, scale, 1.0f)));
					matrix.setTranslation(Vector3((screenPosition.getX() * viewportScale) - width, screenPosition.getY(), 1.0f));

					break;
			}

			m_uiText[i].text->setMatrix(matrix);
			m_uiText[i].text->renderText(renderer);
		}
	}
}

void UIManager::Update( float elapsedTime )
{
	if (m_spriteCollection)
	{
		m_spriteCollection->map();

		// Animate things
		for( int i=0; i<c_totalSprites; ++i )
		{
			if( m_uiSprite[i].type != SPRITE_NONE )
			{
				m_uiSprite[i].elapsedTime += elapsedTime;

				if( m_uiSprite[i].elapsedTime >= m_uiSprite[i].timeInterval )
				{
					Phyre::PSprite::PSpriteAnimationInfo *ai = m_uiSprite[i].animArray[m_uiSprite[i].animIndex];

					m_uiSprite[i].frameIndex += m_uiSprite[i].frameStep;

					switch( m_uiSprite[i].type )
					{
						case SPRITE_LOOP:
							if(m_uiSprite[i].frameIndex == ai->m_subTextureIDs.getCount())
								m_uiSprite[i].frameIndex = 0;
							break;
						case SPRITE_PING_PONG:
							if(m_uiSprite[i].frameIndex == ai->m_subTextureIDs.getCount()-1 || m_uiSprite[i].frameIndex == 0 )
								m_uiSprite[i].frameStep *= -1;
							break;
					}

					// Now look up the texture/sprite information for this frame.
					PUInt32 spriteIndex = ai->m_subTextureIDs[m_uiSprite[i].frameIndex];
					PSprite::PTextureAtlasInfo *atlas=ai->m_textureAtlasInfo;
					PSprite::PSubTextureInfo &spriteInfo = atlas->m_subTextures[spriteIndex];
					m_spriteCollection->setSpriteTextureOriginNormalized(m_uiSprite[i].id, spriteInfo.m_uOrigin, spriteInfo.m_vOrigin);
		
					m_uiSprite[i].elapsedTime = 0.0f;
				}

			}
		}

		m_spriteCollection->unmap();
	}
}

PResult UIManager::UpdateText( int id, PChar& text )
{
	m_uiText[id].text->setText(&text);

	return PE_RESULT_NO_ERROR;
}


PResult UIManager::MoveWorldText( int id, Vector3 worldPos )
{
	m_uiText[id].worldPos = worldPos;

	return PE_RESULT_NO_ERROR;
}

int UIManager::AddScreenText( PChar& text, float screenx, float screeny, Vector3 colour, float scale, bool dynamic, int maxDynamicLength )
{
	int t=0;

	// Find an unused slot
	for( t=0; t<c_totalTextStrings; ++t )
		if( m_uiText[t].type == TEXT_NONE ) break;

	PHYRE_ASSERTMSG( t < c_totalTextStrings , "Exceeded maximum number of text strings" );

	m_uiText[t].dynamic = dynamic;
	m_uiText[t].hidden = false;
	m_uiText[t].type = TEXT_CENTRED_SCREEN_COORDS;
	m_uiText[t].screenx = screenx;
	m_uiText[t].screeny = screeny;
	m_uiText[t].scale = scale;

	if( dynamic )
	{
		m_uiText[t].text->setBufferCount(2);
		m_uiText[t].text->setTextLength( maxDynamicLength );
	}
	else
	{
		m_uiText[t].text->setTextLength(PhyreCheckCast<PUInt32>(strlen(&text)));
	}

	m_uiText[t].text->setText(&text);
	m_uiText[t].material->setColor(colour);

	return t;
}

int UIManager::AddWorldText( PChar& text, Vector3 worldPos, Vector3 colour, float scale, bool dynamic, int maxDynamicLength )
{
	int t=0;

	// Find an unused slot
	for( t=0; t<c_totalTextStrings; ++t )
		if( m_uiText[t].type == TEXT_NONE ) break;

	PHYRE_ASSERTMSG( t < c_totalTextStrings , "Exceeded maximum number of text strings" );

	m_uiText[t].dynamic = dynamic;
	m_uiText[t].hidden = false;
	m_uiText[t].type = TEXT_CENTRED_WORLD_COORDS;
	m_uiText[t].worldPos = worldPos;
	m_uiText[t].scale = scale;

	if( dynamic )
	{
		m_uiText[t].text->setBufferCount(2);
		m_uiText[t].text->setTextLength( maxDynamicLength );
	}
	else
	{
		m_uiText[t].text->setTextLength(PhyreCheckCast<PUInt32>(strlen(&text)));
	}

	m_uiText[t].text->setText(&text);
	m_uiText[t].material->setColor(colour);

	return t;
}
