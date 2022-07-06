#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#ifndef MAIN_H
#define MAIN_H

extern std::string base_dir;
int main(int argc, char *argv[]);
std::string separate_base_dir(std::string expr);
std::string open_file(const char *name);
void write_file(std::string name, std::string content);

#endif