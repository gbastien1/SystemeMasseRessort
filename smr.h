#ifndef smr_h
#define smr_h

#include <stdio.h>
#include <vector>
#include "mesh.h"

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

