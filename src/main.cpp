#include <iostream>
#include "genotype.hpp"

int main(){
        // testing genotype.dump method
        Genotype g(3, 2);
        g.dump();
        g.generate_image();

        // testing genotype.constructor
        Genotype t("1.model");
        t.dump();
        t.generate_image();
        
        return 0;
}
