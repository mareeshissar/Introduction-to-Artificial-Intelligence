//
//  main.cpp
//  assignment3_AI
//
//
//
//

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "map.h"
#include "point.h"

int main(int argc, const char* argv[]) {

    int p1_1, p2_1;
    int p1_2, p2_2;
    int d;
    cout << "Enter the dimensions for the map:";
    cin >> d;
    vector<vector<int>> htable_map1;
    vector<vector<int>> htable_map2;
    htable_map1.resize(4);
    htable_map2.resize(4);
    
    map map1(d);
    map map2 = map1;                            //start with the same map for rule 2
    map map3 = map1;
    
    //************************************************************************ RULE 1 *******************************************************************************************************************
    int iter = 0;
    double total_number_of_steps_rule1 = 0;
    double total_number_of_steps_rule2 = 0;
    while( iter < 100)
    {
        bool target_found_rule1 = false;
        bool target_found_rule2 = false;
        srand(983);
        int count = 0;
        int step1 = 1;
        int step2 = 1;
        
        while (target_found_rule1 == false || target_found_rule2 == false)              //while both have searched the location
        {
            if (count == 0)
            {
                p1_1 = 0 + rand() % (d);
                p2_1 = 0 + rand() % (d);
                p1_2 = p1_1;                    //start the search for rule 2 with the same points
                p2_2 = p2_1;
                count++;
            }
            
            else
            {
                //comment the lines below while using rule 2
                if(target_found_rule1 == false)
                {
                    p1_1 = map1.find_next_cell_rule_1().ReturnY();
                    p2_1 = map1.find_next_cell_rule_1().ReturnX();
                }
                //comment the lines below while using rule 1
                if(target_found_rule2 == false)
                {
                    p1_2 = map2.find_next_cell_rule_2().ReturnY();
                    p2_2 = map2.find_next_cell_rule_2().ReturnX();
                }
            }
            
            if(target_found_rule1 == false)
                target_found_rule1 = map1.search_cell(p1_1, p2_1);
            if(target_found_rule2 == false)
                target_found_rule2 = map2.search_cell(p1_2, p2_2);
            
            if (target_found_rule1 == true && target_found_rule2 == true)               //if both of them are true, find and fix a new target
            {
                point prev = map1.return_target();   //previous target and update for all terrain
                int k = map1.return_target_terrain(prev.ReturnY(),prev.ReturnX());
                htable_map1[k].push_back(step1);
                htable_map2[k].push_back(step2);
                
                
                sleep(5);
                map1 = map3;
                map2 = map3;

                map1.insert_target();
                point p = map1.return_target();
                map2.set_target(p);                         //set the same target
                break;
            }
            if(target_found_rule1 == false)
            {
                map1.update_belief_rule_1(p1_1, p2_1);
                step1++;
            }
            //uncomment the lines below to get results using rule 2
            if(target_found_rule2 == false)
            {
                map2.update_belief_rule_1(p1_2, p2_2);
                map2.update_belief_rule_2(p1_2, p2_2);
                step2++;
            }
            
            
        }
        //cout << "Number of steps:" << step << endl;
        total_number_of_steps_rule1 = total_number_of_steps_rule1 + step1;
        total_number_of_steps_rule2 = total_number_of_steps_rule2 + step2;

        iter = iter + 1;
    }
    
    double iterations = static_cast<double>(iter);
    cout << "Average number of steps used for Rule 1 "<< iterations << " iterations:" << total_number_of_steps_rule1 / iterations << endl;
    cout << "Average number of steps used for Rule 2 "<< iterations << " iterations:" << total_number_of_steps_rule2 / iterations << endl;
    
    cout << "RULE 1 Analysis:" << endl;
    for(int i =0; i<htable_map1.size(); i++)
    {
        cout << i << " : "<< htable_map1[i].size() << " Avg Steps: ";
        int s = 0;
        for(int j=0;j<htable_map1[i].size();j++)
        {
            s = s + htable_map1[i][j];
        }
        cout << s / htable_map1[i].size() << endl;
    }
    
    cout << "RULE 2 Analysis:" << endl;
    for(int i =0; i<htable_map2.size(); i++)
    {
        cout << i << " : "<< htable_map2[i].size() << " Avg Steps: ";
        double s = 0;
        for(int j=0;j<htable_map2[i].size();j++)
        {
            s = s + static_cast<double>(htable_map2[i][j]);
        }
        cout << s / htable_map2[i].size() << endl;
    }
//*************************************************RULE2********************************************************************************************************
    
    /*
    iter = 0;
    total_number_of_steps = 0;
    
    while( iter < 50)
    {
        bool target_found=false;
        srand(983);
        int count = 0;
        int step = 1;
        while (target_found == false)
        {
            if (count == 0)
            {
                p1 = 0 + rand() % (d);
                p2 = 0 + rand() % (d);
                count++;
            }
            
            else
            {
                //comment the lines below while using rule 2
                //p1 = map1.find_next_cell_rule_1().ReturnY();
                //p2 = map1.find_next_cell_rule_1().ReturnX();

                //comment the lines below while using rule 1
                p1 = map2.find_next_cell_rule_2().ReturnY();
                p2 = map2.find_next_cell_rule_2().ReturnX();
            }
            
            target_found=map2.search_cell(p1, p2);
            
            if (target_found == true)
            {
                sleep(5);
                map2.insert_target();
                break;
            }
            
            map2.update_belief_rule_1(p1, p2);

            //uncomment the lines below to get results using rule 2
            map2.update_belief_rule_2(p1, p2);
            
            step++;
        }
        //cout << "Number of steps:" << step << endl;
        total_number_of_steps = total_number_of_steps + step;
        iter = iter + 1;
    }
    
    iterations = static_cast<double>(iter);
    cout << "Average number of steps used for Rule 2 "<< iterations << " iterations:" << total_number_of_steps / iterations << endl;
    */

    return 0;
}
