#pragma once

#include "uit/ducts/intra/put=growing+get=skipping+type=any/a::SconceDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/pooled+inlet=RingIsend+outlet=BlockIrecv_t::PooledIriObiDuct.hpp"
#include "uit/ducts/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/spouts/wrappers/InstrumentationAggregatingSpoutWrapper.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SconceDuct,
  uit::a::MutexSconceDuct,
  uit::t::PooledIriObiDuct
>;

using ImplSpec = uit::ImplSpec<
  message_t,
  ImplSel,
  uit::InstrumentationAggregatingSpoutWrapper,
  2, // N buffer requests
  16 // B
>;
