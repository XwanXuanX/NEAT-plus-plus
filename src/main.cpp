#include <iostream>
#include "genotype.hpp"
#include "prob.hpp"

int main(){
        // testing genotype.dump method
        Genotype g(3, 2);
        GenotypeProbing::dump(g);
        GenotypeProbing::generate_image(g);

        // testing genotype.constructor
        Genotype t("1.model");
        GenotypeProbing::dump(t);
        GenotypeProbing::generate_image(t);
        
        return 0;
}
