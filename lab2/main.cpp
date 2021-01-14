#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <poll.h>
#include <signal.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <vector>

using namespace std;

int getRandom(int min, int max){    //Получение случайного числа в диапазоне от min до max
    while (1){
        int res = 1 + rand() % max;
        if (res>=min){
            return res;
        }
    }
}




queue<string> createTasks(const string& f) {
  queue<string> tasks;
  std::ofstream out(f);
  vector <string> problems = {"Walk with a dog", "Clean the kitchen", "Water the flowers", "Buy some food", "Go to work", "Get dress", "Do homework"};
  int taskNumber = getRandom(5, 15);

  for (int i = 1; i <= taskNumber; ++i) {
    int curTask = getRandom(0,problems.size()-1);
    string task = problems[curTask] + " " +
                  std::to_string(getRandom(1,5));
    out << task + "\n";
    tasks.push(task);
  }

  out.close();
  return tasks;
}

int main(int ac, char* av[]) {

    ofstream z("/home/genesisviva/Parallel/lab2/txtFiles/report");
    z.close();


    srand((unsigned) time(0));
    po::options_description description("Options");
    description.add_options()
    ("p", po::value<int>(), "Sets the number of processes")
    ("f", po::value<string>(), "Sets file name")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, description), vm);
    po::notify(vm); 
    std::cout<<description;   
    std::cout<<"\nInitial numbers are: p = 3, f = /home/genesisviva/Parallel/lab2/txtFiles/input\n";
    

    int p  = 3;
    string f = "/home/genesisviva/Parallel/lab2/txtFiles/input";

    if (vm.count("p")){
        p = vm["p"].as<int>();
    }
    if (vm.count("f")){
        f = vm["f"].as<string>();
    }

  queue<string> tasks = createTasks(f);
  int initialTaskSize = tasks.size();

  int fdTo[p][2];
  int fdFrom[p][2];
  pid_t pid[p];
  struct pollfd fds[p];

  for (int i = 0; i < p; ++i) {
    pipe(fdTo[i]);
    pipe(fdFrom[i]);
    fds[i].fd = fdFrom[i][0];
    fds[i].events = POLLIN;   //Ждем входящие данные
    fds[i].revents = 0;
    pid[i] = fork();

    if (pid[i] == 0) {  //Дочерний процесс. Внутри него цикл, поэтому не выйдет далее этого цикла здесь.
      execl("/home/genesisviva/Parallel/lab2/build/child", " ", std::to_string(fdTo[i][0]).c_str(), std::to_string(fdFrom[i][1]).c_str(), NULL);
      //аргумент 2 - дескриптор чтения pipe, аргумент 3 - декриптор записи pipe, NULL - конец массива указателей    
    } else {           //Родительский процесс
      if (!tasks.empty()) {
      write(fdTo[i][1], tasks.front().c_str(), tasks.front().size());
      printf("Task (%s) is sent\n", tasks.front().c_str());
      tasks.pop();
  }
    }
  }

  while (1) {
    int i;
    if (poll(fds, p, -1)) {   //fd - файловый дескриптор, p - количество событий (количество потоков), -1 - ожидания (бесконечность)
      for (i = 0; i < p; ++i) 
        if (fds[i].revents != 0) break; //Ищем первый среди наблюдаемых дескрипторов, где произошло событие (пришел ответ от дочернего процесса)

      fds[i].revents = 0; //Обнуляем поле revents, чтобы использовать структуру fds в следующей итерации
      char buffer[64] = "";
      int n = read(fdFrom[i][0], buffer, sizeof(buffer));   //Получаем данные от сооттествующего дочернего процесса

      std::ofstream out;    
      out.open("/home/genesisviva/Parallel/lab2/txtFiles/report", ios_base::app);    
      out << buffer << "\n";        //Пишем полученную строку в текстовый файл report


      if (!tasks.empty()) {         //Если еще остались задания
      write(fdTo[i][1], tasks.front().c_str(), tasks.front().size());     //Отправляем освободившемуся процессу
      printf("Task (%s) is sent\n", tasks.front().c_str());
      tasks.pop();                  //Убираем задания из очереди
      }
    }
    initialTaskSize--;
    if (initialTaskSize == 0){      //Если больше заданий не осталось, то выходим из цикла
      break;
    }

  }



  return 0;
}
