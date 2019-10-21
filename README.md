# dpcpp ( a C Preprocessor written in C++ )
Daipayan's C Preprocessor written in C++

For compilation use: g++ -g -fno-stack-protector dpcpp_3_1.cpp datastructure.cpp

For running purpose use: ./a.out example.c

Final output will be "example.i"

Important Note: For the preprocessor to work replace my header file in usr/include with /usr/include/ header in your system as in my header file single spacing is used instead of tab spacing. Better keep backup of your own system header files, currently working on an alternative for the same.

Features working are:-
#include 

comment removal

convert multiline macro into one

predefined macros such as __cplusplus etc.

#undef

#define

