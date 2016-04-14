#ifndef smr_h
#define smr_h
#endif /* smr_h */

#include <stdio.h>
#include "mesh.h"

using namespace std;

class CSMR {
    CMesh* mesh; //pour affichage. MAJ à chaque itération
    list<CParticule* particueles;
    list<CRessort*> ressorts;
};

class CParticule {
    CVertex* vertex; //sommet du maillage associé à la particule
    CPoint3D pos[2];
    CVect3D vel[2];
    float masse;
};

class CRessort {
    CParticule *P0, *P1;
    float longueur_repos;
    float k; //constante de Hooke (rigidité)
    
    CVect3D F() const; //calcule la force du ressort
};

class CIntegrateur {
    CSMR* smr;
    
    void step();
}