#include <iostream>
#include "deck.h"
#include "performance_metrics.h"
#include <chrono>
#include <omp.h>

void runSequential(std::vector<Card>& deck) {
    std::cout << "Sorting deck sequentially..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    sortDeckSequential(deck);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Sequential sorting time: " << elapsed.count() << " seconds." << std::endl;

    PerformanceMetrics::getMetrics("Sequential");

    std::cout << "Sample of the sorted deck (sequential):" << std::endl;
    printSample(deck, 50);
}

void runParallel(std::vector<Card>& deck) {
    std::cout << "Sorting deck in parallel..." << std::endl;

    int maxThreads = omp_get_max_threads();
    std::cout << "Maximum available threads: " << maxThreads << std::endl;

    auto startParallel = std::chrono::high_resolution_clock::now();
    sortDeckParallel(deck);
    auto endParallel = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsedParallel = endParallel - startParallel;
    std::cout << "Parallel sorting time: " << elapsedParallel.count() << " seconds." << std::endl;

    #pragma omp parallel
    {
        #pragma omp single
        {
            std::cout << "Number of threads used: " << omp_get_num_threads() << std::endl;
        }
    }

    PerformanceMetrics::getMetrics("Parallel");

    std::cout << "Sample of the sorted deck (parallel):" << std::endl;
    printSample(deck, 50);
}

int main() {
    int numCards = 4000000;
    std::cout << "Generating deck of " << numCards << " cards..." << std::endl;

    std::vector<Card> deck = generateDeck(numCards);

    std::cout << "Sample of the generated deck:" << std::endl;
    printSample(deck, 10);

    verifyDeck(deck, numCards);

    int choice;
    std::cout << "Select sorting type:" << std::endl;
    std::cout << "1. Sequential" << std::endl;
    std::cout << "2. Parallel" << std::endl;
    std::cout << "3. Compare both (sequential and parallel)" << std::endl;
    std::cout << "Enter your choice (1/2/3): ";
    std::cin >> choice;

    if (choice == 1) {
        runSequential(deck);
    } else if (choice == 2) {
        runParallel(deck);
    } else if (choice == 3) {
        std::vector<Card> deckSequential = deck;
        std::vector<Card> deckParallel = deck;

        std::cout << "\nRunning sequential sorting..." << std::endl;
        runSequential(deckSequential);

        std::cout << "\nRunning parallel sorting..." << std::endl;
        runParallel(deckParallel);
    } else {
        std::cout << "Invalid option. Program will exit." << std::endl;
    }

    return 0;
}
