#pragma once

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

namespace shm{

  template <key_t KEY, typename T, int ELEMENTS_COUNT = 1>
  class Shm {
  private:
    static void get();
    void attach();
    
  public:
    Shm();    
    virtual ~Shm();

    void setElement(const T*);
    const T* getElement();
    static void create();
    static void destroy();
  private:
    void* shm_;
    static int shmid_;
  };

}
