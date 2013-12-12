// STTriangleMesh.cpp
#include "STTriangleMesh.h"

#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <string.h>
#define PI 3.14159265

//
// Initialization
//
STTriangleMesh::STTriangleMesh()
{
}

STTriangleMesh::STTriangleMesh(const std::string& filename)
{
    Read(filename);
    Build();
}

//
// Delete and clean up an existing image.
//
STTriangleMesh::~STTriangleMesh()
{
    for(unsigned int i=0;i<mVertices.size();i++)delete mVertices[i];
    for(unsigned int i=0;i<mFaces.size();i++)delete mFaces[i];
}

//
// Read the triangle mesh from files.
//
bool STTriangleMesh::Read(const std::string& filename)
{
    // Determine the right routine based on the file's extension.
    // The format-specific subroutines are each implemented in
    // a different file.
    std::string ext = STGetExtension( filename );
    if (ext.compare("OBJ") == 0){
        std::ifstream in( filename.c_str(), std::ios::in );

        if( !in ){
            std::cout << "cannot open file" << filename << std::endl;
            return false;
        }

        for(unsigned int i=0;i<mVertices.size();i++)delete mVertices[i];
        mVertices.clear();
        for(unsigned int i=0;i<mFaces.size();i++)delete mFaces[i];
        mFaces.clear();

        //std::string comments;
        //std::string token;
        char comments[256];
        char token[128];
        float x,y,z;
        float nx,ny,nz;
        float u,v;
        int v1,v2,v3;
        int t1,t2,t3;
        int n1,n2,n3;

        std::vector<STVector3> vertices;
        std::vector<STVector3> normals;
        std::vector<STPoint2> tex_coords;

        while(in>>token){
            if(strcmp(token,"#")==0){
                in.getline(comments,256);
            }
            else if(strcmp(token,"v")==0){
                in>>x>>y>>z;
                mVertices.push_back(new STVertex(x,y,z));
            }
            else if(strcmp(token,"vt")==0){
                in>>u>>v;
                tex_coords.push_back(STPoint2(u,v));
            }
            else if(strcmp(token,"vn")==0){
                in>>nx>>ny>>nz;
                normals.push_back(STVector3(nx,ny,nz));
            }
            else if(strcmp(token,"f")==0){
                if(tex_coords.size()==0&&normals.size()==0){    ////f v1 v2 v3
                    in>>v1>>v2>>v3;
                    STFace* f=new STFace(mVertices[v1-1],mVertices[v2-1],mVertices[v3-1]);
                    mFaces.push_back(f);
                }
                else if(tex_coords.size()!=0&&normals.size()==0){     ////f v1/t1 v2/t2 v3/t3
                    char slash;
                    in>>v1>>slash>>t1>>v2>>slash>>t2>>v3>>slash>>t3;
                    STFace* f=new STFace(mVertices[v1-1],mVertices[v2-1],mVertices[v3-1]);
                    f->texPos[0]=tex_coords[t1-1];
                    f->texPos[1]=tex_coords[t2-1];
                    f->texPos[2]=tex_coords[t3-1];
                    mFaces.push_back(f);
                }
                else if(normals.size()!=0&&tex_coords.size()==0){  ////f v1//n1 v2//n2 v3//n3
                    char slash;
                    in>>v1>>slash>>slash>>n1>>v2>>slash>>slash>>n2>>v3>>slash>>slash>>n3;
                    STFace* f=new STFace(mVertices[v1-1],mVertices[v2-1],mVertices[v3-1]);
                    f->v[0]->normal=normals[n1-1];
                    f->v[1]->normal=normals[n2-1];
                    f->v[2]->normal=normals[n3-1];
                    mFaces.push_back(f);
                    //std::cout<<v1<<"//"<<n1<<" "<<v2<<"//"<<n2<<" "<<v3<<"//"<<n3<<std::endl;
                }
                else{   ////f v1/t1/n1 v2/t2/n2 v3/t3/n3
                    char slash;
                    in>>v1>>slash>>t1>>slash>>n1>>v2>>slash>>t2>>slash>>n2>>v3>>slash>>t3>>slash>>n3;
                    STFace* f=new STFace(mVertices[v1-1],mVertices[v2-1],mVertices[v3-1]);
                    f->texPos[0]=tex_coords[t1-1];
                    f->texPos[1]=tex_coords[t2-1];
                    f->texPos[2]=tex_coords[t3-1];
                    f->v[0]->normal=normals[n1-1];
                    f->v[1]->normal=normals[n2-1];
                    f->v[2]->normal=normals[n3-1];
                    mFaces.push_back(f);
                }
            }
        }
        return true;
    }
    else {
        fprintf(stderr,
            "STTriangleMesh::STTriangleMesh() - Unknown file type \"%s\".\n",
            filename.c_str());
        return false;
    }
}

//
// Build topology  and calculate normals for the triangle mesh.
//
bool STTriangleMesh::Build()
{
    BuildTopology();
    CalculateNormal();
    return true;
}


bool STTriangleMesh::BuildTopology()
{
    //Build topology
    std::map<std::pair<STVertex*,STVertex*>,STFace*> he2f;
    for(unsigned int i=0;i<mFaces.size();i++){
        for(unsigned int j=0;j<3;j++){
            mFaces[i]->v[j]->f=mFaces[i];
            he2f.insert(std::pair<std::pair<STVertex*,STVertex*>,STFace*>(std::pair<STVertex*,STVertex*>(mFaces[i]->v[j],mFaces[i]->v[(j+1)%3]),mFaces[i]));
        }
    }
    for(unsigned int i=0;i<mFaces.size();i++){
        for(unsigned int j=0;j<3;j++){
            std::map<std::pair<STVertex*,STVertex*>,STFace*>::iterator itr=he2f.find(std::pair<STVertex*,STVertex*>(mFaces[i]->v[(j+2)%3],mFaces[i]->v[(j+1)%3]));
            if(itr!=he2f.end())
                mFaces[i]->adjF[j]=itr->second;
            else
                mFaces[i]->adjF[j]=0;
        }
    }
    return true;
}

bool STTriangleMesh::CalculateNormal()
{
    //Calculate normals
    for(unsigned int i=0;i<mFaces.size();i++){
        STFace* face=mFaces[i];
        face->normal=STVector3::Cross(face->v[0]->pt-face->v[1]->pt,face->v[0]->pt-face->v[2]->pt);

        // Actually you can do something like this here 
        // when you try to calculate the normals for all the vertices
        // face.v[j]->normal+=face.normal;
    }
    for(unsigned int i=0;i<mVertices.size();i++){
        STVertex* vertex=mVertices[i];
        if(vertex->f==0){vertex->normal=STVector3(1,0,0);continue;}
        vertex->normal=vertex->f->normal;
        STFace* nextface=vertex->f;
        bool boundary=false;
        while((nextface=NextAdjFace(vertex,nextface))!=vertex->f){
            if(nextface==0){
                boundary=true;
                break;
            }
            vertex->normal+=nextface->normal;
        }
        if(boundary){
            nextface=vertex->f;
            while((nextface=NextAdjFaceReverse(vertex,nextface))!=0){
                vertex->normal+=nextface->normal;
            }
        }
    }
    for(unsigned int i=0;i<mFaces.size();i++){
        mFaces[i]->normal.Normalize();
    }
    for(unsigned int i=0;i<mVertices.size();i++){
        mVertices[i]->normal.Normalize();
    }
    return true;
}

bool STTriangleMesh::CalculateTextureCoordinatesViaSphericalProxy()
{
	for(unsigned int i=0;i<mFaces.size();i++){
		STFace* face=mFaces[i];		
		for(int v=0;v<3;v++){
			STPoint3 point=face->v[v]->pt;
			float r=sqrt(pow(point.x,2)+pow(point.y,2)+pow(point.z,2));
			float theta=(float)PI-acos(point.z/r);
			float phi=atan2(point.y,point.x)+(float)PI;
			face->texPos[v].x=phi/(float)2/(float)PI;
			face->texPos[v].y=theta/(float)PI;
		}
		if(face->texPos[1].x-face->texPos[0].x>.5) face->texPos[1].x-=1;
		else if(face->texPos[1].x-face->texPos[0].x<-.5) face->texPos[1].x+=1;

		if(face->texPos[2].x-face->texPos[0].x>.5) face->texPos[2].x-=1;
		else if(face->texPos[2].x-face->texPos[0].x<-.5) face->texPos[2].x+=1;
	}
	return true;
}

bool STTriangleMesh::CalculateTextureCoordinatesViaCylindricalProxy(float h_min,float h_max,float center_x,float center_y,int axis_direction)
{
	for(unsigned int i=0;i<mFaces.size();i++){
		STFace* face=mFaces[i];		
		for(int v=0;v<3;v++){
			STPoint3 point=face->v[v]->pt;
			float phi;
			switch(axis_direction){
			case 1:
				phi=atan2(point.z-center_y,point.y-center_x)+(float)PI;
				face->texPos[v].x=phi/(float)2/(float)PI;
				face->texPos[v].y=(point.x-h_min)/(h_max-h_min);
				break;
			case 2:
				phi=atan2(point.x-center_y,point.z-center_x)+(float)PI;
				face->texPos[v].x=phi/(float)2/(float)PI;
				face->texPos[v].y=(point.y-h_min)/(h_max-h_min);
				break;
			case 3:
				phi=atan2(point.y-center_y,point.x-center_x)+(float)PI;
				face->texPos[v].x=phi/(float)2/(float)PI;
				face->texPos[v].y=(point.z-h_min)/(h_max-h_min);
				break;
			default:
				break;
			}
		}
		if(face->texPos[1].x-face->texPos[0].x>.5) face->texPos[1].x-=1;
		else if(face->texPos[1].x-face->texPos[0].x<-.5) face->texPos[1].x+=1;

		if(face->texPos[2].x-face->texPos[0].x>.5) face->texPos[2].x-=1;
		else if(face->texPos[2].x-face->texPos[0].x<-.5) face->texPos[2].x+=1;
	}
	return true;
}

STFace* STTriangleMesh::NextAdjFace(STVertex *v, STFace *f)
{
    if( v == f->v[0] ) 
        return f->adjF[1];
    else if( v == f->v[1] ) 
        return f->adjF[2];
    else if( v == f->v[2] ) 
        return f->adjF[0];
    else 
        return 0;
}

STFace* STTriangleMesh::NextAdjFaceReverse(STVertex *v, STFace *f)
{
    if( v == f->v[0] ) 
        return f->adjF[2];
    else if( v == f->v[1] ) 
        return f->adjF[0];
    else if( v == f->v[2] ) 
        return f->adjF[1];
    else 
        return 0;
}

void STTriangleMesh::LoopSubdivide()
{
    unsigned int newVerticesStart=mVertices.size();

    // Add Odd Vertices
    std::map<STFace*,std::vector<STVertex *> > oldFaces2newVertices;
    std::vector<STVertex *> empty_vertices_3;for(unsigned int i=0;i<3;i++)empty_vertices_3.push_back(0);
    for(unsigned int i=0;i<mFaces.size();i++){
        oldFaces2newVertices.insert(std::pair<STFace*,std::vector<STVertex *> >(mFaces[i],empty_vertices_3) );
    }
    for(unsigned int i=0;i<mFaces.size();i++){
        for(unsigned int j=0;j<3;j++){
            if(oldFaces2newVertices[mFaces[i]][j]==0){
                if(mFaces[i]->adjF[j]!=0){
                    int adjF_j;
                    for(unsigned int k=0;k<3;k++){
                        if(mFaces[i]->adjF[j]->adjF[k]==mFaces[i]){
                            adjF_j=k;
                            break;
                        }
                    }
                    STPoint3 newPoint=(mFaces[i]->v[(j+1)%3]->pt+mFaces[i]->v[(j+2)%3]->pt)*0.375f
                        +(mFaces[i]->v[j]->pt+mFaces[i]->adjF[j]->v[adjF_j]->pt)*0.125f;
                    STPoint2 newTexPos=(mFaces[i]->v[(j+1)%3]->texPos+mFaces[i]->v[(j+2)%3]->texPos)*0.5f;
                    STVertex* newVertex=new STVertex(newPoint,newTexPos);
                    mVertices.push_back(newVertex);
                    oldFaces2newVertices[mFaces[i]][j]=oldFaces2newVertices[mFaces[i]->adjF[j]][adjF_j]=newVertex;
                }
                else{
                    STPoint3 newPoint=(mFaces[i]->v[(j+1)%3]->pt+mFaces[i]->v[(j+2)%3]->pt)*0.5f;
                    STPoint2 newTexPos=(mFaces[i]->v[(j+1)%3]->texPos+mFaces[i]->v[(j+2)%3]->texPos)*0.5f;
                    STVertex* newVertex=new STVertex(newPoint,newTexPos);
                    mVertices.push_back(newVertex);
                    oldFaces2newVertices[mFaces[i]][j]=newVertex;
                }
            }
        }
    }

    std::vector<STPoint3> newEvenVerticesPoints;newEvenVerticesPoints.resize(newVerticesStart);
    // Adjust Even Vertices
    for(unsigned int i=0;i<newVerticesStart;i++){
        STVertex* vertex=mVertices[i];
        STFace* nextface=vertex->f;
        std::vector<STPoint3> neighborPoints;
        bool boundary=false;
        do {
            if(nextface==0){
                boundary=true;
                break;
            }
            for(int j=0;j<3;j++){
                if(nextface->v[j]==vertex){
                    neighborPoints.push_back(nextface->v[(j+2)%3]->pt);
                    break;
                }
            }
        } while((nextface=NextAdjFace(vertex,nextface))!=vertex->f);

        if(boundary){
            STPoint3 temp=neighborPoints.back();
            neighborPoints.clear();
            neighborPoints.push_back(temp);
            nextface=vertex->f;
            do {
                if(nextface==0)
                    break;
                for(int j=0;j<3;j++){
                    if(nextface->v[j]==vertex){
                        temp=nextface->v[(j+1)%3]->pt;
                        break;
                    }
                }
            } while((nextface=NextAdjFaceReverse(vertex,nextface))!=vertex->f);
            neighborPoints.push_back(temp);
        }

        if(neighborPoints.size()>3){
            float weight=3.0f/8.0f/(float)neighborPoints.size();
            newEvenVerticesPoints[i]=vertex->pt*(5.0f/8.0f);
            for(unsigned j=0;j<neighborPoints.size();j++)
                newEvenVerticesPoints[i]=newEvenVerticesPoints[i]+neighborPoints[j]*weight;
        }
        else if(neighborPoints.size()==3){
            float weight=3.0f/16.0f;
            newEvenVerticesPoints[i]=vertex->pt*(7.0f/16.0f);
            for(unsigned j=0;j<neighborPoints.size();j++)
                newEvenVerticesPoints[i]=newEvenVerticesPoints[i]+neighborPoints[j]*weight;
        }
        else{ // assert(neighborPoints.size()==2) boundary vertex
            newEvenVerticesPoints[i]=vertex->pt*0.75f+neighborPoints[0]*0.125f+neighborPoints[1]*0.125f;
        }
    }
    for(unsigned int i=0;i<newVerticesStart;i++)
        mVertices[i]->pt=newEvenVerticesPoints[i];

    // Rebuild faces
    std::vector<STFace*> newFaces;
    for(unsigned int i=0;i<mFaces.size();i++){
        for(unsigned int j=0;j<3;j++){
            STFace* newFace=new STFace(mFaces[i]->v[j],oldFaces2newVertices[mFaces[i]][(j+2)%3],oldFaces2newVertices[mFaces[i]][(j+1)%3]);
            newFaces.push_back(newFace);
        }
        STFace* newFace=new STFace(oldFaces2newVertices[mFaces[i]][0],oldFaces2newVertices[mFaces[i]][1],oldFaces2newVertices[mFaces[i]][2]);
        newFaces.push_back(newFace);
    }
    std::swap(mFaces,newFaces);
    for(unsigned int i=0;i<newFaces.size();i++)delete newFaces[i];
    
    Build();
}

unsigned int STTriangleMesh::AddVertex(float x, float y, float z, float u, float v)
{
    mVertices.push_back(new STVertex(x,y,z,u,v));
    return mVertices.size()-1;
}

unsigned int STTriangleMesh::AddVertex(const STPoint3& pt, const STPoint2& texPos)
{
    mVertices.push_back(new STVertex(pt,texPos));
    return mVertices.size()-1;
}

unsigned int STTriangleMesh::AddFace(unsigned int id0,unsigned int id1,unsigned int id2)
{
    STFace* newFace = new STFace(mVertices[id0],mVertices[id1],mVertices[id2]);
    mFaces.push_back(newFace);
    return mFaces.size()-1;
}

std::ostream& operator <<(std::ostream& stream, const STVertex& v) {
    stream<<"["<<v.pt.x<<" "<<v.pt.y<<" "<<v.pt.z<<"]";
    return stream;
}

std::ostream& operator <<(std::ostream& stream, const STFace& f) {
    stream<<"{"<<*(f.v[0])<<" "<<*(f.v[1])<<" "<<*(f.v[2])<<"}";
    return stream;
}
