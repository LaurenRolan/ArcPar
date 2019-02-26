#ifndef ompOddEvenSortV2_hpp
#define ompOddEvenSortV2_hpp

#include "oddEvenSort.hpp"
#include "merge_n.hpp"
#include <omp.h>
#include <vector>
#include <utility>
#include <functional>

namespace sorting {

  /**
   * Implémentation OpenMP de l'algorithme de tri odd-even sort. L'algorithme
   * parallèle implémenté est celui du block odd-even transposition sort
   * (normalement dédié aux architectures parallèles à mémoire distribuée),
   * c'est à dire que :
   *   1) Dans un premier temps, les éléments du conteneur à trier sont répartis
   *      par blocs de tailles équivalentes sur les différents threads
   *      disponibles ;
   *   2) Dans un second temps, chaque thread trie son bloc d'éléments via la
   *      version séquentielle du odd-even sort ;
   *   3) Dans un troisième temps, les threads dont le tid est pair échangent
   *      leurs éléments avec leur voisin de droite (tid immédiatement
   *      supérieur) si celui-ci existe. Le thread de gauche fusionne avec ses
   *      éléments et ne conserve (même nombre d'éléments) que les éléments
   *      les plus petits (au sens de la relation d'ordre total). Le thread de
   *      droite effectue la même opération mais conserve les éléments (même
   *      nombre) les plus grands ;
   *   4) Les threads dont le tid est impair effectuent la même opération que
   *      celle réalisée par leurs collègues dont le tid est pair ;
   *   5) Cette opération est répétée  2p fois si p représente le nombre de
   *      threads disponibles (p étapes avec p/2 échanges pair-impair suivis de
   *      p/2 échanges impair-pair ;
   *   6) A l'issue, chaque thread recopie son bloc d'éléments à la même
   *      position dans le conteneur à trier.
   *
   * @param[in] first - un itérateur repérant le premier élément du conteneur à
   *   trier.
   * @param[in] last  - un itérateur repérant l'élément situé juste derrière le
   *   dernier élément du conteneur à trier.
   * @param[in] comp  - un comparateur binaire représentant la relation d'ordre
   *   total.
   */
  template< typename RandomAccessIterator, typename Compare >
  void ompOddEvenSortV2(const RandomAccessIterator& first,
                        const RandomAccessIterator& last,
                        const Compare& comp) {

    // Types synonymes pour le type des éléments du conteneur ainsi que celui
    // de sa taille (et donc du rang de ses éléments).
    typedef std::iterator_traits< RandomAccessIterator > Traits;
    typedef typename Traits::value_type value_type;
    typedef typename Traits::difference_type size_type;

    // Nombre d'éléments du conteneur.
    const size_type taille = last - first;

    // Si le nombre d'éléments est inférieur à un alors nous avons terminé.
    if (taille < 1) {
      return;
    } // if

    // Type synonyme pour un bloc d'éléments.
    typedef std::vector< value_type > Bloc;

    // Nombre de threads disponibles.
    int threads;
#pragma omp parallel
#pragma omp single
    threads = omp_get_num_threads();

    // Bornes inférieures (inclues) et supérieures (exclue) des blocs d'éléments
    // affectés aux threads. Le calcul correspondant est suffisament léger pour
    // le faire réaliser par le seul thread maître et ainsi éviter le phénomène
    // du false sharing).
    size_type basses[threads], hautes[threads];
    {
      size_type quotient = taille / threads;
      size_type reste    = taille % threads;
      for (int i = 0; i < threads; i ++) {
        basses[i] = i * quotient;
        hautes[i] = basses[i] + quotient;
      }
      for (int i = 0; i < reste; i ++) {
        basses[i] += i;
        hautes[i] += i + 1;
      }
      for (int i = reste; i < threads; i ++) {
        basses[i] += reste;
        hautes[i] += reste;
      }
    } // basses et hautes.

    // Tableau de pointeurs permettant à chaque thread d'accéder aux blocs
    // d'éléments locaux de ses collègues. Attention : les adresses contenues
    // dans ce tableau ne sont valables qu'au sein de la région parallèles qui
    // suit.
    Bloc* blocs[threads];

    // Région parallèle dans laquelle chaque thread va travailler en coopération
    // avec ses collègues (attention aux synchronisations !).
#pragma omp parallel
    {

      // Tid de ce thread.
      const int tid = omp_get_thread_num();

      // Alias pour les bornes de l'intervalle d'élément affecté à ce thread.
      const size_type& basse = basses[tid];
      const size_type& haute = hautes[tid];

      // Flag indique si ce thread est le premier du groupe (ne possède pas de
      // voisin à gauche).
      const bool estPremier = tid == 0;

      // Flag indiquant si ce thread est le dernier du groupe (ne possède pas de
      // voisin à droite).
      const bool estDernier = tid == threads - 1;

      // Bloc d'éléments affectés à ce thread.
      Bloc bloc(haute - basse);
      blocs[tid] = &bloc;

      // Opération a) : chaque thread recopie les éléments qui lui sont affectés
      // dans son bloc local puis les trie via la version séquentielle du
      // odd-even sort.
      std::copy(first + basse, first + haute, bloc.begin());
      oddEvenSort(bloc.begin(), bloc.end(), comp);

      // Barrière de synchronisation explicite empêchant un thread de poursuivre
      // son exécution tant que tous n'ont pas achevé les opérations ci-dessus.
#pragma omp barrier

      // Bloc destiné à recevoir les éléments du voisin de gauche si ce dernier
      // existe.
      Bloc gauche;
      if (! estPremier) {
        gauche.resize(hautes[tid - 1] - basses[tid - 1]);
      } // if

      // Bloc destiné à recevoir les éléments du voisin de droite si ce dernier
      // existe.
      Bloc droite;
      if (! estDernier) {
        droite.resize(hautes[tid + 1] - basses[tid + 1]);
      } // if

      // Bloc d'éléments destiné à recevoir le résultat de la fusion (qu'il
      // faudra ensuite recopié dans le bloc local de ce thread). Nous
      // initialisons ce vecteur avec les valeurs du bloc pour gérer le cas
      // des threads (premier ou dernier) qui ne fusionnent rien.
      Bloc fusion(bloc.begin(), bloc.end());

      // Barrière de synchronisation explicite empêchant un thread de poursuivre
      // son exécution tant que tous n'ont pas achevé les opérations ci-dessus.
#pragma omp barrier

      // Boucle sur le nombre maximum d'échanges.
      for (int i = 0; i != 2 * threads; i ++) {

        // Modulo permettant de savoir si ce sont les threads de tid pair ou
        // impair qui échangent au cours de cette itération de la boucle. Les
        // threads dont le tid vérifie cette condition sont dits actifs tandis
        // que les autres sont dits passifs.
        const int modulo = i % 2;

        // Les threads actifs vont chercher les éléments de leurs voisin de
        // droite (s'il existe) puis fusionnent.
        if (tid % 2 == modulo) {
          if (! estDernier) {
            // A compléter.
          } // if
        }
        // tandis que les threads passifs vont chercher ceux de leur voisin de
        // gauche (s'il existe) puis fusionnent.
        else {
          if (! estPremier) {
            // A compléter.
          } // if
        } // if

        // In fine, tous les threads recopient dans leur bloc local.

        // A compléter.

      } // for

      // C'est terminé : chaque thread recopie son bloc dans le conteneur à
      // trier.
      std::copy(bloc.begin(), bloc.end(), first + basse);

    } // omp parallel (et barrière de synchronisation implicite).

  } // ompOddEvenSortV2

  /**
   * Implémentation OpenMP de la fonction ci-dessus pour la relation d'ordre
   * total "strictement inférieur à".
   *
   * @param[in] first - un itérateur repérant le premier élément du conteneur à
   *   trier.
   * @param[in] last  - un itérateur repérant l'élément situé juste derrière le
   *   dernier élément du conteneur à trier.
   * @param[in] comp  - un comparateur binaire représentant la relation d'ordre
   *   total.
   */
  template< typename RandomAccessIterator, typename Compare >
  static void ompOddEvenSortV2(const RandomAccessIterator& first,
                               const RandomAccessIterator& last) {

    // Type synonyme pour le type des éléments du conteneur.
    typedef std::iterator_traits< RandomAccessIterator > Traits;
    typedef typename Traits::value_type value_type;

    // Fabriquer le comparateur less puis invoquer la fonction définie
    // ci-dessus.
    return ompOddEvenSortV2(first, last, std::less< const value_type& >());

  } // ompOddEvenSortV2

} // sorting

#endif
