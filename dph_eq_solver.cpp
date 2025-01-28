#include <iostream>
#include "dph_eq_solver.h"

// ----- Same Function with different types 
void print_int2darray(int** arr, int nrows, int ncols)
{
    for(int i=0; i<nrows; i++)
    {
        std::cout << i << " row: ";
        for(int j=0; j<ncols; j++)
        {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void print_double2darray(double** arr, int nrows, int ncols)
{
    for(int i=0; i<nrows; i++)
    {
        std::cout << i << " row: ";
        for(int j=0; j<ncols; j++)
        {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void print_double1darray(double* arr, int nelems)
{
    std::cout << "Array = [";
    for(int i=0; i<nelems; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "]" << std::endl;
}
void print_int1darray(int* arr, int nelems)
{
    std::cout << "Array = [";
    for(int i=0; i<nelems; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "]" << std::endl;
}
// -----

void generation_fitness(int** generation, double* fitness, const int gen_size, const int* eq_coefs)
{
    for(int i=0; i<gen_size; i++)
    {
        fitness[i] = fitness_function(generation[i], eq_coefs);
    }
}

double mean_fitness(double* fitness, const int size)
{   
    double sum = 0;
    for(int i=0; i<size; i++)
    {
        sum += fitness[i];
    }
    return sum/size;
}

bool search_best_individ(int** generation, double* fitness, int* best_individ, const int gen_size, const int chronom_len)
{
    int max_fit = 1000; 
    int argmax_fit;
    bool opt_solution = false;
    for(int i=0; i<gen_size; i++)
    {
        if(fitness[i] < max_fit)
        {
            max_fit = fitness[i];
            argmax_fit = i;
        }
        if(max_fit == 0)
        {
            opt_solution = true;
            break;
        }
    }

    for(int i=0;i<chronom_len; i++)
    {
        best_individ[i] = generation[argmax_fit][i];
    }
    return opt_solution;
}

int main()
{
    const int GEN_SIZE = 20;
    const int EQ_SIZE = 4;
    const int coefs[5] = {1, 2, 3, 4, -30};
    const int iters_number = 20;
    const double mutation_prob = 0.2;
    
    double parents_fitness[GEN_SIZE];
    double children_fitness[GEN_SIZE];
    int best_individ[EQ_SIZE];
    int **old_generation = init_population(GEN_SIZE, EQ_SIZE, "random");

    for(int i=0; i<iters_number; i++)
    {
        std::cout << std::endl << "Iteration [" << i << "]:" << std::endl;
        generation_fitness(old_generation, parents_fitness, GEN_SIZE, coefs);
        std::cout << "Current fitness coefs: ";
        print_double1darray(parents_fitness, GEN_SIZE);
        bool opt_sol = search_best_individ(old_generation, parents_fitness, best_individ, GEN_SIZE, EQ_SIZE);
        if (opt_sol)
        {
            std::cout << "Optimal solution: ";
            print_int1darray(best_individ, EQ_SIZE);
            break;
        }    
        else
        {
            // print_int2darray(old_generation, GEN_SIZE, EQ_SIZE);
            std::cout << "Current best individ: ";
            print_int1darray(best_individ, EQ_SIZE);
            int** new_generation = create_new_generation(old_generation, parents_fitness, coefs, GEN_SIZE, EQ_SIZE);
            generation_fitness(new_generation, children_fitness, GEN_SIZE, coefs);
            if(mean_fitness(children_fitness, GEN_SIZE) > mean_fitness(parents_fitness, GEN_SIZE))
                std::cout << "New generation has worse fitness than previous generation. Initialize mutation protocol..." << std::endl; 
                generation_mutation(new_generation, mutation_prob, GEN_SIZE, EQ_SIZE);
            old_generation = new_generation;
        }
    }

    std::cout << "Program complete" << std::endl;
    return 0;
}