#include "datastructure.h"
/*********** Stack ***************/
/*****LightWeight DataStructure for Embedded System *******/
// Utility function to add an element x in the Stack
void Stack::push(string x)
{
    if (isFull())
    {
        cout << "OverFlow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }

    cout << "Inserting " << x << endl;
    arr[++top] = x;
}
void Stack::push(string x,int y)
{
    if (isFull())
    {   
        cout << "OverFlow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }   

    cout << "Inserting " << x << endl;
    ++top;
    arr[top] = x;
    line_number[top]= y;
}

// Utility function to pop top element from the Stack
string Stack::pop()
{
    // check for Stack underflow
    if (isEmpty())
    {
        cout << "UnderFlow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }

    cout << "Removing " << peek() << endl;

    // decrease Stack size by 1 and (optionally) return the popped element
    return arr[top--];
}
string Stack::pop(int *y)
{
    // check for Stack underflow
    if (isEmpty())
    {   
        cout << "UnderFlow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }   

    cout << "Removing " << peek() << endl;

    // decrease Stack size by 1 and (optionally) return the popped element
    *y = line_number[top];
    return arr[top--];
}

// Utility function to return top element in a Stack
string Stack::peek()
{
    if (!isEmpty())
        return arr[top];
    else
        exit(EXIT_FAILURE);
}

// Utility function to return the size of the Stack
int Stack::size()
{
    return top + 1;
}

// Utility function to check if the Stack is empty or not
bool Stack::isEmpty()
{
    return top == -1;   // or return size() == 0;
}

// Utility function to check if the Stack is full or not
bool Stack::isFull()
{
    return top == capacity - 1; // or return size() == capacity;
}
/********* Map Implementation *******************/
class HashMap

{
    private:
        HashEntry **table;
    public:   
        HashMap()
        {
            table = new HashEntry * [TABLE_SIZE];
            for (int i = 0; i< TABLE_SIZE; i++)
            {
                table[i] = NULL;
            }
        }
        // Hash Function
        int HashFunc(int key)
        {
            return key % TABLE_SIZE;
        }

        //Insert Element at a key
        void _insert(int key, int value)
        {
            int hash = HashFunc(key);

            while (table[hash] != NULL && table[hash]->key != key)
            {
                hash = HashFunc(hash + 1);
            }

            if (table[hash] != NULL)
                delete table[hash];

            table[hash] = new HashEntry(key, value);
        }

        void _insert(int key, string value)
        {
            int hash = HashFunc(key);

            while (table[hash] != NULL && table[hash]->key != key)
            {
                hash = HashFunc(hash + 1);
            }

            if (table[hash] != NULL)
                delete table[hash];

            table[hash] = new HashEntry(key, value);
        }

        //Search Element at a key
        void* _search(int key,int &type)
        {
            int  hash = HashFunc(key);

            while (table[hash] != NULL && table[hash]->key != key)
            {
                hash = HashFunc(hash + 1);
            }

            if (table[hash] == NULL)
            {
                cout << "Search failed" << endl; 
                return NULL;
            }
            else
            {
                type = table[hash]->type;
                return table[hash]->value;
            }
        }
        //Remove Element at a key

        void _remove(int key)
        {

            int hash = HashFunc(key);

            while (table[hash] != NULL)
            {
                if (table[hash]->key == key)
                    break;

                hash = HashFunc(hash + 1);
            }

            if (table[hash] == NULL)
            {
                std::cout<<"No Element found at key "<<key<<endl;
                return;
            }
            else
            {
                free(table[hash]->value);
                delete table[hash];
            }

            std::cout<<"Element Deleted"<<endl;

        }

        ~HashMap()
        {
            for (int i = 0; i < TABLE_SIZE; i++)
            {
                if (table[i] != NULL)
                {
                    free(table[i]->value);
                    delete table[i];
                }
                delete[] table;
            }
        }

};
