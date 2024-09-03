#pragma once

#include <map>
#include <cstdint>
#include "genotype.hpp"

using std::uint64_t;
using std::int64_t;

/**
 * This file only defines the evaluation framework, which is the interface class
 * for actural evaluation protocals.
 * 
 * To implement your own evaluation protocal, simply:
 * 
 * 1. Inheriate this interface class
 * 2. Implement it's virtual methods
 * 3. Instantiate the class and use it
 */

class EvalInterface{
    public:
        using DataPkt = std::map<uint64_t,long double>;

        /**
         * virtual destructor:
         * - can choose to implement in derived class (in case you want to release some memory)
         * - if not implemented, DO NOTHING destructor will be used!
         */
        virtual ~EvalInterface() { /* DO NOTHING!~ */ };

        /**
         * data collection:
         * - collect necessary data to feed into the network from the outside world (game, etc.)
         * - must implement in derived class
         * - DataPkt contains the input data for each sensor node
         * - marked as const since this method does not need to change any states
         */
        virtual DataPkt collect() const = 0;

        /**
         * acturate (reflect):
         * - using the calculated data to acturate the outside world (game, etc.)
         * - must implement in derived class
         * - DataPkt contains the output data for each output node
         * - must return a boolean to indicate if the evaluation has finished or not
         */
        virtual bool acturate(const DataPkt& pkt) = 0;

        /**
         * calculate score:
         * - calculate the new score based on the previous score
         * - must implement in derived class (your own score update policy)
         */
        virtual int64_t score(const int64_t old_score) const = 0;

        /**
         * game loop:
         * - game is refreshed every game tick
         * - in every game tick 4 things will happen:
         *   ` environment is sensored
         *   ` network is propogated using the sensored data
         *   ` update the environment using the computed data
         *   ` update the genotype's score using the user-defined score update policy
         */
        inline void loop(Genotype& geno){
                bool the_end = false;
                do{
                        // check if the game has end or not
                        the_end = !acturate(geno.evaluate(collect()));
                        // update the geno's score
                        geno.set_score(score(geno.get_score()));
                }while(!the_end);
        }
};
