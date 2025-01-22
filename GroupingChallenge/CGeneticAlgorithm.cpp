#include "CGeneticAlgorithm.h"
#include "CIndividual.h"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace NGroupingChallenge;

CGeneticAlgorithm::CGeneticAlgorithm(int iPopulationSize, int iNumGenes, int iLowerBound, int iUpperBound, double dCrossoverProbabilty,
    double dMutationProbability, int iNumIterations, mt19937* cSharedRandomEngine, CGroupingEvaluator& pcEvaluator)
    : i_population_size(iPopulationSize),
    i_num_genes(iNumGenes),
    i_lower_bound(iLowerBound),
    i_upper_bound(iUpperBound),
    d_crossover_probability(dCrossoverProbabilty),
    d_mutation_probability(dMutationProbability),
    i_num_iterations(iNumIterations),
    c_random_engine(cSharedRandomEngine),
    pc_evaluator(pcEvaluator),
    c_best_individual(cSharedRandomEngine, pcEvaluator),
    d_best_fitness(numeric_limits<double>::max()){}

CGeneticAlgorithm::~CGeneticAlgorithm()
{
    for (auto* individual : v_population)
    {
        delete individual;
    }
    v_population.clear();
}


void CGeneticAlgorithm::vInitializePopulation()
{
    for (int i = 0; i < i_population_size; ++i)
    {
        v_population.push_back(new CIndividual(c_random_engine, pc_evaluator));
    }
}

void CGeneticAlgorithm::vEvaluatePopulation()
{
    for (auto& individual : v_population)
    {
        double d_fitness = individual->dGetFitness();
        if (d_fitness < d_best_fitness)
        {
            d_best_fitness = d_fitness;
            c_best_individual = *individual;
        }
    }
}

CIndividual* CGeneticAlgorithm::cSelectBetterIndividual(CIndividual* cIndividual1, CIndividual* cIndividual2) const
{
    return (cIndividual2->dGetFitness() < cIndividual1->dGetFitness()) ? cIndividual2 : cIndividual1;
}


std::vector<CIndividual*> CGeneticAlgorithm::vSelectParents()
{
    std::vector<CIndividual*> v_parents;
    std::uniform_int_distribution<int> c_distribution(0, i_population_size - 1);

    while (v_parents.size() < i_population_size)
    { 

        int i_candidate1 = c_distribution(*c_random_engine);
        int i_candidate2 = c_distribution(*c_random_engine);

        CIndividual* c_parent = cSelectBetterIndividual(v_population[i_candidate1], v_population[i_candidate2]);
        v_parents.push_back(c_parent);
    }

    return v_parents;
}

void CGeneticAlgorithm::vGenerateNewPopulation(const std::vector<CIndividual*>& vParents)
{
    std::vector<CIndividual*> new_v_population;
    std::uniform_real_distribution<double> c_probability_distribution(d_MIN_PROPABILITY, d_MAX_PROPABILITY);

    for (size_t i = 0; i < vParents.size(); i += i_DEFAULT_PARENTS_SIZE)
    {
        CIndividual* parent1 = vParents[i];
        CIndividual* parent2 = (i + i_DEFAULT_NEXT_PARENT_INDEX < vParents.size()) ? vParents[i + i_DEFAULT_NEXT_PARENT_INDEX] : vParents[i];

        if (c_probability_distribution(*c_random_engine) < d_mutation_probability)
        {;
            new_v_population.push_back(new CIndividual(*parent1));
            new_v_population.push_back(new CIndividual(*parent2));
        }
        else
        {
            std::pair<CIndividual, CIndividual> children = parent1->cCrossover(*parent2);
            children.first.vMutate(d_mutation_probability);
            children.second.vMutate(d_mutation_probability);

            new_v_population.push_back(new CIndividual(std::move(children.first)));
            new_v_population.push_back(new CIndividual(std::move(children.second)));
        }
    }

    for (auto* individual : v_population)
    {
        delete individual;
    }
    v_population.clear();
    v_population = std::move(new_v_population);
}



void CGeneticAlgorithm::vRun()
{
    vInitializePopulation();
    for (int i = 0; i < i_num_iterations; ++i)
    {
        vEvaluatePopulation();
        auto v_parents = vSelectParents();
        vGenerateNewPopulation(v_parents);
    }
}

CIndividual& CGeneticAlgorithm::cGetBestIndividual()
{
    return c_best_individual;
}
