#ifndef bubbleSort_hpp
#define bubbleSort_hpp

#include <utility>
#include <functional>
#include <iostream> //Debug

namespace sorting {

  /**
   * Implémentation séquentielle du tri à bulle dont la complexité au pire est 
   * O(n^2) si n réprésente le nombre d'éléments à trier. 
   *
   * @param[in] first - un itérateur repérant le premier élément de la séquence
   *   à trier.
   * @param[in] last - un itérateur repérant l'élément situé juste derrière le
   *   dernier élément de la séquence à trier.
   * @param[in] comp - un comparateur binaire permettant d'ordonner deux 
   *   éléments.
   */
  template< typename RandomAccessIterator, typename Compare >
  void bubbleSort(const RandomAccessIterator& first,
                  const RandomAccessIterator& last,
                  const Compare& comp) {

    bool isContinue = true;

    while(isContinue) {
    	isContinue = false;
    	for(auto it = first; it < last - 1; it++) {
    		auto& courant = *it;
    		auto& suivant = *(it + 1);
    		//std::cout << comp(*it, *(it + 1)) << " : " << *it << " >= " << *(it + 1) << "   ";
    		if(! comp(courant, suivant)) {
    			std::swap(courant, suivant);
    			isContinue = true;
    		}
    	}
    }
  } // bubbleSort

  /**
   * Implémentation séquentielle du tri à bulle dont la complexité au pire est
   * O(n^2) si n réprésente le nombre d'éléments à trier. La relation d'ordre 
   * total utilisée est "strictement inférieur à".
   *
   * @param[in] first - un itérateur repérant le premier élément de la séquence
   *   à trier.
   * @param[in] last - un itérateur repérant l'élément situé juste derrière le
   *   dernier élément de la séquence à trier.
   */
  template< typename RandomAccessIterator >
  void bubbleSort(const RandomAccessIterator& first, 
                  const RandomAccessIterator& last) {

    // Type synonyme pour le type des éléments conteneur. 
    typedef std::iterator_traits< RandomAccessIterator > Traits;
    typedef typename Traits::value_type value_type;

    // Fabriquer le comparateur less puis invoquer la fonction définie 
    // ci-dessus.
    bubbleSort(first, last, std::less< const value_type& >());

  } // bubbleSort

} // sorting

#endif
