#include <iostream>
#include "World.h"

// This is the main function for the NATIVE version of this project.

int main(int argc, char *argv[])
{
  emp::Random random(2);
  OrgWorld world(random);
  const int num_h_boxes = 1000;
  const int num_w_boxes = 1000;
  const double RECT_SIDE = 10;
  const double width{num_w_boxes * RECT_SIDE};
  const double height{num_h_boxes * RECT_SIDE};
  int myArray[400] = {};

  Organism *new_org = new Organism(&world, 25.0);
  for (int i = 0; i < 50; i++)
  {
    world.Inject(*new_org);
  }
    world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
    world.Resize(num_w_boxes , num_h_boxes );

    //world.SetupSymFile("task1000.data").SetTimingRepeat(10);


  for (int update = 0; update < 400; update++)
  {
    world.Update();
    int org_num = 0;
    double total = 0;
    int total_org = 0;

    //make grid
    //loop through every grid cell
    for (int x = 0; x < num_w_boxes; x++)
    {
      for (int y = 0; y < num_h_boxes; y++)
      {
        if (world.IsOccupied(org_num))
        {
          total += world.GetOrg(org_num).cpu.state.fitness;

          total_org++;
        }
        org_num++;
      }
    }
    myArray[update] = total / total_org;
  }

  double tot = 0;
  for (int i = 0; i < 400; i++)
  {

    tot += myArray[i];
  }
  std::cout << "Average Fitness per update:" ;
  std::cout << tot / 400 << std::endl;
}
