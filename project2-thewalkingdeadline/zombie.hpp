// PROJECT IDENTIFIER: 9504853406CBAC39EE89AA3AD238AA12CA198043
//  zombie.hpp
//  project2-thewalkingdeadline
//
//  Created by Samyukta Iyengar on 5/14/20.
//  Copyright © 2020 Samyukta Iyengar. All rights reserved.
//

#ifndef zombie_hpp
#define zombie_hpp

#include <stdio.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Zombie {
public:
    Zombie(const string& nameIn, size_t distanceIn, size_t speedIn, size_t healthIn, size_t roundsActiveIn = 1)
        : name(nameIn), distance(distanceIn), speed(speedIn),
        health(healthIn), roundsActive(roundsActiveIn) {}


    Zombie(const Zombie& z) {
        name = z.getName();
        distance = z.getDistance();
        speed = z.getSpeed();
        health = z.getHealth();
        roundsActive = z.getRoundsActive();
    }

    Zombie() {}

    void setName(const string& n) {
        name = n;
    }

    string getName() const {
        return name;
    }

    void setDistance(size_t d) {
        distance = d;
    }

    size_t getDistance() const {
        return distance;
    }

    void setSpeed(size_t s) {
        speed = s;
    }

    size_t getSpeed() const {
        return speed;
    }

    void setHealth(size_t h) {
        health = h;
    }

    size_t getHealth() const {
        return health;
    }

    size_t getETA() const {
        if (distance < speed) {
            return 0;
        }
        if (speed == 0) {
            return distance;
        }
        return distance / speed;
    }

    void reduceZombieHealth(size_t healthIn) {
        health -= healthIn; 
    }

    bool isDead() const {
        return health == 0;
    }
    
    void killZombie() {
        health = 0;
    }

    void moveCloser() {
        if (distance > speed) {
            distance = max((size_t)0, (distance - speed));
        }
        else {
            distance = 0;
        }
    }

    void updateRoundsActive() {
        roundsActive++;
    }

    size_t getRoundsActive() const {
        return roundsActive;
    }


private:
    string name;
    size_t distance = 0;
    size_t speed = 0;
    size_t health = 0;
    size_t roundsActive = 1;

};

struct ZombieLessThanOperator {
    bool operator() (Zombie* z1, Zombie* z2) {
        if (z1->getRoundsActive() == z2->getRoundsActive()) {
            return z1->getName() > z2->getName();
        }
        return z1->getRoundsActive() < z2->getRoundsActive();
    }
};

struct ZombieGreaterThanOperator {
    bool operator() (Zombie* z1, Zombie* z2) {
        if (z1->getRoundsActive() == z2->getRoundsActive()) {
            return z1->getName() > z2->getName();
        }
        return z1->getRoundsActive() > z2->getRoundsActive();
    }
};


struct ZombieComparator {
    bool operator() (Zombie* z1, Zombie* z2) const {
        if (!z1 || !z2) {
            return false;
        }

        if (z1->getETA() == z2->getETA()) {
            if (z1->getHealth() == z2->getHealth()) {
                return z1->getName().compare(z2->getName()) > 0;
            }
            return z1->getHealth() > z2->getHealth();
        }
        return z1->getETA() > z2->getETA();
    }
};



#endif /* zombie_hpp */
