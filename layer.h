//
//  layer.h
//  layer
//
//

#ifndef layer_h
#define layer_h

//#include "ExcelFormat.h"

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
	int net_num;
	vector < pair < double , double > > vertices;
	pair <double , double> coordinate_ld;
	pair <double , double> coordinate_ru;
	pair <double , double> center;
	pair <double , double> modified_center;
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
};

struct interval{
	char state; //b e
	char type; //p v
	double xy;
	int num;
};

bool compare (interval,interval);
bool compare2 (double,double);
bool compare3 (shape,shape);

class two_layer{
	public:
		//void write_output(const char *);
		void read_file(const char **);
		void gnuplot_top_pic(const char *);
		void gnuplot_bottom_pic(const char *);
		void gnuplot_all_pic(const char *);
		void find_top_gridline_xy();
		void assign_via();
		void find_path();
		void merge();
		bool check_path(int , int , double *, double *);
	private:
		pair < double , double > Dsgn_ll;
		pair < double , double > Dsgn_ru;
		vector <layer> layers;
		vector <net> nets;
		vector < shape > shapes;
		vector < shape > vias;
		vector <int> record_via;
		vector <interval> vases_x,vases_y;
		vector <double> x_line,y_line,x_bottom_line;
		vector < vector < net_segment > > horizontal_intervals;
		vector < vector < net_segment > > vertical_top_intervals;
		vector < vector < net_segment > > vertical_bottom_intervals;
};



#endif
