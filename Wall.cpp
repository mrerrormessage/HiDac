#include "Wall.h"

Wall::Wall(){
  myType = WALL;
  
}

Wall::Wall(v2f s, v2f e ){
  myType = WALL;
  v2f w;
  v2fCopy(s, start);
  v2fCopy(e, end);
  v2fSub( end, start , w);
  v2fTangent( w, norm );
  //not positive this vector needs to be normalized
  v2fNormalize( norm, norm );


}

//two lines in start + direction format along with their lengths
bool testLineIntersection( v2f p1, v2f d1, float l1, v2f p2, v2f d2, float l2){

  return false;
}

//checking the intersection of a line and a rectangle
bool Wall::isVisible( v2f pos, v2f dir, float vislength, float viswidth){
  
  //quick heuristic - not technically correct
  if( getDistance(pos) > vislength )
    return false;
  
  //otherwise, treat the rectangle as four lines. The wall is then visible if
  //its segment intersects any of the four lines
  
  return true;
}

void Wall::getNorm( v2f ret){
  v2fCopy( norm, ret );

}

//find the closest distance from our line to the point
float Wall::getDistance( v2f pos ){
  v2f v;
  getDirection( pos, v);
  return v2fLen(v);

}

//similar, but in vector form
void Wall::getDirection( v2f pos, v2f res ){
  v2f dir, pmins;
  v2fSub( end, start, dir);
  v2fNormalize( dir, dir );
  v2fSub( pos, start, pmins);
  float t = v2fDot( dir, pmins );
  v2f closept;
  v2fMult( dir, t, closept );
  v2fAdd( closept, start, closept);

  v2fCopy( closept, res);
}

int Wall::getType(){
  return myType;
}

void Wall::getVelocity( v2f ret ){
  v2fMult( ret, 0.0, ret );
}

Wall::Wall * twoWalls( Json::Value w){
  v2f st, en;
  v2fFromJson( w["start"], st);
  v2fFromJson( w["end"], en);
  Wall::Wall * ws = new Wall::Wall[2];
  ws[0] = Wall(st, en);
  ws[1] = Wall(en, st);

  return ws;
}
