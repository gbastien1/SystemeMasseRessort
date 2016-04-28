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
    CParticule(CVertex* _vertex, float _masse, bool fixed) {
        vertex = _vertex;
        pos[0] = vertex[0];
        pos[1] = CPoint3D(0,0,0);
        vel[0] = CVect3D(0,0,0);
        vel[1] = CVect3D(0,0,0);
        masse = _masse;
        force = CVect3D(0,0,0);
        isFixed = fixed;
    }
    
    vector<CRessort*> ressortsAdjacents;
    CVertex* vertex; //sommet du maillage associé à la particule
    CPoint3D pos[2];
    CVect3D vel[2];
    float masse;
    CVect3D force;
    bool isFixed;
    
    void sumForce();
};

class CRessort {
public:
    CRessort(CParticule* _p0, CParticule* _p1, float _long = 0.001, float _k = 50) {
        P0 = _p0;
        P1 = _p1;
        longueur_repos = _long;
        k = _k;
    }
    
    
    CParticule *P0, *P1;
    float longueur_repos;
    float k; //constante de Hooke (rigidité)
    float amortissement = 10;
    
    CVect3D VecteurUnitaire(CVect3D X, CVect3D Y) const{
        if( Module(X-Y) == 0) return CVect3D(0,0,0);
        CVect3D vecUni = (X-Y) / Module(X-Y);
        return vecUni;
    }
    
    //calcule la force du ressort
    CVect3D F(const bool isFirst) const {
        CVect3D ressortForce = CVect3D(0,0,0);
        int i = 0;
        if(isFirst) {
            ressortForce = (-k * (Dist(P0->pos[i], P1->pos[i]) - longueur_repos)) * VecteurUnitaire(P0->pos[i], P1->pos[i]) - 0.0001*(amortissement * P0->vel[i]);
        } else {
            ressortForce =  - (-k * (Dist(P1->pos[i], P0->pos[i]) - longueur_repos) * VecteurUnitaire(P1->pos[i], P0->pos[i]) - 0.0001*(amortissement * P1->vel[i]));
        }
        return ressortForce;
    }
    
    void correctionGeometrique() {
        float lambda = longueur_repos / Module(P0->pos[0] - Module(P1->pos[0]));
        
        if (P0->isFixed && P1->isFixed)
            return;
        else if (P0->isFixed)
            P1->pos[0] = lambda * P1->pos[0] + (1.0f-lambda) * P0->pos[0];
        else if (P1->isFixed)
            P0->pos[0] = lambda * P0->pos[0] + (1.0f-lambda) * P1->pos[0];
        else {
            P0->pos[0] = lambda/2 * (P0->pos[0] - P1->pos[0]);
            P1->pos[0] = lambda/2 * (P1->pos[0] - P0->pos[0]);
        }
            
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
    
    
    //TODO debug adding forces
    void step(){
        
        float gravity = -1;
        float integrationStep = 0.1;
        
        
        for(int i =0 ; i < smr->particules.size(); i++) {
            //force de gravité
            smr->particules[i]->force = CVect3D(0,gravity,0);
            //force du vent
            smr->particules[i]->force = 0.01*CVect3D(0,0,((float)i/(float)40) * (0.01 + 0.01 * sin((float)i/(float)30)));    // Vent
            //force des ressorts
            smr->particules[i]->sumForce();
        }
        
        //on ne touche pas les 40 premieres particules pour qu`elles restent attachées au poteau
        for(int i =0 ; i < 40; i++) {
            smr->particules[i]->force = CVect3D(0,0,0);
        }
        
        //Correction géométrique des longeurs
        for (int i = 0; i < smr->ressorts.size(); i++) {
            CRessort *r = smr->ressorts[i];
            r->correctionGeometrique();
        }
        
        //Updater position et velocite des particules en fonction de la force calculée
        for(int i = 0 ; i < smr->particules.size(); i++) {
            CParticule *p = smr->particules[i];
            //F = ma   -> a = F/M
            CVect3D a = p->force / p->masse;
            
            CVect3D velBefore = p->vel[1];
            CPoint3D posBefore = p->pos[1];
            
            //update des coordonnées du prochain pas d'intégration
            p->vel[1] = (velBefore + (integrationStep * a));
            p->pos[1] = posBefore + (integrationStep * p->vel[1]);
            
            //update des coordonnées courantes
            p->vel[0] = p->vel[1];
            p->pos[0] = p->pos[1];
            
            //update de la position
            *p->vertex += p->pos[0];
        }
        

    }
};





#endif /* smr_h */

