#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_BACKEND_POOLEDBACKEND_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_BACKEND_POOLEDBACKEND_HPP_INCLUDE

#include <tuple>

#include "../../../../../../third-party/Empirical/third-party/robin-hood-hashing/src/include/robin_hood.h"

#include "../../../../../uit_emp/datastructs/tuple_utils.hpp"

#include "../../../../../uitsl/debug/uitsl_assert.hpp"

#include "../../../../setup/InterProcAddress.hpp"

#include "impl/InletMemoryPool.hpp"
#include "impl/OutletMemoryPool.hpp"
#include "impl/PoolSpec.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<
  typename ImplSpec,
  template<typename> typename ProcDuct
>
class PooledBackEnd {

  using T = typename ImplSpec::T;

  using address_t = uit::InterProcAddress;

  using PoolSpec_t = uit::PoolSpec<ImplSpec, ProcDuct>;

public:

  using inlet_pool_t = uit::InletMemoryPool<PoolSpec_t>;
  using outlet_pool_t = uit::OutletMemoryPool<PoolSpec_t>;

private:

  // < caller/target thread/proc > -> inlet pool
  robin_hood::unordered_map<
    std::tuple<
      uitsl::proc_id_t, uitsl::thread_id_t,
      uitsl::proc_id_t, uitsl::thread_id_t
    >,
    inlet_pool_t,
    uit_emp::TupleHash<
      uitsl::proc_id_t, uitsl::thread_id_t,
      uitsl::proc_id_t, uitsl::thread_id_t
    >
  > inlet_pools;

  // < caller/target thread/proc > -> outlet pool
  robin_hood::unordered_map<
    std::tuple<
      uitsl::proc_id_t, uitsl::thread_id_t,
      uitsl::proc_id_t, uitsl::thread_id_t
    >,
    outlet_pool_t,
    uit_emp::TupleHash<
      uitsl::proc_id_t, uitsl::thread_id_t,
      uitsl::proc_id_t, uitsl::thread_id_t
    >
  > outlet_pools;

  bool AreAllInletPoolsInitialized() const {

    // check that all windows are in the same initialization state
    assert( std::adjacent_find(
      std::begin(inlet_pools), std::end(inlet_pools),
      [](const auto& pool_pair1, const auto& pool_pair2) {
        const auto& [key1, pool1] = pool_pair1;
        const auto& [key2, pool2] = pool_pair2;
        return pool1.IsInitialized() != pool2.IsInitialized();
      }
    ) == std::end(inlet_pools) );


    return std::any_of(
      std::begin(inlet_pools), std::end(inlet_pools),
      [](const auto& pool_pair) {
        const auto& [key, pool] = pool_pair;
        return pool.IsInitialized();
      }
    );

  }

  bool AreAllOutletPoolsInitialized() const {

    // check that all windows are in the same initialization state
    assert( std::adjacent_find(
      std::begin(outlet_pools), std::end(outlet_pools),
      [](const auto& pool_pair1, const auto& pool_pair2) {
        const auto& [key1, pool1] = pool_pair1;
        const auto& [key2, pool2] = pool_pair2;
        return pool1.IsInitialized() != pool2.IsInitialized();
      }
    ) == std::end(outlet_pools) );


    return std::any_of(
      std::begin(outlet_pools), std::end(outlet_pools),
      [](const auto& pool_pair) {
        const auto& [key, pool] = pool_pair;
        return pool.IsInitialized();
      }
    );

  }

  bool IsInitialized() const {
    assert(
      AreAllInletPoolsInitialized() == AreAllOutletPoolsInitialized()
      || inlet_pools.empty()
      || outlet_pools.empty()
    );
    return AreAllInletPoolsInitialized() || AreAllOutletPoolsInitialized();
  }

  bool IsEmpty() const {
    return outlet_pools.empty() && inlet_pools.empty();
  }

public:

  void RegisterInletSlot(const address_t& address) {
    assert( !IsInitialized() );
    inlet_pools[ address.WhichProcsThreads() ].Register(address);
  }

  void RegisterOutletSlot(const address_t& address) {
    assert( !IsInitialized() );
    outlet_pools[ address.WhichProcsThreads() ].Register(address);
  }

  void Initialize() {
    assert( !IsInitialized() );

    for (auto& [__, pool] : inlet_pools) pool.Initialize();
    for (auto& [__, pool] : outlet_pools) pool.Initialize();

    assert( IsInitialized() || IsEmpty() );
  }

  inlet_pool_t& GetInletPool(const address_t& address) {
    assert( IsInitialized() );

    auto& pool = inlet_pools.at( address.WhichProcsThreads() );

    uitsl_assert( pool.IsInitialized(), pool.GetSize() );

    return pool;
  }

  outlet_pool_t& GetOutletPool(const address_t& address) {
    assert( IsInitialized() );

    auto& pool = outlet_pools.at( address.WhichProcsThreads() );

    assert( pool.IsInitialized() );

    return pool;
  }

  constexpr static bool CanStep() { return outlet_pool_t::CanStep(); }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_BACKEND_POOLEDBACKEND_HPP_INCLUDE
