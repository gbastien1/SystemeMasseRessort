#ifndef smr_h
#define smr_h

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <math.h>

using namespace std;

#define PI 3.14159


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
    
    vector<CRessort*> ressortsAdjacents;
    CVertex* vertex; //sommet du maillage associé à la particule
    CPoint3D pos[2];
    CVect3D vel[2];
    float masse;
    CVect3D force;
    
    void sumForce();
};

class CRessort {
public:
    CRessort(CParticule* _p0, CParticule* _p1, float _long = 0.0001, float _k = 20) {
        P0 = _p0;
        P1 = _p1;
        longueur_repos = _long;
        k = _k;
    }
    
    
    CParticule *P0, *P1;
    float longueur_repos;
    float k; //constante de Hooke (rigidité)
    float amortissement = 200;
    
    CVect3D VecteurUnitaire(CVect3D X, CVect3D Y) const{
        if( Module(X-Y) == 0) return CVect3D(0,0,0);
        CVect3D vecUni = (X-Y) / Module(X-Y);
        return vecUni;
    }
    
    //calcule la force du ressort
    CVect3D F(const bool isFirst) const {
        CVect3D ressortForce = CVect3D(0,0,0);
        if(isFirst) {
            ressortForce = ((-k * (Module(P0->pos[0] - P1->pos[0]) - longueur_repos)) * VecteurUnitaire(P0->pos[0],P1->pos[0])) - (amortissement * P0->vel[0]);
        } else {
            ressortForce = ((-k * (Module(P1->pos[0] - P0->pos[0]) - longueur_repos)) * VecteurUnitaire(P1->pos[0],P0->pos[0])) - (amortissement * P1->vel[0]);
        }
        return ressortForce;
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
        
        return CVect3D(0, 0, 0.0001*(exp(0.5/y) * amplitude * sin((2*PI*frequence*y + 1))));
    }
};

class CIntegrateur {
public:
    CSMR* smr;
    
    
    //TODO debug adding forces
    void step(){
        
        float gravity = 0;
        float integrationStep = 0.5;
        
        
        for(int i =0 ; i < smr->particules.size(); i++) {
            //force de gravité
            smr->particules[i]->force = CVect3D(0,gravity,0);
            //force du vent
            //smr->particules[i]->force = CVect3D(0,0,((float)i/(float)40) * (0.01 + 0.01 * sin((float)i/(float)30)));    // Vent
            //force des ressorts
            smr->particules[i]->sumForce();
        }
        //Force du vent
        for(int i =0 ; i < smr->particules.size(); i++) {
            //smr->particules[i]->force += smr->fVent(smr->particules[i]->pos[0]);//, t);
            //smr->particules[i]->force += CVect3D(0,0,0.1);
        }
        
        for(int i =0 ; i < 40; i++) {
            smr->particules[i]->force = CVect3D(0,0,0);
        }
        
        //Updater position et velocite des particules en fonction de la force calculée
        //on ne touche pas les 40 premieres particules pour qu`elles restent attachées au poteau
        for(int i = 0 ; i < smr->particules.size(); i++) {
            CParticule *p = smr->particules[i];
            //F = ma   -> a = F/M
            CVect3D a = p->force / p->masse;
            //update des anciennes coordonnées
            p->vel[0] = p->vel[1];
            p->pos[0] = p->pos[1];
            //update des coordonnées courantes
            p->vel[1] = p->vel[0] + (integrationStep * a);
            p->pos[1] = p->pos[0] + (integrationStep * p->vel[1]);
            //update de la position
            *p->vertex += p->pos[0];
        }
        

    }
};





#endif /* smr_h */

