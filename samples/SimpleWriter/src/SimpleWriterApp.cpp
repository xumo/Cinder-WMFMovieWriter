/***********************************
*
* Simple Moview Writer using th
* Windows Media Foundation
*
*	Rodrigo Torres 2017
*	@xumerio
*
*
*
************************************/


#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"

#include "wmf/MovieWriter.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SimpleWriterApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;

	//The movie wirter is a shared pointer
	wmf::MovieWriterRef	mMovieExporter;

	//Variables for keeping track of the frame duration
	double				mTime, mTimeDelta;
	bool				mStarted;

	//The movie writer also uses a textureinput for each frame
	//so in this case the screen will be rendered into a FBO
	ci::gl::FboRef		mFbo;


	//Texture for test pattern
	gl::TextureRef		mTexture;
};

void SimpleWriterApp::setup()
{
	fs::path path = getSaveFilePath();
	mTexture = gl::Texture::create(loadImage(getAssetPath("testpattern.png")));

	
	if (!path.empty()) {
		auto format = wmf::MovieWriter::Format().codec(wmf::MovieWriter::H264);
		mMovieExporter = wmf::MovieWriter::create(path, getWindowWidth(), getWindowHeight(), format);
	}

	gl::Fbo::Format formatFbo;
	formatFbo.setSamples(4);
	mFbo = gl::Fbo::create(getWindowWidth(), getWindowHeight());
	
	mStarted = true;
	mTimeDelta = 1000;
	mTime = getElapsedSeconds();

}

void SimpleWriterApp::update()
{
	const int maxFrames = 150;

	mTimeDelta = getElapsedSeconds() - mTime;
	mTime = getElapsedSeconds();
	

	gl::ScopedFramebuffer fbScp(mFbo);
	gl::color(Color::white());

	gl::draw(mTexture, getWindowBounds());
	//Draw the same scene as the Quicktime movie writer example
	gl::color(Color(CM_HSV, fmod(getElapsedFrames() / 30.0f, 1.0f), 1, 1));
	gl::draw(geom::Circle().center(getWindowCenter()).radius(getElapsedFrames()).subdivisions(100));


	
	if (mStarted && getElapsedFrames() > 1 && getElapsedFrames() < maxFrames) {
		/*****************
		* The WMF expects a unsigned char * array, so a surface is the perfect choice
		* but if the data of a surface is passed, the color red a blue are swaped. 
		*
		* The fastest solution found was to render to a FBO and swap the color in a shader.
		*
		*/

		//mMovieExporter->addFrame(copyWindowSurface(), mTimeDelta );
		mMovieExporter->addFrame(mFbo->getColorTexture(), mTimeDelta);
	}
	else if (mStarted && getElapsedFrames() >= maxFrames) {
		
		mMovieExporter->finish();
		mMovieExporter.reset();
		mStarted = false;
	}
}

void SimpleWriterApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::color(Color::white());
	gl::draw(mFbo->getColorTexture(), getWindowBounds());
	gl::drawString(toString(getAverageFps()) , vec2(20,20));
	gl::drawString("Time delta: " + toString(mTimeDelta), vec2(20, 40));

}

CINDER_APP( SimpleWriterApp, RendererGl )
