# A novelty-search approach to filling an instance-space with diverse and discriminatory instances for the knapsack problem

## Authors:

* Alejandro Marrero - amarrerd@ull.edu.es
* Eduardo Segredo - esegredo@ull.edu.es
* Coromoto León - cleon@ull.edu.es
* Emma Hart - e.hart@napier.ac.uk

## Abstract:
We propose a new approach to generating synthetic instances in the knapsack domain in order to fill an instance-space. The method uses a novelty-search algorithm to search for instances that are diverse with respect to a feature-space but also elicit discriminatory performance from a set of target solvers. We demonstrate that a single run of the algorithm per target solver provides discriminatory instances and broad coverage of the feature-space. Furthermore, the instances also show diversity within the performance-space, despite the fact this is not explicitly evolved for, i.e. for a given ‘winning solver’, the magnitude of the performance-gap between it and other solvers varies across a wide-range. The method therefore provides a rich instance-space which can be used to analyse algorithm strengths/weaknesses, conduct algorithm-selection or construct a portfolio solver.
