#include <poll.h>
#include <signal.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using std::string;
using std::vector;

int main(int argc, char **argv) {

  while (true) {
    char buffer[64] = "";

    int readDecs = atoi(argv[1]);
    int writeDecs = atoi(argv[2]);
    int n = read(readDecs, buffer, sizeof(buffer));   


    string bufferS = buffer;
    size_t pos1 = bufferS.find_first_of("0123456789");
    size_t pos2 = bufferS.find_last_of("0123456789");
    string numberS = bufferS.substr(pos1, pos2);
    int number = stoi(numberS);
    sleep(number);

    string send = buffer;
    send += " - done by pid:" + std::to_string(getpid());

    write(writeDecs, send.c_str(), send.size());
  }

  return 0;
}
