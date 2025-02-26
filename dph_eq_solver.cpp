#include <iostream>
#include "dph_eq_solver.h"
#include <string>

int main()
{
    const int GEN_SIZE = 50;
    const int EQ_SIZE = 4;
    const int coefs[5] = {1, 2, 3, 4, -30};
    const int iters_number = 20;
    const double mutation_prob = 0.5;
    
    double parents_fitness[GEN_SIZE];
    double children_fitness[GEN_SIZE];
    int best_individ[EQ_SIZE];
    std::string gen_init_mode = "random";
    int **old_generation = init_dyn_matrix(GEN_SIZE, EQ_SIZE, gen_init_mode);

    for(int i=0; i<iters_number; i++)
    {
        std::cout << std::endl << "Iteration [" << i << "]:" << std::endl;
        generation_fitness(old_generation, parents_fitness, GEN_SIZE, coefs);
        std::cout << "Current fitness coefs: ";
        print_1darray<double>(parents_fitness, GEN_SIZE);
        bool opt_sol = search_best_individ(old_generation, parents_fitness, best_individ, GEN_SIZE, EQ_SIZE);
        if (opt_sol)
        {
            std::cout << "!!! Optimal solution: ";
            print_1darray<int>(best_individ, EQ_SIZE);
            break;
        }    
        else
        {
            std::cout << "Current best individ: ";
            print_1darray<int>(best_individ, EQ_SIZE);
            int** new_generation = create_new_generation(old_generation, parents_fitness, coefs, GEN_SIZE, EQ_SIZE);
            generation_fitness(new_generation, children_fitness, GEN_SIZE, coefs);
            if(mean_fitness(children_fitness, GEN_SIZE) > mean_fitness(parents_fitness, GEN_SIZE))
                std::cout << "New generation has worse fitness than previous generation. Initialize mutation protocol..." << std::endl; 
                generation_mutation(new_generation, mutation_prob, GEN_SIZE, EQ_SIZE);
            old_generation = new_generation;
        }
    }

    std::cout << "Program complete." << std::endl;
    return 0;
}