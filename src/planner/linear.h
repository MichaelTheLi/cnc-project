//
// Created by Michael Lipinski on 28/01/2018.
//

#ifndef CLION_AVR_LINEAR_H
#define CLION_AVR_LINEAR_H

#define PLAN_SIZE 1024 // TODO check if its enough

typedef struct {
    float x;
    float y;
} Point;

enum PlanItemType {none, x_move, y_move};
enum PlanItemDirection {plan_item_dir_forward, plan_item_dir_backward};
typedef struct {
    enum PlanItemType type;
    enum PlanItemDirection direction;
} PlanItem;

typedef struct {
    PlanItem items[PLAN_SIZE];
} Plan;

enum PlannerResult { planner_full, planner_success, planner_fail};

enum PlannerResult bresenham_line_2d(Point from, Point to, Plan *output);

#endif //CLION_AVR_LINEAR_H
