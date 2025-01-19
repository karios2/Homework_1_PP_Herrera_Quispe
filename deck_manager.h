#ifndef DECK_MANAGER_H
#define DECK_MANAGER_H

#include "deck.h"
#include <vector>
#include <iostream>

class DeckManager {
public:
    static std::vector<Card> generateAndVerify(int numCards) {
        std::cout << "Generating deck of " << numCards << " cards..." << std::endl;
        std::vector<Card> deck = generateDeck(numCards);
        std::cout << "Sample of the generated deck:" << std::endl;
        printSample(deck, 10);
        verifyDeck(deck, numCards);
        return deck;
    }
};

#endif // DECK_MANAGER_H
