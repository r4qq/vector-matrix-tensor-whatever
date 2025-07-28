#include "../Tensor.hpp"

int main(void)
{
    Tensor::Tensor<int> A(1, 2);
    A.fill(5);
    A.print();          
    A = A * 3;
    A.print();

    Tensor::Tensor<int> B(2, 1);
    B.fill(3);
    B.print(); 
    B = 10 * B;
    B.print();    
    B = B.transpose();
    B.print();   

    auto C = A * B;
    C.print();

}