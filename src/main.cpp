#include "ncurses_display.h"
#include "system.h"

#include "iostream"

int main() {
  System system;
  // std::cout<<"Welcome to the Curses Display";
  // std::cout<<system.Kernel();
  NCursesDisplay::Display(system);
}