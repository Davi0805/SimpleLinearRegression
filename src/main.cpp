#include <iostream>
#include <vector>
#include "rapidcsv.h"

# define TARGET_FILENAME "data/data.csv"
# define LEARNING_RATE 0.001


# ifdef PREDICT_ESTIMATE_FUNCTION

// estimatePrice(mileage) = θ0 + (θ1 ∗ mileage)
inline double estimatePrice( double mileage, double theta0, double theta1 ) {

    // (θ1 ∗ mileage)
    auto mileageRelation = theta1 * mileage;
    
    return theta0 + mileageRelation;

}

# else

// estimatePrice(mileage) = θ0 + (θ1 ∗ mileage)
inline double estimatePrice( double mileage ) {

    // (θ1 ∗ mileage)
    auto mileageRelation = 0 * mileage;
    
    return 0 + mileageRelation;

}

# endif





double zero_theta_train( std::vector< double > mileage, std::vector< double > price ) {
    
    u_int32_t m = mileage.size();
    double summation{0};

    for ( u_int32_t it{0}; it < mileage.size() ; it++ )
    {
       summation += estimatePrice(mileage[it]) - price[it];
       //std::cout << "TRAIN | Summation = " << summation << std::endl;
    }

    double learningRateResult = (1 / m - 1) * LEARNING_RATE; 

    return learningRateResult * summation;
}

double first_theta_train( std::vector< double > mileage, std::vector< double > price ) {

    u_int32_t m = mileage.size();
    double summation{0};

    for ( u_int32_t it{0} ; it < mileage.size(); it++ ) {

        summation += (estimatePrice(mileage[it]) - price[it]) * mileage[it];

    }

    double learningRateResult = (1 / m - 1) * LEARNING_RATE; 

    return learningRateResult * summation;

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
    
    
    auto theta0Result = zero_theta_train(mileage, price);
    auto theta1Result = first_theta_train(mileage, price);
    
    std::cout << "TRAIN | Theta0 result = " << theta0Result << std::endl;
    std::cout << "TRAIN | Theta1 result = " << theta1Result << std::endl;

    //for ( u_int32_t it{0}; it < mileage.size(); it++ ) {
    //
    //    std::cout << "TEST: Mileage = " << mileage[it] << " | " << "Price = " << price[it] << std::endl;
    //    std::cout << "TEST: RESULT = " << estimatePrice(mileage[it], 0, 0) << std::endl;
    //    
    //}
    


    return 0;
}
