//
//  Components.hpp
//  8trd147-d2
//
//  Created by Etudiant on 2016-03-25.
//
//

#ifndef Components_hpp
#define Components_hpp

#include <stdio.h>
#include <math.h>
#include "mesh.h"
#include "smr.h"
#include <iostream>

using namespace std;

#endif /* Components_hpp */


/* GB
 * rectangle shape formed with two triangles
 */
class Rectangle : public CMesh {
public:
    Rectangle(int width, int height) {
        
        //create vertices and add them to vertices vector, with u,v coords
        vertices.push_back(new CVertex(0, CPoint3D(0, 0, 0),          0.0, 0.0));
        vertices.push_back(new CVertex(1, CPoint3D(0, 0, height),     0.0, 1.0));
        vertices.push_back(new CVertex(2, CPoint3D(width, 0, height), 1.0, 1.0));
        vertices.push_back(new CVertex(3, CPoint3D(width, 0, 0),      1.0, 0.0));
        
        //create corresponding triangles
        CTriangle* tri1(new CTriangle(vertices[0], vertices[1], vertices[2]));
        CTriangle* tri2(new CTriangle(vertices[0], vertices[2], vertices[3]));
        triangles.push_back(tri1);
        triangles.push_back(tri2);
        
        //adjacent triangles
        vertices[0]->triangles.push_back(tri1);
        vertices[0]->triangles.push_back(tri2);
        vertices[1]->triangles.push_back(tri1);
        vertices[2]->triangles.push_back(tri1);
        vertices[2]->triangles.push_back(tri2);
        vertices[3]->triangles.push_back(tri2);
    }
};

/**
 * GB
 * class to draw a line between two points
 */
class Line : public CMesh {
public:
    Line(float p1, float p2) {
        vertices.push_back(new CVertex(0, CPoint3D(p1, 6, 0),     0.0, 0.0));
        vertices.push_back(new CVertex(1, CPoint3D(p2, 6, 0),     0.0, 1.0));
    }
};

/**
 * GB
 * class for post objects
 */
#define PI 3.14159265
class Cylinder : public CMesh {
public:
    Cylinder(float height, float radius, int slices) {
        float theta = (2 * PI ) / slices;
        int u, v;
        
        //define vertices for top circle
        vertices.push_back(new CVertex(0, CPoint3D(0, height, 0), 0.5, 0.5)); //center of top circle
        for (int i = 0; i < slices; i++) {
            u = (cos(theta*i) + 1) / 2; //to transform range (-1..1) of cos(theta*i) into range (0..1)
            v = (sin(theta*i) + 1) / 2;
            vertices.push_back(new CVertex(i+1, CPoint3D(radius*cos(theta*i), height, radius*sin(theta*i)), u, v));
        }
        
        //define vertices for bottom circle
        vertices.push_back(new CVertex(slices + 1, CPoint3D(0, 0, 0), 0.5, 0.5)); //center of bottom circle
        for (int i = slices; i < slices*2; i++) {
            u = (cos(theta*i) + 1) / 2;
            v = (sin(theta*i) + 1) / 2;
            vertices.push_back(new CVertex(i+2, CPoint3D(radius*cos(theta*i), 0, radius*sin(theta*i)), u, v));
        }
        
        //define triangles for top circle
        CTriangle* tri;
        for (int i = 1; i <= slices; i++) {
            CVertex * v1 = vertices[0];
            CVertex * v2 = vertices[i+1];
            CVertex * v3 = vertices[i];
            if (i == slices)
                v2 = vertices[1];
            
            tri = new CTriangle(v1, v2, v3);
            triangles.push_back(tri);
            
            //adjacent triangles
            vertices[0]->triangles.push_back(tri);
            vertices[i]->triangles.push_back(tri);
            vertices[i+1]->triangles.push_back(tri);
        }
        
        //define triangles for bottom circle
        for (int i = slices+1; i <= slices*2; i++) {
            CVertex * v1 = vertices[slices+1];
            CVertex * v2;
            if (i < slices * 2)
                v2 = vertices[i+2];
            else if (i == slices*2)
                v2 = vertices[slices+2];
            CVertex * v3 = vertices[i+1];
            
            tri = new CTriangle(v1, v2, v3);
            triangles.push_back(tri);
            
            //adjacent triangles
            vertices[slices]->triangles.push_back(tri);
            vertices[i+1]->triangles.push_back(tri);
            if (i < slices*2)
                vertices[i+2]->triangles.push_back(tri);
            else if (i == slices*2)
                vertices[slices+2]->triangles.push_back(tri);
        }
        
        //define triangles for sides
        for (int i = 0; i < slices; i++) { //2 triangles per side
            CTriangle* tri1;
            if (i < slices-1)
                tri1 = new CTriangle(vertices[i+1], vertices[i+2], vertices[i + slices + 2]);
            else if (i == slices-1)
                tri1 = new CTriangle(vertices[i+1], vertices[1], vertices[i + slices + 2]);
                
            triangles.push_back(tri1);
            
            //adjacent triangles
            vertices[i+1]->triangles.push_back(tri1);
            vertices[i+2]->triangles.push_back(tri1);
            vertices[i+slices+1]->triangles.push_back(tri1);
        
            CTriangle* tri2;
            if (i < slices-1)
                tri2 = new CTriangle(vertices[i+2], vertices[i + slices + 3], vertices[i + slices + 2]);
            else if (i == slices-1)
                tri2 = new CTriangle(vertices[1], vertices[slices + 2], vertices[i + slices + 2]);
            triangles.push_back(tri2);
            
            //adjacent triangles
            vertices[i+2]->triangles.push_back(tri2);
            vertices[i+slices+2]->triangles.push_back(tri2);
            vertices[i+slices+1]->triangles.push_back(tri2);
        }
        

    }
};


/**
 * ABD
 * class to draw the sheet that will de distorted with "wind"
 *
 * GB
 * Created Particules
 *
 * AL
 * Created Springs
 */
class Drap : public CMesh {
    CIntegrateur* integrateur;
    
public:
    Drap(CIntegrateur* _integrateur) {
        
        integrateur = _integrateur;
        CSMR* smr = new CSMR();
        integrateur->smr = smr;
        
        float masse = 1000;
        int drapWidth = 40;
        int drapHeight = 30;
        
        
        //Create Vertex and particules
        int index = 0;
        for(float i = 0; i < drapHeight; i++) {
            for(float j = 0; j < drapWidth; j++) {
                CVertex* v = new CVertex(index, CPoint3D(-0 + (j/10), 0 - (i/10), 0), (j / drapWidth), ((i / drapHeight)));
                vertices.push_back(v);
                smr->particules.push_back(new CParticule(v, masse));
                index++;
                
            }
        }
        
        //Create Springs

        for(float i = 0; i < drapHeight; i++) {
            for(float j = 0; j < drapWidth; j++) {
                int particuleIndex = (i*drapWidth) + (j);
                //Lenght 1
                if(i < (drapHeight-1)) { //Vertical
                    smr->ressorts.push_back(new CRessort(smr->particules[particuleIndex],smr->particules[particuleIndex + (drapWidth)]));
                }
                if(j < (drapWidth-1)) { //Horizontal
                    smr->ressorts.push_back(new CRessort(smr->particules[particuleIndex],smr->particules[particuleIndex + 1]));
                }

                //Lenght 2
                if(i < (drapHeight-2)) { //Vertical
                    smr->ressorts.push_back(new CRessort(smr->particules[particuleIndex],smr->particules[particuleIndex + (drapWidth * 2)]));

                }
                if(j < (drapWidth-2)) { //Horizontal
                    smr->ressorts.push_back(new CRessort(smr->particules[particuleIndex],smr->particules[particuleIndex + 2]));
                }
                
                if(i < (drapHeight-1) && j < (drapWidth-1) ) {//Diagonal
                    smr->ressorts.push_back(new CRessort(smr->particules[particuleIndex],smr->particules[particuleIndex + 1 + (drapWidth)]));
                }
                

            }
        }

        //Used To Debug all particules in all ressorts
        /*
        for(int i =0 ; i < smr->ressorts.size(); i++) {
            cout << i << " P0 : " << smr->ressorts[i]->P0 << endl;
            cout << i << " P1 : " << smr->ressorts[i]->P1 << endl;
        }
         */
         
        
        //Create triangle
        for(int i = 0; i < (drapHeight-1); i++) {
            for(int j = 0; j < (drapWidth-1); j++) {
                
                if(i % 2 == 0) {
                    if(j % 2 == 0) {
                        
                        CTriangle* tri1(new CTriangle(vertices[(i * drapWidth) + (j + 1)], vertices[(i * drapWidth) + j], vertices[((i + 1) * drapWidth) + (j + 1)]));
                        CTriangle* tri2(new CTriangle(vertices[((i + 1) * drapWidth) + (j + 1)], vertices[(i * drapWidth) + j], vertices[((i + 1) * drapWidth) + j]));
                        
                        triangles.push_back(tri1);
                        triangles.push_back(tri2);
                        
                        vertices[(i * drapWidth) + (j + 1)]->triangles.push_back(tri1);
                        vertices[(i * drapWidth) + j]->triangles.push_back(tri1);
                        vertices[((i + 1) * drapWidth) + (j + 1)]->triangles.push_back(tri1);
                        
                        vertices[((i + 1) * drapWidth) + (j + 1)]->triangles.push_back(tri2);
                        vertices[(i * drapWidth) + j]->triangles.push_back(tri2);
                        vertices[((i + 1) * drapWidth) + j]->triangles.push_back(tri2);
                        
                    }
                    else {
                        
                        CTriangle* tri1(new CTriangle(vertices[(i * drapWidth) + (j + 1)], vertices[(i * drapWidth) + j], vertices[((i + 1) * drapWidth) + j]));
                        CTriangle* tri2(new CTriangle(vertices[((i + 1) * drapWidth) + (j + 1)], vertices[(i * drapWidth) + (j + 1)], vertices[((i + 1) * drapWidth) + j]));
                        
                        triangles.push_back(tri1);
                        triangles.push_back(tri2);
                        
                        vertices[(i * drapWidth) + (j + 1)]->triangles.push_back(tri1);
                        vertices[(i * drapWidth) + j]->triangles.push_back(tri1);
                        vertices[((i + 1) * drapWidth) + j]->triangles.push_back(tri1);
                        
                        vertices[((i + 1) * drapWidth) + (j + 1)]->triangles.push_back(tri2);
                        vertices[(i * drapWidth) + (j + 1)]->triangles.push_back(tri2);
                        vertices[((i + 1) * drapWidth) + j]->triangles.push_back(tri2);
                    }
                }
                else {
                    if(j % 2 == 0) {
                        
                        CTriangle* tri1(new CTriangle(vertices[(i * drapWidth) + (j + 1)], vertices[(i * drapWidth) + j], vertices[((i + 1) * drapWidth) + j]));
                        CTriangle* tri2(new CTriangle(vertices[((i + 1) * drapWidth) + (j + 1)], vertices[(i * drapWidth) + (j + 1)], vertices[((i + 1) * drapWidth) + j]));
                        
                        triangles.push_back(tri1);
                        triangles.push_back(tri2);
                        
                        vertices[(i * drapWidth) + (j + 1)]->triangles.push_back(tri1);
                        vertices[(i * drapWidth) + j]->triangles.push_back(tri1);
                        vertices[((i + 1) * drapWidth) + j]->triangles.push_back(tri1);
                        
                        vertices[((i + 1) * drapWidth) + (j + 1)]->triangles.push_back(tri2);
                        vertices[(i * drapWidth) + (j + 1)]->triangles.push_back(tri2);
                        vertices[((i + 1) * drapWidth) + j]->triangles.push_back(tri2);
                        
                    }
                    else {
                        
                        CTriangle* tri1(new CTriangle(vertices[(i * drapWidth) + (j + 1)], vertices[(i * drapWidth) + j], vertices[((i + 1) * drapWidth) + (j + 1)]));
                        CTriangle* tri2(new CTriangle(vertices[((i + 1) * drapWidth) + (j + 1)], vertices[(i * drapWidth) + j], vertices[((i + 1) * drapWidth) + j]));
                        
                        triangles.push_back(tri1);
                        triangles.push_back(tri2);
                        
                        vertices[(i * drapWidth) + (j + 1)]->triangles.push_back(tri1);
                        vertices[(i * drapWidth) + j]->triangles.push_back(tri1);
                        vertices[((i + 1) * drapWidth) + (j + 1)]->triangles.push_back(tri1);
                        
                        vertices[((i + 1) * drapWidth) + (j + 1)]->triangles.push_back(tri2);
                        vertices[(i * drapWidth) + j]->triangles.push_back(tri2);
                        vertices[((i + 1) * drapWidth) + j]->triangles.push_back(tri2);
                    }
                }
            }
        }
    }
    
    CIntegrateur* getIntegrateur() {return integrateur;}
};
