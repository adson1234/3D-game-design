#include "weapon_manager.h"

WEAPON_MANAGER::WEAPON_MANAGER(SceneManager *a_SceneMgr) : GAME_OBJ(a_SceneMgr)
{
	mCoolDownTimeForFiring = 0.0;
mMaxCoolDownTime = 0.25;
//
mMaxNum = 128;
mCurBulletsNum = mMaxNum;
for (int i = 0; i < mMaxNum; ++i) {
	mUsedWeaponsArr[i] = false;
	 mBulletsArr[i] = new WEAPON(mSceneMgr);
	 mBulletsArr[i]->createGameObj("b", "sphere.mesh");
mBulletsArr[i]->scale(0.3, 0.3, 0.3);
mBulletsArr[i]->setVisible(false);
mBulletsArr[i]->getEntity( )->setMaterialName("Examples/RustySteel");
}
mFlgTarget = false;
mNumTargetBox=0;
}

void WEAPON_MANAGER::setTarget( const Vector3 &pos, Real radius )
{
		mTargetPos = pos;
		mTargetRadius = radius;
	mFlgTarget = true;
}
void WEAPON_MANAGER::setTargetBox( const Vector3 &pos, Real lx, Real lz )
{
	mTargetBoxPos[mNumTargetBox] = pos;
	mTargetBoxlx[mNumTargetBox] = lx;
	mTargetBoxlz[mNumTargetBox] = lz;
	mNumTargetBox=mNumTargetBox+1;

}


void WEAPON_MANAGER::setMaxBulletsNum(int a_Num)
{
	if (a_Num >= mMaxNum) {
		a_Num = mMaxNum;
	}

	mCurBulletsNum = a_Num;
}

void WEAPON_MANAGER::fire_Normal(const Vector3 &pos, const Vector3 &direction)
{
	std::cout<<"fire_normal"<<std::endl;
GAME_OBJ *g = 0;
for (int i = 0; i < mCurBulletsNum; ++i) {
		if (mUsedWeaponsArr[i] == true) continue;
g = mBulletsArr[i];
mUsedWeaponsArr[i] = true;
break;
}
if (g == 0) return;
	if (mCoolDownTimeForFiring < mMaxCoolDownTime) return;
mCoolDownTimeForFiring = 0;
g->setPosition(pos);
g->setVelocity(direction*400);
g->setSpeedFactor(1);
g->setLife(5, 5);
g->makeAlive(true);
g->setVisible(true);

}

void WEAPON_MANAGER::update(const Ogre::FrameEvent& evt)
{
	mCoolDownTimeForFiring += evt.timeSinceLastFrame;
	if (mCoolDownTimeForFiring > mMaxCoolDownTime)
	{
		mCoolDownTimeForFiring = mMaxCoolDownTime;
	}
	for (int i = 0; i < mMaxNum; ++i) {
		if (mUsedWeaponsArr[i] == false) continue;
		WEAPON *g = mBulletsArr[i];
		g->update(evt);

		if ( mFlgTarget ) {
			g->hitTarget_Sphere( mTargetPos, mTargetRadius );
		}
		for(int j=0;j<mNumTargetBox;j++){
			g->hitTarget_Box( mTargetBoxPos[j], mTargetBoxlx[j],mTargetBoxlz[j] );
		}
		if (!g->isAlive()) {
			mUsedWeaponsArr[i] = false;
		}
	}

}
