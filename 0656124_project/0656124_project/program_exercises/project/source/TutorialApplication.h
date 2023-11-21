//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
/*!
\brief 3D Game Programming
\n
My Name: ¼B ©Ó ¶¶
\n
My ID: 0656124
\n
My Email: ciaskbe@gmail.com

This is an assignment of 3D Game Programming

*/

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include "BaseApplication.h"
#include "main_char.h"
#include "monster_manager.h"
#include "movableText.h"
#include "sound.h"

class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual void createViewports(void);
	virtual void createScene(void);
	virtual void createCamera(void);
	virtual void chooseSceneManager(void);

	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	
	void initParticleSystemForExplosion();
void setOffParticleSystem(
	Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos); 

	SceneNode *mParticleNode[512];

	virtual void createFrameListener();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	OgreBites::Label* mInfoLabel;

	int highscore;
	int score;
	int difficulty;
private:
	/*!
	\brief Create a viewport

	Create a viewport for the entire screen.

	\return The sum of two integers.
	*/
	void createViewport_00(void);
	void createViewport_01(void);
	//
	void createCamera_00();
	void createCamera_01();

	void createScene_00();
	void createScene_01();

	bool mFlgMotion;
	bool mKeyPressed;
	Ogre::Real mToggle;
	Ogre::Light *mLight0;
	Ogre::SceneNode* fNode[512];
	Ogre::SceneNode* mMonsterfNode[512];
	Ogre::SceneNode *mSN_Sphere;
	Ogre::Camera* mCameraArr[8];
	Ogre::SceneManager* mSceneMgrArr[8];
	OgreBites::SdkCameraMan* mCameraManArr[8];
//
	Real mSphereRadius;
	int mKeyPressedZoomMode;
	Real mCameraDistanceSlowDownSpeed;
	Real mCameraDistance;
	Real mCameraDistanceAdjustSpeed;
	MAIN_CHAR *mMainChar;
	MONSTER_MANAGER *mMonsterMgr;

	Entity *mEntity[1024];
	SceneNode *mSceneNode[1024];

	float secondCameraHeight;
	int secondCameraZoomMode;
	float secondCameraZoomSpeed;

	Ogre::MovableText* msg[128];
	SceneNode* MCmsgNode[128];

	void ChangeDifficulty();

	SOUND *mSoundBGM;
	SOUND *mSoundHit;
	SOUND *mSoundShoot;
};

#endif // #ifndef __BasicTutorial_00_h_