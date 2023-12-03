#include "SimulatedAnnealing.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <cmath>
#include <random>
#include <climits>
#include <chrono>

SimulatedAnnealing::SimulatedAnnealing(Adjacency_Matrix& graph, int time, double rate) {
    matrix = graph.getMatrix();
    size = matrix.size();
    timeBound = time;
    coolingRate = rate;
    temperatureBuffer = calculateTemperature();
    std::vector<int> initialPath = greedyPath();

}

void SimulatedAnnealing::apply()
{
    vector<int> best = greedyPath();
    vector<int> permutation = best;
    vector<int> next(permutation);
    std::clock_t start;
    int firstToSwap;
    int secondToSwap;
    double temperature = temperatureBuffer;
    int result = calculatePath(best);;
    double time = 0;
    double foundTime = 0;
    start = std::clock();
    while(true)
    {
        while (temperature >= 0.1)
        {
            int steps =  3*size;
            next = permutation;
            int nextCost = calculatePath(next);
            for (int i = steps; i > 0; i--)
            {
                do
                {
                    firstToSwap = rand() % size;
                    secondToSwap = rand() % size;
                } while (firstToSwap == secondToSwap);
                std::swap(next[firstToSwap],next[secondToSwap]);
                nextCost = calculatePath(next);
                int difference = result - nextCost;
                if (difference > 0)
                {
                    result = nextCost;
                    best = next;
                    foundTime = (std::clock() - start) / (double)CLOCKS_PER_SEC;
                }
                if (difference > 0 || (difference < 0 && getProbability(difference,temperature) > ((double)rand() / RAND_MAX) + 1))
                {
                    permutation = next;
                    break;
                }
                else
                {
                    std::swap(next[firstToSwap], next[secondToSwap]);
                }
                time = (std::clock() - start) / (double)CLOCKS_PER_SEC;
                if (time >= timeBound)
                {
                    cout << "Droga: ";
                    for (int d = 0; d < size; d++)
                    {
                        cout << best[d] << " ";
                    }
                    cout << "\nKoszt: " << result << endl;
                    cout << "Znaleziono po: " << foundTime << " s " << endl;
                    cout << "Temperatura koncowa: "<< temperature << endl;
                    cout << endl;
                    return;
                }
            }
            temperature *= coolingRate;
        }
        temperature = temperatureBuffer;
        permutation = random_permutation(size);
    }
}

vector<int> SimulatedAnnealing::random_permutation(int _size)
{
    std::vector<int> temp;
    temp.reserve(_size);
    for (int i = 0; i < _size; i++)
    {
        temp.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(temp.begin(), temp.end(), g);
    return temp;
}

double SimulatedAnnealing::calculateTemperature()
{
    vector<int> origin;
    int firstToSwap;
    int secondToSwap;
    int delta = 0;
    int buffer = 0;
    for (int i = 0; i < 10000; i++)
    {
        do
        {
            firstToSwap = rand() % size;
            secondToSwap = rand() % size;
        } while (firstToSwap == secondToSwap);
        origin = random_permutation(size);
        vector<int> neighbour(origin);
        std::swap(neighbour[firstToSwap], neighbour[secondToSwap]);
        delta = fabs(calculatePath(origin) - calculatePath(neighbour));
        buffer += delta;
    }
    buffer /= 10000;
    return (-1*buffer)/log(0.99);
}

int SimulatedAnnealing::calculatePath(vector<int> path)
{
    int cost = 0;
    for (int i = 0; i < path.size() - 1; ++i)
    {
        cost += matrix[path[i]][path[i + 1]];
    }
    cost += matrix[path[size - 1]][path[0]];
    return cost;
}

double SimulatedAnnealing::getProbability(int diff, double temperature)
{
    return exp(diff / temperature);
}

std::vector<int> SimulatedAnnealing::greedyPath() {
    std::vector<int> path;
    std::vector<bool> visited(size, false);
    // Start from a random vertex
    int current = rand() % size;
    path.push_back(current);
    visited[current] = true;
    int totalCost = 0;
    for (int i = 1; i < size; ++i) {
        int nearest = -1;
        int minDistance = INT_MAX;

        for (int j = 0; j < size; ++j) {
            if (!visited[j] && matrix[current][j] < minDistance) {
                nearest = j;
                minDistance = matrix[current][j];
            }
        }

        if (nearest != -1) {
            path.push_back(nearest);
            visited[nearest] = true;
            totalCost += minDistance;
            current = nearest;
        }
    }

    // Add the cost to return to the starting node
    totalCost += matrix[current][path[0]];

    // Print the greedy path and its total cost
    std::cout << "Greedy Path: ";
    for (int node : path) {
        std::cout << node << " ";
    }
    std::cout << "\nTotal Cost of Greedy Path: " << totalCost << std::endl;

    return path;
}
SimulatedAnnealing::~SimulatedAnnealing()
{
}