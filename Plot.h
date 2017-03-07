#ifndef PLOT_H
#define PLOT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <iostream>

#include "Postfix.h"
#include "Array.h"

namespace Plot{

	const int W = 800;
	const int H = 600;

	struct Point{
		GLfloat x;
		GLfloat y;
	};

	Array<Postfix*> *funcs;
	
	Point **points;

	Point cross[4] = {
		{-1.0, 0.0}, {1.0, 0.0},
		{0.0, -1.0}, {0.0, 1.0}
	};

	GLuint *vao, *vbo, cVao, cVbo;

	double xRange;
	double yRange;
	double xOffset = 0;
	double yOffset = 0;

	double lastX;
	double lastY;

	bool pressed;

	float colors[][3] = {
		{0.0, 0.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0},
		{1.0, 1.0, 0.0},
		{0.0, 1.0, 1.0},
	};

	int colorLen = 6;

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
	  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	}

	void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
		if(action == GLFW_PRESS) pressed = true;
		else pressed = false;
	}

	void makePoints(){
		for(int i = 0; i<funcs->size; ++i){
			for(int j=0; j<W; ++j){
				points[i][j].x = (float)j/(W/2)-1.0;
				points[i][j].y = funcs->get(i)->evaluate((points[i][j].x+2*xOffset)*xRange)/yRange+2*yOffset;
			}
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*2*W, points[i]);
		}
		cross[0].y=yOffset*2;
		cross[1].y=yOffset*2;
		cross[2].x=-xOffset*2;
		cross[3].x=-xOffset*2;
		glBindBuffer(GL_ARRAY_BUFFER, cVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 8*sizeof(GLfloat), cross);
	}

	void scrollCallback(GLFWwindow *window, double xOff, double yOff){
		double mult = (yOff < 0) ? 1.05 : 0.95; 
		xRange*=mult;
		yRange*=mult;
		xOffset/=mult;
		yOffset/=mult;
		makePoints();
	}

	void cursorPosCallback(GLFWwindow *window, double xPos, double yPos){
		if(pressed){
			double xDiff = lastX - xPos;
			double yDiff = lastY - yPos;
			xOffset += xDiff/W;
			yOffset += yDiff/H;
			makePoints();
		}
		lastX = xPos;
		lastY = yPos;
	}

	void Plot(Array<Postfix*> *fs, double xR, double yR){
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWwindow* window = glfwCreateWindow(W, H, "Graph", nullptr, nullptr);
		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, keyCallback);
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetCursorPosCallback(window, cursorPosCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);

		glewExperimental = GL_TRUE;
		glewInit();

		glViewport(0, 0, W, H);

		xRange = xR;
		yRange = yR;
		xOffset = 0;
		yOffset = 0;

		vao = new GLuint[fs->size];
		vbo = new GLuint[fs->size];

		funcs = fs;
		
		points = new Point*[funcs->size];
		for(int i=0; i<funcs->size; ++i) points[i] = new Point[W];

		for(int i=0; i<funcs->size; ++i){
			glGenBuffers(1, &vbo[i]);
			glGenVertexArrays(1, &vao[i]);
			glBindVertexArray(vao[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*W*2, points[i], GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
			glBindVertexArray(0);
		}

		glGenBuffers(1, &cVbo);
		glGenVertexArrays(1, &cVao);
		glBindVertexArray(cVao);
		glBindBuffer(GL_ARRAY_BUFFER, cVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8, cross, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		makePoints();

		shader sp("/home/simmeboiii/vertex.shader", "/home/simmeboiii/fragment.shader");

		GLuint colorLoc = glGetUniformLocation(sp.program, "inColor");

		sp.use();

		while(!glfwWindowShouldClose(window)){
			glfwPollEvents();
    		glClearColor(1.0, 1.0, 1.0, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glUniform3f(colorLoc, 1.0, 0.0, 1.0);
			glBindVertexArray(cVao);
			glDrawArrays(GL_LINES, 0, 4);
			for(int i=0; i<funcs->size; ++i){
				glUniform3f(colorLoc, colors[i%colorLen][0], colors[i%colorLen][1], colors[i%colorLen][2]);
				glBindVertexArray(vao[i]);
				glDrawArrays(GL_LINE_STRIP, 0, W);
			}
			glBindVertexArray(0);
			glfwSwapBuffers(window);
		}
		for(int i=0; i<funcs->size; ++i){
			glDeleteBuffers(1, &vbo[i]);
	  		glDeleteVertexArrays(1, &vao[i]);
	  	}
	  	delete[] vao;
	  	delete[] vbo;
	  	for(int i=0; i<funcs->size; ++i) delete[] points[i];
	  	delete[] points;
  		glfwTerminate();
	}
}

#endif