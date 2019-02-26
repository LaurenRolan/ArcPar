#ifndef oddEvenSort_hpp
#define oddEvenSort_hpp

#include <utility>
#include <functional>

namespace sorting {

  /**
   * Implémentation séquentielle de l'algorithme de tri odd-even sort dont la 
   * complexité au pire est O(n^2) si n réprésente le nombre d'éléments à trier.
   *
   * @param[in] first - un itérateur repérant le premier élément du conteneur à
   *   trier.
   * @param[in] last  - un itérateur repérant l'élément situé juste derrière le
   *   dernier élément du conteneur à trier.
   * @param[in] comp  - un comparateur binaire représentant la relation d'ordre
   *   total.
   */
  template< typename RandomAccessIterator, typename Compare >
  void oddEvenSort(const RandomAccessIterator& first,
		   const RandomAccessIterator& last,
		   const Compare& comp) {

    // Si le nombre d'éléments à trier est inférieur à 1 alors nous avons
    // terminé.
    if (last - first < 1) {
      return;
    } // if

    // Flag indiquant si un échange entre deux éléments consécutifs s'est pro-
    // duit lors de la dernière itération de la boucle extérieure.
    bool echange = true;

    // Décalage permettant d'alterner les périodes pair-impair puis impair-pair.
    int shift = 0;

    // Boucle extérieure qui ne s'arrête que lorsque plus aucun échange d'élé-
    // ments n'est effectué à l'itération précédente.
    while (echange) {

      // Pour l'instant, aucun échange entre deux éléments n'a encore été
      // effectué pour cette nouvelle itération de la boucle.
      echange = false;

      // Balayage des éléments de rang pair ou impair selon la période.
      for (RandomAccessIterator it = first + shift; it < last - 1; it += 2) {

	// Obtention de l'élément courant et de son successeur. Le mot-clé auto,
	// introduit par la norme 2011, permet de ne pas mentionner le type des
	// éléments.
	auto& courant = *it;
	auto& suivant = *(it + 1);

	// Les éléments ne sont pas correctement ordonnés : il faut procéder à
	// un échange.
	if (! comp(courant, suivant)) {
	  std::swap(courant, suivant);
	  echange = true;
	} // if

      } // for

      // Mise à jour du décalage pour la période suivante.
      shift = 1 - shift;

    } // while

  } //  oddEvenSort

  /**
   * Implémentation OpenMP de la fonction ci-dessus pour la relation d'ordre
   * total "strictement inférieur à".
   *
   * @param[in] first - un itérateur repérant le premier élément du conteneur à
   *   trier.
   * @param[in] last  - un itérateur repérant l'élément situé juste derrière le
   *   dernier élément du conteneur à trier.
   */
  template< typename RandomAccessIterator >
  static void oddEvenSort(const RandomAccessIterator& first, 
			  const RandomAccessIterator& last) {

    // Type synonyme pour le type des éléments du conteneur. 
    typedef std::iterator_traits< RandomAccessIterator > Traits;
    typedef typename Traits::value_type value_type;

    // Fabriquer le comparateur less puis invoquer la fonction définie 
    // ci-dessus.
    return oddEvenSort(first, last, std::less< const value_type& >());

  } // oddEvenSort

} // sorting

#endif
