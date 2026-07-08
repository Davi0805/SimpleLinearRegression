#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include "rapidcsv.h"
#include <boost/program_options.hpp>

#define LEARNING_RATE 0.001

namespace po = boost::program_options;

struct MainParameters {
    std::string dataFilepath;
    rapidcsv::Document documentInstance;
    double      learningRate;
    u_int32_t   epoch;
};

u_int32_t inputLogic(int ac, char **av, MainParameters &param)
{
    po::options_description desc("Parameters");
    desc.add_options()
        ("e", po::value<u_int32_t>(), "Epoch")
        ("lr", po::value<double>(), "Learning rate")
        ("d", po::value<std::string>(), "Data path. CSV file");


    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (!vm.count("e") || !vm.count("lr") || !vm.count("d"))
    {
        std::cout << "ERROR | Missing parameters\n";
        std::cout << desc << std::endl;
        return -1;
    }
    param.dataFilepath = vm["d"].as<std::string>();
    param.epoch = vm["e"].as<u_int32_t>();
    param.learningRate = vm["lr"].as<double>();

    
    std::cout << "Loading | Linear Regression prediction" << std::endl;
    std::cout << "Loading | " << param.dataFilepath << std::endl;

    rapidcsv::Document data_source(param.dataFilepath);
    param.documentInstance = data_source;
    return 0;
}


// estimatePrice(mileage) = θ0 + (θ1 ∗ mileage)
inline double estimatePrice( double mileage, std::optional<double> theta0, std::optional<double> theta1 ) {

    // (θ1 ∗ mileage)
    auto mileageRelation = theta1.value_or(0.0) * mileage;
    
    return theta0.value_or(0.0) + mileageRelation;

}

double zero_theta_train(
    std::vector< double > mileage, std::vector< double > price,
    std::optional< double > theta0, std::optional< double > theta1
) {
    
    u_int32_t m = mileage.size();
    double summation{0};

    for ( u_int32_t it{0}; it < mileage.size() ; it++ )
    {
       summation += estimatePrice(mileage[it], theta0.value_or(NULL), theta1.value_or(NULL)) - price[it];
       //std::cout << "TRAIN | Summation = " << summation << std::endl;
    }

    double learningRateResult = LEARNING_RATE / static_cast<double>(m);

    return theta0.value_or(0.0) - learningRateResult * summation;
}

double first_theta_train( 
    std::vector< double > mileage, std::vector< double > price,
    std::optional< double > theta0, std::optional< double > theta1
) {

    u_int32_t m = mileage.size();
    double summation{0};

    for ( u_int32_t it{0} ; it < mileage.size(); it++ ) {

        summation += (estimatePrice(mileage[it], theta0.value_or(NULL), theta1.value_or(NULL)) - price[it]) * mileage[it];

    }

    double learningRateResult = LEARNING_RATE / static_cast<double>(m);

    return theta1.value_or(0.0) - learningRateResult * summation;

}


void epoch_loop(
    std::function< double(std::vector<double>, std::vector<double>, std::optional< double >, std::optional< double >) > expression0,
    std::function< double(std::vector<double>, std::vector<double>, std::optional< double >, std::optional< double >) > expression1,
    MainParameters &params
) {
    auto mileageVec = params.documentInstance.GetColumn< double >("km");
    auto priceVec   = params.documentInstance.GetColumn< double >("price");

    // mileage lives on a much larger scale than price (1e5 vs 1e4), which makes
    // the loss surface a steep, elongated canyon along theta1 and diverges
    // gradient descent. Train on a standardized mileage feature instead.
    double mileageMean = std::accumulate(mileageVec.begin(), mileageVec.end(), 0.0) / mileageVec.size();
    double mileageVariance{0.0};
    for (double km : mileageVec) {
        mileageVariance += (km - mileageMean) * (km - mileageMean);
    }
    mileageVariance /= mileageVec.size();
    double mileageStd = std::sqrt(mileageVariance);

    std::vector<double> mileageNormVec(mileageVec.size());
    for (size_t it{0}; it < mileageVec.size(); it++) {
        mileageNormVec[it] = (mileageVec[it] - mileageMean) / mileageStd;
    }

    // estimate = theta0Norm + theta1Norm * (mileage - mean) / std
    //          = (theta0Norm - theta1Norm * mean / std) + (theta1Norm / std) * mileage
    auto denormalize = [&](double theta0Norm, double theta1Norm) {
        double theta1Real = theta1Norm / mileageStd;
        double theta0Real = theta0Norm - theta1Norm * mileageMean / mileageStd;
        return std::make_pair(theta0Real, theta1Real);
    };

    double tmpTheta0 = expression0(
        mileageNormVec,
        priceVec,
        NULL,
        NULL
    );
    double tmpTheta1 = expression1(
        mileageNormVec,
        priceVec,
        NULL,
        NULL
    );
    for (auto i{1}; i != 0 && i <= params.epoch; i++)
    {
        auto oldTheta0 = tmpTheta0;
        auto oldTheta1 = tmpTheta1;
        std::cout << "--------------------------------------------------------\n";
        std::cout << "Running " << i << " cycle\n";
        auto [oldTheta0Real, oldTheta1Real] = denormalize(oldTheta0, oldTheta1);
        std::cout << "THETA 0 = " << oldTheta0Real << " | THETA1 = " << oldTheta1Real << "\n-----\n";
        tmpTheta0 = expression0(
            mileageNormVec,
            priceVec,
            oldTheta0,
            oldTheta1
        );
        tmpTheta1 = expression1(
            mileageNormVec,
            priceVec,
            oldTheta0,
            oldTheta1
        );
        auto [theta0Real, theta1Real] = denormalize(tmpTheta0, tmpTheta1);
        std::cout << "TESTING - KM = " << mileageVec[0] << " | PRICE = " << priceVec[0] << "\n" ;
        std::cout << "TESTING - ESTIMATED PRICE = " << estimatePrice(mileageVec[0], theta0Real, theta1Real) << "\n-----\n";
        std::cout << "TESTING - KM = " << mileageVec[1] << " | PRICE = " << priceVec[1] << "\n";
        std::cout << "TESTING - ESTIMATED PRICE = " << estimatePrice(mileageVec[1], theta0Real, theta1Real) << "\n-----\n";
        std::cout << "TESTING - KM = " << mileageVec[2] << " | PRICE = " << priceVec[2] << "\n";
        std::cout << "TESTING - ESTIMATED PRICE = " << estimatePrice(mileageVec[2], theta0Real, theta1Real) << "\n-----\n";
    }
    auto [finalTheta0Real, finalTheta1Real] = denormalize(tmpTheta0, tmpTheta1);
    std::cout << "RESULT - THETA0 = " << finalTheta0Real << " | THETA1 = " << finalTheta1Real << std::endl;
    std::cout << "--------------------------------------------------------\n";
}


int main(int ac, char **av) {

    MainParameters param;

    if ( inputLogic(ac, av, param) ) { return 0; }

    std::cout << "Loading | Found " << param.documentInstance.GetRowCount() << " rows\n";

    std::vector< double > mileage = param.documentInstance.GetColumn< double >("km");
    std::vector< double > price   = param.documentInstance.GetColumn< double >("price");

    std::cout << "Loading | Validating data integrity\n";

    if ( mileage.size() != price.size() ) { std::cout << "ERROR | Missing data in a row\n"; }
    
    
    epoch_loop(zero_theta_train, first_theta_train, param);


    return 0;
}
