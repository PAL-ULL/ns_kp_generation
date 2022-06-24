/**
 * @file Genetic_KNP.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief Code for solving all the KP instances in a specific directory with one
 * GA configuration
 * @version 0.1
 * @date 27/11/19.
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <fmt/core.h>
#include <turing/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <turing/builders/ExpBuilder.h>
#include <turing/builders/ParGABuilder.h>
#include <turing/core/abstract_algorithms/AbstractEvolutionaryAlgorithm.h>
#include <turing/core/abstract_algorithms/AbstractGeneticAlgorithm.h>
#include <turing/core/solutions/Front.h>
#include <turing/crossovers/UniformCrossover.h>
#include <turing/mutations/UniformOneMutation.h>
#include <turing/problems/KnapsackProblemNR.h>
#include <turing/selections/BinaryTournamentSelection.h>
#include <turing/utilities/printer/JSONPrinter.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
namespace fs = std::filesystem;

using S = BoolFloatSolution;
using P = Problem<S>;
using KP = KnapsackProblemNR;
using EA = AbstractEvolutionaryAlgorithm<S>;
using GA = ParallelGeneticAlgorithm<S>;
using ParGA = ParallelGeneticAlgorithm<S>;

int main(int argc, char **argv) {
    auto nArgs = 5;
    if (argc != nArgs) {
        cerr << "Error in args. <path-to-instances> <path-to-results> "
                "<crossrate> <pop-size>"
             << endl;
        exit(EXIT_FAILURE);
    }
    // Parametros del experimento
    auto repetitions = 10;
    auto evaluation = 1e5;
    auto cores = 32;
    string pathToInstance(argv[1]);
    string pathToResults(argv[2]);
    float crossRate = stof(argv[3]);
    auto popsize = stoi(argv[4]);

    for (auto &entry : fs::recursive_directory_iterator(pathToInstance)) {
        if (!entry.is_directory()) {
            // Problema a resolver
            unique_ptr<P> problem = make_unique<KP>(entry.path());
            unique_ptr<P> expProblem = make_unique<KP>(entry.path());
            std::cout << "Instance: " << entry.path() << std::endl;

            // Probabilidad de cruce como 1 / N.
            double mutRate = 1.0 / 100;  // El instanceSize en MEA es N * 2
            string outputFile =
                "GA_" + to_string(crossRate) + entry.path().filename().string();
            unique_ptr<ParGA> algorithm = ParGABuilder<S>::create()
                                              .usingCores(cores)
                                              .toSolve(move(problem))
                                              .with()
                                              .crossover(CXType::Uniform)
                                              .mutation(MutType::UniformOne)
                                              .selection(SelType::Binary)
                                              .withMutRate(mutRate)
                                              .withCrossRate(crossRate)
                                              .with()
                                              .populationOf(popsize)
                                              .runDuring(evaluation);

            unique_ptr<Experiment<S>> experiment =
                ExpBuilder<S>::create(outputFile)
                    .saveResultsIn(pathToResults)
                    .usingAlgorithm(move(algorithm))
                    .toSolve(move(expProblem))
                    .repeat(repetitions);
            experiment->run();
            json data = experiment->to_json();
            auto printer = make_unique<JSONPrinter>("printer");
            printer->print(outputFile, data);
        }
    }

    return EXIT_SUCCESS;
}
