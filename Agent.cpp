#include "Agent.h"

Agent::Agent(){
  myType = AGENT;
  stopping = false; 
  waiting = false; 
  isColliding = false; 
}

Agent::Agent( Json::Value a ){
  myType = AGENT;
  stopping = false;
  waiting = false; 
  isColliding = false; 
  
  v2fMult(force, 0.0, force);

  v2fMult(vel, 0.0, vel);
  v2fMult(repelForce, 0.0, repelForce);
  v2fMult(norm, 0.0, norm);
  Beta = 0.0;

  attractorWeight = a["atWeight"].asDouble();
  wallWeight = a["waWeight"].asDouble();
  obstacleWeight = a["obWeight"].asDouble();
  fallenWeight = a["faWeight"].asDouble();
  acceleration = a["accel"].asDouble();
  maxVelocity = a["maxVel"].asDouble();
  vislong = a["visDist"].asDouble();
  viswide = a["visWid"].asDouble();
  
  //not quite sure how to specify attractor
  //for now we'll just have a CrowdObject contained in the agent
  attractor = CrowdObject( a["attractor"] );
  v2fFromJson( a["pos"], pos );
  
  if(a.isMember( "liveValues" ) == true && a["liveValues"].asBool() == true){
    //all in-progress data will be reported to JSON object

  }
}

void Agent::print(){
  v2fPrint( "position: ", pos);
  return;

}
Json::Value Agent::getJson(){
  Json::Value v;
  v["atWeight"] = attractorWeight;
  v["waWeight"] = wallWeight;
  return v;
}

objtype Agent::getType(){
  return myType;
}

void Agent::getVelocity( v2f ret ){
  v2fCopy( vel , ret );
}

void Agent::setVelocity( v2f set ){
  v2fCopy( set, vel);
}

float Agent::getSpeed( ){
  return v2fLen( vel );
}

void Agent::getDirection( v2f get ){
  if( this->getSpeed() >= 0.0 + EPSILON ){
    v2fNormalize( vel, norm );
  }
  v2fCopy(norm, get);
  return;
}

void Agent::getPos( v2f ret){ 
  v2fCopy( pos, ret );
  return;
}

void Agent::setPos( v2f set ){
  v2fCopy( set, pos );
  return;
}

bool Agent::isVisible( v2f objPos, v2f objDir, float vislength, float viswidth ){
  //procedure: given the circle that the agent has, find the point closest to the 
  //line described by origin: pos, direction: dir, length: vislength. 
  
  //procedure: take the point at the center of the circle representing an agent, and check that the distance between it and the line is less than radius + viswidth

  v2f cpostolpos;
  v2fSub( objPos, pos, cpostolpos);
  
  v2f negDir;
  v2fCopy(objDir, negDir);
  v2fNegate( negDir );
  
  //the effective radius
  float er = radius + viswidth;

  float root = pow( v2fDot(objDir, cpostolpos) , 2) - 
    ( pow(v2fLen(objDir) , 2) * 
      (pow( v2fLen(cpostolpos ), 2) - pow( er, 2 )));

  return root >= ( 0.0 - EPSILON );
  

}

float Agent::getDistance( v2f objPos ){
  v2f diff;
  v2fSub( objPos, pos, diff );
  return v2fLen(diff) - radius;
}

//returns the vector to get from objPos to position of the object
void Agent::getDirection( v2f objPos, v2f res ){
  v2fSub( objPos, pos, res );
  return;
}

//application of the HiDAC algorithm to an agent
void Agent::calculateForces (){
  //calculate perceived density
  float perceivedDensity = 1.0;

  //running total vector
  v2f rt;
  v2fMult(rt, 0.0, rt);
  


  //copy the last force in (term 1 in equation)
  v2fCopy(force, rt);

  //Force towards attractor
  v2f dtoattractor;
  //a problem is that with just the attractor the agent will 'pace' back and forth over it
  attractor.getDirection( pos, dtoattractor );

  v2fMult(dtoattractor, attractorWeight, dtoattractor);

  v2fAdd( rt, dtoattractor, rt);


  //foreach object in visobjects
  std::vector<CrowdObject>::iterator it;
  v2f tempForce;
  
  //declared for use in switch
  float distweight;
  float dcrossv;
  v2f otherVel;
  for( it = visobjects.begin() ; 
       it != visobjects.end();
       it++ ){
    v2fMult( tempForce, 0.0, tempForce);
    switch( it->getType() ){
    case AGENT : { 
      //calculate tangential force
      it->getDirection(pos, tempForce);
      dcrossv = v2fCross( tempForce, vel);
      float dist = v2fLen( tempForce );
      v2fMult(tempForce, dcrossv, tempForce);
      v2fNormalize(tempForce, tempForce);
      distweight = pow( dist -  perceivedDensity, 2);
      //takes into account the difference in direction between the two agents
      float dirweight;
      it->getVelocity(otherVel);
      if( v2fDot( vel, otherVel ) > 0.0 + EPSILON ){
	dirweight = 1.2;
      } else { 
	dirweight = 2.4;
      }
      v2fMult( tempForce, dirweight * distweight, tempForce);
      break;
    }
    case WALL : {
      //avoidance force for wall is wallnormal cross velocity cross wallnormal, normalized
      it->getNorm(tempForce);
      dcrossv = v2fCross( tempForce, vel );
      v2fMult(tempForce, dcrossv , tempForce);
      v2fNormalize(tempForce, tempForce);
 
      break; 
    }
    case OBSTACLE : {
      //for now, obstacles work the same as walls, perhaps in the future that will change
      it->getNorm(tempForce);
      dcrossv = v2fCross( tempForce, vel );
      v2fMult(tempForce, dcrossv , tempForce);
      v2fNormalize(tempForce, tempForce);
 
      break;
    }
      //fallen_agent case not implemented
    default: 
      break;
    }
    
     v2fAdd(rt, tempForce, rt);

  }

  v2fCopy(rt, force);

  //normalize force
  v2fNormalize(force, force);

  //need to add repulsion forces as well. Will test shortly then add
  v2fMult( repelForce, 0.0, repelForce);
}

//stub function
void Agent::computeFallen( v2f ret ){
  v2fMult(ret , 0.0, ret);
  
  //if close enough to a fallen agent, should set the float Beta with the distance to a fallen agent. For now, set to zero
  Beta = 0.0;
}

float Agent::computeAlpha( ){
  if( v2fLen(repelForce) > 0.0 || stopping || waiting )
    return 0.0;
  else 
    return 1.0;
}

float Agent::computeVel( float deltaT ){
  if (v2fLen(vel) == maxVelocity)
    return getSpeed();
  else 
    return getSpeed() + acceleration*deltaT;
}

void Agent::applyForces( float deltaT ){
  //start with the current position = pos
  v2f oldPos; 
  v2fCopy(pos, oldPos);

  //compute normal movement forces
  v2f fallen;
  computeFallen(fallen);
  v2f normForce, normalMove, movement;

  float moveFactor = computeAlpha() * computeVel(deltaT) * deltaT;

  v2fMult(force, (1.0 - Beta), normalMove);

  v2fMult(fallen, Beta, fallen);

  v2fAdd(fallen, normalMove, movement);

  v2fMult(movement, moveFactor, movement);


  //add to repulsive Forces
  v2fAdd(movement, repelForce, movement);
  
  v2fAdd(movement, pos, pos);

  //update velocity value after updating position
  v2fSub( pos, oldPos, vel );


}
