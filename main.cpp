#include <iostream>
#include "tester/tester.h"
#include "matrix.h"


int main() {
    Tester::execute();
    Matrix<int> m(2, 2);
    m.set(0, 0, 7);
    m.set(0, 1, 2);
    m.set(1, 0, 3);
    m.set(1, 1, 4);
    m.print();
    cout<<'\n';
    auto m2 = m*2;
    m2.print();
    cout<<'\n';
    auto m3 = m * m2;
    m3.print();
    cout<<'\n';
    auto m4 = m3 + m2;
    m4.print();
    cout<<'\n';
    auto m5 = m4 - m3;
    m5.print();
    cout<<'\n';
    auto m6 = m5.transpose();
    m6.print();
    return EXIT_SUCCESS;
}

