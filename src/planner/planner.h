//
// Created by Michael Lipinski on 03/02/2018.
//

#ifndef CLION_AVR_PLANNER_H
#define CLION_AVR_PLANNER_H

#ifndef TEST_BUILD
#define PLAN_SIZE 256 // TODO check if its enough
#else
#define PLAN_SIZE 64
#endif

typedef struct {
    float x;
    float y;
} Point;

enum PlanItemType {none, x_move, y_move, z_move};
enum PlanItemDirection {plan_item_dir_forward, plan_item_dir_backward};
typedef struct {
    enum PlanItemType type;
    enum PlanItemDirection direction;
} PlanItem;

typedef struct {
    PlanItem items[PLAN_SIZE];
} Plan;

enum PlannerResult { planner_full, planner_success, planner_fail};

#endif //CLION_AVR_PLANNER_H
