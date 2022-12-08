#ifndef ORG_H
#define ORG_H
#include "CPU.h"
#include "OrgState.h"
#include "emp/Evolve/World_structure.hpp"

class Organism
{
  //instance variable
  

public:
CPU cpu;
  /** 
    * Constructor for Organism.
    * Creates an organism.
    */
  Organism(OrgWorld *world, double points = 0.0) : cpu(world)
  {
    SetPoints(points);
  }

  /**
    * Set points of the organism.
    * @param _in number of points to set.
    */
  void SetPoints(double _in) { cpu.state.points = _in; }

  /**
    * Adds points to what the organism already has.
    * @param _in number of points to add.
    */
  void AddPoints(double _in) { cpu.state.points += _in; }

  /**
    * Gets the total point of the organism.
    * @return number of points the organism has.
    */
  double GetPoints() { return cpu.state.points; }

  /**
    * Gets the name of the last task solved by the organism.
    * @return the name of the last task.
    */
  std::string GetTaskName()
  {
    return cpu.state.name;
  }
    /**
    * Sets genome mutation rate.
    * @param mutation_rate mutation rate to set.
    */


    double getFitness(){
      return cpu.state.fitness;
    }
  /**
    * Resets the genome computer program.
    */
  void Reset()
  {
    cpu.Reset();
  }

  /**
    * Sets genome mutation rate.
    * @param mutation_rate mutation rate to set.
    */
  void SetMutationRate(double mutation_rate)
  {
    cpu.SetMutatationRate(mutation_rate);
  }
  /**
    * Mutates genome of the organism.
    */
  void Mutate()
  {
    cpu.Mutate();
  }

  /**
    * Checks if the oragnism has enough points to reproduce.
    * Mutates the offspring's genome.
    * @return offspring if the organism has reproduced.
    * @return nothing if the organism was not able to reproduce.
    */
  std::optional<Organism> CheckReproduction()
  {
    if (GetPoints() > 20)
    {
      //reproduce
      Organism offspring = *this;
      offspring.Reset();
      offspring.Mutate();
      //parent pays the cost of reproduction
      AddPoints(-20);
      cpu.state.fitness++;
      //std::cout <<  cpu.state.fitness << std::endl;
      return offspring;
    }
    return {};
  }

  /**
    * Set organism's current location.
    * Run the instructions.
    * @param current_location location for the organism.
    */
  void Process(emp::WorldPosition current_location)
  {
    cpu.state.current_location = current_location;
    //run the CPU for 10 cycles
    cpu.RunCPUStep(10);
    //AddPoints(20);
    
  }

  /**
    * Prints genome information of the organism.
    */
  void PrintGenome()
  {
    std::cout << "program ------------" << std::endl;
    cpu.PrintGenome();
    std::cout << "end ---------------" << std::endl;
  }
  
};

#endif