#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "GroupingEvaluator.h"
#include <vector>
#include <random>
#include "CIndividual.h"

using namespace std;

namespace NGroupingChallenge
{
    class CGeneticAlgorithm
    {
    public:
        CGeneticAlgorithm(int iPopulationSize, int iNumGenes, int iLowerBound, int iUpperBound, double dMutationProbability, int iNumIterations);
        ~CGeneticAlgorithm();
        void vRun();
        const CIndividual& cGetBestIndividual() const;
        void vSetEvaluator(CGroupingEvaluator& cEvaluator);

    private:
        void vInitializePopulation();
        void vEvaluatePopulation();
        vector<CIndividual*> vSelectParents();
        void vGenerateNewPopulation(const vector<CIndividual*>& vParents);
        CIndividual* cSelectBetterIndividual(CIndividual* cIndividual1, CIndividual* cIndividual2) const;

        int i_population_size;
        int i_num_genes;
        int i_lower_bound;
        int i_upper_bound;
        double d_mutation_probability;
        int i_num_iterations;

        std::vector<CIndividual*> v_population;
        CIndividual c_best_individual;
        double d_best_fitness;
        mt19937 c_random_engine;
        CGroupingEvaluator* pc_evaluator;

    };
}

#endif
