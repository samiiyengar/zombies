// PROJECT IDENTIFIER: 9504853406CBAC39EE89AA3AD238AA12CA198043
//  player.hpp
//  project2-thewalkingdeadline
//
//  Created by Samyukta Iyengar on 5/14/20.
//  Copyright © 2020 Samyukta Iyengar. All rights reserved.
//

#ifndef player_hpp
#define player_hpp

#include <stdio.h>
#include "zombie.hpp"
#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

template<class _Ty, class _Container = vector<_Ty>,
    class _Pr = less<typename _Container::value_type>>
class PQsubclass : public priority_queue<_Ty, _Container, _Pr> {
public:
    vector<_Ty>& getContainer() {
        return priority_queue<_Ty, _Container, _Pr>::c;
    }
};

struct MedianCalculator {
    priority_queue<size_t, vector<size_t>, greater<size_t>> high;
    priority_queue<size_t, vector<size_t>, less<size_t>> low;
    void insert(size_t roundsActive) {
        if (high.empty() || roundsActive >= high.top()) {
            high.push(roundsActive);
        }
        else {
            low.push(roundsActive);
        }

        if ((int)(high.size() - low.size()) > 1) {
            low.push(high.top());
            high.pop();
        }
        else if ((int)(low.size() - high.size()) > 1) {
            high.push(low.top());
            low.pop();
        }
    }

    size_t getMedian() const {
        size_t median = 0;
        if (!high.empty() || !low.empty()) {
            if (high.size() == low.size()) {
                median = (high.top() + low.top()) / 2;
            }
            else if (high.size() > low.size()) {
                median = high.top();
            }
            else {
                median = low.top();
            }
        }
        return median;
    }

};


class Player {
public:

    Player(size_t maxArrowsIn, bool verboseIn, bool statsIn, bool medianIn)
        : maxArrows(maxArrowsIn), verbose(verboseIn), stats(statsIn), median(medianIn) {};

    /*
    void refillArrows() {
        currentArrows = maxArrows;
    } */

    bool shootZombie(priority_queue<Zombie*, vector<Zombie*>, ZombieComparator>& zombies, string& lastZombieKilledName,
        vector<string>& destroyedZombies, MedianCalculator& medianCalc) {
        currentArrows = maxArrows;
        bool destroyedZombie = false;
        while (currentArrows > 0) {
            if (zombies.empty()) {
                return false;
            }
            Zombie* z = zombies.top();
            if (z->getHealth() <= currentArrows) {
                currentArrows -= z->getHealth();
                z->killZombie();
                zombies.pop();
                destroyedZombie = true;
                lastZombieKilledName = z->getName();
                if (stats || median) {
                    destroyedZombies.push_back(lastZombieKilledName);
                }
                if (median) {
                    medianCalc.insert(z->getRoundsActive());
                }
                if (verbose) {
                    cout << "Destroyed: " << z->getName() << " (distance: " << z->getDistance()
                        << ", speed: " << z->getSpeed() << ", health: " << z->getHealth() << ")\n";
                }
            } else {
                z->reduceZombieHealth(currentArrows);
                break;
            }
        }
        return destroyedZombie;
    }

    size_t getCurrentArrows() const {
        return currentArrows;
    }

private:
    // current number of arrows
    size_t currentArrows = 0;
    // max arrows
    size_t maxArrows = 0;
    // bool verbose enabled
    bool verbose = false;
    bool stats = false;
    bool median = false;

};

#endif /* player_hpp */
