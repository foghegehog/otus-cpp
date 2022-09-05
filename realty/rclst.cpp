#include "io_func.h"

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <dlib/clustering.h>

using namespace std;
using namespace dlib;

int main(int argc, char* argv[])
{    

    int num_clusters;
    std::string modelfname;
    if (argc == 1)
    {
        num_clusters = 10;
        modelfname = "kmeans_model";
    }
    else if (argc == 3)
    {
        num_clusters = std::atoi(argv[1]);
        modelfname = std::string(argv[2]);
    }
    else
    {
        std::cout << "Usage: rclst <num_clusters> <model_fname>" << std::endl;
        return 0;
    }
    
    kcentroid<kernel_type> kc(kernel_type(0.01), 0.5, 8); 
    kkmeans<kernel_type> kmeans_clustering(kc);

    std::vector<feature_point> dataset;
    std::vector<feature_point> initial_centers;

    dlib::rand rnd;

    for(std::string line; getline(cin, line); )
    {        
        dataset.push_back(parse_line(line));
    }

    cout << "Normalizing..." << endl;
    std::vector<feature_point> normalized_dataset;
    dlib::vector_normalizer<feature_point> normalizer;
    normalizer.train(dataset);

    for (unsigned long i = 0; i < dataset.size(); ++i)
        normalized_dataset.push_back(normalizer(dataset[i]));

    kmeans_clustering.set_number_of_centers(num_clusters);
    pick_initial_centers(num_clusters, initial_centers, normalized_dataset, kmeans_clustering.get_kernel());

    cout << "Training..." << endl;
    kmeans_clustering.train(normalized_dataset, initial_centers);
    cout << "Done training." << endl;

    /*for (auto c = 0; c < num_clusters; ++c)
    {
        cout << "samples trained: " << kmeans_clustering.get_kcentroid(c).samples_trained() << std::endl; 
    }*/

    cout << "Saving model." << endl;
    serialize_model(modelfname, kmeans_clustering, normalizer, dataset);
    cout << "Done" << endl;
    return 0;
}



