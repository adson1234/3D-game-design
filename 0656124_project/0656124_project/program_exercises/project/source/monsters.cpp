#include "monsters.h"
#include "BasicTools.h"

MONSTER::MONSTER(SceneManager *a_SceneMgr) : GAME_OBJ(a_SceneMgr)
{
	die=false;
	dieTime=0;
}

/*
Update the view direction of the monster.
*/
void MONSTER::updateViewDirection()
{
	if (mTarget == 0) return;
	//The monsters should look at the main character. But they can rotate about only the y-axis
	this->mSceneNode->lookAt(Vector3(this->mTarget->getPosition().x,this->getPosition().y,this->mTarget->getPosition().z)
		, Node::TS_WORLD, Vector3::UNIT_Z);

}

/*
Update the position and view direction of the monster.
*/
void MONSTER::update(const Ogre::FrameEvent& evt,int difficulty)
{
	if (die){
		dieTime = dieTime + evt.timeSinceLastFrame;
		//std::printf("%f %f\n",evt.timeSinceLastFrame,dieTime);
		mAnimationState = mEntity->getAnimationState("amuse");
		mAnimationState->setLoop(true);
		mAnimationState->setEnabled(true);
		mAnimationState->addTime(2*evt.timeSinceLastFrame);
		if (dieTime >= 55.0/(10+difficulty)){
			dieTime=0;
			die = false;
			mAnimationState->setEnabled(false);

			Vector3 org(3000, 100, 3000);
			Vector3 pos;
			Real x = -1+2*(rand()%100)/100.0;
			Real z = -1+2*(rand()%100)/100.0;
			pos.x = 2800*x;
			pos.z = 2800*z;
			pos.y = 20;
			setPosition(pos+org);
			chase=0;
		}
		return;
	}

    //compute the position of the monster
    //call updateViewDirection( )
	Vector3 d = this->mTarget->getPosition() - this->getPosition();
	Real len = Vector3(d.x,0,d.z).length();
	if (len > 0) d.normalise();
	int Istoward;
	//The monsters chase the main character if they are too far from it.
	//The monsters move away from the main character if they are too close to the main character.
	if (len >= 700 +difficulty*40){
		chase=0;
	}
	if (len < (700 +difficulty*40) && chase>=0){
		chase=1.5 + (float)difficulty*0.1;
	}
	if (len < (200 + difficulty*20) && chase>=0){
		chase=3.5 + (float)difficulty*0.2;
	}
	//if (len < 90 ){
	//	chase=-2.3;
	//}

	//this->setPosition(this->getPosition() + chase * d*this->mRandSpeed*this->mSpeedFactor*evt.timeSinceLastFrame);

	Vector3 pos = this->getPosition() + chase * d*this->mRandSpeed*this->mSpeedFactor*evt.timeSinceLastFrame;
	bool flg = projectScenePointOntoTerrain_PosDirection(pos);
	//std::cout << flg; 

	if (flg == false) {
		bool flg2=projectScenePointOntoTerrain_NegDirection(pos);

		//std::cout <<" " << flg2; 
	}
	 //std::cout <<std::endl; 

	this->setPosition(pos + Vector3(0,20,0));
	//std::cout <<  " update finish" << this->mRandSpeed <<std::endl;

	
	updateViewDirection( );
}
