//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

const float PI = 3.141592654;

BasicTutorial_00::BasicTutorial_00(void)
    : 
mFlg_Root(false), mFlg_Root_Dir(true)

, mFlg_Arm(false), mFlg_Arm_Dir(true)
      
{
	light_rad=0;
	for(int i=0;i<1024;i++){
		moving[i]=false;
	}


}

void BasicTutorial_00::chooseSceneManager()
{
	ResourceGroupManager
		::getSingleton()
		.initialiseAllResourceGroups();


	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	mSceneMgrArr[1] = mRoot
		->createSceneManager(ST_GENERIC, "secondary");
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
	mSceneMgr = mSceneMgrArr[0];	//用第一份場景
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("PlayerCam2");

	//Set the camera position as (0, 1400, 0). Let it look at point (0, 0, 0). Be careful about the bug!
	mCamera->setPosition(Ogre::Vector3(0,1400,-200));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[0] = vp;

}

void BasicTutorial_00::createViewport_01(void)
{
	mCamera = mCameraArr[1];

	//The viewport occupies the upper right corner.
	Ogre::Viewport* vp = mWindow->addViewport(mCamera,1,0.75,0,0.25,0.25);

	//Create another camera and another viewport. The background color of the viewport is YELLOW.
	vp->setBackgroundColour(Ogre::ColourValue(255,255,0));

	//Set the viewport aspect ratio as (4*W/H), where W is the width of the viewport and H is the height of the viewport.
	mCamera->setAspectRatio(
		Ogre::Real(4 * vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[1] = vp;

	//Disable overlays.
	vp->setOverlaysEnabled(false); 

	//Disable sky.
	mViewportArr[1]->setSkiesEnabled(false);
}


void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];

	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  

	//enable linear white fog with parameters (0, linearStart=1400, linearEnd= 1600). Set the fog color to white.
	ColourValue fadeColour(1, 1, 1); 
	mSceneMgr->setFog(FOG_LINEAR, fadeColour, 0.0, 1400, 1600); 
	//mSceneMgr->setWorldGeometry("terrain.cfg"); 
	//mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8, 1000); 

	//Create a skybox.
	mSceneMgr->setSkyBox(
		true, 
		"Examples/SpaceSkyBox"); 

	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	//Create a plane, set a material to it. Make sure that the plane does not cover the entire viewport. The background of the viewport can be seen.
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	ent->setMaterialName("Examples/BeachStones");
	//ent->setMaterialName("Examples/Rockwall");

	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 

	//wall
    /*ent = mSceneMgr->createEntity(
		"Background", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	ent->setMaterialName("Examples/Rockwall");

    //Radian angle((45/180.0)*3.141592654);
    Radian angle(3.141952654/2.0);

    Vector3 axis(1.0, 0.0, 0.0);
    mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
            Vector3(0.0, 0.0, -750.0),
            Quaternion( angle, axis))
		->attachObject(ent);
		*/


	//Create two circles of robots in a space of dimension 600x600.
	String name_en;
	for(int radius=600,turn=0; turn<=1;turn++,radius=radius/2){
	for(int index=0+turn*26;index<=(25+turn*26);index++){
		double rad=(index-turn*26)/(double)26*2*PI;
		
		genNameUsingIndex("robot", index, name_en);
		mEntity[index] = mSceneMgr
			->createEntity( name_en, "robot.mesh" ); ;
		mSceneNode[index] = mSceneMgr
			->getRootSceneNode()
			->createChildSceneNode( 
			name_en, Vector3( radius* cos(rad), 50, radius*sin(rad) )); 

		mSceneNode[index]->attachObject(mEntity[index]);	//mSceneNode[0]
		if (index==0) mSceneNode[index]->scale(2,2,2);
	};
	};
	mCurrentObject = mSceneNode[0]; //choose box用

	//Create a sphere with radius 70.0 and set it the center.
	//String name_en;
	int index=52;
	genNameUsingIndex("sphere", index, name_en);
    mEntity[index] = mSceneMgr
		->createEntity( name_en, "sphere.mesh" ); ;
    mSceneNode[index] = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		name_en, Vector3( 0, 0, 0 ) ); 

    mSceneNode[index]->attachObject(mEntity[index]);	//mSceneNode[0]
	mSceneNode[index]->scale(0.7,0.7,0.7);

	

	Light *light;
	light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(200, 250, 0)); 
	light->setDiffuseColour(0.0, 1.0, 0.0);		
	light->setSpecularColour(0.0, 1.0, 0.0);	
	/*
	light = mSceneMgr->createLight("Light2"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(-150, 300, 250)); 
	light->setDiffuseColour(0.5, 0.5, 0.5);		
	light->setSpecularColour(0.5, 0.5, 0.5);	
	*/
	
	//A rectangle region is defined by dragging the mouse while the LEFT MOUSE KEY is pressed. It must work for the following five cases: 1) Lower left to upper right; 2) upper left to lower right; 3) lower right to upper left; 4) upper right to lower left; and 5) click and release at the same position. 
	// mouse press or release
	PlaneBoundedVolumeList volList;
	mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(volList);

	mSelectionRect = new SelectionRectangle("selectionRect");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionRect);
	mSelectionRect->setLightMask(0);
	mSelectionRect->setCastShadows(false);
	mSelectionRect->setVisible(false);
	

	//Create another camera and another viewport. The background color of the viewport is YELLOW.
	//initialize particle before using it
    initParticleSystemForExplosion(); 


	setOffParticleSystem(mParticleNode, "explosion", mParticleNode->getPosition());
}

void BasicTutorial_00::createScene_01(void) 
{
}

void BasicTutorial_00::createViewports(void)
{
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
    //
    mCameraMan->getCamera()
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

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
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

    }

    if (arg.key == OIS::KC_2 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(-824.52,	468.58,	68.45));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.94,	-0.31,	-0.11));

        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
    }


    if (arg.key == OIS::KC_3 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(19.94,	822.63,	30.79));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.00,	-0.99,	-0.11));
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }
    /////////////////////////////
    //add your own stuff
    /////////////////////////////
	
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

void BasicTutorial_00::dealcollision(){
	
	//Collision handling must be performed among the small robots.
		const int robotRadius=20;
		const int ballRadius=70;
		int collisiondist=robotRadius+robotRadius;
	for(int turn=1;turn<=10;turn++){
	for(int index=0;index<=51;index++){
		for(int j=index+1;j<=51;j++){
			Vector3 d = mSceneNode[j]->getPosition()-mSceneNode[index]->getPosition();
			if (d.length()<collisiondist){
				if (d.length()>0) d.normalise();
				else d.x++;
				mSceneNode[j]->setPosition(mSceneNode[j]->getPosition()+d*(collisiondist-d.length()));

			}
		}
	};
	}

	//While the selected robot(s) move, they must avoid collision with the sphere.
	collisiondist=robotRadius+ballRadius;
	for(int turn=1;turn<=10;turn++){
	for(int j=0;j<=51;j++){
		Vector3 plainVector= Vector3( mSceneNode[j]->getPosition().x,0,mSceneNode[j]->getPosition().z);
		Vector3 d =plainVector-mSceneNode[52]->getPosition();
		if (d.length()<collisiondist){
			if (d.length()>0) d.normalise();
			else d.x++;
			mSceneNode[j]->setPosition(mSceneNode[j]->getPosition()+d*(collisiondist-d.length()));

		}
		
	}
	}
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);

	
    /////////////////////////////
    //add your own stuff
    /////////////////////////////
    mSceneMgr = mSceneMgrArr[0];
	SceneNode *node;

	Light *light;
	light = mSceneMgr->getLight("Light1"); 
	light_rad=light_rad+evt.timeSinceLastFrame*2*PI/8;
	if (light_rad>2*PI) light_rad-=2*PI;
	light->setPosition(Vector3(200*cos(light_rad), 250, 200*sin(light_rad))); 


	for(int index=0;index<=51;index++){
			Vector3 targetpoint = Vector3(destinationX,50,destinationZ);

			if (moving[index]){
				Vector3 d=mSceneNode[index]->getPosition()-targetpoint;
				Real dist=d.length();
				if (d.length()>0) d.normalise();
				Vector3 moves= - d*70*evt.timeSinceLastFrame;
				if (moves.length()>=dist){	//到達
					mSceneNode[index]->setPosition(targetpoint);
					mSceneNode[index]->lookAt(Vector3(mCamera->getPosition().x,mSceneNode[index]->getPosition().y,mCamera->getPosition().z), Node::TS_WORLD, Vector3::UNIT_X);
					moving[index]=false;
					mCurrentObject->showBoundingBox(false);


					mAnimationState[index]->setEnabled(false);
					continue;
				}

				mSceneNode[index]->setPosition(mSceneNode[index]->getPosition()+moves);
				mSceneNode[index]->lookAt(Vector3(targetpoint.x,mSceneNode[index]->getPosition().y,targetpoint.z), Node::TS_WORLD, Vector3::UNIT_X);
				

				Real motion_speed = 0.5*2;
				Real speedFactor = 20;
				Vector3 velocity(1, 0, 0);
				mAnimationState[index]->addTime(
				1*motion_speed*evt.timeSinceLastFrame); // time step 
				mSceneNode[index]->translate(
					  1*motion_speed*speedFactor*velocity
					  *evt.timeSinceLastFrame
				);
				
			}
		
	}
	dealcollision();

    return flg;
}
bool BasicTutorial_00::mouseMoved( const OIS::MouseEvent &arg )
{
		Ray mRay =mTrayMgr->getCursorRay(mCamera);

		Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
		if (scn.x<beginx){
			left = scn.x;right=beginx;
		}else{
			left = beginx;right=scn.x;
		}
		if (scn.y<beginy){
			top = scn.y;bottom=beginy;
		}else{
			top = beginy;bottom=scn.y;
		}

		mSelectionRect->setCorners(left, top, right, bottom);

	
	return BaseApplication::mouseMoved( arg);
	
}

bool BasicTutorial_00::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (id==OIS::MB_Right){
		for(int index=0;index<=51;index++){
			mCurrentObject=mSceneNode[index];
			if (mCurrentObject){
				bool flgShow =  mCurrentObject->getShowBoundingBox();
				if (!flgShow && moving[index]){	//之前在走的，卻已未顯示BoundingBox，停止走
					moving[index]=false;
					mAnimationState[index]->setEnabled(false);
				}

				if (flgShow){
					moving[index]=true;

					mAnimationState[index] = mEntity[index]->getAnimationState("Walk");
					mAnimationState[index]->setLoop(true);
					mAnimationState[index]->setEnabled(true);

					// get window height and width
   Ogre::Real screenWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
   Ogre::Real screenHeight = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
 
   // convert to 0-1 offset
   Ogre::Real offsetX = arg.state.X.abs / screenWidth;
   Ogre::Real offsetY = arg.state.Y.abs / screenHeight;
 
   // set up the ray
   Ray mouseRay = mCamera->getCameraToViewportRay(offsetX, offsetY);
 
   // check if the ray intersects our plane
   // intersects() will return whether it intersects or not (the bool value) and
   // what distance (the Real value) along the ray the intersection is
   Plane mPlane(Vector3::UNIT_Y, 0);
   std::pair<bool, Real> result = mouseRay.intersects(mPlane);
 
    if(result.first) {
           // if the ray intersect the plane, we have a distance value
           // telling us how far from the ray origin the intersection occurred.
           // the last thing we need is the point of the intersection.
           // Ray provides us getPoint() function which returns a point
           // along the ray, supplying it with a distance value.
 
           // get the point where the intersection is
       Vector3 point = mouseRay.getPoint(result.second);
 
           // position our ninja to that point  
       //mNinjaNode->setPosition(point);
					 destinationX=point.x;
					 destinationZ=point.z;
    }

					
				}
			}
		}

		
		mSelectionRect->setVisible(false);
		return BaseApplication::mouseReleased( arg, id );
	}


	
	if (id==OIS::MB_Left){
		Ray mRay =mTrayMgr->getCursorRay(mCamera);
		Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
		for(int index=0;index<=51;index++){
			mCurrentObject=mSceneNode[index];
			if (mCurrentObject){
				mCurrentObject->showBoundingBox(false);
			}else{
				
				std::cout<<id<<" a"<<std::endl;
			}
			
		}
		if (beginx==scn.x && beginy==scn.y){
			mSelectionRect->setVisible(false);
			return BaseApplication::mouseReleased( arg, id );
		}
	}
	

/*
	if (left==right && top==bottom) {
		return BaseApplication::mouseReleased( arg, id );
		
	}
	*/
	/* CEGUI
	Ray topLeft = mCamera->getCameraToViewportRay(left, top);
	Ray topRight = mCamera->getCameraToViewportRay(right, top);
	Ray bottomLeft = mCamera->getCameraToViewportRay(left, bottom);
	Ray bottomRight = mCamera->getCameraToViewportRay(right, bottom);
	*/

	//USING mTrayMgr=============
	Real nleft = left;
	Real nright = right;
	Real ntop = 1+top;
	Real nbottom = 1+bottom;
	//Left = Xmin
	//Right= Xmax
	//Bottom=Ymin
	//Top = Ymax
	std::cout << "nleft:" << nleft << std::endl;
	std::cout << "nright:" << nright << std::endl;
	std::cout << "nbottom:" << nbottom << std::endl;
	std::cout << "ntop:" << ntop << std::endl;
	Ray topLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, ntop));
	Ray topRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, ntop));
	Ray bottomLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, nbottom));
	Ray bottomRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, nbottom));
	//End Using mTrayMgr=============

	// The plane faces the counter clockwise position.
	PlaneBoundedVolume vol;
	int np = 100;
	vol.planes.push_back(Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3)));         // front plane
	vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(np), topRight.getPoint(np)));         // top plane
	vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(np), 	topLeft.getPoint(np)));       // left plane
	vol.planes.push_back(Plane(bottomLeft.getOrigin(), bottomRight.getPoint(np), 	bottomLeft.getPoint(np)));   // bottom plane
	vol.planes.push_back(Plane(bottomRight.getOrigin(), topRight.getPoint(np), 	bottomRight.getPoint(np)));     // right plane 

	PlaneBoundedVolumeList volList;
	volList.push_back(vol);
	mVolQuery->setVolumes(volList);

	SceneQueryResult result = mVolQuery->execute();

	SceneQueryResultMovableList::iterator itr = result.movables.begin();

	// Get the results, set the camera height
	// We are interested in the first intersection. It is ok to traverse all the results.
	
	//Ray mRay =mTrayMgr->getCursorRay(mCamera);
	//Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
	//if (beginx!=scn.x && beginy!=scn.y){
	for (itr = result.movables.begin(); itr != result.movables.end(); ++itr)
	{
		if (*itr)
		{
			mCurrentObject = (*itr)->getParentSceneNode();
			bool flgShow = mCurrentObject->getShowBoundingBox();
			//mCurrentObject->showBoundingBox(!flgShow);
			mCurrentObject->showBoundingBox(true);
		}
	}
	
	//}
	mSceneNode[52]->showBoundingBox(false);	//the ball


	mSelectionRect->setVisible(false);
	return BaseApplication::mouseReleased( arg, id );

}
bool BasicTutorial_00::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	
	Ray mRay =mTrayMgr->getCursorRay(mCamera);

	Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
	left = scn.x;  // (left, top) = (scn.x, scn.y)
	top = scn.y;

	right = scn.x;
	bottom = scn.y;

	beginx= scn.x;
	beginy= scn.y;

	mSelectionRect->setCorners(left, top, right, bottom);
	mSelectionRect->setVisible(true);

	return BaseApplication::mousePressed( arg, id );
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}


void BasicTutorial_00::initParticleSystemForExplosion() {
	mParticleNode = static_cast<SceneNode*>(
		mSceneNode[0]->createChild());

	ParticleSystem* p = mSceneMgr->createParticleSystem(
		"explosion", "Examples/GreenyNimbus");

	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(false);
	mParticleNode->attachObject(p);
	mParticleNode->setPosition(Vector3::ZERO);
	mParticleNode->setVisible(false);
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