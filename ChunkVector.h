//
// ChunkVector.h
//
// A vector-like container.
//  - Instead of reallocating, the container allocates 'chunks' when needed.
//  - Each new chunk is twice the size of the previous chunk.
//  - So, grows without invalidating pointers to existing items.
//  - But, you can't remove items.
//
// Traversal
//  - Fairly good -- O(1) but note the offset is an expression, roughly:
//      chunks[log(i)][i - 2^log(i)]
//  - Traversal could be optimised quite a bit, probably
//
// -- BH 24.6.2016
// -- MIT licensed: http://opensource.org/licenses/MIT
//

#ifndef __ChunkVector_h
#define __ChunkVector_h

#include <vector>

template<class T>
class ChunkVector {
public:
  ChunkVector()
  {
    expand();
  }
  ChunkVector(const ChunkVector &a)
  {
    for (auto *v : a.chunks) {
      chunks.push_back(new chunk(*v));
    }
    size = a.size;
    N = a.N;
  }

  ~ChunkVector()
  {
    for (auto chunk : chunks) {
      delete chunk;
    }
  }

  // Setting
  T& push(T t) {
    auto *c = currentChunk();
    if (c->size() >= size) {
      expand();
      c = currentChunk();
    }
    c->push_back(t);
    ++N;
    return c->back();
  }

  // Retrieving
  T& last() {
    return currentChunk()->back();
  }

  T& operator[](size_t i) {
    size_t i_vec = log_base2(i+2);
    size_t i_row = i+2 - two_to_the(i_vec);
    return (*chunks[i_vec - 1])[i_row];
  }

  template<class SearchFn>
  int find(SearchFn f) {
    for (int i=0, n=int(N); i < n; ++i) {
      if (f((*this)[i])) {
        return i;
      }
    }
    return -1;
  }

  template<class Fn>
  void each(Fn f) {
    for (size_t i=0, n=N; i < n; ++i) {
      f((*this)[i]);
    }
  }

  size_t current_size() const {
    return N;
  }

private:
  ChunkVector operator= (const ChunkVector &) {
    return ChunkVector();
  }
  typedef std::vector<T> chunk;

  size_t size = 1;  // starting chunk size will be 2
  size_t N = 0;
  std::vector<chunk*> chunks;

  void expand() {
    auto *c = new chunk;
    c->reserve(size *= 2);
    chunks.push_back(c);
  }

  chunk* currentChunk() {
    return chunks.back();
  }

  static inline size_t log_base2(size_t x) {
    size_t ind = 0;
    while (x >>= 1) {
      ++ind;
    }
    return ind;
  }
  static inline size_t two_to_the(size_t x) {
    return 1 << x;
  }

  // ChunkVector operator= (const ChunkVector &from) {
  //   ChunkVector *from_unsafe = (ChunkVector*) &from;
  //   chunks.empty();
  //   size = 1;
  //   N = 0;
  //   expand();
  //
  //   for (auto *v : from_unsafe->chunks) {
  //     chunks.push_back(new chunk(*v));
  //   }
  //   size = from.size;
  // }

#ifdef ChunkVector_Test
  friend void test_ChunkVector();
#endif
};

#endif

