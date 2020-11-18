#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstdlib>

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>


using namespace std;


int main(int ac, char* av[])
{
    po::options_description description("Options");
    description.add_options()
        ("p", po::value<int>(), "Sets the number of processes")
        ("m", po::value<int>(), "Sets the number of vectors in files")
        ("t", po::value<int>(), "Sets vector dimension")
        ("i", po::value<string>(), "Sets input file prefix")
        ("o", po::value<string>(), "Sets output file prefix")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, description), vm);
    po::notify(vm); 
    cout<<description;   
    cout<<"\nInitial numbers are: p = 3, m = 3, t = 4, i = input, o = output\n";



    int p = 3;
    int m = 3;
    int t = 4;
    string i = "/home/genesisviva/Parallel/lab1/inputFiles/input", o = "/home/genesisviva/Parallel/lab1/outputFiles/output";

    if (vm.count("p")){
        p = vm["p"].as<int>();
    }

    if (vm.count("m")){
        m = vm["m"].as<int>();
    }
    
    if (vm.count("t")){
        t = vm["t"].as<int>();
    }
    if (vm.count("o")){
        o = vm["o"].as<string>();
    }

    if (vm.count("i")){
        i = vm["i"].as<string>();
    }

    string temp = i;

    for (int j = 1; j < p+1; j++){
        ofstream input;
        temp.append(to_string(j));
        input.open(temp);

        for (int k = 0; k < m; k++){
            for (int l = 0; l < t; l++){
                input << rand()/100000000 << " ";
            }
            if (k!=m-1)
                input << "\n";
        }

        input.close();
        temp = i;
        
    }


   string boo = i, boo2 = o;

   

   for (int j = 0; j<p;j++){
       if (fork() == 0){
        boo.append(to_string(j+1));
        boo2.append(to_string(j+1));

        
        const char *boo_ = boo.c_str();
        const char *boo2_ = boo2.c_str();


        execl("/home/genesisviva/Parallel/lab1/build/child", " ", boo_, boo2_, NULL);
        boo = i;
        boo2 = o;
        break;
       }
   }



    

    return 0;
}
