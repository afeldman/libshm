# libshm

This projects make an easy C++11 access to a shared memory. The Shared Memeory is part of the Linux kernal system and the used device could be found under /dev/shm .
This example shows how to use the project:
```cpp
    #include <iostream>
    #include <string>
    using namespace std;

    #include <libshm/libshm.hpp>
    using namespace shm;

    int main(int argc, char **argv){
     if(argc == 2){
       if(std::string(argv[1]) == "server"){
         int val = 50;
         Shm<0x1234, int>::create();
         Shm<0x1234, int> shm;
         shm.setElement(&val);
       }else
       if(std::string(argv[1]) == "client"){
         Shm<0x1234, int> shm;
         const int* ptr = shm.getElement();
         std::cout <<"Val = " << *ptr <<std::endl;
         Shm<0x1234, int>::destroy();
       }
     }else{
      std::cerr<<"Usage shm [server][client]"<<std::endl;
     }
    return 0;
    }
```
#install libshm

this project is build using WAF
