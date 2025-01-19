#ifndef DECK_H
#define DECK_H

#include <vector>
#include <string>

struct Card {
    int value;         // Card value (1 to 13 for normal suits, 0 for Joker)
    std::string suit;  // Suit (Hearts, Clubs, Spades, Diamonds, Joker)
};

std::vector<Card> generateDeck(int numCards);
void printSample(const std::vector<Card>& deck, int sampleSize);
void verifyDeck(const std::vector<Card>& deck, int numCards);
void sortDeckSequential(std::vector<Card>& deck);
void sortDeckParallel(std::vector<Card>& deck);

#endif // DECK_H
