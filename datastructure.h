#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

using namespace std;


// define default capacity of the stack
const int SIZE=10;
// capacity of Map
const int TABLE_SIZE = 128;
// Class for stack
class Stack
{
    string *arr;
    int *line_number;
    int top;
    int capacity;

    public:
  //  Stack();     // constructor
  //  Stack(int);     // constructor
      Stack()
      {
         arr = new string[SIZE];
         capacity = SIZE;
         top = -1;
      }
      // Constructor to initialize stack
      Stack(int size)
      {
         arr = new string[size];
         capacity = size;
         top = -1;
      }

    void push(string);
    void push(string,int);
    string pop();
    string pop(int*);
    string peek();

    int size();
    bool isEmpty();
    bool isFull();
};

/*stack::~stack()
{
    delete []arr;
}*/


class HashEntry
{

    public:

        int key;
        void* value;
        int type; // 0 is int, 1 or greater than 1 is string

        HashEntry(int key, int val)
        {
            this->key = key;
            int *v;
            v = new int;
            *v = val;
            this->type = 0;
            value = v;
        }

        HashEntry(int key, string val)
        {
            char *str;
            this->key = key;
            size_t len = val.length();
            str = new char[len];
            for(int i=0; i<len; i++)
            {
               str[i] = val.at(i);
            }
            this->type = sizeof(len);
            value =(void *) str;
        }
};
