////////////////////////////////////////
//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
////////////////////////////////////////
// Student Name:¼B©Ó¶¶
// Student ID:0656124
// Student Email:ciaskbe@gmail.com
//
////////////////////////////////////////
// You can delete everything and start from scratch.
////////////////////////////////////////

#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

const float PI = 3.141592654;

BasicTutorial_00::BasicTutorial_00(void) {
	animation_state=false;
	acceleration= Vector3(0.0, -50.8, 0.0);
	velocity= Vector3(0.0, 0, 0.0);
}

void BasicTutorial_00::chooseSceneManager()
{
	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	mSceneMgrArr[1] = mRoot
		->createSceneManager(ST_GENERIC, "secondary");
    //
    // add your own stuff
    //
}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(120,300,600));
	mCamera->lookAt(Ogre::Vector3(120,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	// add your own stuff
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("PlayerCam2");
	mCamera->setPosition(Ogre::Vector3(0,350,0.0));
	mCamera->lookAt(Ogre::Vector3 (0.0001, 0,0.0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
	
}



void BasicTutorial_00::createViewport_00(void)
{
	mWindow->removeAllViewports();		////changed *****

	mCamera = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0.0,1.0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[0] = vp;
}

void BasicTutorial_00::createViewport_01(void)
{
	//mWindow->removeAllViewports();		////changed
    // add your own stuff
	mCamera = mCameraArr[1];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera,1,0.75, 0.0, 0.25, 0.25);		////changed
	vp->setBackgroundColour(Ogre::ColourValue(0.7,0,0));		////changed
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[1] = vp;
}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];


	// Ambient light and shadow. 
    mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) );
 mSceneMgr->setShadowTechnique(
SHADOWTYPE_STENCIL_ADDITIVE);// add your own stuff

 //Create a plane
 Plane plane(Vector3::UNIT_Y, 0);
MeshManager::getSingleton().createPlane(
"ground",
ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
 plane,
 1500,1500, // width, height
 20,20, // x- and y-segments
 true, // normal
 1, // num texture sets
 5,5, // x- and y-tiles
 Vector3::UNIT_Z // upward vector
 );

Entity *ent = mSceneMgr->createEntity(
				"GroundEntity", "ground"); 
mSceneMgr
	->getRootSceneNode()
	->createChildSceneNode()
	->attachObject(ent); 
ent->setCastShadows(false);

// two penguins
Entity *ent1 = mSceneMgr->createEntity( 
				"penguin1", "penguin.mesh" ); 
SceneNode *node1 = mSceneMgr
			->getRootSceneNode()
			->createChildSceneNode( "penguin1Node" ); 
node1->attachObject( ent1 ); 
Entity *ent2 = mSceneMgr->createEntity( 
				"penguin2", "penguin.mesh" ); 
SceneNode *node2 = mSceneMgr
			->getRootSceneNode()
			->createChildSceneNode( "penguin2Node" ); 
node2->attachObject( ent2 ); 

node1->scale( 2, 3, 2 );
node1->setPosition(0, 50, 0);
node2->setPosition(150, 20, 0);
node2->yaw( Degree( -90 ) ); 

//One set of objects form a circle around a large penguin
int numCubes = 72;
double L = 255; 
for (int i = 0; i < numCubes; ++i) {
 String name;
 genNameUsingIndex("c", i, name);
Entity *ent = mSceneMgr->createEntity(name, "cube.mesh");
ent->setMaterialName("Examples/SphereMappedRustySteel");
 AxisAlignedBox bb = ent->getBoundingBox();
 double cubeSize = bb.getMaximum().x - bb.getMinimum().x;
 double x = 0, y = 0, z = -125;
 SceneNode *snode = mSceneMgr
 ->getRootSceneNode()
 ->createChildSceneNode();
 snode->attachObject(ent);
 double fx = i/(double) (numCubes-1); // in range [0,1]
 double h = (1+sin(fx*PI*4))*50; // height
 double circle_radius = 100;
 double x1 = circle_radius*cos(fx*PI*2);
 double z1 = circle_radius*sin(fx*PI*2);
 double unitF = 1.0/cubeSize/numCubes*L*0.8;
snode->scale(unitF, h/cubeSize, unitF);
 snode->setPosition(x1, 50, z1);
} 

// Another set of objects form a row. 
//int numCubes = 72;
//double L = 255; 
for (int i = 0; i < numCubes; ++i) {
 String name;
 genNameUsingIndex("d", i, name);
Entity *ent = mSceneMgr->createEntity(name, "cube.mesh");
ent->setMaterialName("Examples/Chrome");
 AxisAlignedBox bb = ent->getBoundingBox();
 double cubeSize = bb.getMaximum().x - bb.getMinimum().x;
 double x = 0, y = 0, z = 125;	/////adjusted
 SceneNode *snode = mSceneMgr
 ->getRootSceneNode()
 ->createChildSceneNode();
 snode->attachObject(ent);
 double fx = 2*i/(double) (numCubes-1); //i from 0 to numCubes-1 
 x = fx*L - L/2.0;
 double h = (1+cos(fx*3.1415*2.0))*20; // height
 double unitF = 1.0/cubeSize/numCubes*L*0.8;
 snode->scale(unitF, h/cubeSize, unitF);
 snode->setPosition(x, 20, z);
} 


// Create two lights. 
Light *light = mSceneMgr->createLight("Light1");
 light->setType(Light::LT_POINT);
 light->setPosition(Vector3(150, 250, 100));
 light->setDiffuseColour(0.0, 1.0, 0.0);
 light->setSpecularColour(0.0, 1.0, 0.0);
 light = mSceneMgr->createLight("Light2");
 light->setType(Light::LT_POINT);
 light->setPosition(Vector3(-150, 300, 250));
 light->setDiffuseColour(0.5, 0.5, 0.5);
 light->setSpecularColour(0.5, 0.5, 0.5);


    /*
    Radian angle(3.141952654/2.0);

    Vector3 axis(1.0, 0.0, 0.0);
    mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
            Vector3(0.0, 0.0, -750.0),
            Quaternion( angle, axis))
		->attachObject(ent);
    */
}

void BasicTutorial_00::createScene_01(void) 
{
    // add your own stuff
	mSceneMgr = mSceneMgrArr[1];

 //Create a plane
 Plane plane(Vector3::UNIT_Y, 0);
MeshManager::getSingleton().createPlane(
"ground",
ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
 plane,
 1500,1500, // width, height
 20,20, // x- and y-segments
 true, // normal
 1, // num texture sets
 5,5, // x- and y-tiles
 Vector3::UNIT_Z // upward vector
 );

Entity *ent = mSceneMgr->createEntity(
				"GroundEntity", "ground"); 
mSceneMgr
	->getRootSceneNode()
	->createChildSceneNode()
	->attachObject(ent); 
ent->setCastShadows(false);


// Ambient light and shadow. 
    mSceneMgr->setAmbientLight( ColourValue( 0.0,0.0, 0.0 ) );
 mSceneMgr->setShadowTechnique(
SHADOWTYPE_STENCIL_ADDITIVE);


 //One Light
 Light *light = mSceneMgr->createLight("Light3");
 light->setType(Light::LT_POINT);
light->setPosition(Vector3(100, 150, 250));
light->setDiffuseColour(0.0, 0.0, 1.0); //blue
light->setSpecularColour(0.0,0.0, 1.0); //blue


/*Create an object, such as a sphere or a cube. Scale it with different scaling
factors in x-, y- and z-directions. Must place it above the plane. Set it¡¦s
x-coordinate to 50. Its shadow must be seen. */
 ent = mSceneMgr->createEntity("obj", "cube.mesh");
 ent->setMaterialName("Examples/green");
//ent->setMaterialName("Examples/green");
 SceneNode *snode = mSceneMgr
 ->getRootSceneNode()
 ->createChildSceneNode();
 snode->attachObject(ent);
snode->scale(0.3,0.2,0.1);
 snode->setPosition(50,20, -50);	//y=50
 ent->setCastShadows(true);

	Ogre::Viewport* vp = mViewportArr[1];
	vp->setOverlaysEnabled(false);	//Disable overlays. 

}

void BasicTutorial_00::createViewports(void)
{
    //Do not modify
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
    //Do not modify
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
    //Do not modify
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
//
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyPressed( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyPressed ***\n";
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    
    if (arg.key == OIS::KC_C ) {
        
        //How to clear ss?
        ss.str("");
        ss.clear();
        
        //stringstream ss; // Second way

        // Third way?
        //=============

        // How to get camerea position?
        //-----------------------------
        //This is incorrect.
        //Vector3 pos = mCamera->getPosition();
        //-----------------------------
        Vector3 pos = mCameraMan->getCamera()->getPosition(); //Correct
        ss << std::fixed << std::setprecision(2) 
            << "CameraPosition:" 
            << pos.x << "\t" 
            << pos.y << "\t" 
            << pos.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
        ss.str("");
        ss.clear();
        Vector3 dir = mCameraMan->getCamera()->getDirection();
        ss << std::fixed << std::setprecision(2) 
            << "CameraDirection:" 
            << dir.x << "\t" 
            << dir.y << "\t" 
            << dir.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
    }

    if (arg.key == OIS::KC_1 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(98.14,	450.69,	964.20));
        mCameraMan->getCamera()
            ->setDirection(Vector3(-0.01,	-0.30,	-0.95));

        //98.14	450.69	964.20
        //-0.01	-0.30	-0.95
    }

    if (arg.key == OIS::KC_2 ) {
        // add your own stuff
		mCameraMan->getCamera()
            ->setPosition(Vector3(-1463.00,606.45,-513.24));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.88,-0.47,0.10));
        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
    }

    if (arg.key == OIS::KC_3 ) {
        // add your own stuff
		mCameraMan->getCamera()
            ->setPosition(Vector3(-1356.16,634.32,-964.51));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.71,-0.44,0.55));
        //-1356.16	634.32	-964.51
        //0.71	-0.44	0.55
    }

    if (arg.key == OIS::KC_4 ) {
         // add your own stuff
		mCameraMan->getCamera()
            ->setPosition(Vector3(40.39,155.23,251.20));
        mCameraMan->getCamera()
            ->setDirection(Vector3(-0.02,-0.41,-0.91));
        //40.39	155.23	251.20
        //-0.02	-0.41	-0.91
    }

    if (arg.key == OIS::KC_5 ) {
        // add your own stuff
		mCameraMan->getCamera()
            ->setPosition(Vector3(19.94,822.63,30.79));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.00,-0.99,-0.11));
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }

    if (arg.key == OIS::KC_M ) {
        
       Camera *c_ptr1 = mCameraArr[0];
       mWindow->removeViewport(mViewportArr[0]->getZOrder());

	   Camera *c_ptr2 = mCameraArr[1];
	mWindow->removeViewport(mViewportArr[1]->getZOrder());

	Ogre::Viewport* vp = mWindow->addViewport(
        c_ptr1,
        2,
        0,
        0,
        0.5,
        0.3
        );
	vp->setBackgroundColour(Ogre::ColourValue(0,0.5,0.0));
	c_ptr1->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
   //delete mViewportArr[0];    // program crashes
    mViewportArr[0] = vp;       // make sure to save the new pointer
    
	
	
	vp = mWindow->addViewport(
        c_ptr2
        );
	vp->setBackgroundColour(Ogre::ColourValue(0,0.5,0.0));
	c_ptr2->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
   //delete mViewportArr[1];    // program crashes
    mViewportArr[1] = vp;       // make sure to save the new pointer
	vp->setOverlaysEnabled(false); 
	
    }

    if (arg.key == OIS::KC_N ) {
        // add your own stuff
		Camera *c_ptr1 = mCameraArr[0];
       mWindow->removeViewport(mViewportArr[0]->getZOrder());

	   Camera *c_ptr2 = mCameraArr[1];
	mWindow->removeViewport(mViewportArr[1]->getZOrder());

	Ogre::Viewport* vp = mWindow->addViewport(
        c_ptr1,
		1,
		0,
		0,
		1,
		1
		);
	vp->setBackgroundColour(Ogre::ColourValue(0,0.5,0.0));
	c_ptr1->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
   //delete mViewportArr[0];    // program crashes
    mViewportArr[0] = vp;       // make sure to save the new pointer
    
	
	
	vp = mWindow->addViewport(
        c_ptr2,
		3,
		0.5,
		0.25,
		0.5,
		0.25
        );
	vp->setBackgroundColour(Ogre::ColourValue(0,0.5,0.0));
	c_ptr2->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
   //delete mViewportArr[1];    // program crashes
    mViewportArr[1] = vp;       // make sure to save the new pointer
	vp->setOverlaysEnabled(false); 

    }

	if (arg.key == OIS::KC_0 ) {	//toggle the animation of the large penguin at the circle
        animation_state=!animation_state;

    }

    // Do not delete this line
    BaseApplication::keyPressed(arg);

    return flg;
}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
// 
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyReleased( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyReleased ***\n";
    
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    BaseApplication::keyReleased(arg);

    return flg;
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    //
    // add your own stuff
	mSceneMgr = mSceneMgrArr[0];

	if (animation_state){
		velocity+=acceleration*evt.timeSinceLastFrame;
		if (velocity.y>80){
			velocity.y=80;
		}
		if (velocity.y<-80){
			velocity.y=-80;
		}
		
		SceneNode *node1 = mSceneMgr->getSceneNode( "penguin1Node" );
		
		Vector3 position=node1->getPosition()+velocity*evt.timeSinceLastFrame;
		if (position.y<0){
			position.y=0;
			acceleration.y=20.8;
			velocity.y=0;
		}
		if (position.y>280){
			position.y=280;
			acceleration.y=-50.8;
			velocity.y=0;
		}
		
		node1->setPosition(position);
		
		
	}
    //
    return flg;
}
int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
