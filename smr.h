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
    }
    
    CVertex* vertex; //sommet du maillage associé à la particule
    CPoint3D pos[2];
    CVect3D vel[2];
    float masse;
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
    
    CVect3D F() const; //calcule la force du ressort
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
    
    void step();
};


#endif /* smr_h */

