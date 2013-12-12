#include "ExampleScene.h"

void ExampleScene::initializeBitcoin()
{
	rtClear();

	////global settings
	rtCamera(/*eye*/STPoint3(10.f,5.f,19.f),/*up*/STVector3(0.f,1.f,0.f),/*lookat*/STPoint3(10.f,3.f,0.f),/*fov*/45.f,/*aspect*/1.33f);
	rtOutput(/*width*/640,/*height*/480,/*path*/"../Standard_Tests/Bitcoin.png");
	rtBounceDepth(1);
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
	Material mat_metal(/*ambient*/STColor3f(.6f,.4f,.3f),/*diffuse*/STColor3f(.6f,.4f,.3f),/*spec*/STColor3f(.2f,.2f,.2f),/*mirror*/STColor3f(.6f,.4f,.3f),/*shiness*/90.f);

//  rtMaterial(mat_glass1);
//  rtPushMatrix();
//  rtTranslate(10.f,5.f,13.f);
//	rtScale(.01f,.01f,.01f);
//	rtTriangleMesh("../Standard_Tests/custom/Paris/Paris2010_0.obj",true,false);
//	rtPopMatrix();
  
//  rtPushMatrix();
//  rtTranslate(10.f,5.f,13.f);
//	rtScale(.01f,.01f,.01f);
//	rtTriangleMesh("../Standard_Tests/custom/Paris/Paris2010_0.obj",true,false);
//	rtPopMatrix();
//  
  rtPushMatrix();
  rtMaterial(mat_glass2);
  rtTranslate(10.f,5.f,13.f);
	rtScale(.1f,.1f,.1f);
  rtRotate(.3f, 30.f, 10.f); // x, y, z rotations
	rtTriangleMesh("../Standard_Tests/custom/fighter.obj",true,false);
	rtPopMatrix();
  
//	rtMaterial(mat_glass1);
//	rtSphere(STPoint3(6.f,1.01f,5.f),1.f);
//
//	rtMaterial(mat_metal);
//	rtCylinder(STPoint3(9.f,0.01f,5.f),STPoint3(9.f,3.01f,5.f),1.f);
//	
//	rtMaterial(mat_glass2);
//	rtBox(STPoint3(12.f,2.01f,5.f),STVector3(2.f,4.f,.8f));

	////environment box
	Material mat_ground(STColor3f(1.f,1.f,1.f),STColor3f(.4f,.2f,.1f),STColor3f(),STColor3f(),0.f);
	Material mat_wall(STColor3f(0.f,5.f,23.f),STColor3f(.4f,.2f,.1f),STColor3f(),STColor3f(),30.f);
	rtMaterial(mat_ground);
	////ground
	addGround(STPoint3(0.f,0.f,0.f),STVector2(100.f,100.f),true);
	rtMaterial(mat_wall);
	////ceil
	addGround(STPoint3(0.f,0.f,0.f),STVector2(100.f,100.f),false);
	////background wall
	addBackgroundWall(STPoint3(0.f,0.f,0.f),STVector2(100.f,100.f),true);
	////forward wall
	addBackgroundWall(STPoint3(0.f,0.f,100.f),STVector2(100.f,100.f),false);
	////left wall
	addWall(STPoint3(0.f,0.f,0.f),STVector3(0.f,100.f,0.f),STVector3(0.f,0.f,100.f),true);
	////right wall
	addWall(STPoint3(100.f,0.f,0.f),STVector3(0.f,100.f,0.f),STVector3(0.f,0.f,100.f),false);
  
  ////use acceleration structure
  //////aabb tree
//  accel_structure=AABB_TREE;
//  AABBTree* aabb_tree=new AABBTree(objects);
//  aabb_trees.push_back(aabb_tree);
  
  //////uniform grid
  accel_structure=UNIFORM_GRID;
  AABB scene_bounding_box;getObjectsAABB(objects,scene_bounding_box);
  int subdivision[3]={20,20,2};
  uniform_grid=new UniformGrid(objects,scene_bounding_box,subdivision);
  
  ////set focus on sphere 3
//	rtSetApeture(20.f);
//	rtSetFocus(STPoint3(.2f, -.5f, 1.f));
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

	rtLoadTexture("../Standard_Tests/stanford.png");
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