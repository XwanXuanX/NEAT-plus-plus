#include <iostream>
#include "genotype.hpp"
#include "prob.hpp"
#include "xor-game.hpp"

int main(){
        // testing genotype.dump method
        Genotype g(3, 2);
        GenotypeProbing::dump(g);
        g.mutate();
        GenotypeProbing::dump(g, "after-mutation");
#ifndef GITHUB_ACTIONS
        GenotypeProbing::generate_image(g);
#endif

#if false
        // testing genotype.constructor
        Genotype t("1.model");
        GenotypeProbing::dump(t);
        // GenotypeProbing::generate_image(t);

        // testing the XorGame
        XorGame game(2);
        game.loop(g);
        std::cout << g.fitness << std::endl;
#endif

        return 0;
}
