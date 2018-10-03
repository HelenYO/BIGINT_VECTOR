#include "vector.h"
#include <cstring>
#include <vector>

vector::vector() : vector_size(0) {}

vector::vector(std::vector<uint32_t> other) {
    if (other.size() > LIMIT_SIZE) {
        isBig = true;
        bigData = std::shared_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>(other));
    } else {
        for (int i = 0; i < LIMIT_SIZE; i++) {
            smallData[i] = other[i];
        }
    }
}

vector::vector(const vector &other) : vector_size(other.vector_size), isBig(other.isBig) {
    if (other.isBig) {
        bigData =  std::shared_ptr<std::vector<uint32_t>>(other.bigData);
    } else {
        std::copy(other.smallData, other.smallData + LIMIT_SIZE, smallData);
    }
}

vector::vector(size_t size) : vector_size(size) {
    if (vector_size > LIMIT_SIZE) {
        isBig = true;
        bigData =  std::shared_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>(size));
    }
}

vector::vector(size_t size, uint32_t value) : vector_size(size) {
    if (vector_size > LIMIT_SIZE) {
        isBig = true;
        bigData =  std::shared_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>(size, value));
    } else {
        std::fill(smallData, smallData + LIMIT_SIZE, value);
    }
}

vector::~vector() {
    if (isBig) {
        bigData.reset();
    }
}

size_t vector::size() const {
    return vector_size;
}

void vector::do_unique() {
    if (!bigData.unique()) {
        auto ptr = new std::vector<uint32_t>(*(bigData));
        bigData.reset(ptr);
    }
}

void vector::push_back(uint32_t value) {
    if (!isBig && vector_size < LIMIT_SIZE) {
        smallData[vector_size] = value;
    } else {
        if (!isBig) {
            toBig();
        }
        do_unique();
        bigData->push_back(value);
    }
    vector_size++;
}

uint32_t vector::pop_back() {
    if (!isBig) {
        vector_size--;
        return smallData[vector_size];
    } else {
        do_unique();
        uint32_t t = bigData->back();
        bigData->pop_back();
        vector_size--;
        return t;
    }
}

void vector::push_front(uint32_t value) {
    if (!isBig && vector_size < LIMIT_SIZE) {
        for (size_t i = vector_size; i > 0; --i) {
            smallData[i] = smallData[i - 1];
        }
        smallData[0] = value;
    } else {
        if (!isBig) {
            toBig();
        }
        do_unique();
        bigData->insert(bigData->begin(), value);
    }
    vector_size++;
}

uint32_t &vector::operator[](size_t index) {
    if (isBig) {
        do_unique();
        return bigData->at(index);
    } else {
        return smallData[index];
    }
}

const uint32_t &vector::operator[](size_t index) const {
    if (isBig) {
        return bigData->at(index);
    } else {
        return smallData[index];
    }
}

uint32_t &vector::back() {
    if (isBig) {
        return bigData->back();
    } else {
        return smallData[vector_size - 1];
    }
}

void vector::swap(vector &a, vector &b) {
    for (size_t i = 0; i < LIMIT_SIZE; i++) {
        std::swap(a.smallData[i], b.smallData[i]);
    }
    std::swap(a.bigData, b.bigData);
    std::swap(a.isBig, b.isBig);
    std::swap(a.vector_size, b.vector_size);
}

bool operator==(vector const &a, vector const &b) {
    if (a.vector_size != b.vector_size) {
        return false;
    } else {
        for (size_t i = 0; i < a.vector_size; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
    }
    return true;
}

void vector::resize(size_t capacity) {
    if (isBig) {
        do_unique();
        bigData->resize(capacity);
    } else {
        if (capacity > LIMIT_SIZE) {
            toBig();
            bigData->resize(capacity);
        }
    }
    vector_size = capacity;
}

void vector::clear() {
    if (isBig) {
        do_unique();
        bigData->clear();
    }
    vector_size = 0;
}

vector &vector::operator=(vector const &other) {
    vector tmp(other);
    swap(tmp, *this);
    return *this;
}

void vector::toBig() {
    isBig = true;
    uint32_t temp[LIMIT_SIZE];
    std::copy(smallData, smallData + LIMIT_SIZE, temp);
    bigData =  std::shared_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>());
    for (size_t i = 0; i < vector_size; i++) {
        bigData->push_back(temp[i]);
    }
}