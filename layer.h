//
//  layer.h
//  layer
//
//

#ifndef layer_h
#define layer_h

#include "ExcelFormat.h"

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

struct shape{
	int num;
    string name;
	char type;
	vector < pair < double , double > > vertices;
	pair <double , double> coordinate_ld;
	pair <double , double> coordinate_ru;
	pair <double , double> center;
};

struct layer {
	int num;
    string name;
	double width;
	double spacing;
	vector < int > shapes;
	vector < pair < double , double > > grid_points;
};

struct net_segment{
	int net_num;
	int grid_num;
	int layer_num; // 0:top 1:bottom
	int num;
	char HV;
	pair < double , double >  segment;
};

struct net{
	int num;
    string name;
	pair < int , int > Shps;
	vector < net_segment >  path;
	//vector <int> path_y;
	//vector < pair < double , double > > path_node;
};

struct interval{
	char state; //b e
	char type; //p v
	double xy;
	int num;
};

bool compare (interval,interval);
bool compare2 (double,double);

class two_layer{
	public:
		//void write_output(const char *);
		void read_file(const char **);
		void gnuplot_top_pic(const char *);
		void gnuplot_bottom_pic(const char *);
		void gnuplot_all_pic(const char *);
		void find_top_gridline_xy();
		void find_bottom_gridline_x(); //vertical
		void copy_gird();
		void find_path();
		void merge();
		bool check_path(int , int , double *, double *);
	private:
		pair < double , double > Dsgn_ll;
		pair < double , double > Dsgn_ru;
		vector <layer> layers;
		vector <net> nets;
		vector < shape > shapes;
		vector <interval> vases_x,vases_y,vases_v_x,vases_v_y; //
		vector <double> x_line,y_line,x_bottom_line,y_bottom_line;
		//vector < vector < pair < double , double > > > horizontal_intervals;
		vector < vector < net_segment > > horizontal_intervals;
		vector < vector < net_segment > > vertical_top_intervals;
		vector < vector < net_segment > > vertical_bottom_intervals;
};



#endif
