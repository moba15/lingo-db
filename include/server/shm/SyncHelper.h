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
      std::cout << "Semaphore destroyed" << std::endl;
      sem_unlink(name.c_str());
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
   SharedMemoryWrapper(void* data, std::string_view handle, bool freed = false);
   ~SharedMemoryWrapper();
   SharedMemoryWrapper(SharedMemoryWrapper&) = delete;
   SharedMemoryWrapper& operator=(SharedMemoryWrapper&) = delete;
   SharedMemoryWrapper(const SharedMemoryWrapper&) = delete;
   SharedMemoryWrapper& operator=(const SharedMemoryWrapper&) = delete;
   SharedMemoryWrapper(SharedMemoryWrapper&&) noexcept;
   SharedMemoryWrapper& operator=(SharedMemoryWrapper&&) noexcept;

   private:
   void* data;
   bool freed;
   std::string_view handle;
};

arrow::Result<std::unique_ptr<SharedSemaphore>> createAndLockSharedMutex(std::string_view handle);
arrow::Result<std::unique_ptr<SharedMemoryWrapper>>
createAndCopySharedResultMemory(std::string_view handle, const std::shared_ptr<arrow::ResizableBuffer> buffer);
arrow::Result<std::shared_ptr<arrow::Buffer>> readResultSharedMemory(std::string_view handle);

} //namespace util
};
