#ifndef __WEAPON_MGR__
#define __WEAPON_MGR__
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>

#include "game_obj.h"
#include "weapons.h"

class WEAPON_MANAGER : public GAME_OBJ {
//protected:
	public:
	
	Real mMaxCoolDownTime;
	bool mUsedWeaponsArr[128];
	WEAPON *mBulletsArr[128];
	int mMaxNum;
	int mCurBulletsNum;

	Vector3 mTargetPos;
	Real mTargetRadius;
	bool mFlgTarget;

//public:
	WEAPON_MANAGER(SceneManager *a_SceneMgr);
	void setTarget( const Vector3 &pos, Real radius );
virtual void setMaxBulletsNum(int a_Num);

	virtual void fire_Normal(const Vector3 &pos, const Vector3 &direction);
	virtual void update(const Ogre::FrameEvent& evt);

	Real mCoolDownTimeForFiring;

	void WEAPON_MANAGER::setTargetBox( const Vector3 &pos, Real lx, Real lz );
	int mNumTargetBox;
	Vector3 mTargetBoxPos[1024];
	Real mTargetBoxlx[1024];
	Real mTargetBoxlz[1024];
};

#endif