#ifndef BIGINT_VECTOR_H
#define BIGINT_VECTOR_H

#include <vector>
#include <cassert>
#include <memory>
#include <cstdint>

class vector {
    static const size_t LIMIT_SIZE = 1;
    bool isBig = false;
    size_t vector_size;
    uint32_t smallData[LIMIT_SIZE];
    std::shared_ptr<std::vector<uint32_t>> bigData;
    void toBig();
public:
    vector();
    vector(const vector &other);
    vector(size_t size);
    vector(std::vector<uint32_t> other);
    vector(size_t size, uint32_t value);
    ~vector();
    size_t size() const;
    void do_unique();
    void push_back(uint32_t value);
    uint32_t pop_back();
    void push_front(uint32_t value);
    uint32_t &operator[](size_t index);
    const uint32_t &operator[](size_t index) const;
    uint32_t &back();
    static void swap(vector &a, vector &b);
    friend bool operator==(vector const &a, vector const &b);
    vector & operator=(vector const &other);
    void resize(size_t capacity);
    void clear();
};

#endif //BIGINT_VECTOR_H