#include "main_char.h"
#include "BasicTools.h"
#include <OgreInstancedGeometry.h>

MAIN_CHAR::MAIN_CHAR(SceneManager *a_SceneMgr) : GAME_OBJ(a_SceneMgr)
{
	mCamera = 0;
	mVelocity = Vector3(1, 0, 0);
mSpeedFactor = 1;
mActionMode = ACTION_NONE;
mEyePosition = Vector3(0,60, 0);

mFireActionMode = FIRE_ACTION_NONE;

mWeaponMgr = new WEAPON_MANAGER(mSceneMgr);
mTarget = 0; //null
//mCurBulletsNum = 0;
mNumTargetBox=0;
}

WEAPON_MANAGER *MAIN_CHAR::getWeaponManager( )
{
	return mWeaponMgr;
}

void MAIN_CHAR::setMaxBulletsNum(int a_Num)
{
	mCurBulletsNum = a_Num;
	mWeaponMgr->setMaxBulletsNum(mCurBulletsNum);
}

void MAIN_CHAR::attachCamera(Camera *a_Camera)
{
	mCamera = a_Camera;
	FrameEvent evt;
	evt.timeSinceLastFrame = 0;
	walkForward(evt);
}

void MAIN_CHAR::updateViewDirection()
{
	
	Vector3 actualDirection = mCamera->getRealDirection();
	Vector3 e = actualDirection*20;
	mCamera->setPosition(mSceneNode->getPosition()+mEyePosition-e*5);

	//Use mouse move event for controlling the view direction of the main character.
	Vector3 pos = mCamera->getRealDirection() ;
	Vector3 target_pos = mSceneNode->getPosition();
	target_pos.x = target_pos.x + pos.x;
	target_pos.z = target_pos.z + pos.z;

	mSceneNode->lookAt(target_pos, Node::TS_WORLD);
	//mSceneNode->yaw(Radian(3.14159/2));

}

void MAIN_CHAR::walkForward(const Ogre::FrameEvent& evt)
{
	//exit(1);
	Vector3 actualDirection = mQuaternion*mInitDirection;
	Quaternion q = mCamera->getOrientation();
	Quaternion q0 = Quaternion(Radian(0), Vector3(1, 0, 0));
	q = q*q0;
	actualDirection = mCamera->getRealDirection();
	
	//BEGIN: BUG FIXED VERSION
	Vector3 p = mSceneNode->getPosition();
	actualDirection.y = 0.0;
	
	mSceneNode->lookAt(p+ actualDirection*10, Node::TS_WORLD);
	//mSceneNode->yaw(Radian(3.14159*0.5));
	mSceneNode->yaw(Radian(3.14159));
	//END: BUG FIXED VERSION

	/*
	//BUG
	mSceneNode->setOrientation(q);
	mSceneNode->yaw(Radian(3.14159*0.5));
	*/

	actualDirection = mCamera->getRealDirection();
	Vector3 d;
	mSpeedFactor = 100;
	d = actualDirection*mSpeedFactor*evt.timeSinceLastFrame;

	logMessage("Direction\n");
	logMessage(actualDirection);

	logMessage(d);
	mSceneNode->translate(d);

	Vector3 pos = mSceneNode->getPosition();
	bool flg = projectScenePointOntoTerrain_PosDirection(pos);
	if (flg == false) {
		projectScenePointOntoTerrain_NegDirection(pos);
	}
	mSceneNode->setPosition(pos);

		Vector3 e = actualDirection*20;
	mCamera->setPosition(pos+mEyePosition-e*5);

	
}

void MAIN_CHAR::walkBackward(const Ogre::FrameEvent& evt)
{
	FrameEvent Revt;
	Revt.timeSinceLastEvent=-evt.timeSinceLastEvent;
	Revt.timeSinceLastFrame=-evt.timeSinceLastFrame;
	walkForward(Revt);
	/*
	Vector3 pos = mSceneNode->getPosition();
	bool flg = projectScenePointOntoTerrain_PosDirection(pos);
	if (flg == true) {
		projectScenePointOntoTerrain_NegDirection(pos);
	}
	Vector3 actualDirection = mCamera->getRealDirection();

	mCamera->setPosition(pos+mEyePosition+20*actualDirection);
	*/
}


void MAIN_CHAR::setWalkForward()
{
	mActionMode |= ACTION_WALK_FORWARD;
}
void MAIN_CHAR::setWalkBackward()
{
	mActionMode |= ACTION_WALK_BACKWARD;

}

	void MAIN_CHAR::unsetWalkForward()
{
	mActionMode ^= ACTION_WALK_FORWARD;
}
	void MAIN_CHAR::unsetWalkBackward()
	{
	mActionMode ^= ACTION_WALK_BACKWARD;

	}

	Vector3 MAIN_CHAR::getWeaponPosition() const
	{
		Vector3 p = mSceneNode->getPosition();
		p += mEyePosition;
		Vector3 d = mCamera->getRealDirection();
		p += d*50;
		return p;
	}

	void MAIN_CHAR::update(const Ogre::FrameEvent& evt)
	{
		if (mActionMode & ACTION_WALK_FORWARD) {
			walkForward(evt);
		}
				if (mActionMode & ACTION_WALK_BACKWARD) {
			walkBackward(evt);
		}
		if (mFireActionMode){
			fireWeapon();
			mFireActionMode = FIRE_ACTION_NONE;
		}
		updateWeapon(evt);
		Real sf = 2.0;
		//if (mAnimationState == 0) {
			if (
				(mActionMode & ACTION_WALK_FORWARD)
				||
				(mActionMode & ACTION_WALK_BACKWARD)
				) {
					//mAnimationState = mEntity->getAnimationState("Walk");
					mAnimationState = mEntity->getAnimationState("Walk");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);
	if (mActionMode & ACTION_WALK_FORWARD) {
	mAnimationState->addTime(evt.timeSinceLastFrame*sf);
	}
	else {
		mAnimationState->addTime(-evt.timeSinceLastFrame*sf);
	}
	} else {
				//mAnimationState = mEntity->getAnimationState("Idle");
				mAnimationState = mEntity->getAnimationState("Idle1");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);
	mAnimationState->addTime(0.1*evt.timeSinceLastFrame*sf);
			}
			if (mWeaponMgr->mCoolDownTimeForFiring < 0.2){
				mAnimationState = mEntity->getAnimationState("Kick");
				mAnimationState->setLoop(true);
				mAnimationState->setEnabled(true);
				mAnimationState->addTime(5*evt.timeSinceLastFrame*sf);
			}

			//collision detect
			for(int j=0;j<mNumTargetBox;j++){
				Vector3 p=mTargetBoxPos[j];
				float lx=mTargetBoxlx[j];
				float lz=mTargetBoxlz[j];
				Vector3 v = getPosition() - p;
				//float d=v.length();
				//std::cout<<v.x<<" "<<getPosition().x<<std::endl;
				if (abs(v.x)<=(lx+50) && (abs(v.z)<=lz)){
					setPosition(getPosition() + Vector3(v.x * (lx-abs(v.x))/abs(v.x)*0.07 ,0,0));
					//mVelocity.x =- mVelocity.x;
				}
				if (abs(v.z)<=(lz+50) && (abs(v.x)<=lx)){
					setPosition(getPosition() + Vector3(0,0,v.z * (lz-abs(v.z))/abs(v.z)*0.07));
					//mVelocity.z =- mVelocity.z;
				}
			}

			updateViewDirection();
	}

	void MAIN_CHAR::fireWeapon()
	{
		Vector3 pos = mCamera->getRealDirection() ;
		//mWeaponMgr->fire_Normal(mSceneNode->getPosition(),pos);
		mWeaponMgr->fire_Normal(getWeaponPosition(),pos);
	}

	void MAIN_CHAR::updateWeapon(const Ogre::FrameEvent& evt)
	{
		mWeaponMgr->update(evt);
	}

	void MAIN_CHAR::setFireAction_Normal()
	{
		mFireActionMode |= FIRE_ACTION_NORMAL;
	}

	void MAIN_CHAR::setTargetBox( const Vector3 &pos, Real lx, Real lz )
{
	mTargetBoxPos[mNumTargetBox] = pos;
	mTargetBoxlx[mNumTargetBox] = lx;
	mTargetBoxlz[mNumTargetBox] = lz;
	mNumTargetBox=mNumTargetBox+1;

}