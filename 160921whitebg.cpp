#include <iostream>
#include <vector>
#include "GLFW/glfw3.h"
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <algorithm>
#include <cmath>
#include <iostream>

const int width = 500;
const int height = 500;

float* pixels = new float[width*height * 3];

void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
{
	int j;
	for (int i = i0; i <= i1; i++)
	{
		if (i1 - i0 == 0)
		{
			for (int i = j1; i < j0; i++)
			{
				drawPixel(i1, i, 1.0f, 0.0f, 0.0f);
			}
			break;
		}
		else
			j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

		drawPixel(i, j, red, green, blue);
	}
}
void drawOnPixelBuffer()
{
	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	std::fill_n(pixels, width*height * 3, 1.0f);	// white background

													//for (int i = 0; i<width*height; i++) {
													//	pixels[i * 3 + 0] = 1.0f; // red 
													//	pixels[i * 3 + 1] = 1.0f; // green
													//	pixels[i * 3 + 2] = 1.0f; // blue
													//}

	const int i = rand() % width, j = rand() % height;
	drawPixel(i, j, 0.0f, 0.0f, 0.0f);
	/*
	for (int i = 0; i < 10; i++)
	my_object[i]->draw();
	*/
	//TODO: anti-aliasing
	const int i0 = 100, i1 = 200;
	const int j0 = 50, j1 = 80;
	drawLine(i0, j0, i1, j1, 1.0f, 0.0f, 0.0f);

	//TODO: try moving object
}

void draw_thickness(const int& i_center, const int& j_center, const float& red, const float& green, const float& blue, const int &thickness)
{
	for (int j = j_center; j < j_center + thickness; j++)
		for (int i = i_center - thickness; i < i_center; i++)
		{
			drawPixel(i, j, red, green, blue);
		}
}

void drawCircle(const int& x_c, const int& y_c, const int& r, const float& red, const float& green, const float& blue,
	const int& thickness)
{
	for (int i = 1; i <= 1080; i++)
	{
		if ((cos(i)*cos(i)) + (sin(i)*sin(i)) == 1.0)
		{
			draw_thickness(x_c + (int)(r*cos(i)), y_c + (int)(r*sin(i)), red, green, blue, thickness);
		}
	}
}

class Box // NO PARENT
{
	//some variables
public:
	int start_x = 300, start_y = 300;
	int end_x = 350, end_y = 250;

public:
	void draw()
	{
		drawLine(start_x, start_y, end_x, start_y, 0.0f, 1.0f, 0.0f);

		for (int i = 0; i < start_y - end_y; i++)
			drawPixel(end_x, start_y - i, 0.0f, 1.0f, 0.0f);

		drawLine(start_x, end_y, end_x, end_y, 0.0f, 1.0f, 0.0f);

		for (int i = 0; i < start_y - end_y; i++)
			drawPixel(start_x, start_y - i, 0.0f, 1.0f, 0.0f);
	}
};

class Circle // NO PARENT
{
public:

	int x_c = 200, y_c = 200, r = 30;
	float red = 1.0f, green = 0.0f, blue = 0.0f;
	int thickness = 1;


	void draw()
	{
		drawCircle(x_c, y_c, r, red, green, blue, thickness);
	}

};

class GeometricObjectInterface
{
public:
	virtual void draw() = 0;
};

template<class T_TYPE>
class GeometricObject : public GeometricObjectInterface
{
public:
	void draw()
	{
		T_TYPE my_obj;
		my_obj.draw();
	}
};
// And implement an templatized GeometricObject class.

int main()
{

	std::vector<GeometricObjectInterface*> obj_list;

	obj_list.push_back(new GeometricObject<Box>);
	obj_list.push_back(new GeometricObject<Circle>);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

							  /* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		drawOnPixelBuffer();

		//TODO: RGB struct
		//Make a pixel drawing function
		//Make a line drawing function
		std::fill_n(pixels, width*height * 3, 1.0f);

		for (auto itr : obj_list)
			itr->draw();

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array 


	return 0;

}