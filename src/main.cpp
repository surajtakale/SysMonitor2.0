#include "ncurses_display.h"
#include "system.h"

#include "iostream"

int main() {
  System system;
  // std::cout<<"Welcome to the Curses Display";
  // std::cout<<system.Kernel();
  // NCursesDisplay::Display(system);
  for (int i=0; i < system.Processes().size(); i++){
    auto process = system.Processes()[i];
    std::cout<<process.Command()<<std::endl;
    std::cout<<process.CpuUtilization()<<std::endl;
    std::cout<<process.Pid()<<std::endl;
    std::cout<<process.Ram()<<std::endl;
    std::cout<<process.UpTime()<<std::endl;
    std::cout<<process.User()<<std::endl;
    std::cout<<"________________________________________________________________"<<std::endl;
  }
}