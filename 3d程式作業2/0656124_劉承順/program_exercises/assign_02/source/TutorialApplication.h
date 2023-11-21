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
\n Date: 2017/10/27

This is an assignment of 3D Game Programming

*/

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include "BaseApplication.h"
#include "selection_rectangle.h"


using namespace Ogre;

class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);
	virtual void createViewports(void);
	virtual void createScene(void);
	virtual void createCamera(void);
	virtual void chooseSceneManager(void);
    //
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
    //
protected:
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
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );

	void dealcollision();

    void createHierarchyStructure();
protected:
    Ogre::Viewport* mViewportArr[8];
	Ogre::Camera* mCameraArr[8];
	Ogre::SceneManager* mSceneMgrArr[8];
	OgreBites::SdkCameraMan* mCameraManArr[8];
    //
    bool mFlg_Penguin_Jump;
    bool mFlg_Penguin_Dir;
    //
    bool mFlg_Root;
    bool mFlg_Root_Dir;
    bool mFlg_Arm;
    bool mFlg_Arm_Dir;
    //
    SceneNode *mSceneNode[1024];
    Entity *mEntity[1024];

	double light_rad;
	SceneNode *mCurrentObject;
	PlaneBoundedVolumeListSceneQuery *mVolQuery;
	Real left, top, right, bottom;
	Real beginx,beginy;
	SelectionRectangle *mSelectionRect;
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


	bool moving[1024];
	int destinationX;
	int destinationZ;

	AnimationState *mAnimationState[1024];
	void initParticleSystemForExplosion();
	void setOffParticleSystem(
		Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos);
	SceneNode *mParticleNode;
};

#endif // #ifndef __BasicTutorial_00_h_