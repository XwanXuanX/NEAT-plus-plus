#include <iostream>
#include "genotype.hpp"

int main(){
        // testing genotype.dump method
        Genotype g(3, 2);
        g.dump();

        // testing genotype.constructor
        Genotype t("../build/1.model");
        t.dump();
        
        return 0;
}
