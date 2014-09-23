#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Effect.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

class EffectApp : public ci::app::AppNative 
{
public:
	void					draw();
	void					setup();
private:
	ci:: gl::GlslProgRef	mGlsl;
};

using namespace ci;
using namespace ci::app;
using namespace std;

void EffectApp::setup()
{
	//mGlsl = gl::GlslProg::create();
}

void EffectApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );	

	mGlsl->bind();
	gl::VertBatch vb( GL_TRIANGLES );
		vb.color( 1, 0, 0 );
		vb.vertex( getWindowWidth() / 2, 50 );
		vb.color( 0, 1, 0 );
		vb.vertex( getWindowWidth() - 50, getWindowHeight() - 50 );
		vb.color( 0, 0, 1 );
		vb.vertex( 50, getWindowHeight() - 50 );
	vb.draw();
}

CINDER_APP_NATIVE( EffectApp, RendererGl )
 