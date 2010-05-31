#include "Render.h"

DrawObject::DrawObject(){
}

DrawObject::DrawObject ( string meshname){
  createEntity( meshname);
}

void DrawObject::createEntity( string meshname ){
  Render::Render * r = Render::getInstance();
  entity = r->newEntity( r->generateName(), meshname );
  node = r->attach( r->generateName(), entity);
  return;
}

DrawAgent::DrawAgent( Agent::Agent * ag, string mesh ) : DrawObject(mesh){
  a = ag;
  float r = a->getRadius();
  node->setScale( Ogre::Vector3( r / 2.0, 1.0, r / 2.0) ); 
}


void DrawAgent::update(){
  v2f p, d, z;
  v2fMult(z, 0.0, z);
  a->getDirection(z, p);
  node->setPosition(p[0], 0.0, -p[1]);
  a->getNorm( d );
  float theta = std::atan2( -d[1], -d[0] ) - std::atan2( 1.0, 0.0 );
  node->setOrientation( Ogre::Quaternion( Ogre::Radian( theta ), Ogre::Vector3(0.0, 1.0, 0.0) ) );

}



DrawWall::DrawWall( Wall::Wall * wall, string meshname) : DrawObject( meshname ){
  w = wall;
  v2f wv, e, s, n, center;
  w->getStart(s);
  w->getEnd(e);
  w->getNorm( n );
  v2fSub(e, s, wv);
  float len = v2fLen(wv);
  node->setScale( len/ 2.0, 1.0, 1.0 );

  //translation
  v2fAdd( s, e, center);
  v2fMult(center, 0.5, center);
  node->setPosition( center[0] + 0.5, 0.0, -center[1]);

  //rotation:
  float theta = std::atan2( -n[1], -n[0] ) - std::atan2( 1.0, 0.0 );
  node->setOrientation( Ogre::Quaternion( Ogre::Radian( theta ), Ogre::Vector3(0.0, 1.0, 0.0) ) ); 

}

void DrawWall::update(){

}


Render * Render::instance;

void setupResourceLocations()
{
   // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load("config/resources.cfg");
  
  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
  
  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements())
    {
      secName = seci.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
      {
         typeName = i->first;
         archName = i->second;
	 Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
	 Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup( secName );
      }

   }

  Ogre::MaterialManager::getSingleton().initialise();
}


Render::Render(){
  initialized =false;
  namenum = 0;
  root = new Ogre::Root( "config/plugins.cfg" , "config/ogre.cfg", "Ogre.log");

  //  if( !root->restoreConfig() )
    root->showConfigDialog();
  Ogre::RenderWindow * w = root->initialise( true, "Rendering!");
  w->setVisible( true );
  setupResourceLocations();

  //create camera
  sceneMgr = root->createSceneManager(Ogre::ST_GENERIC, "scenemgr");
  cam = sceneMgr->createCamera("RenderCam");


  cam->setPosition( 0.0, 30.0, 0.5 );
  cam->lookAt( 0.0, 0.0, 0.0 );
  cam->roll(Ogre::Radian( 3.14159 ) );
  cam->setNearClipDistance(4);



  //create window
  Ogre::Viewport * vp = root->getAutoCreatedWindow()->addViewport(cam);

  vp->setBackgroundColour( Ogre::ColourValue(0.5, 0.5, 0.5) );

  cam->setAspectRatio( Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));



  Ogre::Light * l = sceneMgr->createLight("Light");
  l->setType(Ogre::Light::LT_DIRECTIONAL);
  l->setDirection(Ogre::Vector3( -1.0, -1.0, 0.0) );
  l->setDiffuseColour( 0.5, 0.5, 0.5 );
  l->setSpecularColour( .75, .75, .75);

  initialized = true;
}

Render::~Render(){
  delete root;
}

Render * Render::getInstance(){
  if(instance == NULL){
    instance = new Render();
  }
  return instance;
}

Render * Render::destroyInstance(){
  if(instance != NULL)
    delete instance;
  return 0;
}

string Render::generateName(){
  char thisnum[50];
  sprintf(thisnum, "%i", namenum);
  namenum++;
  return std::string(thisnum);
}

Ogre::SceneNode * Render::attach(string nodename, Ogre::Entity * e){

  Ogre::SceneNode * n = sceneMgr->getRootSceneNode()->createChildSceneNode( nodename, Ogre::Vector3(0, 0, 0));
  n->attachObject(e);
  return n;

}

Ogre::Entity * Render::newEntity( string name, string model){
  Ogre::Entity * e = sceneMgr->createEntity( name, model );
  return e;
}


void Render::update( float f) {
  for( std::vector< DrawObject *>::iterator a = drawObjects.begin();
       a != drawObjects.end();
       a++){
  (*a)->update();
  }
  
  root->renderOneFrame();
  return;
}



/*void Render::drawThis( CrowdObject::CrowdObject * ob, string meshname ){
  DrawObject::DrawObject * d = new DrawObject( ob, meshname );
  drawObjects.push_back( d );
}
*/

void Render::drawThis( Agent::Agent * a, string m ){
  DrawAgent::DrawAgent * da = new DrawAgent( a, m);
  drawObjects.push_back( da );
}

void Render::drawThis( Wall::Wall * w, string meshname){
  DrawWall::DrawWall * dw = new DrawWall( w, meshname);
  drawObjects.push_back( dw );
}

bool Render::isInitialized(){
  return initialized;
}
