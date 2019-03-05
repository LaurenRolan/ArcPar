#ifndef pipelinedBubbleSort_hpp
#define pipelinedBubbleSort_hpp

#include "bubbleSort.hpp"
#include <tbb/pipeline.h>
#include <vector>
#include <algorithm>

namespace sorting {

  /**
   * Implémentation pipeline de l'algorithme du bubble sort.
   *
   * @param[in] first - un itérateur repérant le premier élément de la séquence
   *   à trier.
   * @param[in] last - un itérateur repérant l'élément situé juste derrière le
   *   dernier élément de la séquence à trier.
   * @param[in] comp - un comparateur binaire permettant d'ordonner deux 
   *   éléments.
   * @param[in] chunks - nombre de tronçons maximum en circulation dans le
   *   pipeline à un instant donné.
   */
  template< typename RandomAccessIterator, typename Compare >
  void pipelinedBubbleSort(const RandomAccessIterator& first,
                           const RandomAccessIterator& last,
                           const Compare& comp,
                           const size_t& chunks) {

    typedef std::iterator_traits< RandomAccessIterator > Traits;
    typedef typename Traits::value_type value_type;
    typedef std::vector<value_type> Item;
    
    class EntryStage : public tbb::filter {
    private:
      RandomAccessIterator it_;
      const RandomAccessIterator& last_;
      const size_t chunkSize_;
    public:
      EntryStage(const RandomAccessIterator & first,
                 const RandomAccessIterator & last,
                 const size_t& chunkSize) 
                : tbb::filter(tbb::filter::serial_out_of_order),
          it_(first), 
          last_(last),
          chunkSize_(chunkSize) { }

      void * operator()(void * item) override {
        if (it_ == last_) {
          return NULL;
        }

        auto start = it_;
        for(int i = 0; i < chunkSize_ && it_ != last_; i++, it_ ++);
        
        return new Item(start, it_);
      }
    };

    class MiddleStage : public tbb::filter {
    private:
      const Compare& comp_;
      Item * item_;
    public:
      MiddleStage(const Compare& comp) : tbb::filter(tbb::filter::parallel),
      comp_(comp) { }
      void * operator()(void * item) override {
        item_ = static_cast<Item*>(item);
        bubbleSort(item_->begin(), item_->end(), comp_);
        return item_;
      }
    };

    class OutputStage : public tbb::filter {
    private:
      RandomAccessIterator first_;
      RandomAccessIterator it_;
      const Compare& comp_; 
      Item * item_;
    public:
      OutputStage(const RandomAccessIterator & first, const Compare& comp) 
                  : tbb::filter(tbb::filter::serial_out_of_order),
                    first_(first),
                    comp_(comp),
                    it_(first) { }
      void * operator()(void * item) override {
         item_ = static_cast<Item*>(item);
         auto stop = std::copy(item_->begin(), item_->end(), it_);
         delete item_;
         std::inplace_merge(first_, it_, stop, comp_);
         it_ = stop;
         return NULL;
      }
    };
    
    EntryStage stage1(first, last, 16);
    MiddleStage stage2(comp);
    OutputStage stage3(first, comp);

    tbb::pipeline pipe;

    pipe.add_filter(stage1);
    pipe.add_filter(stage2);
    pipe.add_filter(stage3);

    pipe.run(chunks);

  } // pipelinedBubbleSort

  /**
   * Implémentation pipeline de l'algorithme du bubble sort. La relation d'ordre
   * total utilisée est "strictement inférieur à".
   *
   * @param[in] first - un itérateur repérant le premier élément de la séquence
   *   à trier.
   * @param[in] last - un itérateur repérant l'élément situé juste derrière le
   *   dernier élément de la séquence à trier.
   * @param[in] chunks - nombre de tronçons maximum en circulation dans le
   *   pipeline à un instant donné.
   */
  template< typename RandomAccessIterator >
  void pipelinedBubbleSort(const RandomAccessIterator& first,
                           const RandomAccessIterator& last,
                           const size_t& chunks) {

    // Type synonyme pour le type des éléments conteneur. 
    typedef std::iterator_traits< RandomAccessIterator > Traits;
    typedef typename Traits::value_type value_type;

    // Fabriquer le comparateur less et invoquer la fonction définie ci-dessus.
    pipelinedBubbleSort(first, 
			last, 
			std::less< const value_type& >(),
			chunks);

  } // pipelinedBubbleSort
} // sorting

#endif
