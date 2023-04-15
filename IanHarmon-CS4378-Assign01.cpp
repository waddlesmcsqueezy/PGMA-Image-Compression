// IanHarmon-CS4378-Assign01.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "IanHarmon-CS4378-Assign01.h"

using namespace std;

int quad_max_level(const vector<vector<int>>& quad)
{
	int max = 0;

	for (int x = 0; x < quad.size(); x++)
	{
		for (int y = 0; y < quad[0].size(); y++)
		{
			if (quad[x][y] > max)
				max = quad[x][y];
		}
	}
	return max;
}

int quad_pixel_count(const vector<vector<int>>& quad)
{

	return quad.size() * quad[0].size();
}

int quad_pixel_sum(const vector<vector<int>>& quad, int mod = 0)
{
	int sum = 0;

	for (int x = 0; x < quad.size(); x++)
	{
		for (int y = 0; y < quad[0].size(); y++)
		{
			sum += quad[x][y];
			sum -= mod;
		}
	}

	return sum;
}

// sum of pixels values in a quad divided by the number of pixel in the quad
int quad_arithmetic_mean(const vector<vector<int>>& quad)
{
	return quad_pixel_sum(quad) / quad_pixel_count(quad);
}

int quad_arithmetic_variance(const vector<vector<int>>& quad)
{
	const int thingy = quad_pixel_count(quad) - 1; // in the notes this value is (1 / n - 1)

	int sum = 0;
	int mean = quad_arithmetic_mean(quad);

	for (int x = 0; x < quad.size(); x++)
	{
		for (int y = 0; y < quad[0].size(); y++)
		{
			sum += pow(quad[x][y] - mean, 2);
		}
	}

	return sum / thingy;
}

bool quad_is_uniform(const vector<vector<int>>& quad, const int variance_threshold)
{
	if (quad_arithmetic_variance(quad) > variance_threshold)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// makes all the pixels in a quad the mean value of the quad
vector<vector<int>> quad_uniformize(const vector<vector<int>>& read_quad)
{
	vector<vector<int>> write_quad = read_quad;
	const int mean_value = quad_arithmetic_mean(write_quad);
	for (int x = 0; x < write_quad.size(); x++)
	{
		for (int y = 0; y < write_quad[0].size(); y++)
		{

			write_quad[x][y] = mean_value;
		}
	}
	
	return write_quad;
}

// create a vector of 4 quads (2d vector) from a single quad by subdividing equally
vector<vector<vector<int>>> quad_subdivide(const vector<vector<int>>& read_quad)
{
	const int x_axis = floor(read_quad.size() / 2);
	const int y_axis = floor(read_quad[0].size() / 2);

	vector<vector<vector<int>>> write_quad = vector<vector<vector<int>>>(
		4, vector<vector<int>>(x_axis, vector<int>(y_axis)));

	for (int q = 0; q < 4; q++) {
		
		int x_lower,
			x_upper,
			y_lower,
			y_upper;

		if (q % 2 == 0)
		{
			x_lower = 0;
			x_upper = x_axis;
		}
		else
		{
			x_lower = x_axis;
			x_upper = read_quad.size();
		}

		if (q < 2)
		{
			y_lower = 0;
			y_upper = y_axis;
		}
		else
		{
			y_lower = y_axis;
			y_upper = read_quad[0].size();
		}

		for (int x = 0; x < x_axis; x++)
		{
			for (int y = 0; y < y_axis; y++)
			{
				write_quad[q][x][y] = read_quad[x + x_lower][y + y_lower];
			}
		}
	}
	return write_quad;
}

vector<vector<int>> quad_reassemble(vector<vector<vector<int>>> sub_quads)
{
	vector<vector<int>> write_quad (sub_quads[0].size() * 2, vector<int>(sub_quads[0][0].size() * 2));

	const int x_axis = floor(sub_quads[0].size());
	const int y_axis = floor(sub_quads[0][0].size());

	for (int q = 0; q < 4; q++) {
		int x_lower,
			x_upper,
			y_lower,
			y_upper;

		if (q % 2 == 0)
		{
			x_lower = 0;
			x_upper = x_axis;
		}
		else
		{
			x_lower = x_axis;
			x_upper = sub_quads[0].size();
		}

		if (q < 2)
		{
			y_lower = 0;
			y_upper = y_axis;
		}
		else
		{
			y_lower = y_axis;
			y_upper = sub_quads[0][0].size();
		}

		for (int x = 0; x < x_axis; x++)
		{
			for (int y = 0; y < y_axis; y++)
			{
				write_quad[x + x_lower][y + y_lower] = sub_quads[q][x][y];
			}
		}
	}

	return write_quad;
}

/*
*	Recursive Function quads()
*
*	Purpose:
*		Use variance value to determine if a quad is uniform
*
*	Termination condition:
*		When the Quad given is now uniform
*		Region is one pixel
*
*	Recursion:
*		Divide the quad into 4 new quads
*		Call quads() 4 times
*
*	After quads() finishes, we are back into the driver and can output the array as a file there
*
*	Current curiosity: figure out how to determine where a quad is located in the parent quad
*
*
*	Might be a worthy investment to try just directly editing the original image quad/vectors instead of writing to a new image quad/vectors
*/

vector<vector<int>> quads(const vector<vector<int>>& read_quad, const int variance_threshold)
{
	vector<vector<int>> write_quad = read_quad;
	//is the quad uniform? 
	if (quad_pixel_count(write_quad) == 1 || quad_is_uniform(read_quad, variance_threshold))
	{
		//yes, so, uniformize the quad, return the quad, end this recursion
		return write_quad = quad_uniformize(write_quad);
	}
	else 
	{
		//no, so split into 4 sub-arrays then recur each one
		vector<vector<vector<int>>> sub_quads(4);
		sub_quads = quad_subdivide(write_quad);
		// TODO: reassemble all 4 of the subquads back into write quad somehow
		for (int q = 0; q < 4; q++) {
			sub_quads[q] = quads(sub_quads[q], variance_threshold);
		}
		return write_quad = quad_reassemble(sub_quads);
	}
}

int main()
{
	// determines the variance threshold (info in assignment rubric, user input)
	int variance_threshold,
		// width and height of the image (retrieved from the file itself)
		image_width,
		image_height,
		// grayscale of the image (retrieved from the file itself)
		image_intensity;

	// determines the name of the input image for the program (user input)
	string image_name,
	       output_name,
		// string of the current line of the input image
		   line;

	ifstream fin;
	stringstream ss;

	image_name;

	cout << "MAKE SURE TO INCLUDE DOUBLE SLASHES IN THE FILE NAME, EG; C:\\\\Users\\\\name\\\\Downloads\\\\baboon\\\\baboon.pgma" << endl;
	// request user input of image input and output name
	cout << "Please enter the exact path name of the INPUT image: ";
	cin >> image_name;
	cout << "Please enter the exact path name of the desired OUTPUT image: ";
	cin >> output_name;

	// request user input of the desired variance threshold for the image/program
	cout << "Please enter desired variance threshold: ";
	cin >> variance_threshold;

	// open the requested file
	fin.open(image_name);

	if (fin.fail() == true) {
		cout << "ERROR: File " << image_name << " not found";
		return -1;
	}

	// check if is a P2 image, exit if not
	getline(fin, line);
	if (line.compare("P2")) {
		cout << "ERROR: input file is not of type \"P2\", please fix the image file and try again.";
		return -1;
	}
	// get comment line from input file, do nothing with it
	getline(fin, line);
	// Continue with a stringstream
	ss << fin.rdbuf();
	// Third line : size
	ss >> image_width >> image_height >> image_intensity;


	// initialize the vectors we will be using to modify the image
	vector<vector<int>> image_vector(image_width, vector<int>(image_height));
	vector<vector<int>> result_vector(image_width, vector<int>(image_height));

	
	//reading the actual pixel values
	int current_x = 0,
	current_y = 0;

	if (fin.is_open()) {
		while (fin.good()) {

			getline(fin, line);

			string word;
			while (ss >> word) {
				image_vector[current_x][current_y] = stoi(word);
				current_x += 1;

				if (current_x == result_vector.size()) {
					current_x = 0;
					current_y += 1;
				}
			}
		}
	}

	current_x = 0,
	current_y = 0;

	/*
	 * The part where we actually use the quads method
	 */
	result_vector = quads(image_vector, variance_threshold);


	int linebreak_threshold = 0;

	ofstream fout;
	fout.open(output_name);  // eg; "C:\\Users\\name\\Downloads\\baboon\\output.pgma"

	fout << "P2" << endl
		<< "# output.pgma created by Ian Harmon's program IanHarmon-CS4378-Assign01." << endl
		<< result_vector.size() << "  " << result_vector[0].size() << endl
		<< quad_max_level(result_vector) << endl;

	if (fout.is_open()) {
		while (current_y < image_height) {
			fout << result_vector[current_x][current_y] << "  ";

			current_x += 1;
			linebreak_threshold += 1;
			if (linebreak_threshold == 12) {
				fout << "\n";
				linebreak_threshold = 0;
			}
			if (current_x == image_width) {
				current_x = 0;
				current_y += 1;
			}
		}
	}



	fout.close();

	fin.close();

	cout << "Program finished with no errors. Output written to " << output_name << endl;

	return 0;
}