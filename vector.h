#pragma once
#ifndef SHL_VECTOR_H
#define SHL_VECTOR_H

///*** Own namespace shl ***///

namespace shl {
    template <typename typ> class vector;
    class string;
}

///*** Definition shl::vector ***///

template <typename typ>
class shl::vector
{
private:
    typ* _data;
    unsigned long _size;
    unsigned long _capacity;
    vector& at_begin(typ);

public:
    vector();
    vector(unsigned long);

    void clear();
    //void pop_back();
    //void pop_begin();
    void fit_to_size();
    typ& operator[](unsigned long);
    void operator+=(unsigned long);
    vector& reverse();
    vector& part(unsigned long, unsigned long);
    vector& operator<<(typ);
    vector& operator<<(vector<typ>&);
    vector& operator>>=(int);
    vector& operator<<=(int);
    unsigned long& capacity();
    unsigned long& size();
};

using namespace shl;

template <typename typ>
vector<typ>::vector() {
    this->_data = new typ;
    this->_size = 0;
    this->_capacity = 1;
}

template <typename typ>
vector<typ>::vector(unsigned long count) : vector() {
    this->operator+=(count);
}

template <typename typ>
typ& vector<typ>::operator[](unsigned long pos) {
    return this->_data[pos];
}

template <typename typ>
void vector<typ>::operator+=(unsigned long count) {
    for (unsigned long i = 0; i < count; i++)
        this->operator<<(typ());
}

template <typename typ>
vector<typ>& vector<typ>::operator<<=(int count) {
    if (this->size() == 0)
        return *this;
    for (int i = 0; i < count; i++)
    {
        typ tmp = this->_data[0];
        for (int j = 0; j < this->size() - 1; j++)
            this->_data[j] = this->_data[j + 1];
        this->_data[this->size() - 1] = tmp;
    }

    return *this;
}

template <typename typ>
vector<typ>& vector<typ>::operator>>=(int count) {
    if (this->size() == 0)
        return *this;
    for (int i = 0; i < count; i++)
    {
        typ tmp = this->_data[this->size() - 1];
        for (int j = this->size() - 1; j > 1; j--)
            this->_data[j] = this->_data[j - 1];
        this->_data[0] = tmp;
    }

    return *this;
}

template <typename typ>
vector<typ>& vector<typ>::operator<<(typ data) {
    if (this->size() == this->capacity()) {
        this->capacity() *= 2;

        typ* tmp = this->_data;
        this->_data = new typ[this->capacity()];

        for (size_t i = 0; i < this->size(); i++)
            *(this->_data + i) = tmp[i];
        delete[] tmp;
    }

    this->_data[this->size()] = data;
    this->size()++;

    return *this;
}

template <typename typ>
vector<typ>& vector<typ>::operator<<(vector<typ>& vec) {
    vector<typ> tmp;

    for (unsigned long i = 0; i < vec.size(); i++)
        tmp << vec[i];

    for (unsigned long i = 0; i < tmp.size(); i++)
        this->operator<<(tmp[i]);

    return *this;
}

template <typename typ>
vector<typ>& vector<typ>::at_begin(typ data) {
    if (this->size() == this->capacity())
        this->capacity() *= 2;

    typ* tmp = this->_data;
    this->_data = new typ[this->capacity()];

    for (unsigned long i = 0; i < this->size(); i++)
        this->_data[i + 1] = tmp[i];
    delete[] tmp;

    this->_data[0] = data;
    this->size()++;

    return *this;
}

template <typename typ>
vector<typ>& vector<typ>::reverse() {
    typ* rev = new typ[this->capacity()];

    for (int i = 0; i < this->size(); i++)
        rev[i] = this->_data[this->size() - i - 1];

    delete[] this->_data;
    this->_data = rev;

    return *this;
}

template <typename typ>
vector<typ>& vector<typ>::part(unsigned long start, unsigned long end) {
    vector<typ>* tmp = new vector<typ>;
    if (start > this->size() - 1)
        return *tmp;
    for (unsigned long i = start; i <= end && i < this->size(); i++)
        *tmp << this->_data[i];

    return *tmp;
}

template <typename typ>
void vector<typ>::clear() {
    delete[] _data;
    _data = new typ;
    this->size() = 0;
    this->capacity() = 1;
}

template <typename typ>
void shl::vector<typ>::fit_to_size() {
    if (this->capacity() > this->size() * 2) {
        this->capacity() /= 2;
        typ* tmp = new typ[this->capacity()];

        for (unsigned long i = 0; i < this->capacity(); i++) {
            tmp[i] = this->_data[i];
        }
        delete[] this->_data;
        this->_data = tmp;
    }
}

template <typename typ>
unsigned long& shl::vector<typ>::size() {
    return this->_size;
}

template <typename typ>
unsigned long& shl::vector<typ>::capacity() {
    return this->_capacity;
}

#endif 