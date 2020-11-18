#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <cmath>
#include <fstream>
#include <unistd.h>

using namespace std;


int main(int ac, char* av[])
{
    auto start = std::chrono::system_clock::now();
    string temp;
    //string pathInput = "/home/genesisviva/Parallel/lab1/inputFiles/input1";    //Default path
    //string pathOutput = "/home/genesisviva/Parallel/lab1/outputFiles/output1"; //Default path
    string pathInput = av[1];
    string pathOutput = av[2];
    ifstream input(pathInput);
    int count = 0;

    vector <int> resVec;
    string x;

    int boo = 1;

    while (!input.eof()){
        getline(input, temp);
        stringstream buff(temp);

        int k = 0;

        while (temp != " "){
            buff >> x;
            size_t pos = temp.find(x);
            pos = pos + x.size();
            temp = temp.substr(pos);
            if (boo == 1){
                resVec.push_back(stoi(x));
            }
            else{
                resVec[k] += stoi(x);
            }
            k++;
        }
        boo = 0;
        count++;
    }
    int m = count; //Number of vectors in file
    int t = resVec.size(); //Vector size
    double modRes = 0; //Vector modulus
    int id = getpid(); //PID

    for (int i = 0; i < t; i++){
        modRes = modRes + pow(resVec[i], 2);
    }
    modRes = sqrt(modRes);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double, milli> elapsed = end - start; //Elapased Time
    

    ofstream out(pathOutput);
    out <<"Name:" << pathInput<<endl;
    out <<"Count:" << m << endl;
    out <<"Module:" << modRes << endl;
    out <<"Pid:" << id << endl;
    out <<"Time" << elapsed.count() << " ms";


    return 0;
}