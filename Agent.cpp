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

  radius = a["radius"].asDouble();
  
  personalSpace = a["pspace"].asDouble();

  //not quite sure how to specify attractor
  //for now we'll just have a CrowdObject contained in the agent
  attractor = CrowdObject( a["attractor"] );
  v2fFromJson( a["pos"], pos );
  
  if(a.isMember( "liveValues" ) == true && a["liveValues"].asBool() == true){
    //all in-progress data will be reported to JSON object

  }
}

Agent::~Agent(){

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
  if( this->getSpeed() >= 0.0 + MY_EPSILON ){
    v2fNormalize( vel, norm );
  }
  v2fCopy(norm, get);
  return;
}

float Agent::getPersonalSpace(){
  return personalSpace;
}

float Agent::getRadius(){
  return radius;
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
  
  //procedure: take the point at the center of the circle representing an agent
  // check that the distance between it and the line is less than radius + viswidth

  //the technical procedure is out of geometric tools for computer games
  float d = ptToLineDist( pos, objPos, objDir, vislength);

  float er = radius + viswidth;
  if( d <= er )
    return true;

  return false;

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

void Agent::calculateRepelForce(){

  //repulsion forces are incurred only for those objects which are colliding with the agent
  //this means that for each c in collideObjects, we check what it is, the calc
  //based on the type what the repulsion is. Add them all together and store the result in repelForce (cleared on reset)

  /*overarching formula:
    repelForce = sum( repelForce(Walls)) 
    + obstacles (not implemented) 
    + lambda * sum(repelForce(Agents) ) 

    lambda is set to 0.3 if there are collisions with other obstacles to give preference to avoiding walls and obstacles over agents
   */

  float lambda = 1.0;
  v2f forceFromAgents, forceFromWalls;
  v2fMult( forceFromAgents, 0.0, forceFromAgents);
  v2fMult( forceFromWalls, 0.0, forceFromWalls);

  for( std::vector<CrowdObject *>::iterator c = collideObjects.begin();
       c != collideObjects.end();
       c++){
    switch( (*c)->getType()){
      case AGENT: {
	/*i is this agent, j is the other agent
	  d_ji is the distance between their centers
	  ep is the person
	  formula for agent: (pos_i - pos_j)*(r_i + ep_i + r_j - d_ji)/ d_ji
	  
	*/
	
	v2f pos_j; 
	(*c)->getPos( pos_j );
	v2f jtoi;
	v2fSub( pos, pos_j, jtoi);
	/*getDistance subtracts out the radius of the agent
	  j->getDistance( pos ) = d_ji - r_j
	  => -getDistance_ji = -(d_ji - r_j) = -d_ji + r_j 
	*/
	float k = ( getRadius() + getPersonalSpace() + (*c)->getDistance( pos )) 
	  /
	  ((*c)->getDistance( pos ) + (*c)->getRadius() );
	v2fAdd(forceFromAgents, jtoi, k, forceFromAgents);
      }
      case WALL: {

	lambda = 0.3;
	/* for walls, the formula is
	   n is the normal 
	   n * (r_i + ep_i - d_wi)/ d_wi
	*/
	//compute multiplicative factor, get the walls normal, then mult into wallForce. 
	//set lambda to 0.3 to give precendence to avoiding any agents
	float k = (radius + personalSpace - (*c)->getDistance(pos)) / (*c)->getDistance(pos);
	//k is sometimes memory-corrupt
	std::cout << "cdist: " << (*c)->getDistance(pos) << "\n";
	v2f norm;
	v2f currentforce;
	(*c)->getNorm( norm );

	v2fMult( norm, k, currentforce);
	//only add those forces which oppose the agents movement 
	//this is a consequence of the fact that walls are represented as two 
	//back-to-back sections
	if( v2fDot( currentforce, vel ) <= 0.0 ){
	  v2fAdd( forceFromWalls, currentforce, forceFromWalls);
	  v2fPrint( "current force: ", currentforce);
	  v2fPrint( "force from walls: ",  forceFromWalls);
	}
     }
	//in the paper's model, there are also obstacles. I have excluded those for now
      default: {
	

      }
      }
  }

  /* carry out the overarching computation */
  v2fPrint( "agent forces: ",  forceFromAgents);
  v2fPrint( "force from walls: ",  forceFromWalls);
  if( v2fDot(vel, forceFromAgents) < 0 && !panic ){
    stopping = true;
    stoptime = std::rand() % 50;
  }
  v2fMult(forceFromAgents, lambda,forceFromAgents);
  v2fAdd(forceFromWalls, forceFromAgents, repelForce);
  v2fPrint( "repulsion forces: ", repelForce);
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


  //foreach object in visObjects
  std::vector<CrowdObject *>::iterator it;
  v2f tempForce;
  
  //declared for use in switch
  float distweight;
  float dcrossv;
  v2f otherVel;
  for( it = visObjects.begin() ; 
       it != visObjects.end();
       it++ ){
    v2fMult( tempForce, 0.0, tempForce);
    switch( (*it)->getType() ){
    case AGENT : { 
      //calculate tangential force
      (*it)->getDirection(pos, tempForce);
      dcrossv = v2fCross( tempForce, vel);
      float dist = v2fLen( tempForce );
      v2fMult(tempForce, dcrossv, tempForce);
      v2fNormalize(tempForce, tempForce);
      distweight = pow( dist -  perceivedDensity, 2);
      //takes into account the difference in direction between the two agents
      float dirweight;
      (*it)->getVelocity(otherVel);
      if( v2fDot( vel, otherVel ) > 0.0 + MY_EPSILON ){
	dirweight = 1.2;
      } else { 
	dirweight = 2.4;
      }
      v2fMult( tempForce, dirweight * distweight, tempForce);
      break;
    }
    case WALL : {
      //avoidance force for wall is wallnormal cross velocity cross wallnormal, normalized
      (*it)->getNorm(tempForce);
      dcrossv = v2fCross( tempForce, vel );
      v2fMult(tempForce, dcrossv , tempForce);
      v2fNormalize(tempForce, tempForce);
      v2fMult( tempForce, wallWeight, tempForce );
      break; 
    }
    case OBSTACLE : {
      //for now, obstacles work the same as walls, perhaps in the future that will change
      (*it)->getNorm(tempForce);
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

  
  //calculate repulsionForces (they will be added later)
  if( isColliding ){
    calculateRepelForce();
  } 
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
  v2f normalMove, movement;

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

//functions to update visibility and collision vectors
void Agent::checkCollide( CrowdObject::CrowdObject * c ){
  v2f wallVec;
  c->getDirection( pos, wallVec);

  if(c->getDistance( pos ) < radius ){
    collideObjects.push_back( c );
    isColliding = true;
  }
}

void Agent::checkVisible( CrowdObject::CrowdObject * c ){
  v2f d; 
  getDirection( d );
  
  if( c->isVisible(pos, d, vislong, viswide) ){
    visObjects.push_back( c );
  } 

}

//function to 'reset' at the end of a simulation step 
void Agent::reset(){
  isColliding = false;
  stoptime--;
  if(stoptime == 0){
    stopping = false;
  }
  visObjects.clear();
  collideObjects.clear();
  v2f zero;
  v2fMult( zero, 0.0, zero );
  v2fCopy(zero, repelForce);


}
