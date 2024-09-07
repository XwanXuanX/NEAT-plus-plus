#pragma once

#include <random>
#include "eval-interface.hpp"

class XorGame : public EvalInterface{
    public:
        // specify the number of input pins to the XOR gate
        [[nodiscard]] explicit XorGame(const std::uint32_t in_pin);

    private: // private member functions
        // generate random bits as inputs to the xor gate
        virtual DataPkt collect() const override final;
        // check if the produced output matches the expected output, and end the game
        virtual bool acturate(const DataPkt& pkt) override final;
        // increase the score if the output is correct
        virtual int64_t upd_score(const int64_t old_score) const override final;

    private: // private member variables
        const std::uint32_t in_pin;
        mutable DataPkt rand_input;
};
