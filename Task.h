#ifndef TASK_H
#define TASK_H

#include <cmath>
#include <string>

/**
 * Task.h
 *
 * This class specifies tasks for the organisms to be rewarded for doing.
 */

class Task
{
public:
  /**
    * Checks if the organism solved a task.
    * @param output points recived for solving/not solving the task.
    * @param state array holding the most recent 4 input values.
    */
  virtual double CheckOutput(float output, float inputs[4]) = 0;

  /**
    * Returns the name of a specific task.
    * @return name of the task.
    */
  virtual std::string name() const = 0;
};

#endif

/**
 * This subclass specifies a specific task for the organisms.
 * Checks if an organism is able to square an input.
 */
class NewTask1 : public Task
{
public:
  double CheckOutput(float output, float inputs[4]) override
  {

    for (int i = 0; i < 4; i++)
    {
      // For each input value in the array
      // Calculate the square of it
      float square = inputs[i] * inputs[i];

      // Since these are floats, allowing for some floating point error
      if (fabs(output - square) < 0.001)
      {
        // They squared an input, yay!
        //std::cout << "Squared!" << std::endl;
        // Give them 5 points for solving this task
        return 5.0;
      }
    }
  }

  std::string name() const
  {
    return "square";
  }
};

class NewTask2 : public Task
{
public:
  double CheckOutput(float output, float inputs[4]) override
  {

    for (int i = 0; i < 4; i++)
    {
      // For each input value in the array
      // Calculate the square of it
      float cube = inputs[i] * inputs[i] * inputs[i];

      // Since these are floats, allowing for some floating point error
      if (fabs(output - cube) < 0.001)
      {
        // They squared an input, yay!
        //std::cout << "Squared!" << std::endl;
        // Give them 5 points for solving this task
        return 10.0;
      }
    }
  }

  std::string name() const
  {
    return "cube";
  }
};

class NewTask3 : public Task
{
public:
  double CheckOutput(float output, float inputs[4]) override
  {

    for (int i = 0; i < 4; i++)
    {
      // For each input value in the array
      float doubled = inputs[i] * 2;

      // Since these are floats, allowing for some floating point error
      if (fabs(output - doubled) < 0.001)
      {
        // They squared an input, yay!
        //std::cout << "Squared!" << std::endl;
        // Give them 5 points for solving this task
        return 2.0;
      }
    }
  }

  std::string name() const
  {
    return "doubled";
  }
};

class NewTask4 : public Task
{
public:
  double CheckOutput(float output, float inputs[4]) override
  {

    for (int i = 0; i < 4; i++)
    {
      // For each input value in the array
      // Calculate the square of it
      float half = inputs[i]/2;

      // Since these are floats, allowing for some floating point error
      if (fabs(output - half) < 0.001)
      {
        // They squared an input, yay!
        //std::cout << "Squared!" << std::endl;
        // Give them 5 points for solving this task
        return 6.0;
      }
    }
  }

  std::string name() const
  {
    return "half";
  }
};