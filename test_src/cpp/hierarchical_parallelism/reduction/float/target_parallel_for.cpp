#include <iostream>
#include <stdexcept>


#include <cmath>
#include <limits>



bool almost_equal(float x, float y, int ulp) {

     return std::fabs(x-y) <= std::numeric_limits<float>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<float>::min();

}



void test_target_parallel_for(){

 // Input and Outputs
 
 const int L = 5;

float counter{};

// Main program

#pragma omp target parallel for  reduction(  +  :counter)   map(tofrom:counter) 

    for (int i = 0 ; i < L ; i++ )

{




counter += float { 1 };



}


// Validation
if ( !almost_equal(counter,float { L }, 10)  ) {
    std::cerr << "Expected: " << L << " Get: " << counter << std::endl;
    throw std::runtime_error( "target_parallel_for give incorect value when offloaded");
}

}
int main()
{
    test_target_parallel_for();
}