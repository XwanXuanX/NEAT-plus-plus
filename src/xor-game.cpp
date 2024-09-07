#include "xor-game.hpp"
#include "utility.hpp"
#include <stdexcept>

// specify the number of input pins to the XOR gate
XorGame::XorGame(const std::uint32_t in_pin) : in_pin{in_pin}{
        if(in_pin < 2) // not defined for XOR gates
                throw std::invalid_argument(make_errmsg(__FILE__,__LINE__,"pin # must be at least 2"));
}

// generate random bits as inputs to the xor gate
XorGame::DataPkt XorGame::collect() const{
        // assuming node 1~in_pin is used for inputs
        DataPkt pkt;
        std::random_device rd{};
        std::uniform_int_distribution<int> uid(0,99);

        // generating random inputs
        auto gen_random_bits = [&uid, &rd]() { return uid(rd) >= 50; };
        for(int i = 1; i <= in_pin; ++i)
                pkt.insert(std::make_pair(i, gen_random_bits()));
        
        // also record the same input for validation
        rand_input = pkt;
        return pkt;
}




// check if the produced output matches the expected output, and end the game
bool XorGame::acturate(const DataPkt& pkt){

}

// increase the score if the output is correct
int64_t XorGame::upd_score(const int64_t old_score) const{

}
