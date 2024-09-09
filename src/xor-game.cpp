#include "xor-game.hpp"
#include "utility.hpp"
#include <stdexcept>
#include <cassert>

// specify the number of input pins to the XOR gate
XorGame::XorGame(const std::uint32_t in_pin) : in_pin{in_pin}{
        if(in_pin < 2) // not defined for XOR gates
                throw std::invalid_argument(make_errmsg(__FILE__,__LINE__,"pin # must be at least 2"));
}

// initialize the genotype's fitness to 0
void XorGame::initialize(Genotype& geno){
        geno.fitness = 0;
}

// generate random bits as inputs to the xor gate
XorGame::DataPkt XorGame::collect() const{
        // assuming node 1~in_pin is used for inputs
        DataPkt pkt;

        // generating random inputs
        auto gen_random_bits = []() { return rand_select({0, 99}) >= 50; };
        for(int i = 1; i <= in_pin; ++i)
                pkt.insert(std::make_pair(i, gen_random_bits()));
        
        // also record the same input for validation
        rand_input = pkt;
        return pkt;
}

// check if the produced output matches the expected output, and end the game
bool XorGame::acturate(const DataPkt& pkt){
        assert(pkt.size() == 1); // only one output is allowed
        // produce the expected output
        assert(rand_input.at(1) == 1 || rand_input.at(1) == 0);
        bool expected = static_cast<bool>(rand_input.at(1));
        for(int i = 2; i <= in_pin; ++i){
                assert(rand_input.at(i) == 1 || rand_input.at(i) == 0);
                expected ^= static_cast<bool>(rand_input.at(i));
        }
        // check correctness and continue the game
        return expected == pkt.begin()->second;
}

// increase the score if the output is correct
long double XorGame::upd_score(const long double old_score) const{
        return old_score + 1;
}
