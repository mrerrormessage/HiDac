#ifndef _RENDER_H_
#define _RENDER_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Ogre.h"
#include "OgreConfigFile.h"
#include "CrowdObject.h"
#include "constants.h"
#include "Agent.h"

// Static plugins declaration section
// Note that every entry in here adds an extra header / library dependency
#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
     // dx10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#    if OGRE_USE_D3D11
#      define OGRE_STATIC_Direct3D11
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#     undef OGRE_STATIC_CgProgramManager
#     undef OGRE_STATIC_GL
#     define OGRE_STATIC_GLES 1
#     ifdef __OBJC__
#       import <UIKit/UIKit.h>
#     endif
#  endif
#  include "OgreStaticPluginLoader.h"
#endif


/* Render is a class that will take care of Rendering the world in our program
 * It is a singleton. 
 */ 


using namespace std;
class DrawObject {
 protected: 
  //pointer to object to be rendered

  Ogre::SceneNode * node;
  Ogre::Entity * entity;

 public:
  DrawObject();
  DrawObject( string m );

  void createEntity(string meshname);

  //possibly needs to be pure-virtual
  virtual void update() = 0;

};

class DrawAgent : public DrawObject {
 private: 
  Agent::Agent * a;

 public: 
  DrawAgent();
  DrawAgent(Agent::Agent * a, string meshname);

  void update();
};

class DrawWall : public DrawObject {
 private: 
  Wall::Wall * w;
  
 public: 
  DrawWall();
  DrawWall( Wall::Wall * wall, string meshname);

  void update();

};

class Render {
 private:

  //Ogre objects
  Ogre::Root * root;
  Ogre::SceneManager * sceneMgr;
  Ogre::Camera * cam;
  
  //used to generate names
  int namenum;

  //singleton maintenance
  static Render * instance;

  Render();

  ~Render();

  std::vector<DrawObject *> drawObjects;

  bool initialized;
 public:
  //singleton maintenance
  static Render * getInstance();
  static Render * destroyInstance();
  
  //creates a new Entity
  Ogre::Entity * newEntity( string name, string model);

  //attaches an entity to render. Returns the node to which the entity is attached
  Ogre::SceneNode * attach(string nodename, Ogre::Entity * e);

  //generates a unique name from the current render context
  string generateName();

  void drawThis( Agent::Agent * a, string meshname);
  void drawThis( Wall::Wall * w, string meshname);

  //updates i.e. renders to the screen
  void update(float deltaTime);
  bool isInitialized();
};

#endif
