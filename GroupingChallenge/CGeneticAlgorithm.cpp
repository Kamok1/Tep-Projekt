﻿#include "CGeneticAlgorithm.h"
#include "CIndividual.h"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace NGroupingChallenge;

CGeneticAlgorithm::CGeneticAlgorithm(int iPopulationSize, double dCrossoverProbabilty,
    double dMutationProbability, int iNumIterations, mt19937& cSharedRandomEngine, CGroupingEvaluator& pcEvaluator)
    : i_population_size(iPopulationSize),
    d_crossover_probability(dCrossoverProbabilty),
    d_mutation_probability(dMutationProbability),
    i_num_iterations(iNumIterations),
    c_random_engine(cSharedRandomEngine),
    pc_evaluator(pcEvaluator),
    c_best_individual(cSharedRandomEngine, pcEvaluator),
    d_best_fitness(numeric_limits<double>::max()){}

CGeneticAlgorithm::~CGeneticAlgorithm()
{
    std::for_each(v_population.begin(), v_population.end(), [](CIndividual* individual) { delete individual; });
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

        int i_candidate1 = c_distribution(c_random_engine);
        int i_candidate2 = c_distribution(c_random_engine);

        CIndividual* c_parent = cSelectBetterIndividual(v_population[i_candidate1], v_population[i_candidate2]);
        v_parents.push_back(c_parent);
    }

    return v_parents;
}

void CGeneticAlgorithm::vGenerateNewPopulation(const std::vector<CIndividual*>& vParents)
{
    std::vector<CIndividual*> new_v_population;
    for (size_t i = 0; i < vParents.size(); i += i_DEFAULT_PARENTS_SIZE)
    {
        CIndividual* parent1 = vParents[i];
        CIndividual* parent2 = (i + i_DEFAULT_NEXT_PARENT_INDEX < vParents.size()) ? vParents[i + i_DEFAULT_NEXT_PARENT_INDEX] : vParents[i];

        std::pair<CIndividual*, CIndividual*> children = parent1->cCrossover(*parent2, d_crossover_probability);
        children.first->vMutate(d_mutation_probability);
        children.second->vMutate(d_mutation_probability);

        new_v_population.push_back(children.first);
        new_v_population.push_back(children.second);
    }
    std::for_each(v_population.begin(), v_population.end(), [](CIndividual* individual) { delete individual; });
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
        v_parents.clear();
    }
}

CIndividual& CGeneticAlgorithm::cGetBestIndividual()
{
    return c_best_individual;
}
