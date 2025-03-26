#include "lingodb/server/shm/SyncHelper.h"

#include <thread>
namespace server {
namespace util {

arrow::Status SharedSemaphore::post() const {
   if (sem_post(sem) == -1) {
      return arrow::Status::IOError("Failed to post semaphore");
   }
   return arrow::Status::OK();
}

arrow::Status SharedSemaphore::wait() const {
   struct timespec ts;
   if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
      perror("clock_gettime");
      return arrow::Status::IOError("Failed to get realtime time for sem_timedwait");
   }
   ts.tv_sec += 10;
   if (sem_timedwait(sem, &ts) == -1) { return arrow::Status::IOError("Failed to wait for semaphore"); }
   return arrow::Status::OK();
}
SharedMemoryWrapper::SharedMemoryWrapper(const std::string handle, int shmFd, bool freed)
   : freed(freed), handle(handle), shmFd(shmFd) {}
SharedMemoryWrapper::~SharedMemoryWrapper() {
   PrintIfDebugSyncHelper("Shared memory is beeing freed (" << getpid() << ") freed: " << freed);
   shm_unlink(handle.c_str());
   close(shmFd);
   if (!freed) {
      freed = true;
   }
}
SharedMemoryWrapper::SharedMemoryWrapper(SharedMemoryWrapper&& other) noexcept
   : freed(other.freed), handle(other.handle) {
   other.freed = true;
}
SharedMemoryWrapper& SharedMemoryWrapper::operator=(SharedMemoryWrapper&& other) noexcept {
   freed = other.freed;
   other.freed = true;
   handle = other.handle;
   return *this;
}

arrow::Result<std::unique_ptr<SharedSemaphore>> createAndLockSharedMutex(const std::string handle) {
   std::string semName{"SEM."};
   semName.append(handle);
   auto* sharedSem = sem_open(semName.c_str(), O_CREAT | O_EXCL, 0, 0);
   if (sharedSem == SEM_FAILED) {
      std::cerr << " : " << strerror(errno) << std::endl;
      return arrow::Status::IOError("Failed to opem semaphore ", strerror(errno));
   }
   PrintIfDebugSyncHelper("SEMA: (+)" << semName);

   return arrow::Result(std::move(std::make_unique<SharedSemaphore>(sharedSem, semName)));
}

arrow::Result<std::unique_ptr<SharedMemoryWrapper>> createSharedMemory(std::string handle) {
   auto shmFd = shm_open(handle.c_str(), O_CREAT | O_RDWR | O_EXCL, 0666);
   PrintIfDebugSyncHelper("Created shared memory: " << std::to_string(shmFd)) if (shmFd < 0) {
      return arrow::Status::IOError("Failed to open shm ", strerror(errno));
   }

   return std::make_unique<SharedMemoryWrapper>(handle, shmFd, false);
}

arrow::Result<void*> createAndCopySharedResultMemory(SharedMemoryWrapper& sharedMemoryWrapper, std::shared_ptr<arrow::Buffer> buffer) {
   auto shmFd = sharedMemoryWrapper.getShmFd();
   ftruncate(shmFd, buffer->size());

   auto* sharedMemory = mmap(nullptr, buffer->size(), PROT_WRITE, MAP_SHARED, shmFd, 0);
   PrintIfDebugSyncHelper("mmp");
   if (sharedMemory == MAP_FAILED) { return arrow::Status::IOError("mmap failed", strerror(errno)); }
   std::memcpy(sharedMemory, buffer->data(), buffer->size());
   PrintIfDebugSyncHelper("memcpy");
   // munmap(sharedMemory, buffer->size());
   PrintIfDebugSyncHelper("New buffer size: " << buffer->size());
   // buffer.reset();
   PrintIfDebugSyncHelper("Buffer count " << buffer.use_count());
   return sharedMemory;
}

arrow::Result<std::shared_ptr<arrow::Buffer>> readResultSharedMemory(SharedMemoryWrapper& sharedMemoryWrapper) {
   auto shmFd = sharedMemoryWrapper.getShmFd();
   PrintIfDebugSyncHelper("   read: " + std::to_string(shmFd));

   struct stat shmStat;
   if (fstat(shmFd, &shmStat) == -1) { return arrow::Status::IOError("Failed to get shared memory stats", strerror(errno)); }
   auto* sharedMemory = mmap(nullptr, shmStat.st_size, PROT_READ, MAP_SHARED, shmFd, 0);
   if (sharedMemory == nullptr || sharedMemory == MAP_FAILED) { return arrow::Status::IOError("Map failed ", strerror(errno)); }
   auto buffer = std::make_shared<arrow::Buffer>(static_cast<uint8_t*>(sharedMemory), shmStat.st_size);
   //munmap(sharedMemory, shmStat.st_size);
   sharedMemoryWrapper.address = sharedMemory;
   sharedMemoryWrapper.size = shmStat.st_size;
   return arrow::Result(std::move(buffer));
}
};
}; //namespace server::util
