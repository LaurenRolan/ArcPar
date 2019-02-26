#ifndef CountIf_hpp
#define CountIf_hpp

#include <omp.h>
#include <algorithm>
#include <vector>
#include <iostream>

namespace paralgos {

  /**
   * @class CountIf CountIf.hpp
   *
   * Version OpenMP de l'algorithme count_if de la bibliothèque standard.
   */
  class CountIf {
  public:

    /**
     * Forme générale de l'algorithme.
     *
     * @param[in, out] first - itérateur repérant le premier élément à traiter.
     * @param[in, out] last - itérateur repérant l'élément situé juste derrière
     *   le dernier élément à traiter.
     * @param[in] pred - prédicat unaire.
     * @return le nombre d'éléments de l'intervalle @c [first, last[ 
     *   satisfaisant @c pred.
     */
    template< typename InputIterator, typename UnaryPredicate >
    static typename std::iterator_traits< InputIterator >::difference_type
    apply(InputIterator first, InputIterator last, UnaryPredicate pred) {

      // Obtention du nombre de threads. Si celui-ci vaut 1, nous utilisons
      // directement la version séquentielle de l'algorithme, c'est à dire celle
      // de la bibliothèque standard.
      int threads;
#pragma omp parallel
#pragma omp single
      threads = omp_get_num_threads();
      if (threads == 1) {
      	return std::count_if(first, last, pred);
      }

      // Obtention du type des itérateurs first et last (nous passons par le
      // template iterator_traits de la bibliothèque standard afin de pouvoir 
      // gérer tous les types de RandomAccessIterator, y compris les simples 
      // pointeurs).
      typedef std::iterator_traits< InputIterator > Traits;

      // Instanciation de l'implémentation (sous-classe) adéquate.
      typedef Impl< typename Traits::iterator_category > Impl;

      // Invocation de la version dédiée de cet algorithme.
      return Impl::apply(first, last, pred, threads);

    } // apply

  public:

    /**
     * Implémentation de cet algorithme dédiée aux InputIterator.
     */
    template< typename Category >
    class Impl {
    public:

      // Déclaration d'amitié envers la classe mère : seule cette dernière peut
      // invoquer la méthode de classe privée @c apply.
      friend class CountIf;

    private:

     /**
       * Implémentation de count_if.
       *
       * @param[in] first - itérateur repérant le premier élément à traiter.
       * @param[in] last - itérateur repérant l'élément situé juste derrière le
       *   dernier élément à traiter.
       * @param[in] pred - prédicat unaire.
       * @param[in] threads - le nombre de threads disponibles.
       * @return le nombre d'éléments de l'intervalle @c [first, last[ 
       *   satisfaisant @c pred.
       */
      template< typename InputIterator, typename UnaryPredicate >
      static 
      typename std::iterator_traits< InputIterator >::difference_type
      apply(const InputIterator& first,
	    const InputIterator& last,
	    const UnaryPredicate& pred,
	    const int& threads) {

	return strategyB(first, last, pred, threads);

      } // apply

    private:

      /**
       * Implémentation basée sur le découpage du conteneur en tronçons, ces
       * derniers étant représentés par des tableaux de pointeurs sur les
       * éléments du conteneur. Une parallélisation via une boucle for et la
       * clause de scheduling auto est alors possible. Le revers de la médaille
       * est l'obligation d'utiliser un tronçon de taille arbitraire 
       * (éventuellement fonction du nombre de threads disponibles) car compter
       * le nombre d'éléments du conteneur reviendrait à le parcourir une
       * première fois. L'autre inconvénient est la nécessité d'effectuer des
       * recopies de pointeurs.
       *
       * @param[in] first - itérateur repérant le premier élément à traiter.
       * @param[in] last - itérateur repérant l'élément situé juste derrière le
       *   dernier élément à traiter.
       * @param[in] pred - prédicat unaire.
       * @param[in] threads - le nombre de threads disponibles.
       * @return le nombre d'éléments de l'intervalle @c [first, last[ 
       *   satisfaisant @c pred.
       */
      template< typename InputIterator, typename UnaryPredicate >
      static 
      typename std::iterator_traits< InputIterator >::difference_type
      strategyA(const InputIterator& first,
		const InputIterator& last,
		const UnaryPredicate& pred,
		const int& threads) {

	// Taille d'un tronçon fixée arbitrairement.
	const int taille = 512 * threads;

	// Obtention du type des éléments du conteneur.
	typedef std::iterator_traits< InputIterator > Traits;
	typedef typename Traits::value_type Elt;

	// Tronçon constitué de pointeurs vers les éléments du conteneur.
	std::vector< const Elt* > troncon(taille);

	// Compteur des éléments satisfaisant le prédicat.
	typename std::iterator_traits< InputIterator >::difference_type acc = 0;
      InputIterator itList = first;
      int combien;
      #pragma omp parallel
      { 
          while (itList != last) {
          #pragma omp barrier  
          #pragma omp single
          {
            for(combien = 0; combien < taille && itList != last; combien++) {
                troncon[combien] = &(*itList);
                itList++;
            }
          }
          #pragma omp for schedule(auto) reduction (+:acc)
        	for(int i = 0; i < combien; i++) {
            if (pred(*troncon[i])) acc++;
          }
        }  
    }

	// C'est terminé.
	return acc;

      } // strategyA

      /**
       * Implémentation toujours basée sur l'idée d'un découpage du conteneur 
       * en tronçons mais cette fois-ci, nous allons utiliser la notion de
       * tâches afin d'éviter les recopies de pointeurs dans un tableau. La
       * La contrepartie est un scheduling au niveau des tâches, scheduling sur
       * lequel nous avons peu de prise.
       *
       * @param[in] first - itérateur repérant le premier élément à traiter.
       * @param[in] last - itérateur repérant l'élément situé juste  derrière le
       *   dernier élément à traiter.
       * @param[in] pred - prédicat unaire.
       * @param[in] threads - le nombre de threads disponibles.
       * @return le nombre d'éléments de l'intervalle @c [first, last[ 
       *   satisfaisant @c pred.
       *
       * @note Cette implémentation s'apparente à la clause de scheduling 
       *   schedule(dynamic, size).
       */
      template< typename InputIterator, typename UnaryPredicate >
      static 
      typename std::iterator_traits< InputIterator >::difference_type
      strategyB(const InputIterator& first,
		const InputIterator& last,
		const UnaryPredicate& pred,
		const int& threads) {

	// Taille d'un tronçon fixée arbitrairement.
	const int taille = 128;

	// Compteur des éléments satisfaisant le prédicat.
	typename std::iterator_traits< InputIterator >::difference_type acc = 0;

	#pragma omp parallel
  {
    #pragma omp single
    {
    //Creation des taches

    auto it = first;
    auto start = first;
    int i;

    while (it != last) {
      
      i = 0;
      while (it != last && i < taille) {
        it ++;
        i ++;
      }

#pragma omp task firstprivate(start, it) shared(acc, pred)
        {
#pragma omp atomic        
          acc += std::count_if(start, it, pred);
        } // omp task

      start = it;

  } // while

  } // omp single

  } // omp parallel
  
	// C'est terminé.
	return acc;

      } // strategyB

      /**
       * Implémentation basée sur l'idée d'une répartition cyclique des
       * éléments du conteneur entre les différents threads disponibles.
       *
       * @param[in] first - itérateur repérant le premier élément à traiter.
       * @param[in] last - itérateur repérant l'élément situé juste derrière le
       *   dernier élément à traiter.
       * @param[in] pred - prédicat unaire.
       * @param[in] threads - le nombre de threads disponibles.
       * @return le nombre d'éléments de l'intervalle @c [first, last[ 
       *   satisfaisant @c pred.
       *
       * @note Cette implémentation s'apparente à la clause de scheduling 
       *   schedule(static).
       */
      template< typename InputIterator, typename UnaryPredicate >
      static 
      typename std::iterator_traits< InputIterator >::difference_type
      strategyC(const InputIterator& first,
		const InputIterator& last,
		const UnaryPredicate& pred,
		const int& threads) {

	// Compteur des éléments satisfaisant le prédicat.
	typename std::iterator_traits< InputIterator >::difference_type acc = 0;

    //std::vector<InputIterator> threadData(threads);

  // omp for schedule(static, 1)

  #pragma omp parallel reduction(+:acc)
  {

    InputIterator it = first;

    int tid = omp_get_thread_num();

    for (int i = 0; i < tid && it != last; i ++, it ++);

    while(it != last) {
      if (pred(*it)) ++acc;

      for (int i = 0; i < threads && it != last; i ++, it ++);
    }

} // omp parallel


	// C'est terminé.
	return acc;

      } // strategyC

    }; //  Impl< not RandomAccessIterator >

  }; // Count

  /**
   * Implémentation de cet algorithme dédiée aux RandomAccessIterator.
   */
  template<>
  class CountIf::Impl< std::random_access_iterator_tag > {
  public:

    // Déclaration d'amitié envers la classe mère : seule cette dernièrer peut
    // invoquer la méthode de classe privée @c apply.
    friend class CountIf;

  private:

    /**
     * Implémentation de count sous la forme d'une procédure.
     *
     * @param[in] first - itérateur repérant le premier élément à traiter.
     * @param[in] last - itérateur repérant l'élément situé juste derrière le 
     *   dernier élément à traiter.
     * @param[in] pred - prédicat unaire.
     * @param[in] threads - le nombre de threads disponibles.
     * @return le nombre d'éléments de l'intervalle @c [first, last[ 
     *  satisfaisant @c pred.
     */
    template< typename RandomAccessIterator, typename UnaryPredicat > 
    static 
    typename std::iterator_traits< RandomAccessIterator >::difference_type
    apply(const RandomAccessIterator& first,
	  const RandomAccessIterator& last,
	  const UnaryPredicat& pred,
	  const int& threads) {

      // Compteur des éléments satisfaisant le prédicat.
      typename std::iterator_traits< RandomAccessIterator >::difference_type 
	acc = 0;
      

      #pragma omp parallel for schedule(auto) reduction (+:acc)
      for(auto it = first; it < last; it++ ){
          if (pred(*it)) ++acc;
      }

      // C'est terminé.
      return acc;

    } // apply

  }; // // Impl< RandomAccessIterator >
  
} // paralgos

#endif
