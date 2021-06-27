#include "io_func.h"

#include <iostream>

int main(int argc, char* argv[])
{    
    using namespace dlib;
    using namespace std;

    std::string modelfname;
    if (argc == 1)
    {
        modelfname = "kmeans_model";
    }
    else if (argc == 2)
    {
        modelfname = std::string(argv[1]);
    }
    else
    {
        std::cout << "Usage: rclss <model_fname>" << std::endl;
        return 0;
    }

    kcentroid<kernel_type> kc;
    kkmeans<kernel_type> kmeans_clustering(kc);
    dlib::vector_normalizer<feature_point> normalizer;
    std::vector<std::vector<feature_point>> clusters;

    cout << "Deserializing." << endl;
    deserialize_model(modelfname, kmeans_clustering, normalizer, clusters);

    cout << "Reading input." << endl;
    for(std::string line; getline(cin, line); )
    {
        auto item = parse_line(line);
        auto item_normalized = normalizer(item);
        auto cluster_index = kmeans_clustering(item_normalized);
        auto kernel = kmeans_clustering.get_kcentroid(cluster_index).get_kernel();
        distance_function<kernel_type> distance_func(kernel, item_normalized); 

        auto cluster = clusters[cluster_index];
        std::map<float, int> ordered_points;
        for(auto p = 0; p < cluster.size(); ++p)
        {
            ordered_points.emplace(distance_func(cluster[p]), p);
        } 

        cout << "Closest offers: " << std::endl;
        for(const auto& pair: ordered_points)
        {
            cout << cluster[pair.second] << endl;
        }
    }

    return 0;
}