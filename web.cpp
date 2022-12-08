#define UIT_VENDORIZE_EMP
#define UIT_SUPPRESS_MACRO_INSEEP_WARNINGS
#include "emp/math/Random.hpp"
#include "emp/web/Animate.hpp"
#include "emp/web/Document.hpp"
#include "emp/web/web.hpp"
#include "World.h"
#include "Org.h"
#include "ConfigSetup.h"
#include "emp/config/ArgManager.hpp"
#include "emp/prefab/ConfigPanel.hpp"
#include "emp/web/UrlParams.hpp"

emp::web::Document doc("target");
emp::web::Document settings("settings");
emp::web::Document desc("description");
MyConfigType config;

class AEAnimator : public emp::web::Animate
{

    //arena width and height
    const int num_h_boxes = 30;
    const int num_w_boxes = 30;
    const double RECT_SIDE = 10;
    const double width{num_w_boxes * RECT_SIDE};
    const double height{num_h_boxes * RECT_SIDE};

    //create world
    emp::Random random{5};
    OrgWorld world{random};
    emp::web::Canvas canvas{width, height, "canvas"};

public:
    /** 
    *Constructor for AEAnimator.
    *Creates control buttons.
    *Creates a grid and configuration system GUI.
    *Puts organisms in the world.
    */
    AEAnimator()
    {
        //use config value to change the seed
        random.ResetSeed(config.SEED());

        // apply configuration query params and config files to config
        auto specs = emp::ArgManager::make_builtin_specs(&config);
        emp::ArgManager am(emp::web::GetUrlParams(), specs);
        am.UseCallbacks();
        if (am.HasUnused())
            std::exit(EXIT_FAILURE);

        //setup configuration panel
        emp::prefab::ConfigPanel config_panel(config);

        //shove canvas into the div
        //along with a control button and a configuration panel
        settings << config_panel;
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        //Description of how to use our site
        desc << "<br> <b>How to use our site</b></br>";
        desc << "The world model above shows the organisms mutating and reproducing over time by solving various tasks.";
        desc << " Every time a task is solved successfully, the organism gains points.";
        desc << " Once they have enough points, the organisms will be able to reproduce. The offspring will be added to the world and the parent will gain fitness. The offspring will have a point mutation applied to it before getting added to the world.";
        desc << " Fitness of an organism is measuring their ability to reproduce so the more children an organism has, the fitter it will be.";
        desc << " Each organism is depicted in blue in the world above and once an organism has a fitness of over 9, they will turn yellow.";
        desc << " If they have a fitness below 9, they will remain blue.";
        desc << " Use the configuration panel next to the world to set the random seed, initial population of the world and mutation rate. You can check to see if any of these contribute to a greater/lower fitness.";

        desc << "<br> <b>What we are meauring</b></br>";
        desc << "Our study is trying to measure the average fitness of the population as the intitial population size changes.";
        desc << " For our expansion, we tried changing the point mutation rate of the offsprings that are added into the world and see how that will change the results.";
        desc << " To go even further, we added additional tasks that the organisms could solve to see if their fitness would be affected by the amount of tasks they are solving.";

        //set range for the initital population size config panel
        config_panel.SetRange("INIT_POP_SIZE", "25", "1000");
        //set range for the mutation rate config panel
        config_panel.SetRange("MUTATION_RATE", "0.0", "0.07");

        Organism *org = new Organism(&world, 25.0);
        org->SetMutationRate(config.MUTATION_RATE());

        //changes the population structure to a Grid
        world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
        world.Resize(num_h_boxes, num_w_boxes);


        //Random
        for (int i = 0; i < config.INIT_POP_SIZE(); i++) {
            world.Inject(*org);
        }
    }

    /**
    * Method called at each step of the animation.
    */
    void DoFrame() override
    {
        canvas.Clear();

        //update the world and organism behavior
        world.Update();

        int org_num = 0;

        //make grid
        //loop through every grid cell
        for (int x = 0; x < num_w_boxes; x++)
        {
            for (int y = 0; y < num_h_boxes; y++)
            {
                if (world.IsOccupied(org_num))
                {
                    if (world.GetOrg(org_num).cpu.state.fitness <= 9)
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "blue", "black");
                    }
                    if (world.GetOrg(org_num).cpu.state.fitness > 9)
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "yellow", "black");
                    }
                }
                else
                {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
                }
                org_num++;
            }
        }
    }
};

AEAnimator animator;

int main()
{
    //Have animator call DoFrame once to start
    animator.Step();
}