//
// Created by mor on 20.11.24.
//

#pragma once
#include <iostream>
#include <string>
#include <arrow/api.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
namespace server {
namespace util {
struct SharedSemaphore {
   public:
   explicit SharedSemaphore(sem_t* sem, std::string name) : sem(sem), name{name} {}
   SharedSemaphore(const SharedSemaphore&) = delete;
   SharedSemaphore& operator=(const SharedSemaphore&) = delete;
   SharedSemaphore(const SharedSemaphore&&) = delete;
   SharedSemaphore& operator=(const SharedSemaphore&&) = delete;
   ~SharedSemaphore() {

      sem_unlink(name.c_str());
      sem_close(sem);
      sem_destroy(sem);
      std::cout << "SEMA: (-)" << name << std::endl;
   }
   arrow::Status post() const;
   arrow::Status wait() const;

   private:
   sem_t* sem;
   bool owner = false;
   std::string name;
};
struct SharedMemoryWrapper {
   public:
   SharedMemoryWrapper(std::string handle, int shmFd, bool freed = false);
   ~SharedMemoryWrapper();
   SharedMemoryWrapper(const SharedMemoryWrapper&) = delete;
   SharedMemoryWrapper& operator=(const SharedMemoryWrapper&) = delete;
   SharedMemoryWrapper(SharedMemoryWrapper&) = delete;
   SharedMemoryWrapper& operator=(SharedMemoryWrapper&) = delete;

   SharedMemoryWrapper(SharedMemoryWrapper&&) noexcept;
   SharedMemoryWrapper& operator=(SharedMemoryWrapper&&) noexcept;

   int getShmFd() const { return shmFd; }
   void* address;
   size_t size;

   private:
   bool freed;
   std::string handle;
   int shmFd;
};

arrow::Result<std::unique_ptr<SharedMemoryWrapper>> createSharedMemory(std::string handle);
arrow::Result<std::unique_ptr<SharedSemaphore>> createAndLockSharedMutex(std::string handle);
arrow::Result<void*> createAndCopySharedResultMemory(SharedMemoryWrapper& sharedMemoryWrapper, std::shared_ptr<arrow::ResizableBuffer> buffer);
arrow::Result<std::shared_ptr<arrow::Buffer>> readResultSharedMemory(SharedMemoryWrapper& sharedMemoryWrapper);

} //namespace util
};
