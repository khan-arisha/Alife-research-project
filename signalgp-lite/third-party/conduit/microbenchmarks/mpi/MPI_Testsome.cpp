#include <deque>

#include <benchmark/benchmark.h>
#include <mpi.h>

#include "uitsl/debug/benchmark_utils.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/mpi/mpi_init_utils.hpp"
#include "uitsl/nonce/ScopeGuard.hpp"

#include "uit/setup/ImplSpec.hpp"

const uitsl::MpiGuard guard;

constexpr size_t buffer_size{ uit::DEFAULT_BUFFER };

static void MPI_Testsome_nopending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> requests(uit::DEFAULT_BUFFER);
  emp::vector<int> buffers(uit::DEFAULT_BUFFER);

  for (size_t i = 0; i < uit::DEFAULT_BUFFER; ++i) {
    UITSL_Irecv(
      &buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &requests[i] // MPI_Request * request
    );
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    thread_local std::array<int, uit::DEFAULT_BUFFER> out_indices;

    UITSL_Testsome(
      requests.size(), // int count
      requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    );

  }

  // log results
  state.counters.insert({
    {
      "Processes",
      benchmark::Counter(
        uitsl::get_nprocs(),
        benchmark::Counter::kAvgThreads
      )
    }
  });

  // clean up
  for (auto& request : requests)  UITSL_Cancel(&request);

}

static void MPI_Testsome_onepending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> recv_requests(uit::DEFAULT_BUFFER);
  emp::vector<int> recv_buffers(uit::DEFAULT_BUFFER);

  for (size_t i = 0; i < uit::DEFAULT_BUFFER; ++i) {
    UITSL_Irecv(
      &recv_buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &recv_requests[i] // MPI_Request * request
    );
  }

  MPI_Request send_request;
  int send_buffer{};
  UITSL_Isend(
    &send_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int dest
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &send_request // MPI_Request * request
  );
  UITSL_Wait(&send_request, MPI_STATUS_IGNORE);

  // benchmark
  for (auto _ : state) {

    int flag{};

    thread_local std::array<int, uit::DEFAULT_BUFFER> out_indices;

    UITSL_Testsome(
      recv_requests.size(), // int count
      recv_requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    );

  }

  // clean up
  for (auto& request : recv_requests) {
    if (!uitsl::test_completion(request)) UITSL_Cancel(&request);
  }

}

static void MPI_Testsome_somepending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> recv_requests(uit::DEFAULT_BUFFER);
  emp::vector<int> recv_buffers(uit::DEFAULT_BUFFER);

  for (size_t i = 0; i < uit::DEFAULT_BUFFER; ++i) {
    UITSL_Irecv(
      &recv_buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &recv_requests[i] // MPI_Request * request
    );
  }

  for (size_t i = 0; i < uit::DEFAULT_BUFFER/2; ++i) {
    MPI_Request send_request;
    int send_buffer{};
    UITSL_Isend(
      &send_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int dest
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &send_request // MPI_Request * request
    );
    UITSL_Wait(&send_request, MPI_STATUS_IGNORE);
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    thread_local std::array<int, uit::DEFAULT_BUFFER> out_indices;

    UITSL_Testsome(
      recv_requests.size(), // int count
      recv_requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    );

  }

  // clean up
  for (auto& request : recv_requests) {
    if (!uitsl::test_completion(request)) UITSL_Cancel(&request);
  }

}

static void MPI_Testsome_manypending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> recv_requests(uit::DEFAULT_BUFFER);
  emp::vector<int> recv_buffers(uit::DEFAULT_BUFFER);

  for (size_t i = 0; i < uit::DEFAULT_BUFFER; ++i) {
    UITSL_Irecv(
      &recv_buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &recv_requests[i] // MPI_Request * request
    );
  }

  for (size_t i = 0; i < uit::DEFAULT_BUFFER; ++i) {
    MPI_Request send_request;
    int send_buffer{};
    UITSL_Isend(
      &send_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int dest
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &send_request // MPI_Request * request
    );
    UITSL_Wait(&send_request, MPI_STATUS_IGNORE);
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    thread_local std::array<int, uit::DEFAULT_BUFFER> out_indices;

    UITSL_Testsome(
      recv_requests.size(), // int count
      recv_requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    );

  }

  // clean up
  for (auto& request : recv_requests) {
    if (!uitsl::test_completion(request)) UITSL_Cancel(&request);
  }

}

static void MPI_Testsome_manymanypending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> recv_requests(uit::DEFAULT_BUFFER);
  emp::vector<int> recv_buffers(uit::DEFAULT_BUFFER);

  for (size_t i = 0; i < uit::DEFAULT_BUFFER; ++i) {
    UITSL_Irecv(
      &recv_buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &recv_requests[i] // MPI_Request * request
    );
  }

  int send_buffer{};
  for (size_t i = 0; i < uit::DEFAULT_BUFFER*2; ++i) {
    MPI_Request send_request;
    UITSL_Isend(
      &send_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int dest
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &send_request // MPI_Request * request
    );
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    thread_local std::array<int, uit::DEFAULT_BUFFER> out_indices;

    UITSL_Testsome(
      recv_requests.size(), // int count
      recv_requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    );

  }

  // clean up
  for (auto& request : recv_requests) {
    if (!uitsl::test_completion(request)) UITSL_Cancel(&request);
  }

  for (size_t i = 0; i < uit::DEFAULT_BUFFER; ++i) {
    MPI_Request request;
    int buffer{};
    UITSL_Irecv(
      &buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &request // MPI_Request * request
    );
    UITSL_Wait(&request, MPI_STATUS_IGNORE);
  }

}

// register benchmarks
const uitsl::ScopeGuard registration{[](){

  uitsl::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_nopending",
      MPI_Testsome_nopending
    )
  );

  uitsl::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_onepending",
      MPI_Testsome_onepending
    )
  );

  uitsl::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_somepending",
      MPI_Testsome_somepending
    )
  );

  uitsl::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_manypending",
      MPI_Testsome_manypending
    )
  );

  uitsl::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_manymanypending",
      MPI_Testsome_manymanypending
    )
  );
}};

int main(int argc, char** argv) {

  // only root runs benchmark
  if (uitsl::is_root()) {

    benchmark::Initialize(&argc, argv);

    benchmark::RunSpecifiedBenchmarks();

  }

}
