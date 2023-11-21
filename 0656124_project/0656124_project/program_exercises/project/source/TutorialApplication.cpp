//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#include "TutorialApplication.h"
#include "BasicTools.h"
#include "read_data.h"
#include <string>

#include "OgreTextAreaOverlayElement.h"
#include "OgreStringConverter.h"

#include "TextRenderer.h"
#include "movableText.h"

#include <sstream>
#define MAX_WCHAR 100
#define PI 3.1415926

using namespace Ogre;
BasicTutorial_00::BasicTutorial_00(void) {
	mFlgMotion = true;
	mToggle = 2.5;
	mKeyPressed = false;
	//
	mMainChar = 0;
	mCameraDistance = 0;
	
	mKeyPressedZoomMode = 0;
mCameraDistanceAdjustSpeed = 0;
mCameraDistanceSlowDownSpeed = 0;

secondCameraHeight =1300;	//Distance of second camera (above character)
secondCameraZoomMode =0;
secondCameraZoomSpeed =5000;
	srand((unsigned)time(NULL));

	highscore=0;
	score=0;
	difficulty=1;

	std::cout<<"soundBGM play"<<std::endl;
	mSoundBGM = new SOUND;
	mSoundBGM->init("bokuzyo.wav");
	//mSoundBGM->setFileName("bokuzyo.wav");
	mSoundBGM->play();
	std::cout<<"soundBGM end"<<std::endl;

	/*std::cout<<"soundhit play"<<std::endl;
	mSoundHit = new SOUND;
	mSoundHit->init();
	mSoundBGM->play();
	std::cout<<"soundhit end"<<std::endl;
	
	mSoundShoot = new SOUND;
	mSoundShoot->init();
	mSoundShoot->setFileName("stereo.wav");
	mSoundBGM->play();*/
}

//
//handle the mouse motion (passive) event
//
bool BasicTutorial_00::mouseMoved( const OIS::MouseEvent &arg )
{
	bool flg = BaseApplication::mouseMoved(arg);
	//mMainChar->updateViewDirection();
	return flg;
}

//handle the key pressed event
bool BasicTutorial_00::keyPressed( const OIS::KeyEvent &arg )
{
	//bool flg = BaseApplication::keyPressed(arg);
	bool flg = true;
	
	if (arg.key == OIS::KC_W) {
		mMainChar->setWalkForward();
	}
	/*
	if (arg.key == OIS::KC_T) {
		//Create a main character ¡§robot¡¨ and control it to move around. The main character can be animated based on pre-defined animations. 
		mMainChar->setWalkForward();
	}
	

	if (arg.key == OIS::KC_G)
	{
		mMainChar->setWalkBackward();
	}
	*/
	if (arg.key == OIS::KC_S)
	{
		mMainChar->setWalkBackward();
	}
	if (arg.key == OIS::KC_S) {
		//Press ¡§S¡¨ to zoom-in and press ¡§D¡¨ to zoom-out.
		secondCameraZoomMode = 1;
	}

	if (arg.key == OIS::KC_D)
	{
		secondCameraZoomMode = -1;
	}

	if (arg.key == OIS::KC_SPACE)
	{
		mMainChar->setFireAction_Normal();
	}

	return flg;
}

//handle the key released event
bool BasicTutorial_00::keyReleased( const OIS::KeyEvent &arg )
{
	bool flg = true;
	
	if (arg.key == OIS::KC_W) {
		//Create a main character ¡§robot¡¨ and control it to move around. The main character can be animated based on pre-defined animations. 
		mMainChar->unsetWalkForward();
	}
	

	if (arg.key == OIS::KC_S)
	{
		mMainChar->unsetWalkBackward();
	}

	/*
	if (arg.key == OIS::KC_S) {
		//Press ¡§S¡¨ to zoom-in and press ¡§D¡¨ to zoom-out.
		secondCameraZoomMode = 0;
	}

	if (arg.key == OIS::KC_D)
	{
		secondCameraZoomMode = 0;
	}
	*/
	if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}
	return flg;
}
void BasicTutorial_00::chooseSceneManager()
{
	mSceneMgrArr[0] = mRoot
		->createSceneManager(
	ST_EXTERIOR_CLOSE, "primary");
	mSceneMgrArr[1] =mSceneMgrArr[0];

}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(600,0,600));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("PlayerCam2");
	//mCamera->setPosition(Ogre::Vector3(600,0,600));
	mCamera->setPosition(Ogre::Vector3(0,2000,0));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller

}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera,
		1,
		0,
		0,
		1,
		1
		);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
    mCamera->setAspectRatio( 4.0 / 1.0 );
}

void BasicTutorial_00::createViewport_01(void)
{
	mCamera = mCameraArr[1];

	Ogre::Viewport* vp = mWindow->addViewport(
		mCamera,
		2,
		0.75,
		0,
		0.25,
		0.25
		);
	vp->setBackgroundColour(Ogre::ColourValue(1,1,1));
	//mCamera->setAspectRatio(1.0/4.0);
	mCamera->setAspectRatio( 4.0 / 1.0 );
	//Disable overlays.
	vp->setOverlaysEnabled(false);

}



void BasicTutorial_00::createScene_00(void) 
{
    /////////////////////////////////////
	DATA_READER::readData(); //Read the data file here.
	bt_Init(mTrayMgr, mSceneMgrArr[0], mCameraArr[0]); //Set the pointers of the objects so that the other functions can use the objects.
    /////////////////////////////////////

	mSceneMgr = mSceneMgrArr[0];
	
	ColourValue fadeColour(0.9, 0.9, 0.9); 

	Vector3 org = Vector3(750, 51, 750);

	//Setup an ambient light.
    mSceneMgr->setAmbientLight( ColourValue( 0.7, 0.7, 0.7 ) ); 

	initParticleSystemForExplosion();
	//initParticleSystemForExplosion();
	//Vector3 pos;
	//setOffParticleSystem(fNode, "explosion", pos);

	//Setup a spot light.
	Light *light;
	light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT);
	//light->setType(Light::LT_DIRECTIONAL);
	light->setPosition(Vector3(0, 450, 250)+org); 
	light->setDiffuseColour(0.7, 0.7, 0.7);		
	light->setSpecularColour(1.0, 1.0, 1.0);	
	light->setDirection(Vector3(1, 1, 1));
	mLight0 = light;

	//Enable exponential fog and shadow. The fog density and the status of shadow are stored in a data file.
	if (DATA_READER::isEnabledExpFog()){
		mSceneMgr->setFog(FOG_EXP, fadeColour, DATA_READER::getExpFogDensity());
	}
	if (DATA_READER::isEnabledShadow()){
		mSceneMgr->setShadowTechnique(	SHADOWTYPE_STENCIL_ADDITIVE	); 
		//SHADOWTYPE_TEXTURE_ADDITIVE);
		//SHADOWTYPE_STENCIL_MODULATIVE);
	}

	//Draw a height map and use it to create a terrain.
	mSceneMgr->setWorldGeometry("terrain.cfg"); 
	//Create a skybox.
	mSceneMgr->setSkyBox(true, "Examples/TrippySkyBox");
	//SpaceSkyBox

	Plane plane(Vector3::UNIT_Y,0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		6000,6000, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 
	Entity *ent = mSceneMgr->createEntity("GroundEntity", "ground"); 
	ent->setMaterialName(DATA_READER::getWaterMaterialName());
	std::cout << DATA_READER::getWaterMaterialName() <<std::endl;
	//ent->setMaterialName("Examples/Rockwall");
	SceneNode *planeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		planeNode->attachObject(ent); 
		planeNode->translate(3000,50,3000);


	//Create a big sphere and make it move up and down.
	String name_en;
	int index=0;
	genNameUsingIndex("sphere", index, name_en);
   	mEntity[index] = mSceneMgr->createEntity( name_en, "sphere.mesh" ); ;
	mEntity[index]->setMaterialName("Examples/SphereMappedRustySteel");
    	mSceneNode[index] = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		name_en, Vector3( 3000, 500, 3000 ) ); 	//center at (0,0,0)
	mSceneNode[index]->attachObject(mEntity[index]);
	mSceneNode[index]->scale(2,2,2);		//radius=200

	//Create Barrel
	index=100;
	genNameUsingIndex("Barrel", index, name_en);
   	mEntity[index] = mSceneMgr->createEntity( name_en, "Barrel.mesh" ); ;
	//mEntity[index]->setMaterialName("Examples/SphereMappedRustySteel");
    	mSceneNode[index] = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		name_en, Vector3( 1000, 100, 1000 ) ); 	//center at (0,0,0)
	mSceneNode[index]->attachObject(mEntity[index]);
	mSceneNode[index]->scale(10,10,10);		//radius=200

	//Create athene
	index=200;
	float radius =300;
	for(int i=0;i<12;i++){
		index=200+i;
		genNameUsingIndex("athene", index, name_en);
   	mEntity[index] = mSceneMgr->createEntity( name_en, "athene.mesh" ); ;
	mEntity[index]->setMaterialName("Examples/SphereMappedRustySteel");
    	mSceneNode[index] = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		name_en, Vector3( 3000 + radius*cos((float)i*2*PI/12) , 100, 3000 + radius*sin((float)i*2*PI/12)) ); 	//center at (0,0,0)
		//std::cout<<i/12*2*PI<< " "<< cos(i/12*2*PI)<<" "<<sin(i/12*2*PI)<<std::endl;
	mSceneNode[index]->attachObject(mEntity[index]);
	mSceneNode[index]->scale(1,1,1);		//radius=200
	mSceneNode[index]->yaw(Degree(i*360/12));

	}

	//Create tudorhouse
	index=300;
	genNameUsingIndex("tudorhouse", index, name_en);
   	mEntity[index] = mSceneMgr->createEntity( name_en, "tudorhouse.mesh" ); ;
	//mEntity[index]->setMaterialName("Examples/SphereMappedRustySteel");
    	mSceneNode[index] = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		name_en, Vector3( 4500, 300, 1500 ) );
	mSceneNode[index]->attachObject(mEntity[index]);
	mSceneNode[index]->scale(1,1,1);

	index=301;
	genNameUsingIndex("tudorhouse", index, name_en);
   	mEntity[index] = mSceneMgr->createEntity( name_en, "tudorhouse.mesh" ); ;
    	mSceneNode[index] = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		name_en, Vector3( 1300, 300, 4800 ) ); 	//center at (0,0,0)
	mSceneNode[index]->attachObject(mEntity[index]);
	mSceneNode[index]->scale(1,1,1);
	index=302;
	genNameUsingIndex("tudorhouse", index, name_en);
   	mEntity[index] = mSceneMgr->createEntity( name_en, "tudorhouse.mesh" ); ;
    	mSceneNode[index] = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		name_en, Vector3( 3900, 300, 4100 ) );
	mSceneNode[index]->attachObject(mEntity[index]);
	mSceneNode[index]->scale(1,1,1);
	index=303;
	genNameUsingIndex("tudorhouse", index, name_en);
   	mEntity[index] = mSceneMgr->createEntity( name_en, "tudorhouse.mesh" ); ;
    	mSceneNode[index] = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		name_en, Vector3( 5500, 300, 5100 ) );
	mSceneNode[index]->attachObject(mEntity[index]);
	mSceneNode[index]->scale(1,1,1);


	//Create a main character ¡§robot¡¨ and control it to move around. The main character can be animated based on pre-defined animations. 

	mMainChar = new MAIN_CHAR(mSceneMgrArr[0]);
	mMainChar->createGameObj("m", "ninja.mesh");
	mMainChar->scale(0.5,0.5,0.5);
	mMainChar->attachCamera(mCameraArr[0]);
	mMainChar->setMaxBulletsNum(DATA_READER::getMaxBulletsNum());
	mMainChar->setPosition(Vector3(2000,0,2000));
	//If a ball hits the big target, i.e., the sphere, it should bounce off.
	mMainChar->getWeaponManager()->setTarget(Vector3( 3000, 500, 3000 ),200);
	mMainChar->getWeaponManager()->setTargetBox(Vector3( 4500, 300, 1500 ),320,380);
	mMainChar->getWeaponManager()->setTargetBox(Vector3( 1300, 300, 4800 ),320,380);
	mMainChar->getWeaponManager()->setTargetBox(Vector3( 3900, 300, 4100 ),320,380);
	mMainChar->getWeaponManager()->setTargetBox(Vector3( 5500, 300, 5100 ),320,380);
	mMainChar->setTargetBox(Vector3( 4500, 300, 1500 ),320,380);
	mMainChar->setTargetBox(Vector3( 1300, 300, 4800 ),320,380);
	mMainChar->setTargetBox(Vector3( 3900, 300, 4100 ),320,380);
	mMainChar->setTargetBox(Vector3( 5500, 300, 5100 ),320,380);

	mMonsterMgr = new MONSTER_MANAGER(mSceneMgrArr[0]);
	mMonsterMgr->setTargetForMonsters(mMainChar);
	int MaxMonstersNum=DATA_READER::getMaxMonstersNum();
	mMonsterMgr->setMaxMonstersNum(MaxMonstersNum);

	
	//for(int i=0;i<mMonsterMgr->mCurMonstersNum;i++){
	//	initParticleSystemForExplosion(mMonsterMgr->mMonstersArr[i]->getSceneNode(),i);
		//mMonsterMgr->mMonstersArr[i]->getSceneNode()->getPosition();
		//break;
	//}

	mMainChar->unsetWalkForward();
	Ogre::FrameEvent evt;
	evt.timeSinceLastEvent=0.05;
	evt.timeSinceLastFrame=0.05;
	frameStarted(evt);
	mMainChar->unsetWalkForward();

	std::cout <<"teststart"<<std::endl;
	//new TextRenderer();
	//std::cout <<"test1"<<std::endl;

	//TextRenderer::getSingleton();
	//std::cout <<"test2"<<std::endl;
	//TextRenderer::getSingleton().addTextBox("txtGreeting", "Helloturtyutu", 30, 30, 70, 20, Ogre::ColourValue::Black);

	std::cout <<"test1"<<std::endl;
	msg[0] = new Ogre::MovableText("TXT_0", "Difficulty : 1");

	(msg[0])->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_ABOVE); // Center horizontally and display above the node
	msg[0]->showOnTop();
	/* msg->setAdditionalHeight( 2.0f ); //msg->setAdditionalHeight( ei.getRadius() ) // apparently not needed from 1.7*/
	MCmsgNode[0]=mMainChar->getSceneNode()->createChildSceneNode();
	MCmsgNode[0]->attachObject(msg[0]);
	MCmsgNode[0]->translate(Vector3(0,130,0));
	MCmsgNode[0]->scale(10,10,15);

	msg[1] = new Ogre::MovableText("TXT_1", "score : 0");
	(msg[1])->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_ABOVE); // Center horizontally and display above the node
	msg[1]->showOnTop();
	/* msg->setAdditionalHeight( 2.0f ); //msg->setAdditionalHeight( ei.getRadius() ) // apparently not needed from 1.7*/
	MCmsgNode[1]=mMainChar->getSceneNode()->createChildSceneNode();
	MCmsgNode[1]->attachObject(msg[1]);
	MCmsgNode[1]->translate(Vector3(0,120,0));
	MCmsgNode[1]->scale(10,10,15);
	
	ChangeDifficulty();

	//mSoundBGM->play();
	
}
void BasicTutorial_00::createFrameListener()
{
	BaseApplication::createFrameListener(); 
	 mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
	 //this code must be only here of in framerendering, error if in createscene
	 mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
	 mInfoLabel->show();
}

bool BasicTutorial_00::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	char str[30];

    sprintf(str, "HighScore : %d", highscore);
	 //mInfoLabel->setCaption(strcat("Score : ", str));
	//Ogre::DisplayString s ;
	/*
	wchar_t wchartemp[MAX_WCHAR];
	std::string text = s;
	mbstowcs(wchartemp,text.c_str(),MAX_WCHAR);
	*/
	 mInfoLabel->setCaption(str);

	//adjust text
    sprintf(str, "score : %d", score);
	std::string s(str);
	msg[1]->setCaption(s);

	sprintf(str, "Difficulty : %d", difficulty);
	msg[0]->setCaption(str);

	 return BaseApplication::frameRenderingQueued(evt);
}
void BasicTutorial_00::createScene_01(void) 
{
	return;
}

/*
Implement the game logic in this function.
*/
bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
	mToggle += evt.timeSinceLastFrame;
    //
	
	mMainChar->update(evt);
	mShutDown=false;
    mMonsterMgr->update(evt);
	score=score-mMonsterMgr->hitCharCountThisUpdate*50;
	if (score<0) score=0;
	
	WEAPON_MANAGER *w = mMainChar->getWeaponManager();
	for (int i = 0; i < w->mMaxNum; ++i) {
		if (w->mUsedWeaponsArr[i] == false) continue;
		WEAPON *g = w->mBulletsArr[i];

		for (int j = 0; j < mMonsterMgr->mCurMonstersNum; ++j) {
			if (!mMonsterMgr->mMonstersArr[j]->isAlive()) continue;
			bool hit = g->hitTarget_Sphere( mMonsterMgr->mMonstersArr[j]->getPosition(),  60 );
			if (hit){
				if (mMonsterMgr->mMonstersArr[j]->die== false){
					score=score+100;
					if (highscore<score) highscore=score;
				}else{
					score=score-1;
					if (score<0) score=0;
				}
				//mMonsterMgr->mMonstersArr[j]->setVisible(false);
				//mMonsterMgr->mMonstersArr[j]->makeAlive(false);
				mMonsterMgr->mMonstersArr[j]->die= true;
				
				g->makeAlive(false);
				g->setVisible(false);
			}
		}
	};

	for(int i=0;i<512;i++){
		mParticleNode[i]->setVisible(false);
	}
	for (int j = 0; j < mMonsterMgr->mCurMonstersNum; ++j) {
			if (!mMonsterMgr->mMonstersArr[j]->isAlive()) continue;
			if (mMonsterMgr->mMonstersArr[j]->die== true){
				char str[30];
				sprintf(str, "explision%d", j);
				setOffParticleSystem(mParticleNode[j],str,mMonsterMgr->mMonstersArr[j]->getPosition());
			}
	}
	/*
	for (int j = 0; j < mMonsterMgr->mCurMonstersNum; ++j) {
		Vector3 pos = mMonsterMgr->mMonstersArr[j]->getPosition();
		flg = projectScenePointOntoTerrain_PosDirection(pos);
		if (flg == false) {
			projectScenePointOntoTerrain_NegDirection(pos);
		}
		mMonsterMgr->mMonstersArr[j]->setPosition(pos);
	}*/

	/*
	for(int i=0;i<mMonsterMgr->mCurMonstersNum;i++){
		if (mMonsterMgr->mMonstersArr[i]->chase)
			setOffParticleSystem(fNode[i], "explosion", Vector3::ZERO);
		else{
			fNode[i]->setVisible(false);
		}
	}
	*/

	//The spot light should follow the main character. 
	//You can simply add an offset to the position of the main character for computing the light position.
	mLight0->setPosition( mMainChar->getPosition()+Vector3(0, 150, 0 ));

	if (secondCameraZoomMode != 0){
		secondCameraHeight = secondCameraHeight + secondCameraZoomMode*secondCameraZoomSpeed *evt.timeSinceLastEvent;
		if (secondCameraHeight< 200) secondCameraHeight =200;
	}
	Vector3 pos = mMainChar->getPosition();
	mCameraArr[1]->setPosition(pos.x +1000 ,pos.y + secondCameraHeight,pos.z +2000);
	mCameraArr[1]->lookAt(pos.x+1000,pos.y,pos.z+2000);
	//std::cout<< mMainChar->getPosition() <<"  "<< secondCameraHeight<<std::endl;

	
	ChangeDifficulty();

	return flg;
}

void BasicTutorial_00::createViewports(void)
{
	createViewport_00();
	createViewport_01();
	mCamera = mCameraArr[0];
}

void BasicTutorial_00::createCamera(void) {
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
	mCamera = mCameraArr[0];
}

void BasicTutorial_00::createScene( void ) {
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
}

void BasicTutorial_00::initParticleSystemForExplosion() {
	for(int i=0;i<512;i++){

	char str[30];
	sprintf(str, "explision%d", i);

	mParticleNode[i] = static_cast<SceneNode*>(
		mSceneMgr->getRootSceneNode()->createChild());
	ParticleSystem* p = mSceneMgr->createParticleSystem(
		str, "Examples/GreenyNimbus");

	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(false);

	mParticleNode[i]->attachObject(p);
	mParticleNode[i]->setPosition(Vector3::ZERO);
	mParticleNode[i]->setVisible(false);

	}
}

void BasicTutorial_00::setOffParticleSystem(
	Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos) 
{
	fNode->setPosition(pos);
	fNode->setVisible(true);
	MovableObject *s = fNode->getAttachedObject(pname);
	ParticleSystem* p =static_cast<ParticleSystem*>(s);
	if (p == NULL) return;
	p->setVisible(true);
	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(true);
}

void BasicTutorial_00::ChangeDifficulty(){
	if (difficulty != score/500 +1 ){
		difficulty =score/500 +1;
		mMonsterMgr->setMaxMonstersNum(mMonsterMgr->mCurMonstersNum + difficulty*2);
		mMonsterMgr->difficulty=difficulty;
	}
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}

