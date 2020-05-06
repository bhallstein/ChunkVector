#include "_test.h"

#define ChunkVector_Test
#include "../ChunkVector.h"

struct X {
  int a;
  float b;
};

void test_ChunkVector() {
  p_header("ChunkVector: construction");
  ChunkVector<X> cv;
  p_assert(cv.chunks.size() == 1);
  p_assert(cv.chunks[0]->size() == 0 && cv.N == 0);
  p_assert(cv.chunks[0]->capacity() == 2);

  p_header("ChunkVector: pushing");
  cv.push({1, 1.0});
  p_assert(cv.chunks.size() == 1);
  p_assert(cv.chunks[0]->size() == 1 && cv.N == 1);

  cv.push({2, 2.0});
  p_assert(cv.chunks.size() == 1);
  p_assert(cv.chunks[0]->size() == 2 && cv.N == 2);

  p_header("ChunkVector: auto-expanding");
  cv.push({3, 3.0});
  p_assert(cv.chunks.size() == 2);
  p_assert(cv.chunks[0]->capacity() == 2);
  p_assert(cv.chunks[0]->size() == 2);
  p_assert(cv.chunks[1]->capacity() == 4);
  p_assert(cv.chunks[1]->size() == 1);
  p_assert(cv.N == 3);

  p_header("ChunkVector: last()");
  p_assert(cv.last().a == 3 && cv.last().b == 3.0);

  p_header("ChunkVector: operator[]");
  p_assert(cv[0].a == 1);
  p_assert(cv[1].a == 2);
  p_assert(cv[2].a == 3);
  for (int i=0; i < 100; ++i) {
    cv.push({ i, (float) i });
  }
  p_assert(cv[102].a == 99 && cv.last().a == 99);
  p_assert(cv.N == 103);
    // The ChunkVector now has 103 items in it

  p_header("ChunkVector: find()");
  int ind = cv.find([](X &x) {
    return (x.a == 50 && x.b == 50.0);
  });
  p_assert(cv[ind].a == 50);
  ind = cv.find([](X &x) {
    return x.a == -134;
  });
  p_assert(ind == -1);

  p_header("ChunkVector: each()");
  std::vector<X> results;
  cv.each([&](X &x) {
    results.push_back(x);
  });
  bool matched = true;
  for (int i=0; i < 3; ++i) {
    if (results[i].a != i + 1) {
      matched = false;
    }
  }
  p_assert(matched == true);
  for (int i=0; i < 100; ++i) {
    if (results[i+3].a != i) {
      matched = false;
    }
  }
  p_assert(matched == true);
}


int main() {
  test_ChunkVector();
  return 0;
}

