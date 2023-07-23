#include "ncurses_display.h"
#include "system.h"

#include "iostream"

int main() {
  System system;
  // std::cout<<"Welcome to the Curses Display";
  // std::cout<<system.Kernel();
  // NCursesDisplay::Display(system);
  for (int i=0; i < system.Processes().size(); i++){
    std::cout<<system.Processes()[i].Command();
  }
}