#ifndef EXAMPLE_SCENE_H
#define EXAMPLE_SCENE_H

#include "Scene.h"

class ExampleScene : public Scene
{
public:
    using Scene::Render;
    using Scene::initializeSceneFromScript;
    
    ExampleScene(){}
    ~ExampleScene(){}

    ////initializing scenes
    ////example scenes assignment 4: basic geometry and shading
    void initializeSceneBasicGeometry();
    void initializeSceneBasicLightingAndShading();
    void initializeSceneTransform();

    ////example scenes for assignment 5: acceleration structures
    void initializeSceneAccelerationStructureGrid();   ////uniform distributed
    void initializeSceneAccelerationStructureBVH();   ////clustered objects

    ////example assignment 6: transparent objects and camera effects
    void initializeBitcoin();
	void initializeSceneTransparentObject2();
    void initializeSceneDepthOfField();

	////example scenes for final
	void initializeSceneObjMesh();
	void initializeSceneTexture();
	
	////void commonly used functions
	void addGround(const STPoint3& min_corner,const STVector2& size,bool counterclockwise=true);
	void addBackgroundWall(const STPoint3& min_corner,const STVector2& size,bool counterclockwise=true);
	void addWall(const STPoint3& min_corner,const STVector3& u,const STVector3& v,bool counterclockwise=true);
};

#endif

