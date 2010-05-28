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
  if( len == 0.0){
    v2fCopy(v, res); 
    return;
  }
  res[0] = v[0] / len;
  res[1] = v[1] / len;
  return; 
}

float v2fCross( v2f v1, v2f v2){

  return (v1[0] * v2[1]) - (v1[1] * v2[0]);

}

//gets the tangent vector in the 
void v2fTangent( v2f v, v2f ret ){
  ret[0] = v[1];
  ret[1] = -v[0];
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

void v2fAdd( v2f v1, v2f v2, float t, v2f res){
  res[0] = v1[0] + (v2[0] * t);
  res[1] = v1[1] + (v2[1] * t);

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

void v2fPrint( const char * label, v2f p){
  std::cout << label << "<" << p[0] << "," << p[1] << ">" << "\n";
}

/*procedure taken from geometric tools for computer graphics*/
float ptToLineDist( v2f pos, v2f start, v2f dir, float len ){
  
  v2fNormalize(dir, dir);
  v2f diff, end;
  v2fSub( pos, start, diff );
  float t = v2fDot(dir, diff);
  if (t >= len ){

    v2fAdd( start, dir, len, end );
    v2fSub( pos, end, diff );
    return v2fLen( diff);
  }
  if (t <= 0.0 ){
    v2fSub( pos, start, diff);
    return v2fLen( diff);
  }


  v2fAdd( start, dir, t, end );
  v2fSub( pos, end, diff );
  return v2fLen( diff);
    
}
