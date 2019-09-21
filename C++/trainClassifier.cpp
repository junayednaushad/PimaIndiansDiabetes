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

void getCentroids(std::string filename, std::vector<double> &p, std::vector<double> &n)
{
    assert(p.size() == 8 && n.size() == 8);

    std::ifstream trainingFile;
    trainingFile.open(filename);
    if(!trainingFile.is_open())
    {
        std::cerr << "unable to open " << filename << std::endl;
        std::exit(0);
    }
    
    int numOfPositive = 0;
    int numOfNegative = 0;

    std::string preg, gluc, press, skin, ins, bmi, dpf, age, classification;
    double preg_d, gluc_d, press_d, skin_d, ins_d, bmi_d, dpf_d, age_d, classification_d;  
    while(!trainingFile.eof())
    {
        getline(trainingFile, preg, ',');
        preg_d = std::stod(preg);
        preg_d /= 28;

        getline(trainingFile, gluc, ',');
        gluc_d = std::stod(gluc);
        gluc_d /= 200;

        getline(trainingFile, press, ',');
        press_d = std::stod(press);
        press_d /= 125;

        getline(trainingFile, skin, ',');
        skin_d = std::stod(skin);
        skin_d /= 100;

        getline(trainingFile, ins, ',');
        ins_d = std::stod(ins);
        ins_d /= 850;

        getline(trainingFile, bmi, ',');
        bmi_d = std::stod(bmi);
        bmi_d /= 68;

        getline(trainingFile, dpf, ',');
        dpf_d = std::stod(dpf);
        dpf_d /= 2.45;

        getline(trainingFile, age, ',');
        age_d = std::stod(age);
        age_d /= 100;

        getline(trainingFile, classification, '\n');
        classification_d = std::stod(classification);

        if(classification_d == 0)
        {
            numOfNegative += 1;
            n[0] += preg_d;
            n[1] += gluc_d;
            n[2] += press_d;
            n[3] += skin_d;
            n[4] += ins_d;
            n[5] += bmi_d;
            n[6] += dpf_d;
            n[7] += age_d;
        }
        else
        {
            numOfPositive += 1;
            p[0] += preg_d;
            p[1] += gluc_d;
            p[2] += press_d;
            p[3] += skin_d;
            p[4] += ins_d;
            p[5] += bmi_d;
            p[6] += dpf_d;
            p[7] += age_d;
        }
    }
    double cons_p = 1.0/numOfPositive;
    double cons_n = 1.0/numOfNegative;
    multConstant(cons_p, p);
    multConstant(cons_n, n);

    std::cout << "Total training data: " << numOfPositive+numOfNegative << std::endl;
    trainingFile.close();
}

double getThreshold(std::vector<double> const &p, std::vector<double> const &n)
{
    std::vector<double> sum = add(p,n);
    std::vector<double> diff = subtract(p,n);
    double dot_product = dotProduct(sum,diff);
    return 0.5*dot_product;
}

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        std::cerr << "usage: trainClassifier trainingData.csv" << std::endl;
        std::exit(0);
    }

    std::string trainingFile = argv[1]; 

    std::vector<double> p = {0,0,0,0,0,0,0,0};
    std::vector<double> n = {0,0,0,0,0,0,0,0};
    
    getCentroids(trainingFile,p,n);
    std::vector<double> w = subtract(p,n);
    double t = getThreshold(p,n);
    
    printVector(w);
    std::cout << t << std::endl;

    return 0;
}