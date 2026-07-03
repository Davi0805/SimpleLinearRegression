#include <iostream>
#include <vector>
#include "rapidcsv.h"

# define TARGET_FILENAME "data/data.csv"


// estimatePrice(mileage) = θ0 + (θ1 ∗ mileage)
inline double estimatePrice( double mileage, double theta0, double theta1 ) {

    // (θ1 ∗ mileage)
    auto mileageRelation = theta1 * mileage;
    
    return theta0 + mileageRelation;

}

int main() {
    
    std::cout << "Loading | Linear Regression prediction" << std::endl;
    std::cout << "Loading | " << TARGET_FILENAME << std::endl;

    rapidcsv::Document data_source(TARGET_FILENAME);

    std::cout << "Loading | Found " << data_source.GetRowCount() << " rows\n";

    std::vector< double > mileage = data_source.GetColumn< double >("km");
    std::vector< double > price   = data_source.GetColumn< double >("price");

    std::cout << "Loading | Validating data integrity\n";

    if ( mileage.size() != price.size() ) { std::cout << "ERROR | Missing data in a row\n"; }

    for ( u_int32_t it{0}; it < mileage.size(); it++ ) {

        std::cout << "TEST: Mileage = " << mileage[it] << " | " << "Price = " << price[it] << std::endl;
        std::cout << "TEST: RESULT = " << estimatePrice(mileage[it], 0, 0) << std::endl;
        
    }
    


    return 0;
}
