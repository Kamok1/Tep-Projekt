#include "CGeneticAlgorithm.h"
#include "CIndividual.h"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace NGroupingChallenge;

CGeneticAlgorithm::CGeneticAlgorithm(int iPopulationSize, int iNumGenes, int iLowerBound, int iUpperBound, double dMutationProbability, int iNumIterations)
    : i_population_size(iPopulationSize),
    i_num_genes(iNumGenes),
    i_lower_bound(iLowerBound),
    i_upper_bound(iUpperBound),
    d_mutation_probability(dMutationProbability),
    i_num_iterations(iNumIterations),
    c_random_engine(random_device{}()),
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
    uniform_int_distribution<int> c_distribution(0, i_population_size - 1);

    while (v_parents.size() < i_population_size)
    {
        int i_first_candidate = c_distribution(c_random_engine);
        int i_second_candidate = c_distribution(c_random_engine);
        CIndividual* c_parent1 = cSelectBetterIndividual(v_population[i_first_candidate], v_population[i_second_candidate]);

        int i_third_candidate = c_distribution(c_random_engine);
        int i_fourth_candidate = c_distribution(c_random_engine);
        CIndividual* c_parent2 = cSelectBetterIndividual(v_population[i_third_candidate], v_population[i_fourth_candidate]);

        v_parents.push_back(c_parent1);
        v_parents.push_back(c_parent2);
    }

    return v_parents;
}


void CGeneticAlgorithm::vGenerateNewPopulation(const std::vector<CIndividual*>& vParents)
{
    std::vector<CIndividual*> new_v_population;

    for (size_t i = 0; i < vParents.size(); i += 2)
    {
        if (i + 1 >= vParents.size()) break; //wywalic tego break
        std::pair<CIndividual, CIndividual> children = vParents[i]->cCrossover(*vParents[i + 1]);
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
