// PROJECT IDENTIFIER: 9504853406CBAC39EE89AA3AD238AA12CA198043
//  zombbb.cpp
//  project2-thewalkingdeadline
//
//  Created by Samyukta Iyengar on 5/15/20.
//  Copyright © 2020 Samyukta Iyengar. All rights reserved.
//

#include <stdio.h>
#include <string>
#include "zombie.hpp"
#include "player.hpp"
#include "P2random.h"
#include <time.h>

void print_helper() {
    cout << "Options: " << "\n"
    << "\t-h, --help\t\t Help message" << "\n"
    << "\t-m, --median\t Print median zombie lifetime after round" << "\n"
    << "\t-v, --verbose\t Print verbose output" << "\n"
    << "\t-s, --stats\t Display statistics after game is finished" << "\n";
}

bool readRoundInfo(size_t& totalZombies, const size_t& currentRoundNumber, size_t& roundNumberToBeRead, vector<Zombie*>& allZombies,
    string& zombieThatKilledPlayer, priority_queue<Zombie*, vector<Zombie*>, ZombieComparator>& zombies, bool verbose) {
    bool playerAlive = true;
    string word;
    while (cin >> word) {
        if (word.compare("---") == 0) {
            break;
        }
        if (word.compare("round:") == 0 || word.compare("r") == 0) {
            cin >> word;
            roundNumberToBeRead = (size_t)stoi(word);
            if (roundNumberToBeRead != currentRoundNumber) {
                return playerAlive;
            }
        }
        else if (word.compare("random-zombies:") == 0 || word.compare("rndzmbs") == 0) {
            cin >> word;
            size_t randomZombies = (size_t)stoi(word);
            totalZombies += randomZombies;
            for (size_t i = 0; i < randomZombies; i++) {
                Zombie* z = new Zombie();
                z->setName(P2random::getNextZombieName());
                z->setDistance(P2random::getNextZombieDistance());
                z->setSpeed(P2random::getNextZombieSpeed());
                z->setHealth(P2random::getNextZombieHealth());
                allZombies.push_back(z);
                if (!z->isDead()) {
                    if (z->getDistance() == 0) {
                        playerAlive = false;
                        if (zombieThatKilledPlayer.empty()) {
                            zombieThatKilledPlayer = z->getName();
                        }
                    }
                    else {
                        zombies.push(z);
                    }
                }
                if (verbose) {
                    cout << "Created: " << z->getName() << " (distance: " << z->getDistance()
                        << ", speed: " << z->getSpeed() << ", health: " << z->getHealth() << ")\n";
                }
            }
        }
        else if (word.compare("named-zombies:") == 0 || word.compare("named") == 0) {
            cin >> word;
            size_t namedZombies = (size_t)stoi(word);
            totalZombies += namedZombies;
            for (size_t i = 0; i < namedZombies; i++) {
                cin >> word;
                Zombie* z = new Zombie;
                z->setName(word);
                cin >> word;
                cin >> word;
                z->setDistance((size_t)stoi(word));
                cin >> word;
                cin >> word;
                z->setSpeed((size_t)stoi(word));
                cin >> word;
                cin >> word;
                z->setHealth((size_t)stoi(word));
                allZombies.push_back(z);
                if (!z->isDead()) {
                    if (z->getDistance() == 0) {
                        playerAlive = false;
                        if (zombieThatKilledPlayer.empty()) {
                            zombieThatKilledPlayer = z->getName();
                        }
                    }
                    else {
                        zombies.push(z);
                    }
                }
                if (verbose) {
                    cout << "Created: " << z->getName() << " (distance: " << z->getDistance()
                        << ", speed: " << z->getSpeed() << ", health: " << z->getHealth() << ")\n";
                }
            }
        }
    }
 
    return playerAlive;
}

void displayStats(size_t statsNum, size_t numAliveZombies, const vector<string>& destroyedZombies, vector<Zombie*>& allZombies){
    cout << "Zombies still active: " << numAliveZombies << endl;

    size_t numZombiesToPrint = min(statsNum, destroyedZombies.size());
    size_t i = 0;

    cout << "First zombies killed:" << endl;
    for (const string& z : destroyedZombies) {
        cout << z << " " << ++i << endl;
        if (i == numZombiesToPrint) {
            break;
        }
    }

    cout << "Last zombies killed:" << endl;
    for (vector<string>::const_reverse_iterator iter = destroyedZombies.rbegin(); iter != destroyedZombies.rend(); iter++) {
        cout << *iter << " " << i-- << endl;
        if (i == 0) {
            break;
        }
    }

    numZombiesToPrint = min(statsNum, allZombies.size());

    priority_queue<Zombie*, vector<Zombie*>, ZombieLessThanOperator> mostActive(allZombies.begin(),allZombies.end());
    priority_queue<Zombie*, vector<Zombie*>, ZombieGreaterThanOperator> leastActive(allZombies.begin(), allZombies.end());
    cout << "Most active zombies:" << endl;

    for (size_t i = 0; i < numZombiesToPrint; i++) {
        Zombie* z = mostActive.top();
        mostActive.pop();
        cout << z->getName() << " " << z->getRoundsActive() << endl;
    }

    cout << "Least active zombies:" << endl;
    for (size_t i = 0; i < numZombiesToPrint; i++) {
        Zombie* z = leastActive.top();
        leastActive.pop();
        cout << z->getName() << " " << z->getRoundsActive() << endl;
    }
}

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    bool verbose = false;
    bool stats = false;
    size_t statsNum = 0;
    bool median = false;
    //cout << "before reading in command line" << endl;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_helper();
            return 0;
        }
        else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        }
        else if (arg == "-s" || arg == "--statistics") {
            stats = true;
            i++;
            statsNum = (size_t)(stoi(argv[i]));
        }
        else if (arg == "-m" || arg == "--median") {
            median = true;
        }
        else {
            if (arg[0] == '-') {
                for (size_t j = 1; j < arg.length(); j++) {
                    if (arg[j] == 'm') {
                        median = true;
                    }
                    else if (arg[j] == 'v') {
                        verbose = true;
                    }
                    else if (arg[j] == 's') {
                        stats = true;
                        if (isdigit(arg[j + 1])) {
                            size_t index = j + 1;
                            string number;
                            while (isdigit(arg[index]) && index < arg.length()) {
                                number += arg[index];
                                index++;
                            }
                            statsNum = (size_t)(stoi(number));
                        }
                        else {
                            i++;
                            statsNum = (size_t)(stoi(argv[i]));
                        }
                    }
                }
            }
        }
    }
    //cout << "after reading in command line" << endl;
    vector<Zombie*> allZombies;
    unsigned int randomSeed = 0;
    unsigned int maxRandDistance = 0;
    unsigned int maxRandSpeed = 0;
    unsigned int maxRandHealth = 0;
    size_t maxArrows = 0;

    string word;

    //skip first line
    string comment;
    getline(cin, comment);
    //cout << "before reading in file info" << endl;
    while (cin >> word) {
        if (word.compare("quiver-capacity:") == 0 || word.compare("quiver") == 0) {
            cin >> word;
            maxArrows = (unsigned int)(stoi(word));
        }
        else if (word.compare("random-seed:") == 0 || word.compare("seed") == 0) {
            cin >> word;
            randomSeed = (unsigned int)(stoi(word));
        }
        else if (word.compare("max-rand-distance:") == 0 || word.compare("rand-dist") == 0) {
            cin >> word;
            maxRandDistance = (unsigned int)(stoi(word));
        }
        else if (word.compare("max-rand-speed:") == 0 || word.compare("rand-speed") == 0) {
            cin >> word;
            maxRandSpeed = (unsigned int)(stoi(word));
        }
        else if (word.compare("max-rand-health:") == 0 || word.compare("rand-health") == 0) {
            cin >> word;
            maxRandHealth = (unsigned int)(stoi(word));
        }
        else if (word.find("---") == 0) {
            break;
        }
    }
    //cout << "after reading in file info" << endl;
    P2random::initialize(randomSeed, maxRandDistance, maxRandSpeed, maxRandHealth);

    // for stats
    vector<string> destroyedZombies;
    size_t roundNumberToBeRead = 1;
    size_t currentRoundNumber = 0;
    size_t totalZombies = 0;
    Player p(maxArrows, verbose, stats, median);
    bool playerAlive = true;
    string zombieThatKilledPlayer;
    string lastZombieKilledName;
    size_t roundWherelastZombieWasKilled = 0;
    size_t numAliveZombies = 0;
    MedianCalculator medianCalc;
    bool atLeastOneZombieAlive = false;
    PQsubclass<Zombie*, vector<Zombie*>, ZombieComparator> zombies;
    //cout << "before while loop" << endl;
    do {
        currentRoundNumber++;

        if (verbose) {
            cout << "Round: " << currentRoundNumber << endl;
        }
        //p.refillArrows();
        
        for (Zombie* z : allZombies) {
            if (!z->isDead()) {
                z->moveCloser();
                if (verbose) {
                    cout << "Moved: " << z->getName() << " (distance: " << z->getDistance()
                        << ", speed: " << z->getSpeed() << ", health: " << z->getHealth() << ")\n";
                }
                if (z->getDistance() == 0) {
                    playerAlive = false;
                    // keep track of the zombie that killed player
                    if (zombieThatKilledPlayer.empty()) {
                        zombieThatKilledPlayer = z->getName();
                    }
                }
            }
        }
        
        if (!playerAlive) {
            break;
        }

        if (currentRoundNumber >= roundNumberToBeRead) {
            playerAlive = readRoundInfo(totalZombies, currentRoundNumber, roundNumberToBeRead, allZombies, zombieThatKilledPlayer, zombies, verbose);
        }

        if (!playerAlive) {
            numAliveZombies = allZombies.size();
            break;
        }

        if (p.shootZombie(zombies, lastZombieKilledName, destroyedZombies, medianCalc)) {
            roundWherelastZombieWasKilled = currentRoundNumber;
        }

        numAliveZombies = zombies.size();
        if (median) {
            cout << "At the end of round " << currentRoundNumber
                << ", the median zombie lifetime is " << medianCalc.getMedian() << endl;
        }

        atLeastOneZombieAlive = !zombies.empty();
        if (atLeastOneZombieAlive) {
            vector<Zombie*> &v = zombies.getContainer();
            //cout << "Container size = " << v.size() << endl;
            for (Zombie* z : v) {
                z->updateRoundsActive();
            }
        }

    } while (playerAlive && (atLeastOneZombieAlive || (!atLeastOneZombieAlive && roundNumberToBeRead >= currentRoundNumber)));

    // check who won and print appropriate message
    if (playerAlive) {
        size_t playerWonInRound = currentRoundNumber;
        if (roundWherelastZombieWasKilled < currentRoundNumber - 1) {
            playerWonInRound = roundNumberToBeRead;
        }
        cout << "VICTORY IN ROUND " << playerWonInRound << "! " << lastZombieKilledName
            << " was the last zombie." << endl;
    }
    else {
        cout << "DEFEAT IN ROUND " << currentRoundNumber << "! " << zombieThatKilledPlayer
            << " ate your brains!" << endl;
    }

    if (stats) {
        displayStats(statsNum, numAliveZombies, destroyedZombies, allZombies);
    } 

    for (size_t i = 0; i < allZombies.size(); i++) {
        delete allZombies[i];
    }

}
