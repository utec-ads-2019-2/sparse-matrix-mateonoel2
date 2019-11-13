#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    Node<T> *next, *down;
    int posX{}, posY{};
    T Data;
public:
    explicit Node(): next(nullptr), down(nullptr){}
    ~ Node(){
        next = nullptr;
        down = nullptr;
    }
    friend class Matrix<T>;
};

template <class T>
class SparseMatrix {
private:
    Node<T>* target;
public:
    explicit SparseMatrix():target(nullptr){}
    ~ SparseMatrix(){
        target = nullptr;
    }
    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
