//
// Created by Michael Lipinski on 01/02/2018.
//

#include "plannerVisualizer.h"

#include <stdlib.h>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>

#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

void renderCoordLines();

int w = 800;
int h = 800;

typedef struct {
    Plan plan;
    Point pos;
    float stepSize;
    char renderType;
} RenderPlan;

#define RENDER_PLANS_SIZE (1024 * 1024 * 1024)
int plan_i = 0;
RenderPlan render_plans[RENDER_PLANS_SIZE];

Point render_real_start;
Point render_real_end;

void addPlanToRender(Plan *output, Point *pos, float stepSize, char renderType)
{
    RenderPlan temp_render_plan = {
            .plan = *output,
            .pos = *pos,
            .stepSize = stepSize,
            .renderType = renderType
    };

    render_plans[plan_i++] = temp_render_plan;
}

void addRealLine(Point *start, Point *end)
{
    render_real_start = *start;
    render_real_end = *end;
}

void renderReal(Point *start, Point *end)
{
    glLineWidth(1);
    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_LINES);
    glVertex3f(start->x, start->y - 10, 0.0);
    glVertex3f(end->x, end->y - 10, 0);
    glEnd();
}
void renderPlan(Plan *output, Point *pos, float stepSize, char renderType)
{
    glLineWidth(1.0);

    float colorStep = 1.0f / PLAN_SIZE;
    float colorStep1 = 0;
    float colorStep2 = 0;
    float colorStep3 = 0;

    float color1 = 0.0;
    float color2 = 0.0;
    float color3 = 0.0;

    if (0) {
        if (renderType == 1) {
            color1 = 1.0;
            color2 = 0.0;
            color3 = 0.0;
//        colorStep1 = -colorStep;
//        colorStep2 = colorStep;
        }
        if (renderType == 2) {
            color1 = 0.0;
            color2 = 1.0;
            color3 = 0.0;
//        colorStep1 = -colorStep;
//        colorStep3 = colorStep;
        }

        if (renderType == 3) {
            color1 = 0.0;
            color2 = 0.0;
            color3 = 1.0;
//        colorStep2 = -colorStep;
//        colorStep3 = colorStep;
        }
    } else {
        color1 = 1.0;
        color2 = 0.0;
        color3 = 0.0;
    }

    if (0) {
        glColor3f(color1, color2, color3);
        glPointSize(2.0);

        glBegin(GL_POINTS);
        glVertex3f(pos->x, pos->y, 0);
        glEnd();
    }
//
//    glPointSize(1.0);
//    glLineWidth(1.0);

    for (int i = 0; i < PLAN_SIZE; ++i) {
        glColor3f(color1, color2, color3);
        color1 += colorStep1;
        color2 += colorStep2;
        color3 += colorStep3;
        PlanItem item = output->items[i];
        if (item.type == none) {
            break;
        }

        float dx = 0;
        float dy = 0;
        if (item.type == x_move) {
            dx = item.direction == plan_item_dir_forward
                    ? stepSize
                    : -stepSize;
        } else if (item.type == y_move) {
            dy = item.direction == plan_item_dir_forward
                 ? stepSize
                 : -stepSize;
        }

        glBegin(GL_LINES);
        glVertex3f(pos->x, pos->y, 0);
        glVertex3f(pos->x + dx, pos->y + dy, 0);
        glEnd();

        pos->x += dx;
        pos->y += dy;
    }
}

void paint(void)
{
    glClearColor(0.3,0.3,0.3,0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glLoadIdentity();
    glutSwapBuffers();

    renderCoordLines();

    renderReal(&render_real_start, &render_real_end);

    for (int i = 0; i < plan_i; ++i) {
        RenderPlan plan = render_plans[i];
        if(plan.stepSize != 0) {
            renderPlan(&plan.plan, &plan.pos, plan.stepSize, plan.renderType);
        }
    }

    glFlush();
}

void renderCoordLines() {
    glLineWidth(1);
    glColor3f(0.65, 0.65, 0.65);

    glOrtho(0, w, 0, h, -300.0, 300.0);

    glBegin(GL_LINES);
    glVertex3f(0, h/2, 0.0);
    glVertex3f(w, h/2, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(w/2, 0, 0.0);
    glVertex3f(w/2, h, 0);
    glEnd();

    int wSize = 200;
    int hSize = 200;
    int centerX = -100;
    int centerY = -130;

//    int wSize = 200;
//    int hSize = 200;
//    int centerX = -50;
//    int centerY = -50;

    glLoadIdentity();
    glOrtho(
            -(wSize / 2 + centerX),
            (wSize / 2 - centerX),
            -(hSize / 2 + centerY),
            (hSize / 2 - centerY),
            -100.0,
            100.0
    );

    int delim = 10;
    int size = 1;
    for (int i = -hSize/2; i < hSize/2; i += delim ) {
        if (i % (delim * 5) == 0) {
            glColor3f(0.3, 0.3, 1);
            size = 3;
        } else {
            glColor3f(0.65, 0.65, 0.65);
            size = 1;
        }

        glBegin(GL_LINES);
        glVertex3f(-size, i, 0.0);
        glVertex3f(size, i, 0);
        glEnd();
    }

    for (int i = -wSize/2; i < wSize/2; i += delim ) {
        if (i % (delim * 5) == 0) {
            glColor3f(0.3, 0.3, 1);
            size = 3;
        } else {
            glColor3f(0.65, 0.65, 0.65);
            size = 1;
        }
        glBegin(GL_LINES);
        glVertex3f(i, -size, 0.0);
        glVertex3f(i, size, 0);
        glEnd();
    }

}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

int startPlannerVisualization()
{
    int argc = 1;
    char *argv[1] = {"test"};
    glutInit(&argc, argv);
    glutInitWindowSize(w, h);
    // glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitDisplayMode( GLUT_RGBA );
    glutCreateWindow("Triangle");

    glutDisplayFunc(paint);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return EXIT_SUCCESS;
}
