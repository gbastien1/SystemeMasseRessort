#ifndef smr_h
#define smr_h

#include <stdio.h>
#include <vector>

using namespace std;

#define PI 3.14159



/*CVect3D fVent(CPoint3D pos, float t) {
 float amplitude = 0.08;
 float frequence = 0.5;
 float vitesse = 4.0;
 float x = pos[0];
 float y = pos[1];
 
 return CVect3D(0, 0, exp(0.5/y) * amplitude * sin((2*PI*frequence*y + 1) + (vitesse*t)));
 }*/


class CParticule {
public:
    CParticule(CVertex* _vertex, float _masse) {
        vertex = _vertex;
        pos[0] = vertex[0];
        pos[1] = CPoint3D(0,0,0);
        vel[0] = CVect3D(0,0,0);
        vel[1] = CVect3D(0,0,0);
        masse = _masse;
        force = CVect3D(0,0,0);
    }
    
    CVertex* vertex; //sommet du maillage associé à la particule
    CPoint3D pos[2];
    CVect3D vel[2];
    float masse;
    CVect3D force;
};

class CRessort {
public:
    CRessort(CParticule* _p0, CParticule* _p1, float _long = 0.1, float _k = 100) {
        P0 = _p0;
        P1 = _p1;
        longueur_repos = _long;
        k = _k;
    }
    
    
    CParticule *P0, *P1;
    float longueur_repos;
    float k; //constante de Hooke (rigidité)
    
    //calcule la force du ressort
    CVect3D F() const {
        return CVect3D(0,0,0);
    }
    //calcule la force du ressort
    CVect3D F2() const {
        return CVect3D(0,0,0);
    }
};

class CSMR {
public:
    CMesh* mesh; //pour affichage. MAJ à chaque itération
    vector<CParticule*> particules;
    vector<CRessort*> ressorts;
    
    CVect3D fVent(CPoint3D pos) {
        float amplitude = 0.08;
        float frequence = 0.5;
        //float vitesse = 4.0;
        //float x = pos[0];
        float y = pos[1];
        
        return CVect3D(0, 0, exp(0.5/y) * amplitude * sin((2*PI*frequence*y + 1)));
    }
};

class CIntegrateur {
public:
    CSMR* smr;
    
    //TODO debug adding forces
    void step(){
        
        //Force de gravité
        for(int i =0 ; i < smr->particules.size(); i++) {
            
            smr->particules[i]->force = CVect3D(0,-0.001,0);
        }
        //Force des ressorts
        for(int i =0 ; i < smr->ressorts.size(); i++) {
            //cout << i << " F : " << smr->ressorts[i]->P1 << endl;
            //smr->ressorts[i]->P0->force += smr->ressorts[i]->F();
            //smr->ressorts[i]->P1->force += smr->ressorts[i]->F2();
        }
        //Force du vent
        for(int i =0 ; i < smr->particules.size(); i++) {
            //smr->particules[i]->force += smr->fVent(smr->particules[i]->pos[0]);//, t);
        }

        
        //Updater position et velocite des particules en fonction de la force calculée
        for(int i =0 ; i < smr->particules.size(); i++) {
            CParticule *p = smr->particules[i];
            //F = ma   -> a = F/M
            CVect3D a = p->force / p->masse;
            //cout << "force: (" << p->force[0] << ", " << p->force[1] << ")" << endl;
            p->vel[0] = p->vel[1];
            p->pos[0] = p->pos[1];
            //cout << "pos: 1(" << p->pos[1][0] << ", " << p->pos[1][1] << ")";
            
            p->vel[1] += a;
            p->pos[1] += p->vel[1];
            
            *p->vertex += p->pos[1];
            //cout << "pos 2: (" << p->pos[1][0] << ", " << p->pos[1][1] << ")";
        }
    }
};


#endif /* smr_h */

