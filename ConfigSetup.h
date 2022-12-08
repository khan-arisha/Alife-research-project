#include "emp/config/ArgManager.hpp"
#include "emp/prefab/ConfigPanel.hpp"
#include "emp/web/UrlParams.hpp"
EMP_BUILD_CONFIG(MyConfigType,
    VALUE(SEED, int, 5, "What value should the random seed be?"), 
    VALUE(INIT_POP_SIZE, int, 30, "Number of cells to seed population with"), 
    VALUE(MUTATION_RATE, double, 0.02, "Percent probability that each bit in the binary representation of the genome is flipped"),
)