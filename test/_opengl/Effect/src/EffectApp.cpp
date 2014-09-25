#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Effect.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

class EffectApp : public ci::app::AppNative 
{
public:
	void							draw();
	void							keyDown( ci::app::KeyEvent event );
	void							prepareSettings( ci::app::AppNative::Settings* settings );
	void							setup();
private:
	ci::gl::effect::FragmentTexture	mEffectFrag;
	ci::gl::GlslProgRef				mGlslProg;
	ci::gl::TextureRef				mTexture;
};

#include "cinder/ImageIo.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void EffectApp::draw()
{
	gl::viewport( getWindowSize() );
	gl::clear();
	gl::setMatricesWindow( getWindowSize() );

	if ( mGlslProg && mTexture ) {
		gl::ScopedGlslProg glsl( mGlslProg );
		mGlslProg->uniform( mEffectFrag.getTextureUniform(), 0 );
		mTexture->bind( 0 );

		gl::color( ColorAf( 1.0f, 0.5f, 0.5f, 1.0f ) );
		gl::drawSolidRect( getWindowBounds() );

		mTexture->unbind();
	}
}

void EffectApp::keyDown( KeyEvent event )
{
	quit();
}

void EffectApp::prepareSettings( Settings* settings )
{
	settings->setWindowSize( 1024, 1024 );
}

void EffectApp::setup()
{
	gl::enable( GL_TEXTURE_2D );
	using namespace gl::effect;

	FragmentOperation opFrag = mEffectFrag * FragmentColor();
	console() << VertexPassThrough() << endl << endl;
	console() << opFrag << endl << endl;

	try {
		mGlslProg = gl::GlslProg::create( VertexPassThrough(), opFrag );
	} catch ( gl::GlslProgCompileExc ex ) {
		console() << ex.what() << endl;
	}
	mTexture = gl::Texture::create( loadImage( loadAsset( "texture.jpg" ) ) );
}

CINDER_APP_NATIVE( EffectApp, RendererGl )
 