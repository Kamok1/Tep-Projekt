#include "GaussianGroupingEvaluatorFactory.h"
#include "GroupingEvaluator.h"
#include <iostream>
#include <vector>
#include "CGeneticAlgorithm.h"

using namespace std;
using namespace NGroupingChallenge;

#define NUMBER_OF_GROUPS 10
#define NUMBER_OF_POINTS 20
#define POPULATION_SIZE 10
#define LOWER_BOUND 1
#define UPPER_BOUND NUMBER_OF_GROUPS
#define MUTATION_PROBABILITY 0.2
#define NUM_ITERATIONS 10
#define DIMENSIONS 6
#define DIMENSION_MEAN_MIN -100
#define DIMENSION_MEAN_MAX 100
#define DIMENSION_STANDARD_DEVIATION_MIN 1.0
#define DIMENSION_STANDARD_DEVIATION_MAX 1.0
#define BEST_GENES_MESSAGE "Best individual genes: "
#define BEST_FITNESS_MESSAGE "Best fitness: "

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
    int iNumberOfGroups = NUMBER_OF_GROUPS;
    int iNumberOfPoints = NUMBER_OF_POINTS;
    int iPopulationSize = POPULATION_SIZE;
    int iNumGenes = NUMBER_OF_POINTS;
    int iLowerBound = LOWER_BOUND;
    int iUpperBound = UPPER_BOUND;
    double dMutationProbability = MUTATION_PROBABILITY;
    int iNumIterations = NUM_ITERATIONS;

    CGaussianGroupingEvaluatorFactory c_factory(iNumberOfGroups, iNumberOfPoints);
    for (int i = 0; i < DIMENSIONS; i++)
    {
        c_factory.cAddDimension(DIMENSION_MEAN_MIN, DIMENSION_MEAN_MAX, DIMENSION_STANDARD_DEVIATION_MIN, DIMENSION_STANDARD_DEVIATION_MAX);
    }

    CGroupingEvaluator* pc_evaluator(c_factory.pcCreateEvaluator());

    if (iLowerBound < 0 || iUpperBound < 0 || iUpperBound < iLowerBound)
    {
		cerr << "Invalid bounds" << endl;
		return 1;
	}

	if (iPopulationSize <= 0 || iNumGenes <= 0 || dMutationProbability < 0 || dMutationProbability > 1 || iNumIterations <= 0)
	{
        cerr << "Invalid algorithm parameters" << endl;
        return 1;
	}

    CGeneticAlgorithm c_algorithm(iPopulationSize, iNumGenes, iLowerBound, iUpperBound, dMutationProbability, iNumIterations, &c_factory.cGetRandomEngine());
    c_algorithm.vSetEvaluator(*pc_evaluator);
    c_algorithm.vRun();

    const CIndividual& c_best_individual = c_algorithm.cGetBestIndividual();
    delete pc_evaluator;

    PrintBestIndividual(c_best_individual);

    return 0;
}
