#include "server/shm/SyncHelper.h"
namespace  server {
namespace util {

arrow::Status SharedSemaphore::post() const {
   if (sem_post(sem) == -1) {
      std::cout << "OK" << std::endl;
      return arrow::Status::IOError("Failed to post semaphore");
   }
   return arrow::Status::OK();
}

arrow::Status SharedSemaphore::wait() const {
   if (sem_wait(sem) == -1) { return arrow::Status::IOError("Failed to wait for semaphore"); }
   return arrow::Status::OK();
}
SharedMemoryWrapper::SharedMemoryWrapper(void* data, const std::string_view handle, bool freed)
   : data(data), handle(handle), freed(freed) {}
SharedMemoryWrapper::~SharedMemoryWrapper() {
   if (!freed) {

      freed = true;
   }
}
SharedMemoryWrapper::SharedMemoryWrapper(SharedMemoryWrapper&& other) noexcept
   : data(other.data), freed(other.freed), handle(other.handle) {
   other.freed = true;
   other.data = nullptr;
}
SharedMemoryWrapper& SharedMemoryWrapper::operator=(SharedMemoryWrapper&& other) noexcept {
   freed = other.freed;
   other.freed = true;
   data = other.data, other.data = nullptr;
   handle = other.handle;
   return *this;
}

arrow::Result<std::unique_ptr<SharedSemaphore>> createAndLockSharedMutex(const std::string_view handle) {
   const auto semName = "SEMA" + std::string(handle);
   auto* sharedSem = sem_open(semName.c_str(), O_CREAT, 0, 0);
   if (sharedSem == SEM_FAILED) { return arrow::Status::IOError("Failed to open semaphore2"); }

   return arrow::Result(std::move(std::make_unique<SharedSemaphore>(sharedSem, semName)));
}

arrow::Result<std::unique_ptr<SharedMemoryWrapper>>
createAndCopySharedResultMemory(std::string_view handle, const std::shared_ptr<arrow::ResizableBuffer> buffer) {
   auto shmFd = shm_open(handle.cbegin(), O_CREAT | O_EXCL | O_RDWR, 0666);
   if (shmFd < 0) { return arrow::Status::IOError("Failed to open shm"); }
   ftruncate(shmFd, buffer->size());
   auto* sharedMemory = mmap(nullptr, buffer->size(), PROT_WRITE, MAP_SHARED, shmFd, 0);
   if (sharedMemory == MAP_FAILED) { return arrow::Status::IOError("mmap failed"); }
   std::memcpy(sharedMemory, buffer->data(), buffer->size());
   auto wrapper = std::make_unique<SharedMemoryWrapper>(sharedMemory, handle, false);
   return std::move(wrapper);
}

arrow::Result<std::shared_ptr<arrow::Buffer>> readResultSharedMemory(const std::string_view handle) {
   auto shmFd = shm_open(handle.cbegin(), O_RDONLY, 0666);
   if (shmFd < 0) { return arrow::Status::IOError("Failed to open shm"); }

   struct stat shmStat;
   if (fstat(shmFd, &shmStat) == -1) { return arrow::Status::IOError("Failed to get shared memory stats"); }
   auto* sharedMemory = mmap(nullptr, shmStat.st_size, PROT_READ, MAP_SHARED, shmFd, 0);
   if (sharedMemory == nullptr || sharedMemory == MAP_FAILED) { return arrow::Status::IOError("Map failed"); }
   auto buffer = std::make_shared<arrow::Buffer>(static_cast<uint8_t*>(sharedMemory), shmStat.st_size);
   return arrow::Result(std::move(buffer));
}
};
}; //namespace server::util
