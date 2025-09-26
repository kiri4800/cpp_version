//
// Created by kiri4800 on 21.08.2025.
//

#ifndef CPP_MODEL_H
#define CPP_MODEL_H
//#include <torch/script.h>
#include <opencv2/opencv.hpp>
#include <iostream>
class Model {
public :
    Model(const std::string& path);
    void test(const std::string& pathToImage);

private:
//    torch::jit::script::Module module;
};


#endif//CPP_MODEL_H
