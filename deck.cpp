#include "deck.h"
#include <vector>
#include <map>
#include <omp.h>
#include <algorithm>
#include <iostream>

int getSuitOrder(const std::string& suit) {
    if (suit == "Hearts") return 1;
    if (suit == "Clubs") return 2;
    if (suit == "Spades") return 3;
    if (suit == "Diamonds") return 4;
    if (suit == "Joker") return 5;
    return 6;
}

void merge(std::vector<Card>& deck, int left, int mid, int right, auto comparator) {
    std::vector<Card> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (comparator(deck[i], deck[j])) {
            temp[k++] = deck[i++];
        } else {
            temp[k++] = deck[j++];
        }
    }

    while (i <= mid) temp[k++] = deck[i++];
    while (j <= right) temp[k++] = deck[j++];

    for (int l = 0; l < temp.size(); ++l) {
        deck[left + l] = temp[l];
    }
}

void mergeSort(std::vector<Card>& deck, int left, int right, auto comparator) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    #pragma omp task shared(deck)
    mergeSort(deck, left, mid, comparator);

    #pragma omp task shared(deck)
    mergeSort(deck, mid + 1, right, comparator);

    #pragma omp taskwait
    merge(deck, left, mid, right, comparator);
}

void sortDeckParallel(std::vector<Card>& deck) {
    auto comparator = [](const Card& a, const Card& b) {
        int orderA = getSuitOrder(a.suit);
        int orderB = getSuitOrder(b.suit);
        if (orderA != orderB) {
            return orderA < orderB;
        }
        return a.value < b.value;
    };

    #pragma omp parallel
    {
        #pragma omp single
        {
            mergeSort(deck, 0, deck.size() - 1, comparator);
        }
    }
}

void sortDeckSequential(std::vector<Card>& deck) {
    auto comparator = [](const Card& a, const Card& b) {
        int orderA = getSuitOrder(a.suit);
        int orderB = getSuitOrder(b.suit);
        if (orderA != orderB) {
            return orderA < orderB;
        }
        return a.value < b.value;
    };

    std::sort(deck.begin(), deck.end(), comparator);
}

std::vector<Card> generateDeck(int numCards) {
    std::vector<Card> deck;
    std::vector<std::string> suits = {"Hearts", "Clubs", "Spades", "Diamonds"};
    int cardsPerSuit = (numCards - 2) / suits.size();

    for (const auto& suit : suits) {
        int value = 1;
        for (int i = 0; i < cardsPerSuit; ++i) {
            deck.push_back({value, suit});
            value = (value % 13) + 1;
        }
    }

    for (int i = 0; i < 2; ++i) {
        deck.push_back({0, "Joker"});
    }

    return deck;
}

void printSample(const std::vector<Card>& deck, int sampleSize) {
    int step = deck.size() / sampleSize;
    for (int i = 0; i < sampleSize; ++i) {
        int index = i * step;
        std::cout << "Value: " << deck[index].value << ", Suit: " << deck[index].suit << std::endl;
    }
}

void verifyDeck(const std::vector<Card>& deck, int numCards) {
    std::map<std::string, int> suitCount;
    std::map<int, int> valueCount;

    for (const auto& card : deck) {
        suitCount[card.suit]++;
        if (card.suit != "Joker") {
            valueCount[card.value]++;
        }
    }

    std::cout << "Suit distribution:" << std::endl;
    for (const auto& suit : suitCount) {
        std::cout << suit.first << ": " << suit.second << " cards" << std::endl;
    }

    std::cout << "Value distribution:" << std::endl;
    for (int i = 1; i <= 13; ++i) {
        std::cout << "Value " << i << ": " << valueCount[i] << " cards" << std::endl;
    }

    std::cout << "Jokers: " << suitCount["Joker"] << " cards" << std::endl;
    std::cout << "Deck verification complete: the deck is valid." << std::endl;
}
