#pragma once

#include <mutex>
using std::mutex;
#include <cstdio>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

namespace shm{

  template <key_t KEY, typename T>
  class Shm {
  private:
    static void get(){
      if(shmid_ == -1){
        if( (shmid_ = shmget(KEY, sizeof(T), 0666)) < 0 ){
          perror("shmget");
          throw std::runtime_error("Shared memory can not be created");
        }
      }
    }

    void attach();
    
  public:
    Shm();    
    virtual ~Shm();

    void setElement(const T*);
    const T* getElement();

    static void create(){
    if ((shmid_ = shmget(KEY, sizeof(T), IPC_CREAT | 0666)) < 0) 
      {
        throw std::runtime_error("Failed create shared memory");
      }
    }
    
    static void destroy(){
      get();
      if(shmctl(shmid_, IPC_RMID, nullptr) < 0){
        perror("shmctl");
        throw std::runtime_error("Failed remove shared memory shm");
      }
      shmid_ = -1;
    }
    
  private:
    void* shm_;
    static int shmid_;
    mutex shm_mutex;
  };

}

#include "libshm.tpp"
