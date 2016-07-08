#include <stdexcept>
#include <iostream>
#include <string.h>

#include <libshm.hpp>

namespace shm{
  template <key_t KEY, typename T> 
  int Shm<KEY, T>::shmid_ = -1;
  
  template <key_t KEY, typename T>
  Shm<KEY, T>::Shm():shm_(0){
    get();
    attach();
  }

  template <key_t KEY, typename T>
  Shm<KEY, T>::~Shm(){
    if(shm_ != nullptr){
      shmdt(shm_);
      shm_ = 0;
    }
  }

  template <key_t KEY, typename T>
  void Shm<KEY, T>::setElement(const T *data){
    memcpy(shm_, data, sizeof(T));
  }

  template <key_t KEY, typename T>
  const T* Shm<KEY, T>::getElement(){
    T* tmp_ptr = new(shm_) T;
    T* ptr = new T;
    memcpy(ptr, tmp_ptr, sizeof(T));
    return ptr;
  }

  template <key_t KEY, typename T>
  void Shm<KEY, T>::attach(){
    if ((shm_ = shmat(shmid_, NULL, 0)) == (char *) -1){
      throw std::runtime_error("Failed attach shm");
    }

  }
}
