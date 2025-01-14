#include "GaussianGroupingEvaluatorFactory.h"
#include "GroupingEvaluator.h"
#include <iostream>
#include <vector>
#include "CGeneticAlgorithm.h"
using namespace std;
using namespace NGroupingChallenge;

int main()
{
    //przerobić żeby był jeden 
    //popdawac define zamist hardcowania wartosci
    int iNumberOfGroups = 10;
    int iNumberOfPoints = 20;
    CGaussianGroupingEvaluatorFactory c_factory(iNumberOfGroups, iNumberOfPoints);

    c_factory
        .cAddDimension(-100, 100, 1.0, 1.0)
        .cAddDimension(-100, 100, 1.0, 1.0)
        .cAddDimension(-100, 100, 1.0, 1.0)
        .cAddDimension(-100, 100, 1.0, 1.0)
        .cAddDimension(-100, 100, 1.0, 1.0)
        .cAddDimension(-100, 100, 1.0, 1.0);
        
    CGroupingEvaluator* pc_evaluator = c_factory.pcCreateEvaluator();

    int iPopulationSize = 10;
    int iNumGenes = iNumberOfPoints;
    int iLowerBound = 1;
    int iUpperBound = iNumberOfGroups;
    double dMutationProbability = 0.2;
    int iNumIterations = 10;

    CGeneticAlgorithm c_algorithm(iPopulationSize, iNumGenes, iLowerBound, iUpperBound, dMutationProbability, iNumIterations);
    c_algorithm.vSetEvaluator(*pc_evaluator);
    c_algorithm.vRun();

    const CIndividual& c_best_individual = c_algorithm.cGetBestIndividual();
    cout << "Best individual genes: ";
    for (auto gene : c_best_individual.vGetGenes())
    {
        cout << gene << " ";
    }
    cout << endl;

    cout << "Best fitness: " << c_best_individual.dGetFitness() << endl;

    delete pc_evaluator;

    return 0;
}
