﻿#include "CGeneticAlgorithm.h"
#include "CIndividual.h"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace NGroupingChallenge;

CGeneticAlgorithm::CGeneticAlgorithm(int iPopulationSize, int iNumGenes, int iLowerBound, int iUpperBound, double dMutationProbability, int iNumIterations, mt19937* cSharedRandomEngine)
    : i_population_size(iPopulationSize),
    i_num_genes(iNumGenes),
    i_lower_bound(iLowerBound),
    i_upper_bound(iUpperBound),
    d_mutation_probability(dMutationProbability),
    i_num_iterations(iNumIterations),
    c_random_engine(cSharedRandomEngine),
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
        v_population.push_back(new CIndividual(i_num_genes, i_lower_bound, i_upper_bound, c_random_engine));
    }
}


void CGeneticAlgorithm::vSetEvaluator(CGroupingEvaluator& cEvaluator)
{
    pc_evaluator = &cEvaluator;
}

void CGeneticAlgorithm::vEvaluatePopulation()
{
    for (auto& individual : v_population)
    {
        double d_fitness = pc_evaluator->dEvaluate(individual->vGetGenes());
        individual->vSetFitness(d_fitness);
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

    for (size_t i = 0; i < vParents.size(); i += DEFAULT_PARENTS_SIZE)
    {
        CIndividual* parent1 = vParents[i];
        CIndividual* parent2 = (i + DEFAULT_NEXT_PARENT_INDEX < vParents.size()) ? vParents[i + DEFAULT_NEXT_PARENT_INDEX] : vParents[i];

        std::pair<CIndividual, CIndividual> children = parent1->cCrossover(*parent2);
        children.first.vMutate(d_mutation_probability);
        children.second.vMutate(d_mutation_probability);

        new_v_population.push_back(new CIndividual(std::move(children.first)));
        new_v_population.push_back(new CIndividual(std::move(children.second)));
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

const CIndividual& CGeneticAlgorithm::cGetBestIndividual() const
{
    return c_best_individual;
}
