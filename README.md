# Effects Of population size and mutation rate on the evolution of mutational robustness
We replicate a [paper](https://onlinelibrary.wiley.com/doi/10.1111/j.1558-5646.2007.00064.x) that uses Avida to look at mutational robustness, or the ability of a gene to withstand mutations but still show the original phenotype. It hypothesizes that small populations will “favor robustness mechanisms” while large populations favor removing harmful mutations, masking the effect of the mutations. They performed an experiment on 160 populations, testing different mutation rates and population sizes.

Our exapnsions include changing mutation rate and tasks. For full replication report, refer to the pdf report on the Github page.

# GUI

The world model above shows the organisms mutate over time solving various tasks. Every time a task is solved successfully, the organism use the organism will game points. Once enough points, the organisms will be capable to reproduce. The offspring will be added to the world and the parent will gain fitness. The offspring will have a point mutation applied to it before getting added to the world. Fitness of an organism is measuring their ability to reproduce so the more children an organism has the fitter it will be. Each organism is depicted in blue in the world above and once an organism has a fitness of over 9, they will turn yellow. Use the configuration panel next to the world to set the random seed and initial population of the world.

# Setup

Use ```compile-run.sh``` to run the program in the web browser.
