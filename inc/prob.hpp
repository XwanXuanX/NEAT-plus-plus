#pragma once

#include "genotype.hpp"

// this class is a collection of static methods that print/display the genotype information
// when you want to call one of it's method, simply provide with a genotype instance
// to use GenotypeProbing utils in Genotype methods, simply write:
// {
//      ...
//      GenotypeProbing::print_node(*this);
//      ...
// }
struct GenotypeProbing{
    public:
        // write the node genes and connection genes to a .model file
        static void dump(const Genotype& geno);
        static void dump(const Genotype& geno, const std::string& file_name);

        // print the node genes and connection genes for debugging
        static void print_node(const Genotype& geno);
        static void print_connection(const Genotype& geno);

        // generate the graph representation of above nodes and edges
        static void generate_image(const Genotype& geno);

    private:
        // helper method for printing to file
        static void dumpfile(const Genotype& geno, const std::string& file_name);

        // helper method to generate the dot file to generate image
        static void generate_dot(const Genotype& geno, const std::filesystem::path& dot_file);
};
