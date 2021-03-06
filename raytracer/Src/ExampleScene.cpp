#include "ExampleScene.h"

void ExampleScene::initializeBitcoin()
{
	rtClear();

	////global settings
	rtCamera(/*eye*/STPoint3(10.f,5.f,19.f),/*up*/STVector3(0.f,1.f,0.f),/*lookat*/STPoint3(10.f,3.f,0.f),/*fov*/45.f,/*aspect*/1.33f);
	rtOutput(/*width*/1280,/*height*/960,/*path*/"../Standard_Tests/Bitcoin.png");
	rtBounceDepth(3);
	rtUseTransparentShadow(true);
	rtShadowBias(.001f);
	rtSampleRate(1);

	////lighting
	rtAmbientLight(STColor3f(.1f,.1f,.1f));
	rtPointLight(STPoint3(15.f,10.f,15.f),STColor3f(.6f,.6f,.6f));
	rtPointLight(STPoint3(5.f,10.f,15.f),STColor3f(.6f,.6f,.6f));
	//rtPointLight(STPoint3(10.f,15.f,4.f),STColor3f(.2f,.2f,.2f));

	Material mat_glass1(/*ambient*/STColor3f(),/*diffuse*/STColor3f(),/*spec*/STColor3f(0.f,0.f,0.f),/*mirror*/STColor3f(0.f,0.f,0.f),/*shiness*/0.f,/*refr*/STColor3f(.9f,.3f,.1f),/*sn*/1.3f);
	Material mat_glass2(/*ambient*/STColor3f(),/*diffuse*/STColor3f(),/*spec*/STColor3f(1.f,1.f,1.f),/*mirror*/STColor3f(.1f,.1f,.1f),/*shiness*/30.f,/*refr*/STColor3f(.7f,.6f,.9f),/*sn*/1.3f);
	Material mat_metal(/*ambient*/STColor3f(.8f,.6f,.3f),/*diffuse*/STColor3f(.6f,.4f,.3f),/*spec*/STColor3f(.2f,.2f,.2f),/*mirror*/STColor3f(.6f,.4f,.3f),/*shiness*/90.f);
  Material mat_metal_yellow(/*ambient*/STColor3f(.8f,.6f,.3f),/*diffuse*/STColor3f(.6f,.4f,.3f),/*spec*/STColor3f(.2f,.2f,.2f),/*mirror*/STColor3f(.6f,.4f,.3f),/*shiness*/90.f);

//  rtMaterial(mat_glass1);
//  rtPushMatrix();
//  rtTranslate(10.f,5.f,13.f);
//	rtScale(.01f,.01f,.01f);
//	rtTriangleMesh("../Standard_Tests/custom/Paris/Paris2010_0.obj",true,false);
//	rtPopMatrix();
    
    Material mat_coin(/*ambient*/STColor3f(1.f,1.f,0.f),/*diffuse*/STColor3f(1.f,1.f,0.f),/*specular*/STColor3f(0.f,0.f,0.f),/*mirror*/STColor3f(6.f,4.f,2.f),/*shiness*/30.f,/*refr*/STColor3f(.7f,.6f,.9f));
    
    for(float i = 5; i <= 19.5; i += 1){
      rtPushMatrix();
      rtTranslate(15.f/log(i)-0.5,6.f/log(i)+0.2,14.f/log(i));
      rtScale(.2f,.2f,.2f);
      rtRotate(1.f * rand(), 500.f/rand(), 1.f*rand());
      rtMaterial(mat_coin);
      rtTriangleMesh("../Standard_Tests/custom/coin.obj",true,false);
      rtPopMatrix();
      
      rtPushMatrix();
      rtTranslate(15.f/log(i)-1,6.f/log(i)-0.4,14.f/log(i));
      rtScale(.2f,.2f,.2f);
      rtRotate(1.f * rand(), 500.f/rand(), 1.f*rand());
      rtMaterial(mat_coin);
      rtTriangleMesh("../Standard_Tests/custom/coin.obj",true,false);
      rtPopMatrix();
    }
  
    // Plane closest to the camera
    rtPushMatrix();
      rtMaterial(mat_metal);
      rtTranslate(10.4f,5.f,15.f);
      rtScale(.1f,.1f,.1f);
      rtRotate(.5f, 30.f, 10.f); // x, y, z rotations
      rtTriangleMesh("../Standard_Tests/custom/fighter.obj",true,false);
    rtPopMatrix();
  
    // Plane on the top right
    rtPushMatrix();
      rtMaterial(mat_metal);
      rtTranslate(12.f,6.f,13.f);
      rtScale(.05f,.05f,.05f);
      rtRotate(.5f, 31.f, 10.f); // x, y, z rotations
      rtTriangleMesh("../Standard_Tests/custom/fighter.obj",true,false);
    rtPopMatrix();
  
    // Plane on the bottom right
    rtPushMatrix();
      rtMaterial(mat_metal);
      rtTranslate(11.f,3.2f,13.f);
      rtScale(.05f,.05f,.05f);
      rtRotate(.5f, 30.f, 10.f); // x, y, z rotations
      rtTriangleMesh("../Standard_Tests/custom/fighter.obj",true,false);
    rtPopMatrix();

	////environment box
	Material mat_ground(STColor3f(1.f,1.f,1.f),STColor3f(.4f,.2f,.1f),STColor3f(),STColor3f(),0.f);
	Material mat_wall(STColor3f(0.f,5.f,23.f),STColor3f(.4f,.2f,.1f),STColor3f(),STColor3f(),30.f);
	rtMaterial(mat_ground);

  // Back wall
  rtLoadTexture("../Standard_Tests/custom/cityscape.jpg"); // Open texture and set it to id = 1
  rtBindTexture(0); // Attempt to bind the texture to the next object

  // Trying something else to make textures work on walls
  STPoint3 min_corner = STPoint3(-5.f,-7.f,0.f);
  STVector2 size = STVector2(30.f,20.f);
  
  // bottom left, bottom right, top right
  rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z), STPoint2(0.f, 0.f), STPoint2(1.f, 0.f), STPoint2(1.f, 1.f));
  
  // bottom left, top right, top left
  rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z),STPoint3(min_corner.x,min_corner.y+size.y,min_corner.z), STPoint2(0.f, 0.f), STPoint2(1.f, 1.f), STPoint2(0.f, 1.f));

  rtUnbindTexture();
  
  
  ////use acceleration structure
  //////aabb tree
  accel_structure=AABB_TREE;
  AABBTree* aabb_tree=new AABBTree(objects);
  aabb_trees.push_back(aabb_tree);
  
  //////uniform grid
//  accel_structure=UNIFORM_GRID;
//  AABB scene_bounding_box;getObjectsAABB(objects,scene_bounding_box);
//  int subdivision[3]={20,20,2};
//  uniform_grid=new UniformGrid(objects,scene_bounding_box,subdivision);
  
  ////set focus on sphere 3
	rtSetApeture(1.5f);
	rtSetFocus(STPoint3(9.4f,10.f,13.7f));
}


void ExampleScene::initializeSceneTexture()
{
	rtClear();

	////global settings
	rtCamera(/*eye*/STPoint3(0.f,0.f,12.f),/*up*/STVector3(0.f,1.f,0.f),/*lookat*/STPoint3(0.f,0.f,0.f),/*fov*/45.f,/*aspect*/1.f);
	rtOutput(/*width*/512,/*height*/512,/*path*/"../Standard_Tests/Texture.png");
	rtBounceDepth(1);
	rtUseShadow(true);
	rtShadowBias(1e-4f);

	rtLoadTexture("../Standard_Tests/custom/sunset.jpg");
	////lighting
	rtAmbientLight(STColor3f(.1f,.1f,.1f));
	rtPointLight(/*location*/STPoint3(-3.f,-3.f,6.f),STColor3f(.8f,.8f,.8f));

	////objects
	////sphere
	Material mat_sphere(/*ambient*/STColor3f(1.f,1.f,1.f),/*diffuse*/STColor3f(1.f,1.f,0.f),/*specular*/STColor3f(0.f,0.f,0.f),/*mirror*/STColor3f(0.f,0.f,0.f),/*shiness*/0.f);
	rtMaterial(mat_sphere);
	rtSphere(STPoint3(1.f,1.f,0.f),.5f);

	////triangle
	Material mat_tri(/*ambient*/STColor3f(1.f,1.f,1.f),/*diffuse*/STColor3f(1.f,1.f,1.f),/*specular*/STColor3f(.2f,.2f,.2f),/*mirror*/STColor3f(0.f,0.f,0.f),/*shiness*/40.f);
	rtMaterial(mat_tri);
	rtBindTexture(0);
	rtTriangle(/*v1*/STPoint3(-4.f,-4.f,0.f),/*v2*/STPoint3(4.f,-4.f,0.f),/*v3*/STPoint3(4.f,4.f,0.f),
		/*uv1*/STPoint2(0.f,0.f),/*uv2*/STPoint2(1.f,0.f),/*uv3*/STPoint2(1.f,1.f));
	rtTriangle(/*v1*/STPoint3(-4.f,-4.f,0.f),/*v2*/STPoint3(4.f,4.f,0.f),/*v3*/STPoint3(-4.f,4.f,0.f),
		/*uv1*/STPoint2(0.f,0.f),/*uv2*/STPoint2(1.f,1.f),/*uv3*/STPoint2(0.f,1.f));
	rtUnbindTexture();
}

void ExampleScene::addGround(const STPoint3& min_corner,const STVector2& size,bool counterclockwise/*=true*/)
{
	if(counterclockwise){
		rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z));
		rtTriangle(min_corner,STPoint3(min_corner.x,min_corner.y,min_corner.z+size.y),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y));
	}
	else{
		rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y));
		rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y),STPoint3(min_corner.x,min_corner.y,min_corner.z+size.y));
	}
}

void ExampleScene::addBackgroundWall(const STPoint3& min_corner,const STVector2& size,bool counterclockwise/*=true*/)
{
	if(counterclockwise){
		rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z));
		rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z),STPoint3(min_corner.x,min_corner.y+size.y,min_corner.z));
	}
	else{
		rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z));
		rtTriangle(min_corner,STPoint3(min_corner.x,min_corner.y+size.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z));
	}
}

void ExampleScene::addWall(const STPoint3& min_corner,const STVector3& u,const STVector3& v,bool counterclockwise/*=true*/)
{
	if(counterclockwise){
		rtTriangle(min_corner,min_corner+u,min_corner+u+v);
		rtTriangle(min_corner,min_corner+u+v,min_corner+v);
	}
	else{
		rtTriangle(min_corner,min_corner+u+v,min_corner+u);
		rtTriangle(min_corner,min_corner+v,min_corner+u+v);
	}
}