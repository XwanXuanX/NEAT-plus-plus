#include <iostream>
#include "genotype.hpp"
#include "prob.hpp"
#include "xor-game.hpp"

int main(){
        // testing genotype.dump method
        Genotype g(3, 2);
        GenotypeProbing::dump(g);
        // GenotypeProbing::generate_image(g);

        // testing genotype.constructor
        Genotype t("1.model");
        GenotypeProbing::dump(t);
        // GenotypeProbing::generate_image(t);

        // testing the XorGame
        XorGame game(2);
        game.loop(g);
        std::cout << g.fitness << std::endl;

        return 0;
}
