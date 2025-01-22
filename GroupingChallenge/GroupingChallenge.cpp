#include "GaussianGroupingEvaluatorFactory.h"
#include "GroupingEvaluator.h"
#include <iostream>
#include <vector>
#include "CGeneticAlgorithm.h"

using namespace std;
using namespace NGroupingChallenge;

const int i_NUMBER_OF_GROUPS = 5;
const int i_NUMBER_OF_POINTS = 30;
const int i_POPULATION_SIZE = 20;
const int i_NUMBER_OF_ITERATIONS = 20000;
const int i_DIMENSIONS = 3;
const int i_DIMENSION_MEAN_MIN = -100;
const int i_DIMENSION_MEAN_MAX = 100;

const double d_MUTATION_PROBABILITY = 0.2;
const double d_CROSSOVER_PROBABILITY = 0.8;
const double d_DIMENSION_STANDARD_DEVIATION_MIN = 1.0;
const double d_DIMENSION_STANDARD_DEVIATION_MAX = 1.0;

const string s_BEST_GENES_MESSAGE = "Best individual genes: ";
const string s_BEST_FITNESS_MESSAGE = "Best fitness: ";
const string s_INVALID_PARAMETERS_MESSAGE = "Invalid algorithm parameters";

void PrintBestIndividual(CIndividual& c_best_individual)
{
    cout << s_BEST_GENES_MESSAGE;
    for (auto gene : c_best_individual.vGetGenes())
    {
        cout << gene << " ";
    }
    cout << endl;

    cout << s_BEST_FITNESS_MESSAGE << c_best_individual.dGetFitness() << endl;
}

int main()
{
    CGaussianGroupingEvaluatorFactory c_factory(i_NUMBER_OF_GROUPS, i_NUMBER_OF_POINTS);
    for (int i = 0; i < i_DIMENSIONS; i++)
    {
        c_factory.cAddDimension(i_DIMENSION_MEAN_MIN, i_DIMENSION_MEAN_MAX, d_DIMENSION_STANDARD_DEVIATION_MIN, d_DIMENSION_STANDARD_DEVIATION_MAX);
    }

    CGroupingEvaluator* pc_evaluator(c_factory.pcCreateEvaluator());


    if (i_POPULATION_SIZE <= 0 || i_NUMBER_OF_POINTS < 0 || d_MUTATION_PROBABILITY > 1 || i_NUMBER_OF_ITERATIONS <= 0)
    {
        cerr << s_INVALID_PARAMETERS_MESSAGE << endl;
        return 1;
    }

    CGeneticAlgorithm c_algorithm(
        i_POPULATION_SIZE,
        d_CROSSOVER_PROBABILITY,
        d_MUTATION_PROBABILITY,
        i_NUMBER_OF_ITERATIONS,
        c_factory.cGetRandomEngine(),
        *pc_evaluator
    );
    c_algorithm.vRun();

    CIndividual& c_best_individual = c_algorithm.cGetBestIndividual();
    PrintBestIndividual(c_best_individual);
    delete pc_evaluator;

    return 0;
}
