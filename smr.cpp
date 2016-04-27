//
//  smr.cpp
//  8trd147-d2
//
//  Created by Etudiant on 2016-04-14.
//
//

#include <stdio.h>
#include "mesh.h"
#include "smr.h"

using namespace std;

void CParticule::sumForce() {
    for(int i = 0; i < ressortsAdjacents.size(); i++) {
        CRessort* r = ressortsAdjacents[i];
        if (this == r->P0)
            force += r->F(true);
        else
            force += r->F(false);
    }
}