#include "buffer.h"
#include <string>
#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
    Buffer buf;
   string strtest("hello world!");
   buf.wirtein(strtest.c_str(),strtest.size());
   int n=buf.writefd(STDOUT_FILENO,strtest.size());
   cout<<n;
   return 0;
}
