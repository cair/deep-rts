//
// Created by per on 11/6/19.
//
#pragma once
#ifndef DEEPRTS_RANDOM_H
#define DEEPRTS_RANDOM_H

#include <random>
#include <chrono>
#include "../Constants.h"

class Random
{
public:

    static Random& getInstance(){
        static Random instance;
        return instance;
    }

    static int randInt(int min, int max) {
        static Random instance; // Guaranteed to be destroyed.
        // Instantiated on first use.

        return std::uniform_int_distribution<int>{min, max}(instance.eng);
    }

    static int randAction(){
        static Random instance;
        return std::uniform_int_distribution<int>{Constants::ACTION_MIN, Constants::ACTION_MAX}(instance.eng);
    }

    static void seed(int s) {
        static Random instance; // Guaranteed to be destroyed.
        instance.eng.seed(s);
    }



private:
    std::mt19937 eng{std::random_device{}()};

    Random() {
        auto now = std::chrono::system_clock::now();
        auto value = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        eng.seed(value.count());

    }


public:
    Random(Random const&)               = delete;
    void operator=(Random const&)  = delete;

};

#endif //DEEPRTS_RANDOM_H
