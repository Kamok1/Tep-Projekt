#include "CIndividual.h"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace NGroupingChallenge;

CIndividual::CIndividual(int iNumGenes, int iLowerBound, int iUpperBound, mt19937& cRandomEngine)
    : i_lower_bound(iLowerBound), i_upper_bound(iUpperBound), c_random_engine(&cRandomEngine)
{
    uniform_int_distribution<int> c_distribution(iLowerBound, iUpperBound); //tutaj sprdzenie które jest wieksze
    v_genes.resize(iNumGenes);
    generate(v_genes.begin(), v_genes.end(), [&]() { return c_distribution(*c_random_engine); });
    d_fitness = 0.0;
}


CIndividual::CIndividual(): d_fitness(0.0), i_lower_bound(0), i_upper_bound(0), c_random_engine(c_random_engine) {}

CIndividual::CIndividual(const CIndividual& other): v_genes(other.v_genes), d_fitness(other.d_fitness),
i_lower_bound(other.i_lower_bound), i_upper_bound(other.i_upper_bound),c_random_engine(other.c_random_engine) {}


CIndividual& CIndividual::operator=(const CIndividual& other)
{
    if (this != &other)
    {
        v_genes = other.v_genes;
        d_fitness = other.d_fitness;
        i_lower_bound = other.i_lower_bound;
        i_upper_bound = other.i_upper_bound;
        c_random_engine = other.c_random_engine;
    }
    return *this;
}

void CIndividual::vSetFitness(double dFitness)
{
    d_fitness = dFitness;
}


void CIndividual::vMutate(double dMutationProbability){
    uniform_real_distribution<double> c_probability_distribution(0.0, 1.0); //to do constow
    uniform_int_distribution<int> c_value_distribution(i_lower_bound, i_upper_bound);

    for (auto& gene : v_genes)
    {
        if (c_probability_distribution(*c_random_engine) < dMutationProbability)
        {
            gene = c_value_distribution(*c_random_engine);
        }
    }
}

std::pair<CIndividual, CIndividual> CIndividual::cCrossover(const CIndividual& cOther) const{
    uniform_int_distribution<int> c_point_distribution(1, static_cast<int>(v_genes.size()) - 1);
    int i_crossover_point = c_point_distribution(*c_random_engine);

    vector<int> v_child1_genes(v_genes.begin(), v_genes.begin() + i_crossover_point);
    vector<int> v_child2_genes(cOther.v_genes.begin(), cOther.v_genes.begin() + i_crossover_point);

    v_child1_genes.insert(v_child1_genes.end(), cOther.v_genes.begin() + i_crossover_point, cOther.v_genes.end());
    v_child2_genes.insert(v_child2_genes.end(), v_genes.begin() + i_crossover_point, v_genes.end());

    CIndividual c_child1(static_cast<int>(v_genes.size()), i_lower_bound, i_upper_bound, *c_random_engine);
    CIndividual c_child2(static_cast<int>(v_genes.size()), i_lower_bound, i_upper_bound, *c_random_engine);

    c_child1.vSetGenes(v_child1_genes);
    c_child2.vSetGenes(v_child2_genes);

    return { c_child1, c_child2 };
}


double CIndividual::dGetFitness() const{
    return d_fitness;
}

void CIndividual::vSetGenes(const vector<int>& vGenes){
    v_genes = vGenes;
}

const std::vector<int>& CIndividual::vGetGenes() const{
    return v_genes;
}
