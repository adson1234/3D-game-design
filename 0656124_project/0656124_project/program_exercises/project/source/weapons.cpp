#include "weapons.h"
#include "BasicTools.h"

WEAPON::WEAPON(SceneManager *a_SceneMgr) : GAME_OBJ(a_SceneMgr)
{
}

//return true if it hits the sphere with radius r.
bool WEAPON::hitTarget_Sphere(const Vector3 &p, Real r )
{
	bool flg = false;

	Vector3 v = getPosition() - p;
	float d=v.length();
	if (d-r < 10){
		flg=true;
		setPosition(getPosition() + v * (d-r)/d*0.05);
		mVelocity = v *mVelocity.length()/d;
	}

	return flg;
}

bool WEAPON::hitTarget_Box(const Vector3 &p, Real lx ,Real lz)
{
	bool flg = false;

	Vector3 v = getPosition() - p;
	//std::cout << v.x <<" "<<getPosition().x <<" "<< p.x<<std::endl;
	//float d=v.length();
	if (abs(v.x)<=(lx+15) && (abs(v.z)<=lz)){
		flg=true;
		//setPosition(getPosition() + v.x * (l-abs(v.x))/abs(v.x)*0.05);
		mVelocity.x =- mVelocity.x;
	}
	if (abs(v.z)<=(lz+15) && (abs(v.x)<=lx)){
		flg=true;
		//setPosition(getPosition() + v.x * (l-abs(v.x))/abs(v.x)*0.05);
		mVelocity.z =- mVelocity.z;
	}

	return flg;
}

//
// Decrease its life and disable it if its life is negative
// Compute the position of the weapon
// Check collision between it and the terrain surface
// Adjust its position if it his the terrain surface
//
//
void WEAPON::update(const Ogre::FrameEvent& evt)
{
	mLifeTime =mLifeTime-evt.timeSinceLastFrame;
	if (mLifeTime<0){
		makeAlive(false);
		setVisible(false);
		return;
	}
	Vector3 v=mVelocity *mSpeedFactor;
	Vector3 new_pos= getPosition() + v*evt.timeSinceLastFrame;
	//std::cout << getPosition().x <<"  "<<getPosition().y<<"  "<<getPosition().z<< "  "<<std::endl; 
    
    //Check up direction
	Vector3 pos;
	float r =0.05;
    bool flg = projectScenePointOntoTerrain_PosDirection(new_pos);
    if (flg) {
		projectScenePointOntoTerrain_NegDirection(new_pos);
        //pos = new_pos+Vector3(0,1.0, 0)+Vector3(0, 1, 0)*r;
        mVelocity.y = -mVelocity.y*0.9;
    }

	setPosition(new_pos);
	mVelocity.y = mVelocity.y - 3*9.8*evt.timeSinceLastFrame;
    //
    //Check down direction. 
    //Why do we need to do it?
    
}