#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <random>

#define DEFAULT_CROSSOVER_POINT 1
#define MIN_PROPABILITY 0.0
#define MAX_PROPABILITY 1.0
#define DEFAULT_FITNESS 0.0
#define DEFAULT_UPPER_BOUND 0
#define DEFAULT_LOWER_BOUND 0
#define MIN_CROSSOVER_POINT 1
using namespace std;

namespace NGroupingChallenge
{
    class CIndividual
    {
    public:
        CIndividual();
        CIndividual(int iNumGenes, int iLowerBound, int iUpperBound, mt19937* cRandomEngine);
        CIndividual(const CIndividual& other);
        CIndividual& operator=(const CIndividual& other);
        void vMutate(double dMutationProbability);
        void vSetFitness(double dFitness);
        pair<CIndividual, CIndividual> cCrossover(const CIndividual& cOther) const;
        double dGetFitness() const;
        void vSetGenes(const vector<int>& vGenes);
        const vector<int>& vGetGenes() const;

    private:
        vector<int> v_genes;
        double d_fitness;
        int i_lower_bound;
        int i_upper_bound;
        mt19937* c_random_engine;
    };
}

#endif