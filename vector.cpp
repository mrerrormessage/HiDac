#include "constants.h"


void v2fCopy( v2f src, v2f dst ){
  dst[0] = src[0];
  dst[1] = src[1];
  return;
}

float v2fDot( v2f v1, v2f v2){
  return (v1[0] * v2[0] + v1[1] * v2[1]);
}

float v2fLen( v2f v ) {
  float aSquared = pow( v[0], 2);
  float bSquared = pow( v[1], 2);
  
  return sqrt(aSquared + bSquared);
}

void v2fNormalize( v2f v, v2f res ){
  float len = v2fLen( v );
  res[0] = v[0] / len;
  res[1] = v[1] / len;
  return; 
}

float v2fCross( v2f v1, v2f v2){

  return (v1[0] * v2[1]) - (v1[1] * v2[0]);

}

void v2fTangent( v2f v, v2f ret ){
  ret[0] = -ret[1];
  ret[1] = ret[0];
}

void v2fNegate( v2f v){
  v[0] = -v[0];
  v[1] = -v[1];
}

void v2fSub( v2f v1, v2f v2, v2f res ){
  res[0] = v1[0] - v2[0];
  res[1] = v1[1] - v2[1];
}

void v2fAdd( v2f v1, v2f v2, v2f res ){
  res[0] = v1[0] + v2[0];
  res[1] = v1[1] + v2[1]; 
}

void v2fMult( v2f v, float f, v2f res ){
  res[0] = f * v[0];
  res[1] = f * v[1];
}

void v2fFromJson( Json::Value v, v2f res){
  res[0] = v[0u].asDouble();
  res[1] = v[1u].asDouble();
}

void v2fPrint( v2f p ){
  std::cout << "<" << p[0] << "," << p[1] << ">";
  return;
}
