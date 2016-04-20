#ifndef smr_h
#define smr_h

#include <stdio.h>
#include <vector>

using namespace std;


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
};

class CIntegrateur {
public:
    CSMR* smr;
    
    void step(){
        
        //Force de gravité
        for(int i =0 ; i < smr->particules.size(); i++) {
            smr->particules[i]->force = CVect3D(0,-10,0);
        }
        //Force des ressorts
        for(int i =0 ; i < smr->ressorts.size(); i++) {
            //cout << i << " F : " << smr->ressorts[i]->P1 << endl;
            //smr->ressorts[i]->P0->force += smr->ressorts[i]->F();
            //smr->ressorts[i]->P1->force += smr->ressorts[i]->F2();
        }
        //Force du vent
        
        
        
        //Updater position et velocite des particules en fonction de la force calculée
        for(int i =0 ; i < smr->particules.size(); i++) {
            
        }
    }
};


#endif /* smr_h */

