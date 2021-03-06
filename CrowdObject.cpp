#include "CrowdObject.h"

CrowdObject::CrowdObject( ){

}

CrowdObject::CrowdObject( Json::Value c){
  std::string s = c["type"].asString();
  std::string fa = "fallen agent";
  std::string a = "attractor";
  std::string w = "wall";
  std::string ag = "agent";
  std::string ob = "obstacle";
  if (s.compare(fa) == 0)
    myType = FALLEN_AGENT; 
  if (s.compare(a) == 0)
    myType = ATTRACTOR;
  if (s.compare(w) == 0)
    myType = WALL;
  if (s.compare(ag) == 0)
    myType = AGENT;
  if (s.compare(ob) == 0)
    myType = OBSTACLE;

  v2fFromJson( c["norm"], norm);
  v2fFromJson( c["pos"], pos);
}

//all methods of CrowdObject will be overridden. 
bool CrowdObject::isVisible( v2f pos, v2f dir, float vislength, float viswidth){
  std::cout << "crap!\n";
  return false; 
}


void CrowdObject::getNorm( v2f ret ){
  v2fCopy( norm, ret );
  return;
}


float CrowdObject::getDistance( v2f otherPos ){
  v2f v;
  getDirection( otherPos, v);
  std::cout << "BAD THINGS!\n";
  return v2fLen( v );
}


void CrowdObject::getDirection( v2f otherPos, v2f res ){
  v2fSub(pos, otherPos, res);
  return;
}


int CrowdObject::getType(){
  return myType;
}


void CrowdObject::getVelocity(v2f ret){
  std::cout << "Crap!\n";
  v2fMult( ret, 0.0, ret );
}
