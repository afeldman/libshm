/*!\file libshm.hpp
 *
 *\author Anton Feldmann (anton.feldmann@gmail.com)
 *\version 0.1.0
 *
 *Copyright (c) 2016 Anton Feldmann <br>
 *Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <mutex>
using std::mutex;
#include <cstdio>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

/*!\namespace shm
 *
 *\provide a shared memory C++ Style
 */
namespace shm{

  /*!\brief make shm
   *
   * the Linux shm realisation is set into a C++ class structure
   *
   *\tparam Key the key of the memory
   *\tparam T storage data Type
   *
   *\code{.cpp}
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
   \endcode
   */
  template <key_t KEY, typename T>
  class Shm {
  private:
    /*!\brief get shared memory
     *
     * create the shared memory unsing the key
     *
     */
    static void get(){
      if(shmid_ == -1){
        if( (shmid_ = shmget(KEY, sizeof(T), 0666)) < 0 ){
          perror("shmget");
          throw std::runtime_error("Shared memory can not be created");
        }
      }
    }

    /*!\brief attach the shm in to the address space of a colling process
     *
     *attaches the shared memory segment identified by #shmid_ to the address space of the calling process.
     */
    void attach();

  public:
    /*!\brief constructor
     *
     * create the shm using the key and attach the shm to the process
     */
    Shm();
    /*\brief destructor
     *
     *delete the shm and close the object
     */
    virtual ~Shm();

    /*!\brief set element
     *
     * set the object to the shm. The type of the element is set using the template
     */
    void setElement(const T*);

    /*!\brief get element
     *
     *get the element of type T. The type is set in the template
     */
    const T* getElement();

    /*!\brief create shared memory
     *
     * returns the identifier of the shared memory segment associated with the value of the argument key
     *
     */
    static void create(){
    if ((shmid_ = shmget(KEY, sizeof(T), IPC_CREAT | 0666)) < 0)
      {
        throw std::runtime_error("Failed create shared memory");
      }
    }

    /*!\brief destror memory
     *
     *destroy the shared memory. The key is known given by the template
     *
     */
    static void destroy(){
      get();
      if(shmctl(shmid_, IPC_RMID, nullptr) < 0){
        perror("shmctl");
        throw std::runtime_error("Failed remove shared memory shm");
      }
      shmid_ = -1;
    }

  private:
    void* shm_;         //!< shm struct
    static int shmid_;  //!< shm id
    mutex shm_mutex;    //!< for thread safe work, use mutex
  };

}

#include "libshm.tpp"
