#include "CIndividual.h"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace NGroupingChallenge;

CIndividual::CIndividual(int iNumGenes, int iLowerBound, int iUpperBound, std::mt19937* cRandomEngine)
    : i_lower_bound(iLowerBound),
    i_upper_bound(iUpperBound),
    c_random_engine(cRandomEngine),
    d_fitness(DEFAULT_FITNESS)
{
    if (i_lower_bound > i_upper_bound)
    {
        std::swap(i_lower_bound, i_upper_bound);
    }

    v_genes.resize(iNumGenes);

    if (!c_random_engine)
    {
        std::fill(v_genes.begin(), v_genes.end(), i_lower_bound);
    }
    else
    {
        std::uniform_int_distribution<int> c_distribution(i_lower_bound, i_upper_bound);
        std::generate(v_genes.begin(), v_genes.end(), [&]() { return c_distribution(*c_random_engine); });
    }
}

CIndividual::CIndividual()
    : d_fitness(DEFAULT_FITNESS), i_lower_bound(DEFAULT_LOWER_BOUND), i_upper_bound(DEFAULT_UPPER_BOUND), c_random_engine(nullptr){}

CIndividual::CIndividual(const CIndividual& other)
    : v_genes(other.v_genes),
    d_fitness(other.d_fitness),
    i_lower_bound(other.i_lower_bound),
    i_upper_bound(other.i_upper_bound),
    c_random_engine(other.c_random_engine)
{
}

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

void CIndividual::vMutate(double dMutationProbability)
{
    if (!c_random_engine)
    {
        return;
    }

    std::uniform_real_distribution<double> c_probability_distribution(MIN_PROPABILITY, MAX_PROPABILITY);
    std::uniform_int_distribution<int> c_value_distribution(i_lower_bound, i_upper_bound);

    for (auto& gene : v_genes)
    {
        if (c_probability_distribution(*c_random_engine) < dMutationProbability)
        {
            gene = c_value_distribution(*c_random_engine);
        }
    }
}

std::pair<CIndividual, CIndividual> CIndividual::cCrossover(const CIndividual& cOther) const
{
    if (v_genes.empty() || cOther.v_genes.empty())
    {
        return { *this, cOther };
    }

    int i_crossover_point = DEFAULT_CROSSOVER_POINT;
    if (c_random_engine)
    {
        std::uniform_int_distribution<size_t> c_point_distribution(MIN_CROSSOVER_POINT, v_genes.size() - 1);
        i_crossover_point = c_point_distribution(*c_random_engine);
    }

    std::vector<int> v_child1_genes(v_genes.begin(), v_genes.begin() + i_crossover_point);
    std::vector<int> v_child2_genes(cOther.v_genes.begin(), cOther.v_genes.begin() + i_crossover_point);

    v_child1_genes.insert(v_child1_genes.end(), cOther.v_genes.begin() + i_crossover_point, cOther.v_genes.end());
    v_child2_genes.insert(v_child2_genes.end(), v_genes.begin() + i_crossover_point, v_genes.end());

    CIndividual c_child1(static_cast<int>(v_genes.size()), i_lower_bound, i_upper_bound, c_random_engine);
    CIndividual c_child2(static_cast<int>(v_genes.size()), i_lower_bound, i_upper_bound, c_random_engine);

    c_child1.vSetGenes(v_child1_genes);
    c_child2.vSetGenes(v_child2_genes);

    return { c_child1, c_child2 };
}

double CIndividual::dGetFitness() const
{
    return d_fitness;
}

void CIndividual::vSetGenes(const std::vector<int>& vGenes)
{
    v_genes = vGenes;
}

const std::vector<int>& CIndividual::vGetGenes() const
{
    return v_genes;
}
