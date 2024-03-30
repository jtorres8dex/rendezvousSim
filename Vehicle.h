#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>


    class Vehicle
    {
    public:
        int id;

        Vehicle(int id, std::vector<double> ics);
        ~Vehicle();
        

        std::vector<int> neighbors;

        struct Commands {
            double v;
            double w;
        };
        Commands cmds;

        struct State {
            double x;
            double y;
            double theta;
            double v;
            double w;

        };

        bool has_neighbors;
        State state;
        State goal_state;
        
        struct Geometry {
            double width = 2.5;
            double length = 10.0;
            double wheel_radius_m = 0.05;

        };

        enum FSM {
            INIT,
            APPROACHING,
            AT_GOAL,
            DONE,
            ERROR,
            TERMINATED 
        };
        FSM FSM;

        void update_state(State &current_state, Commands cmds);
        std::vector<double> controller(double e_x, double e_y, double e_theta);
        void stop(State &current_state);
    };

#endif 