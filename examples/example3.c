#include <stdio.h>
#if hello \
    "world" \
    "bye"  \
    "what" 
#define DEBUG_str "debug"

int main()
{

   printf(__cplusplus);
   printf(DEBUG_str);

#undef DEBUG_str

   printf(DEBUG_str);


}

