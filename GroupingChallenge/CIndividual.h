#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <random>
#include "SharedValues.h"
#include "GroupingEvaluator.h"
using namespace std;

namespace NGroupingChallenge
{
    class CIndividual
    {
    public:
        CIndividual(mt19937& cRandomEngine, CGroupingEvaluator& pcEvaluator);
        CIndividual(const CIndividual& other);
        CIndividual& operator=(const CIndividual& other);
        void vMutate(double dMutationProbability);
        void vSetFitness(double dFitness);
        pair<CIndividual, CIndividual> cCrossover(const CIndividual& cOther, double dCrossoverProbabilty) const;
        void vSetGenes(const vector<int>& vGenes);
        const vector<int>& vGetGenes() const;
        double dGetFitness();

    private:
        const int i_DEFAULT_CROSSOVER_POINT = 1;
        const double d_DEFAULT_FITNESS = std::numeric_limits<double>::max();
        const int i_DEFAULT_UPPER_BOUND = 10;
        const int i_DEFAULT_LOWER_BOUND = 0;
        const int i_MIN_CROSSOVER_POINT = 1;

        vector<int> v_genes;
        double d_fitness;
        int i_lower_bound;
        int i_upper_bound;
        mt19937& c_random_engine;
        CGroupingEvaluator& pc_evaluator;

    };
}

#endif