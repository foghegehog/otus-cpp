#include <iostream>
#include <string>
#include <vector>

#include <dlib/clustering.h>
#include <dlib/rand.h>

using namespace std;
using namespace dlib;

const int DIMENTIONS = 7;
const char DELIMITER = ';';

// The price field is numericaly much bigger than others and can affect the computations 
// such as all other fields will become neglible
const float PRICE_NORMILIZER = 100000.0;
const float ROOMS_NORMALIZER = 0.1;

typedef matrix<double,DIMENTIONS,1> feature_point;
typedef radial_basis_kernel<feature_point> kernel_type;

int main(int argc, char* argv[])
{    

    int num_clusters;
    std::string modelname;
    if (argc == 1)
    {
        num_clusters = 10;
        modelname = "";
    }
    else if (argc == 3)
    {
        num_clusters = std::atoi(argv[1]);
        modelname = std::string(argv[2]);
    }
    else
    {
        std::cout << "Usage: rclst <num_clusters> <model_name>" << std::endl;
        return 0;
    }
    

    kcentroid<kernel_type> kc(kernel_type(0.001), 0.9, 8);
    kkmeans<kernel_type> kmeans_clustering(kc);

    std::vector<feature_point> dataset;
    std::vector<feature_point> initial_centers;

    dlib::rand rnd;

    for(std::string line; getline(cin, line); )
    {
        string::size_type start = 0;
        feature_point m;
        for (auto f = 0; f < DIMENTIONS - 1; ++f)
        {
            auto end = line.find_first_of(DELIMITER, start);
            m(f) = atof(line.substr(start, end - start).c_str());
            start = end + 1;
        }

        m(DIMENTIONS - 1) = 1.0;
        auto end = line.find_first_of(DELIMITER, start);
        auto floor = atoi(line.substr(start, end - start).c_str());
        if (floor == 1)
        {
            m(DIMENTIONS - 1) = 0.0;
        }
        else
        {
            start = end + 1;
            end = line.size();
            auto storeys = atoi(line.substr(start, end - start).c_str());
            if (storeys == floor)
            {
                m(DIMENTIONS - 1) = 0.0;
            }
        }

        m(2) /= ROOMS_NORMALIZER;
        m(3) /=  PRICE_NORMILIZER;
        dataset.push_back(m);
    }

    kmeans_clustering.set_number_of_centers(num_clusters);

    // You need to pick some initial centers for the k-means algorithm.  So here
    // we will use the dlib::pick_initial_centers() function which tries to find
    // n points that are far apart (basically).  
    pick_initial_centers(num_clusters, initial_centers, dataset, kmeans_clustering.get_kernel());

    cout << "Running training..." << endl;
    kmeans_clustering.train(dataset, initial_centers);
    cout << "Done training." << endl;

    /*for (auto c = 0; c < num_clusters; ++c)
    {
        cout << "num dictionary vectors: " << kmeans_clustering.get_kcentroid(c).dictionary_size() << endl;
        cout << "samples trained: " << kmeans_clustering.get_kcentroid(c).samples_trained() << std::endl; 
    }*/

    cout << "Saving model" << endl;
    serialize(modelname) << kmeans_clustering;
    
    /*deserialize(modelname) >> kmeans_clustering;

    // now loop over all our samples and print out their predicted class.  In this example
    // all points are correctly identified.
    for (unsigned long i = 0; i < dataset.size(); ++i)
    {
        cout << kmeans_clustering(dataset[i]) << " " << std::endl;
    }*/

    cout << "Done" << endl;
    return 0;
}



