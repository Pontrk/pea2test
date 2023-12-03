#include <iostream>
#include "adjacency_matrix.h"
#include "SimulatedAnnealing.h"

int main() {
    Adjacency_Matrix graf;
    graf.loadFromFile("ftv55.atsp");
    int timeLimit = 120; // Time limit in seconds
    double coolingRate = 0.95; // Cooling rate for the annealing process

    // Create a SimulatedAnnealing instance
    SimulatedAnnealing sa(graf, timeLimit, coolingRate);

    // Run the algorithm
    sa.apply();
    return 0;
}
