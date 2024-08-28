#include "prob.hpp"
#include "utility.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void GenotypeProbing::dumpfile(const Genotype &geno, const std::string& file_name){
        // open the output file, if non, create one
        std::ostringstream full_file_name;
        full_file_name << file_name << ".model";
        std::ofstream outfile(full_file_name.str());
        if(!outfile.is_open()){
                throw std::runtime_error(make_errmsg(__FILE__,__LINE__,"cannot open target .model file"));
        }

        // write node genes
        // write node number
        outfile << std::to_string(geno.node_genes.size()) << std::endl;
        for(auto& node : geno.node_genes)
                outfile << std::to_string(node.node_number) << ' ';
        outfile << std::endl;
        // write node type
        for(auto& node : geno.node_genes)
                outfile << Node::get_nodetype(node.node_type) << ' ';
        outfile << std::endl;

        // write connection genes
        outfile << std::to_string(geno.connection_genes.size()) << std::endl;
        for(auto& connect : geno.connection_genes)
                outfile << Connection::make_connect(connect) << std::endl;
}

void GenotypeProbing::dump(const Genotype &geno, const std::string& file_name){
        dumpfile(geno, file_name);
}

void GenotypeProbing::dump(const Genotype &geno){
        dumpfile(geno, std::to_string(geno.id));
}

// print the node genes and connection genes for debugging
void GenotypeProbing::print_node(const Genotype& geno){
        std::cout << geno.node_genes.size() << std::endl;
        for(auto& node : geno.node_genes)
                std::cout << node.node_number << ' ';
        std::cout << std::endl;
        for(auto& node : geno.node_genes)
                std::cout << Node::get_nodetype(node.node_type) << ' ';
        std::cout << std::endl;
}

void GenotypeProbing::print_connection(const Genotype& geno){
        std::cout << geno.connection_genes.size() << std::endl;
        for(auto& connection : geno.connection_genes)
                std::cout << Connection::make_connect(connection) << std::endl;
}

void GenotypeProbing::generate_dot(const Genotype& geno, const std::filesystem::path& dot_file){
        // open the dot file, if non, create one
        std::ofstream dotfile(dot_file);
        if (!dotfile.is_open()) {
                throw std::runtime_error(make_errmsg(__FILE__, __LINE__, "cannot open target .dot file"));
        }

        // dot file opening line and style settings
        dotfile << "digraph G {\n";
        dotfile << "    node [shape=circle, style=filled, fontname=\"Helvetica\", fontsize=12];\n";
        dotfile << "    edge [fontname=\"Helvetica\", fontsize=10];\n";
        
        // generate node genes
        // generate nodes number and type
        for (auto& node : geno.node_genes) {
                std::string color;
                switch (node.node_type) {
                        case NodeType::sensor: color = "green2"; break;
                        case NodeType::hidden: color = "grey"; break;
                        case NodeType::output: color = "pink"; break;
                }
                dotfile << "    " << node.node_number << " [label=\"" << node.node_number 
                        << " (" << Node::get_nodetype(node.node_type) << ")\", fillcolor=" << color << "];\n";
        }
        
        // generate connection genes
        for (auto& connect : geno.connection_genes) {
                dotfile << "    " << connect.in << " -> " << connect.out 
                        << " [label=\"Weight: " << connect.weight 
                        << "\", color=" << (connect.enable ? "blue" : "red") << "];\n";
        }
        
        dotfile << "}\n";
}

// generate the graph representation of above nodes and edges
void GenotypeProbing::generate_image(const Genotype& geno){
        // construct the DOT file name
        std::ostringstream dot_file_path;
        dot_file_path << std::to_string(geno.id) << ".dot";
        // generate the dot file first
        generate_dot(geno, dot_file_path.str());
        
        // construct the PNG file name
        std::ostringstream png_file_path;
        png_file_path << std::to_string(geno.id) << ".png";
        // create the command to convert DOT to PNG using Graphviz's dot tool
        std::ostringstream command;
        command << "dot -Tpng -o " << png_file_path.str() << " " << dot_file_path.str();
        // execute the command
        int result = std::system(command.str().c_str());
        //check if the command was successful
        if (result != 0) {
                throw std::runtime_error(make_errmsg(__FILE__, __LINE__, "cannot generate image from DOT file"));
        }
}
