#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Effect.h"

class EffectApp : public ci::app::AppNative 
{
public:
	void keyDown( ci::app::KeyEvent event );
	void setup();
};

using namespace ci;
using namespace ci::app;
using namespace std;

void EffectApp::keyDown( KeyEvent event )
{
	quit();
}

void EffectApp::setup()
{
	using namespace gl::effect;

	console() << VertexPassThrough() << endl << endl;
	console() << FragmentTexture() << endl << endl;
}

CINDER_APP_NATIVE( EffectApp, RendererGl )
 