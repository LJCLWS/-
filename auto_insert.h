#pragma once

#include <iostream>
#include <cstdlib>
#include <iterator>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "object.h"
using namespace std;

extern int terminal_count;
extern int fuction_count;


char* change_type(string s);
char* insert_log(char* s, char* t);
char* read_str(char* filename);
char* insert_str(char* s1, char* s2, int n);
int auto_match(char* s, char* t);
char* connect(char* a, char* b, char* c);
char* auto_insert(char* filename, vector<TokenElementType> FuctListQueue, object& result);
string start_sco(char* t);
string end_sco(char* t);
char* go_to_space(char* v);