#include "CIndividual.h"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace NGroupingChallenge;

CIndividual::CIndividual(std::mt19937& cRandomEngine, CGroupingEvaluator& pcEvaluator)
    : i_lower_bound(pcEvaluator.iGetLowerBound()),i_upper_bound(pcEvaluator.iGetUpperBound()), c_random_engine(cRandomEngine), d_fitness(d_DEFAULT_FITNESS), pc_evaluator(pcEvaluator)
    {
    if (i_lower_bound > i_upper_bound)
    {
        std::swap(i_lower_bound, i_upper_bound);
    }

    v_genes.resize(pcEvaluator.iGetNumberOfPoints());
    std::fill(v_genes.begin(), v_genes.end(), i_lower_bound);
}

CIndividual::CIndividual(const CIndividual& other)
    : v_genes(other.v_genes),
    d_fitness(other.d_fitness),
    i_lower_bound(other.i_lower_bound),
    i_upper_bound(other.i_upper_bound),
    c_random_engine(other.c_random_engine),
    pc_evaluator(other.pc_evaluator)
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

CIndividual::CIndividual(CIndividual&& other)
    : v_genes(std::move(other.v_genes)),
    d_fitness(other.d_fitness),
    i_lower_bound(other.i_lower_bound),
    i_upper_bound(other.i_upper_bound),
    c_random_engine(other.c_random_engine), 
    pc_evaluator(other.pc_evaluator)
{
    other.d_fitness = d_DEFAULT_FITNESS;
}

void CIndividual::vSetFitness(double dFitness)
{
    d_fitness = dFitness;
}

void CIndividual::vMutate(double dMutationProbability)
{
    std::uniform_real_distribution<double> c_probability_distribution(d_MIN_PROPABILITY, d_MAX_PROPABILITY);
    std::uniform_int_distribution<int> c_value_distribution(i_lower_bound, i_upper_bound);

    for (auto& gene : v_genes)
    {
        if (c_probability_distribution(c_random_engine) < dMutationProbability)
        {
            gene = c_value_distribution(c_random_engine);
        }
    }
}

std::pair<CIndividual*, CIndividual*> CIndividual::cCrossover(const CIndividual& cOther, double dCrossoverProbabilty) const
{
    std::uniform_real_distribution<double> c_probability_distribution(d_MIN_PROPABILITY, d_MAX_PROPABILITY);

    if (v_genes.empty() || cOther.v_genes.empty() || c_probability_distribution(c_random_engine) > dCrossoverProbabilty)
    {
        return { new CIndividual(*this), new CIndividual(cOther) };
    }

    std::uniform_int_distribution<size_t> c_point_distribution(i_MIN_CROSSOVER_POINT, v_genes.size() - 1);
    int i_crossover_point = c_point_distribution(c_random_engine);

    std::vector<int> v_child1_genes(v_genes.begin(), v_genes.begin() + i_crossover_point);
    std::vector<int> v_child2_genes(cOther.v_genes.begin(), cOther.v_genes.begin() + i_crossover_point);

    v_child1_genes.insert(v_child1_genes.end(), cOther.v_genes.begin() + i_crossover_point, cOther.v_genes.end());
    v_child2_genes.insert(v_child2_genes.end(), v_genes.begin() + i_crossover_point, v_genes.end());

    CIndividual* c_child1 = new CIndividual(c_random_engine, pc_evaluator);
    CIndividual* c_child2 = new CIndividual(c_random_engine, pc_evaluator);

    c_child1->vSetGenes(v_child1_genes);
    c_child2->vSetGenes(v_child2_genes);

    return { c_child1, c_child2 };
}



double CIndividual::dGetFitness()
{
    if (d_fitness == d_DEFAULT_FITNESS)
    {
        d_fitness = pc_evaluator.dEvaluate(v_genes);
    }
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
