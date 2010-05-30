#include "Render.h"

DrawObject::DrawObject(){
}

DrawObject::DrawObject( CrowdObject::CrowdObject * c, string mesh ){
  Render::Render * r = Render::getInstance();
  entity = r->newEntity( r->generateName(), mesh );
  node = r->attach( r->generateName(), entity);

}

void DrawObject::update(){

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
  namenum = 0;
  root = new Ogre::Root( "config/plugins.cfg" , "config/ogre.cfg", "Ogre.log");

  root->showConfigDialog();
  Ogre::RenderWindow * w = root->initialise( true, "Rendering!");
  w->setVisible( true );
  setupResourceLocations();

  //create camera
  sceneMgr = root->createSceneManager(Ogre::ST_GENERIC, "scenemgr");
  cam = sceneMgr->createCamera("RenderCam");


  cam->setPosition( 0.0, 30.0, 0.5 );
  cam->lookAt( 0.0, 0.0, 0.0 );
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




  Ogre::Entity * e = newEntity(" hello!" , "Agent.mesh");
  Ogre::SceneNode * n = attach( "hnode", e);
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

void Render::drawThis( CrowdObject::CrowdObject * ob, string meshname ){
  DrawObject::DrawObject * d = new DrawObject( ob, meshname );
  drawObjects.push_back( d );
}
