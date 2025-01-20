#include "GaussianGroupingEvaluatorFactory.h"
#include "GroupingEvaluator.h"
#include <iostream>
#include <vector>
#include "CGeneticAlgorithm.h"

using namespace std;
using namespace NGroupingChallenge;

#define NUMBER_OF_GROUPS 20
#define NUMBER_OF_POINTS 100
#define POPULATION_SIZE 10
#define LOWER_BOUND 1
#define UPPER_BOUND NUMBER_OF_GROUPS
#define MUTATION_PROBABILITY 0.2
#define CROSSOVER_PROBABILITY 0.8
#define NUMBER_OF_ITERATIONS 1000
#define DIMENSIONS 6
#define DIMENSION_MEAN_MIN -100
#define DIMENSION_MEAN_MAX 100
#define DIMENSION_STANDARD_DEVIATION_MIN 1.0
#define DIMENSION_STANDARD_DEVIATION_MAX 1.0
#define BEST_GENES_MESSAGE "Best individual genes: "
#define BEST_FITNESS_MESSAGE "Best fitness: "
#define INVALID_BOUNDS_MESSAGE "Invalid bounds"
#define INVALID_PARAMETERS_MESSAGE "Invalid algorithm parameters"

void PrintBestIndividual(const CIndividual& c_best_individual)
{
    cout << BEST_GENES_MESSAGE;
    for (auto gene : c_best_individual.vGetGenes())
    {
        cout << gene << " ";
    }
    cout << endl;

    cout << BEST_FITNESS_MESSAGE << c_best_individual.dGetFitness() << endl;
}

int main()
{
    CGaussianGroupingEvaluatorFactory c_factory(NUMBER_OF_GROUPS, NUMBER_OF_POINTS);
    for (int i = 0; i < DIMENSIONS; i++)
    {
        c_factory.cAddDimension(DIMENSION_MEAN_MIN, DIMENSION_MEAN_MAX, DIMENSION_STANDARD_DEVIATION_MIN, DIMENSION_STANDARD_DEVIATION_MAX);
    }

    CGroupingEvaluator* pc_evaluator(c_factory.pcCreateEvaluator());

    if (LOWER_BOUND < 0 || UPPER_BOUND < 0 || UPPER_BOUND < LOWER_BOUND)
    {
		cerr << INVALID_BOUNDS_MESSAGE << endl;
		return 1;
	}

	if (POPULATION_SIZE <= 0 || NUMBER_OF_POINTS < 0 || MUTATION_PROBABILITY > 1 || NUMBER_OF_ITERATIONS <= 0)
	{
        cerr << INVALID_PARAMETERS_MESSAGE << endl;
        return 1;
	}

    CGeneticAlgorithm c_algorithm(POPULATION_SIZE, NUMBER_OF_POINTS, LOWER_BOUND, UPPER_BOUND, CROSSOVER_PROBABILITY, MUTATION_PROBABILITY, NUMBER_OF_ITERATIONS, &c_factory.cGetRandomEngine());
    c_algorithm.vSetEvaluator(*pc_evaluator);
    c_algorithm.vRun();     

    const CIndividual& c_best_individual = c_algorithm.cGetBestIndividual();
    delete pc_evaluator;

    PrintBestIndividual(c_best_individual);

    return 0;
}
