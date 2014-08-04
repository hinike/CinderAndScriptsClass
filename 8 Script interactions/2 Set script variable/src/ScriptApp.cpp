#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Script.h"
#include "AssetManager.h"
#include "Registration.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ScriptApp : public AppNative {
  public:
	void setup();
    void update();
	void draw();
    
    //! setup the engine, registers the app interface and load scripts
	void setupScriptEngine();
    
    //! console wrapper
    void print( const std::string &s );
    
    //! our script file
    as::ScriptRef mScript;
};

void ScriptApp::setup()
{
    getWindow()->setAlwaysOnTop();
    
    setupScriptEngine();
    
    // watch our script for change and reloads it when needed
    AssetManager::load( "main.script", [this]( DataSourceRef sketch ){
        mScript = as::Script::create( sketch, "" );
        
        if( mScript ){
            mScript->call( "void setup()" );
        }
    });
}

void ScriptApp::setupScriptEngine()
{
    // get a reference to our global engine.
    asIScriptEngine* engine = as::Script::getEngine();
    
    // register a print function.
    int r = engine->RegisterGlobalFunction("void print( const string &in )", asMETHOD(ScriptApp,print), asCALL_THISCALL_ASGLOBAL, this );  assert( r >= 0 );
    
    // register cinder
    as::registerCinder( engine );
    
}


void ScriptApp::update()
{
    if( mScript ){
        mScript->call( "void update()" );
        
        // set the global script variable named "time"
        // Be sure to cast to the right type (getElapsedSeconds() returns
        // a float, and "time" is a float in our script. If you end up
        // providing the wrong type, it's unsure what would happen because
        // we are accessing the memory of the object to change its value.
        mScript->set( "time", static_cast<float>( getElapsedSeconds() ) );
    }
}

void ScriptApp::draw()
{
    if( mScript ){
        mScript->call( "void draw()" );
    }
}

//! console wrapper
void ScriptApp::print( const std::string &s )
{
    console() << s << endl;
}

CINDER_APP_NATIVE( ScriptApp, RendererGl )
