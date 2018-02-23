//
// Created by Michael Lipinski on 15/01/2018.
//

#include "plannerVisualizer.h"
#include "plannerFunctionalTest.h"
#include "testCommands.h"
#include "testCommandYoda.h"
#include "testCommandsFull.h"

int main (int argc, char *argv[]) {
//    testLinearInterpolation();
//    testArcInterpolation();
//    testArcCCWInterpolation();

    char **commands = fishFull;
    int size = sizeof(fishFull) / sizeof(commands[0]);

    testRealCommand(size, commands);

    startPlannerVisualization();
}
