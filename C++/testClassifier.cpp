#include <vector>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

double dotProduct(std::vector<double> const &a, std::vector<double> const &b)
{
    assert(a.size() == b.size());
    double dot_product = 0;
    for(int i = 0; i < a.size(); i++)
        dot_product += (a.at(i) * b.at(i));
    return dot_product;
}

std::vector<double> add(std::vector<double> const &a, std::vector<double> const &b)
{
    assert(a.size() == b.size());
    std::vector<double> sum;
    for(int i = 0; i < a.size(); i++)
        sum.push_back(a.at(i) + b.at(i));
    return sum;
}

std::vector<double> subtract(std::vector<double> const &a, std::vector<double> const &b)
{
    assert(a.size() == b.size());
    std::vector<double> diff;
    for(int i = 0; i < a.size(); i++)
        diff.push_back(a.at(i) - b.at(i));
    return diff;
}

void multConstant(double constant, std::vector<double> &a)
{
    for(int i = 0; i < a.size(); i++)
        a[i] *= constant;
}

void printVector(std::vector<double> const &a)
{
    for(int i = 0; i < a.size(); i++)
        std::cout << a[i] << " ";
    std::cout << std::endl; 
}

void testClassifier(std::string filename, std::vector<double> const &w, double t)
{
    assert(w.size() == 8);
    std::ifstream testingFile;
    testingFile.open(filename);
    if(!testingFile.is_open())
    {
        std::cerr << "unable to open " << filename << std::endl;
        std::exit(0);
    }

    std::vector<double> x = {0,0,0,0,0,0,0,0};
    double numOfTests = 0;
    double numOfErrors = 0;
    double prediction;
    std::string preg, gluc, press, skin, ins, bmi, dpf, age, classification;
    double preg_d, gluc_d, press_d, skin_d, ins_d, bmi_d, dpf_d, age_d, classification_d;  
    while(!testingFile.eof())
    {
        numOfTests++;

        getline(testingFile, preg, ',');
        preg_d = std::stod(preg);
        preg_d /= 28;
        x[0] = preg_d;

        getline(testingFile, gluc, ',');
        gluc_d = std::stod(gluc);
        gluc_d /= 200;
        x[1] = gluc_d;

        getline(testingFile, press, ',');
        press_d = std::stod(press);
        press_d /= 125;
        x[2] = press_d;

        getline(testingFile, skin, ',');
        skin_d = std::stod(skin);
        skin_d /= 100;
        x[3] = skin_d;

        getline(testingFile, ins, ',');
        ins_d = std::stod(ins);
        ins_d /= 850;
        x[4] = ins_d;

        getline(testingFile, bmi, ',');
        bmi_d = std::stod(bmi);
        bmi_d /= 68;
        x[5] = bmi_d;

        getline(testingFile, dpf, ',');
        dpf_d = std::stod(dpf);
        dpf_d /= 2.45;
        x[6] = dpf_d;

        getline(testingFile, age, ',');
        age_d = std::stod(age);
        age_d /= 100;
        x[7] = age_d;

        getline(testingFile, classification, '\n');
        classification_d = std::stod(classification);

        double dot_product = dotProduct(x,w);
        if(dot_product > t)
            prediction = 1;
        else
            prediction = 0;
        
        if(prediction != classification_d)
            numOfErrors++;
    }
    std::cout << "Number of tests: " << numOfTests << std::endl;
    double errorRate = numOfErrors/numOfTests;
    std::cout << "Error rate: " << errorRate << std::endl;
    testingFile.close();
}

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        std::cerr << "usage: testClassifier testData.csv" << std::endl;
        std::exit(0);
    }

    std::string testingFile = argv[1];  

    std::vector<double> w = {0.0540101, 0.157382, 0.0196776, 0.0258488, 0.0442285, 0.0785162, 0.0545552, 0.0545369};
    double t = 0.194649;
    
    testClassifier(testingFile,w,t);

    return 0;
}