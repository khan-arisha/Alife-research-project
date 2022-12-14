#pragma once
#ifndef UITSL_PARALLEL_RECURSIVEEXCLUSIVELOCK_HPP_INCLUDE
#define UITSL_PARALLEL_RECURSIVEEXCLUSIVELOCK_HPP_INCLUDE

#include <optional>
#include <shared_mutex>

#include "RecursiveMutex.hpp"
#include "thread_utils.hpp"

namespace uitsl {

class RecursiveExclusiveLock {

  uitsl::RecursiveMutex& mutex;
  std::optional<std::unique_lock<std::shared_mutex>> lock;

public:

  RecursiveExclusiveLock(uitsl::RecursiveMutex& mutex_)
  : mutex(mutex_)
  {
    if (not mutex.HasThreadOwnership()) {
      lock.emplace(mutex.Mutex());
      mutex.RegisterThreadOwnership();
    }
  }

  ~RecursiveExclusiveLock() { if (lock) mutex.DeregisterThreadOwnership(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_RECURSIVEEXCLUSIVELOCK_HPP_INCLUDE
