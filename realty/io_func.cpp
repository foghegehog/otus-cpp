#include "io_func.h"

feature_point parse_line(const std::string& line)
{    
    using namespace std;

    feature_point p;
    string::size_type start = 0;

    for (auto f = 0; f < DIMENTIONS - 1; ++f)
    {
        auto end = line.find_first_of(DELIMITER, start);
        p(f) = atof(line.substr(start, end - start).c_str());
        start = end + 1;
    }

    p(DIMENTIONS - 1) = 1.0;
    auto end = line.find_first_of(DELIMITER, start);
    auto floor = atoi(line.substr(start, end - start).c_str());
    if (floor == 1)
    {
        p(DIMENTIONS - 1) = 0.0;
    }
    else
    {
        start = end + 1;
        end = line.size();
        auto storeys = atoi(line.substr(start, end - start).c_str());
        if (storeys == floor)
        {
            p(DIMENTIONS - 1) = 0.0;
        }
    }

    return p;
}

void serialize_model(
    const std::string& filename,
    const dlib::kkmeans<kernel_type>& kmeans_model,
    const dlib::vector_normalizer<feature_point>& normalizer,
    const std::vector<feature_point>& dataset)
{
    using namespace dlib;

    auto serializer = serialize(filename);
    serializer << kmeans_model;
    serializer << normalizer;

    std::vector<std::vector<int>> cluster_indexes(kmeans_model.number_of_centers());
    for(auto i = 0; i < dataset.size(); ++i)
    {
        auto cluster = kmeans_model(normalizer(dataset[i]));
        cluster_indexes[cluster].push_back(i);
    }

    for (auto cluster = 0; cluster < cluster_indexes.size(); ++cluster)
    {
        for(auto index = 0; index < cluster_indexes[cluster].size(); ++index)
        {
            serializer << dataset[index];
        }
    }
}

void deserialize_model(const std::string& filename,
    dlib::kkmeans<kernel_type>& kmeans_model,
    dlib::vector_normalizer<feature_point>& normalizer,
    std::vector<std::vector<feature_point>>& clusters)
{
    using namespace dlib;

    auto deserializer = deserialize(filename);

    deserializer >> kmeans_model;
    deserializer >> normalizer;
    
    auto num_centers = kmeans_model.number_of_centers();
    clusters.reserve(num_centers);
    for(unsigned long cluster = 0; cluster < num_centers; ++cluster)
    {                       
        auto num_points = kmeans_model.get_kcentroid(cluster).samples_trained();
        clusters[cluster] = std::vector<feature_point>();        
        clusters[cluster].reserve(num_points);
        for(auto p = 0; p < num_points; ++p)
        {
            feature_point item;
            deserializer >> item; 
            clusters[cluster].push_back(item);
        }
    }
}