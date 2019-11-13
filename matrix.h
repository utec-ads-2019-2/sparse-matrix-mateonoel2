#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <iomanip>
#include "node.h"
#include <vector>
using namespace std;

template <typename T>
class Matrix {
private:
    Node<T> * root;
    vector<SparseMatrix<T>*> vectorX;
    vector<SparseMatrix<T>*> vectorY;
    unsigned rows{}, columns{};

    Node<T>* findNode(unsigned row,unsigned column) const{
        if (row > rows or column > columns)
            throw invalid_argument("Index out of range");
        Node<T>* currentNode = vectorY[row]->target;
        while (currentNode) {
            if (currentNode->posX == column)
                return currentNode;
            currentNode = currentNode->next;
        }
        return nullptr;
    }

    void deleteB(Node<T>* node){
        if(node->next) deleteB(node->next);
        delete node;
    }

    void deleteRows(){
        for(unsigned i=0; i<rows; i++){
            deleteB(vectorY[i]->target);
        }
    }
public:
    Matrix(unsigned rows, unsigned columns){
        this->rows = rows;
        this->columns = columns;
        for(unsigned i = 0; i<rows; i++){
            vectorY.push_back(new SparseMatrix<T>);
        }
        for(unsigned i = 0; i<columns; i++){
            vectorX.push_back(new SparseMatrix<T>);
        }
    }

    void set(unsigned row, unsigned column, T Data){
        if (row > rows or column > columns or row < 0 or column < 0)
            throw invalid_argument("Index out of range");

        SparseMatrix<T>* sourceRow = vectorY[row];
        SparseMatrix<T>* sourceColumn = vectorX[column];

        bool dataIs0 = false;
        if (is_same<T, char>::value){
            if(Data=='0') dataIs0 = true;
        }
        if(Data==0) dataIs0 = true;

        if (findNode(row, column)) {
            if(dataIs0){
                Node<T>* toDelete=findNode(row, column), *toDeleteNext = toDelete->next, *toDeleteDown = toDelete->down;
                if (sourceRow->target == toDelete){
                    sourceRow->target = toDeleteNext;
                }
                else{
                    Node<T>* rowIt = sourceRow->target;
                    while (rowIt->next != toDelete) rowIt = rowIt->next;
                    rowIt->next = toDeleteNext;
                }
                if (sourceColumn->target == toDelete) {
                    sourceColumn->target = toDeleteDown;
                }
                else {
                    Node<T>* columnIt = sourceColumn->target;
                    while (columnIt->down != toDelete) columnIt = columnIt->down;
                    columnIt->down = toDeleteDown;
                }
                delete toDelete;
                return;
            }
            else{
                auto temp = findNode(row, column);
                if(temp->Data != Data) {
                    temp->Data = Data;
                    return;
                }
            }
        }
        else{
            if(!dataIs0){
                auto node = new Node<T>();
                node->posY = row;
                node->posX = column;
                node->Data = Data;
                if (!sourceRow->target)
                    sourceRow->target = node;
                else{
                    auto rowIt = sourceRow->target;
                    if (column < rowIt->posX) {
                        sourceRow->target = node;
                        node->next = rowIt;
                    }
                    else if (column > rowIt->posX) {
                        while (rowIt->next and rowIt->next->posX < column) rowIt = rowIt->next;
                        auto nodePrev = rowIt, nodeNext = rowIt->next;
                        nodePrev->next = node;
                        node->next = nodeNext;
                    }
                }
                if (!sourceColumn->target)
                    sourceColumn->target = node;
                else{
                    Node<T>* columnIt = sourceColumn->target;
                    if (row > columnIt->posY) {
                        sourceColumn->target = node;
                        node->down = columnIt;
                    }
                    else if (row < columnIt->posY) {
                        while (columnIt->down and columnIt->down->posY < row) columnIt = columnIt->down;
                        auto nodeUp = columnIt, nodeDown = columnIt->down;
                        nodeUp->down = node;
                        node->down = nodeDown;
                    }
                }
                return;
            }
        }
    }

    T operator()(unsigned row, unsigned column){
        Node<T>* temp = findNode(row, column);
        if(temp) return temp->Data;
        else return 0;
    }

    Matrix<T> operator*(T scalar) const{
        Matrix<T> temp(rows,columns);
        if(scalar)
            for(int i=0;i<rows;++i){
                auto nodeIt = vectorY[i]->target;
                while(nodeIt){
                    temp.set(nodeIt->posY, nodeIt->posX, nodeIt->Data*scalar);
                    nodeIt = nodeIt->next;
                }
            }
        return temp;
    }

    //Memory lost
    Matrix<T> operator*(Matrix<T> other) const{
        if(columns!=other.rows) throw invalid_argument("Can not be solved");
        auto temp = new Matrix<T>(rows,other.columns);
        for(int i=0; i<rows; ++i) {
            for (int j = 0; j < other.columns; ++j) {
                T sum = 0;
                for (int z = 0; z < columns; ++z){
                    if(findNode(i, z) and other.findNode(z,j)) sum += findNode(i, z)->Data * other.findNode(z, j)->Data;
                }
                temp->set(i, j, sum);
            }
        }
        return *temp;
    }

    //Memory lost?
    Matrix<T> operator+(Matrix<T> other) const{
        if(rows!=other.rows or columns!=other.columns) throw invalid_argument("Can not be solved");
        Matrix<T> temp (rows, columns);
        for(int i=0; i<rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if(findNode(i, j) and other.findNode(i,j)){
                    T sum = findNode(i, j)->Data + other.findNode(i, j)->Data;
                    temp.set(i, j, sum);
                    continue;
                }
                if(findNode(i, j)){
                    T sum = findNode(i, j)->Data;
                    temp.set(i, j, sum);
                    continue;
                }
                if(other.findNode(i,j)){
                    T sum = other.findNode(i, j)->Data;
                    temp.set(i, j, sum);
                    continue;
                }
            }
        }
        return temp;
    }

    //Memory lost?
    Matrix<T> operator-(Matrix<T> other) const{
        if(rows!=other.rows or columns!=other.columns) throw invalid_argument("Can not be solved");
        Matrix<T> temp (rows, columns);
        for(int i=0; i<rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if(findNode(i, j) and other.findNode(i,j)){
                    T rest = findNode(i, j)->Data - other.findNode(i, j)->Data;
                    temp.set(i, j, rest);
                    continue;
                }
                if(findNode(i, j)){
                    T rest = findNode(i, j)->Data;
                    temp.set(i, j, rest);
                    continue;
                }
                if(other.findNode(i,j)){
                    T rest = 0 - other.findNode(i, j)->Data;
                    temp.set(i, j, rest);
                    continue;
                }
            }
        }
        return temp;
    }

    //Memory lost?
    Matrix<T> transpose() const{
        Matrix<T> temp (rows, columns);
        for(int i=0; i<rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if(findNode(i,j)) temp.set(j, i, findNode(i,j)->Data);
            }
        }
        return temp;
    }

    void print() const{
        for (int i = 0; i < rows; ++i) {
            Node<T>* nodeIt = vectorY[i]->target;
            if (nodeIt) {
                for (int j = 0; j < columns; ++j) {
                    cout << setw(10);
                    if (nodeIt) {
                        if (nodeIt->posX == j) {
                            cout << nodeIt->Data;
                            nodeIt = nodeIt->next;
                        }
                        else
                            cout << 0;
                    }
                    else
                        cout << 0;
                }
            }
            else
                for (int j = 0; j < columns; ++j)
                    cout << setw(10) << 0;
            cout << endl;
        }
    }

    ~Matrix(){
        //deleteRows();
    }
};
#endif //SPARSE_MATRIX_MATRIX_H