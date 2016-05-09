#ifndef SHU_UI_H
#define SHU_UI_H

#include <Phyre.h>
#include <Rendering/PhyreRendering.h>
#include <Text/PhyreText.h>
#include <PhyreAssert.h>
#include <Framework/PhyreFramework.h>
#include <Sprite/PhyreSprite.h>


#include <../Samples/Common/PhyreSamplesCommon.h>
#include <../Samples/Common/PhyreSamplesCommonScene.h>


using namespace Phyre;
using namespace Vectormath::Aos;
using namespace PText;
using namespace PSamplesCommon;
using namespace PFramework;
using namespace Phyre::PRendering;
using namespace Phyre::PGeometry;


enum TextType
{
	TEXT_NONE					= 0,
	TEXT_CENTRED_SCREEN_COORDS,
	TEXT_CENTRED_WORLD_COORDS,
};

enum SpriteType
{
	SPRITE_NONE					= 0,
	SPRITE_LOOP,
	SPRITE_PING_PONG,
};

// A singleton pattern class which encapsulates the common UI functionality
class UIManager
{
	public:

		// There is no public constructor so this is the only way to access the class.
		// It automatically creates the instance if it doesn't already exist.
		static UIManager& GetInstance()
		{
			static UIManager instance;	// guaranteed to be destroyed 
			return instance;			// instantiated on first use
		}

		~UIManager( void ); // Destructor still public to allow control of destrution order.

		void Render( PRendering::PRenderer &renderer, PCamera &camera );
		void Update( float elapsedTime ); 
		int AddScreenText( PChar& text, float xpos, float ypos, Vector3 colour, float scale = 1.0f, bool dynamic = false, int maxDynamicLength = -1   ); // Returns ID of text string
		int AddWorldText( PChar& text, Vector3 worldPos, Vector3 colour, float scale = 1.0f, bool dynamic = false, int maxDynamicLength = -1   ); // Returns ID of text string
		PResult RemoveText( int id );
		PResult UpdateText( int id, PChar& text ); // only works on dynamic text
		PResult MoveScreenText( int id, float xpos, float ypos ); 
		PResult MoveWorldText( int id, Vector3 worldPos ); 
		void HideText( int id, bool hidden = true ) { m_uiText[ id ].hidden = hidden; } 
		PResult InitUISprites( PCluster* pWorldCluster);
		PResult InitUIText(PCluster* pWorldCluster);
		PResult AddUISprite( const char* animTag, SpriteType spriteType, int x, int y, int width, int height, int animIndex, float frameStep );

	private:

		UIManager( void ); // Construct private

		struct UIText
		{
			TextType	type;
			float		screenx;
			float		screeny;
			float		scale;
			Vector3		worldPos;
			Phyre::PText::PBitmapTextMaterial *material;	// The material used to render the text with.
			Phyre::PText::PBitmapFontText *text;			// The text objects to be rendered.
			bool dynamic;
			bool hidden;
		};

		struct UISprite
		{
			SpriteType		type;
			Phyre::PUInt32	id; 
			float		screenx;
			float		screeny;
			int			frameIndex;
			int			frameStep;
			PArray<Phyre::PSprite::PSpriteAnimationInfo *> animArray;
			int			animIndex;								// Current animation index
			float		timeInterval;							// The time (in seconds) between the frames.
			float		elapsedTime;							// The time (in seconds) since the last frame.
		};

		static const Phyre::PUInt32			c_totalTextStrings = 10;	
		static const Phyre::PUInt32			c_totalSprites = 20;

		Phyre::PSprite::PSpriteCollection   *m_spriteCollection;
		Phyre::PRendering::PMaterial		*m_textShader;
		Phyre::PText::PBitmapFont			*m_bitmapFont;
		Phyre::PRendering::PMaterial		*m_spriteShader;
		Phyre::PRendering::PTexture2D		*m_spriteTexture;
		Phyre::PCluster						*m_spriteCluster;
		Phyre::PCameraOrthographic			m_orthoCamera;			// A orthographic camera.

		UIText	 m_uiText[c_totalTextStrings];	
		UISprite m_uiSprite[c_totalSprites];
};


#endif // SHU_UI_H