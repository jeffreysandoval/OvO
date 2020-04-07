#include <iostream>
#include <stdexcept>
#include <omp.h>

#include <cmath>
#include <limits>
template<class T>
bool almost_equal(T x, T y, int ulp) {
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<T>::min();
}

template<class T>
void test_target__parallel__for__simd(){

 // Input and Outputs
 
 const int L = 5;
 const int M = 6;

T counter{};

// Main program

#pragma omp target   defaultmap(tofrom:scalar) 

{


#pragma omp parallel  reduction(+:counter)  

{


#pragma omp for  

    for (int i = 0 ; i < L ; i++ )

{


#pragma omp simd  reduction(+:counter)  

    for (int j = 0 ; j < M ; j++ )

{




counter = counter + 1;



}

}

}

}


// Validation
if ( !almost_equal(counter,T{ L*M }, 1)  ) {
    std::cerr << "Expected: " << L*M << " Get: " << counter << std::endl;
    throw std::runtime_error( "target__parallel__for__simd give incorect value when offloaded");
}

}
int main()
{
    test_target__parallel__for__simd<double>();
}
