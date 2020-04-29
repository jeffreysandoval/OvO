#include <cmath>
#include <limits>
#include <iostream>
#include <cstdlib>
using namespace std;
bool almost_equal(bool x, bool y, int ulp) {
    return x == y ; 
}
void test_signbit(){
   float in0 {  0.42 };
   bool out1_host;
   bool out1_device;
    out1_host =  signbit( in0);
   #pragma omp target map(from: out1_device )
   {
     out1_device =  signbit( in0);
   }
   if ( !almost_equal(out1_host,out1_device, 4) ) {
        std::cerr << "Host: " << out1_host << " GPU: " << out1_device << std::endl;
        std::exit(112);
    }
}
int main()
{
    test_signbit();
}
