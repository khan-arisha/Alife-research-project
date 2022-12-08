#ifndef WORLD_H
#define WORLD_H
#include "emp/Evolve/World.hpp"
#include "Org.h"
#include "Task.h"

class OrgWorld : public emp::World<Organism>
{
  //instance variables
  emp::Random &random;
  std::vector<emp::WorldPosition> reproduce_queue;
  //vector of the tasks that are available for organisms to solve
  std::vector<Task *> tasks{new NewTask1(),new NewTask2(),new NewTask3(),new NewTask4()};

  emp::Ptr<emp::DataMonitor<int>> data_node_symcount;
  emp::Ptr<emp::DataMonitor<double, emp::data::Histogram>> data_node_fitval;

public:
  /** 
    * Constructor for the world.
    * Creates the world.
    */
  OrgWorld(emp::Random &_random) : emp::World<Organism>(_random), random(_random) {}

 ~OrgWorld() {
    if(data_node_fitval) data_node_fitval.Delete();
    if(data_node_symcount) data_node_symcount.Delete();
}

const pop_t &GetPopulation() { return pop; }

emp::DataMonitor<int>& GetSymCountDataNode() {
if(!data_node_symcount) {
    data_node_symcount.New();
    OnUpdate([this](size_t){
    data_node_symcount -> Reset();
    for (size_t i = 0; i < pop.size(); i++)
        if(IsOccupied(i))
        data_node_symcount->AddDatum(1);
    });
}
return *data_node_symcount;
}


emp::DataMonitor<double, emp::data::Histogram>& GetFitValDataNode() {
  if (!data_node_fitval) {
    data_node_fitval.New();
    OnUpdate([this](size_t){
        data_node_fitval->Reset();
        for (size_t i = 0; i< pop.size(); i++)
        if (IsOccupied(i))
            data_node_fitval->AddDatum(pop[i]->getFitness());
      });
  }
  data_node_fitval->SetupBins(0.0, 1.1, 11);
  return *data_node_fitval;
}

emp::DataFile & SetupSymFile(const std::string & filename) {
    auto & file = SetupFile(filename);
    auto & node = GetFitValDataNode();
    auto & node1 = GetSymCountDataNode();

    file.AddVar(update, "update", "Update");
    file.AddMean(node, "mean_fitness", "Average organism fitness value");
    file.AddTotal(node1, "count", "Total number of symbionts");

    file.PrintHeaderKeys();

    return file;
  }

  

  /**
    * Loops through every organism to update their points.
	  * Puts an offspring in the world if an organism has reproduced.
    * Checks if the organism solved a task.
    */
  void Update()
  {
    emp::World<Organism>::Update();

    //Process each organism
    emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
    for (int i : schedule)
    {
      if (!IsOccupied(i))
      {
        continue;
      }
      //Set organism's location
      pop[i]->Process(i);
    }

    //Time to allow reproduction for any organisms that ran the reproduce instruction
    for (emp::WorldPosition location : reproduce_queue)
    {
      //is the location occupied?
      if (!IsOccupied(location))
      {
        return;
      }
      //check if the organism has reproduced
      std::optional<Organism> offspring =
          pop[location.GetIndex()]->CheckReproduction();
      if (offspring.has_value())
      {
        //add offspring in the world
        DoBirth(offspring.value(), location.GetIndex());

      }
    }
    reproduce_queue.clear();
  }

  /**
    * Checks if the organism solved a task.
    * @param output points recieved for solving/not solving the task.
    * @param state array holding the most recent 4 input values.
    */
  void CheckOutput(float output, OrgState &state)
  {
    //loop through each task
    for (Task *task : tasks)
    {
      //did the organism solve the task?
      if (task->CheckOutput(output, state.last_inputs) > 0)
      {
        //save the name of the last task solved
        state.name = task->name();

        //adds the points recieved by solving the task
        state.points += task->CheckOutput(output, state.last_inputs);
      }
    }
  }

  void ReproduceOrg(emp::WorldPosition location)
  {
    // Wait until after all organisms have been processed to perform
    // reproduction. If reproduction happened immediately then the child could
    // ovewrite the parent, and then we would be running the code of a deleted
    // organism
    reproduce_queue.push_back(location);
  }
};

#endif