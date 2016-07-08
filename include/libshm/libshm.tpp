#include <stdexcept>
#include <iostream>
#include <string.h>

#include <thread>

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
    
    std::lock_guard<std::mutex> lock(shm_mutex);
    memcpy(shm_,data,sizeof(T));

  }

  template <key_t KEY, typename T>
  const T* Shm<KEY, T>::getElement(){
    
    std::lock_guard<std::mutex> lock(shm_mutex);
    T *ptr = new T;
    T *tmp = (new(shm_) T);
    memcpy(ptr,tmp,sizeof(T));

    return ptr;
  }

  template <key_t KEY, typename T>
  void Shm<KEY, T>::attach(){
    if ((shm_ = shmat(shmid_, NULL, 0)) == (char *) -1){
      throw std::runtime_error("Failed attach shm");
    }

  }
}
