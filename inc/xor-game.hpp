#pragma once

#include <random>
#include "eval-interface.hpp"

class XorGame : public EvalInterface{
    public:
        // specify the number of input pins to the XOR gate
        [[nodiscard]] explicit XorGame(const std::uint32_t in_pin);

    private: // private member functions
        // initialize the genotype's fitness to 0
        virtual void initialize(Genotype& geno) override final;

        // generate random bits as inputs to the xor gate
        virtual DataPkt collect() const override final;

        // check if the produced output matches the expected output, and end the game
        virtual bool acturate(const DataPkt& pkt) override final;
        
        // increase the score if the output is correct
        virtual long double upd_score(const long double old_score) const override final;

    private: // private member variables
        const std::uint32_t in_pin;
        mutable DataPkt rand_input;
};
