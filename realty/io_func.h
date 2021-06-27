#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <dlib/clustering.h>
#include <string>
#include <vector>

const int DIMENTIONS = 7;
const char DELIMITER = ';';

typedef dlib::matrix<float, DIMENTIONS, 1> feature_point;

typedef dlib::radial_basis_kernel<feature_point> kernel_type;

feature_point parse_line(const std::string& line);

void serialize_model(
    const std::string& filename,
    const dlib::kkmeans<kernel_type>& kmeans_model,
    const dlib::vector_normalizer<feature_point>& normalizer,
    const std::vector<feature_point>& dataset);

void deserialize_model(const std::string& filename,
    dlib::kkmeans<kernel_type>& kmeans_model,
    dlib::vector_normalizer<feature_point>& normalizer,
    std::vector<std::vector<feature_point>>& clusters);

#endif