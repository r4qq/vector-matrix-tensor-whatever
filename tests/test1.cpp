#include "../Tensor.hpp"

int main(void)
{
    Tensor::Tensor<float> A(3, 3);
    A.fill(5.0);
    A.print();          
    A = A * 3.0;
    A.print();

    Tensor::Tensor<int> B(3, 3);
    B.fill(3);
    B.print(); 
    B = 10 * B;
    B.print();       

 

}