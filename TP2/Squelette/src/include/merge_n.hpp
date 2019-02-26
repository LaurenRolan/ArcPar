#ifndef merge_n_hpp
#define merge_n_hpp

#include <algorithm>

namespace merging {

  /**
   * Permet de fusionner les n premiers éléments de deux conteneurs pré-ordonnés
   * selon la même relation d'ordre total.
   *
   * @param[in] first1 - un itérateur repérant le premier élément du premier
   *   conteneur.
   * @param[in] last1  - un itérateur repérant l'élément situé derrière le
   *   dernier élément du premier conteneur.
   * @param[in] first2 - un itérateur repérant le premier élément du second
   *   conteneur.
   * @param[in] last2  - un itérateur repérant l'élément situé derrière le
   *   dernier élément du second conteneur.
   * @param[in] result - un itérateur repèrant le prochain emplacement d'accueil
   *   dans le conteneur cible.
   * @param[in] n      - le nombre maximum d'éléments à fusionner.
   * @param[in] comp   - un comparateur binaire représentant la relation d'ordre
   *   total.
   * @return un itérateur repèrant l'élément situé juste derrière le dernier
   *   élément accueilli dans le conteneur cible de la fusion.
   */
  template< typename InputIterator1,
            typename InputIterator2,
            typename OutputIterator,
            typename Size,
            typename Compare >
  OutputIterator
  merge_n(InputIterator1 first1, InputIterator1 last1,
          InputIterator2 first2, InputIterator1 last2,
          OutputIterator result,
          Size n,
          Compare comp) {

    
    while ( true ) {
      if ( first1 == last1 ) return std::copy_n(first2, n, result);
      if ( first2 == last2 ) return std::copy_n(first1, n, result);
      *result ++ = comp(*first1, *first2) ? *first1++ : *first2++;
    }

    // Retourner l'itérateur balayant le conteneur cible.
    return result;

  } // merge_n

  /**
   * Permet de fusionner les n premiers éléments de deux conteneurs pré-ordonnés
   * selon la relation d'ordre total "strictement inférieur à".
   *
   * @param[in] first1 - un itérateur repérant le premier élément du premier
   *   conteneur.
   * @param[in] last1  - un itérateur repérant l'élément situé derrière le
   *   dernier élément du premier conteneur.
   * @param[in] first2 - un itérateur repérant le premier élément du second
   *   conteneur.
   * @param[in] last2  - un itérateur repérant l'élément situé derrière le
   *   dernier élément du second conteneur.
   * @param[in] result - un itérateur repèrant le prochain emplacement d'accueil
   *   dans le conteneur cible.
   * @param[in] n      - le nombre maximum d'éléments à fusionner.
   * @return un itérateur repèrant l'élément situé juste derrière le dernier
   *   élément accueilli dans le conteneur cible de la fusion.
   */
  template< typename InputIterator1,
            typename InputIterator2,
            typename OutputIterator,
            typename Size >
  OutputIterator
  merge_n(InputIterator1 first1, InputIterator1 last1,
          InputIterator2 first2, InputIterator1 last2,
          OutputIterator result,
          Size n) {

    // Type synonyme pour le type des éléments du premier conteneur (qui est
    // supposé évidemment compatible avec ceux du second).
    typedef std::iterator_traits< InputIterator1 > Traits;
    typedef typename Traits::value_type value_type;

    // Fabriquer le comparateur less puis invoquer la fonction définie
    // ci-dessus.
    return merge_n(first1, last1,
                   first2, last2,
                   result,
                   n,
                   std::less< const value_type& >());

  } // merge_n

} // merging

#endif
