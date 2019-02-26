#include "oddEvenSort.hpp"
#include "ompOddEvenSortV1.hpp"
#include "ompOddEvenSortV2.hpp"
#include "Metrics.hpp"
#include <omp.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cstdlib>

/**
 * Programme principal.
 *
 * @return @c EXIT_SUCCESS systématiquement.
 */
int
main() {

  // Tableau des éléments à trier, ces derniers étant ici des nombres
  // pseudo-réels double précision. La taille de ce vecteur est importante car
  // nous préférons éviter de trier plusieurs fois un petit tableau afin
  // de moyenner les durées d'exécution (dans le cas d'OpenMP, le coût de
  // création puis destruction des threads serait alors exhorbitant).
  std::vector< int > tableau(1021 * 128);

  // Relation d'ordre total pour le tri : "strictement inférieur à".
  const auto comp = std::less< const int& >();

  // Instants représentant respectivement le démarrage et l'arrêt du
  // chronomètre.
  double start, stop;

  // Chronométrage de la version séquentielle. A chaque itération de la boucle,
  // nous utilisons l'algorithme iota de la bibliothèque standard pour (re)
  // placer le tableau à trier dans la pire des configurations pour odd-even
  // sort.
  std::iota(tableau.rbegin(), tableau.rend(), 0);
  start = omp_get_wtime();
  sorting::oddEvenSort(tableau.begin(), tableau.end(), comp);
  stop = omp_get_wtime();
  const double seq = stop - start;

  // Affichage des résultats de la version séquentielle. Nous utilisons le
  // nouvel algorithme is_sorted, introduit par la norme C++ 2011, pour vérifier
  // que le tableau est trié selon la relation d'ordre total souhaité.
  std::cout << "--[ oddEvenSort: begin ]--" << std::endl;
  std::cout << "\tDurée:\t\t" << seq << " sec." << std::endl;
  std::cout << "\tVerdict:\t\t"
            << std::boolalpha
            << std::is_sorted(tableau.begin(), tableau.end(), comp)
            << std::endl;
  std::cout << "--[ oddEvenSort: end ]--" << std::endl;
  std::cout << std::endl;

  // Nombre de threads disponibles.
  int threads;
#pragma omp parallel
#pragma omp single
  threads = omp_get_num_threads();

  // Chronométrage de la première version parallèle OpenMP.
  std::iota(tableau.rbegin(), tableau.rend(), 0);
  start = omp_get_wtime();
  sorting::ompOddEvenSortV1(tableau.begin(), tableau.end(), comp);
  stop = omp_get_wtime();
  const double paraV1 = stop - start;

  // Affichage des résultats de la première version parallèle OpenMP.
  std::cout << "--[ ompOddEvenSortV1: begin ]--" << std::endl;
  std::cout << "\tThread(s):\t" << threads << std::endl;
  std::cout << "\tDurée:\t\t" << paraV1 << " sec." << std::endl;
  std::cout << "\tVerdict:\t\t"
            << std::boolalpha
            << std::is_sorted(tableau.begin(), tableau.end(), comp)
            << std::endl;
  std::cout << "\tSpeedup:\t"
            << Metrics::speedup(seq, paraV1)
            << std::endl;
  std::cout << "\tEfficiency:\t"
            << Metrics::efficiency(seq, paraV1, threads)
            << std::endl;
  std::cout << "--[ ompOddEvenSortV1: end ]--" << std::endl;
  std::cout << std::endl;

  // Chronométrage de la troisième version parallèle OpenMP.
  std::iota(tableau.rbegin(), tableau.rend(), 0);
  start = omp_get_wtime();
  sorting::ompOddEvenSortV2(tableau.begin(), tableau.end(), comp);
  stop = omp_get_wtime();
  const double paraV3 = stop - start;

  // Affichage des résultats de la troisième version parallèle OpenMP.
  std::cout << "--[ ompOddEvenSortV2: begin ]--" << std::endl;
  std::cout << "\tThread(s):\t" << threads << std::endl;
  std::cout << "\tDurée:\t\t" << paraV3 << " sec." << std::endl;
  std::cout << "\tVerdict:\t\t"
            << std::boolalpha
            << std::is_sorted(tableau.begin(), tableau.end(), comp)
            << std::endl;
  std::cout << "\tSpeedup:\t"
            << Metrics::speedup(seq, paraV3)
            << std::endl;
  std::cout << "\tEfficiency:\t"
            << Metrics::efficiency(seq, paraV3, threads)
            << std::endl;
  std::cout << "--[ ompOddEvenSortV2: end ]--" << std::endl;
  std::cout << std::endl;

  // Tout se passe toujours bien.
  return EXIT_SUCCESS;

}
