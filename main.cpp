//
//  main.cpp
//  layer
//
//

#include <stdio.h>
#include <iostream>

#include "layer.h"
//#include "ExcelFormat.h"
using namespace std;

int main(const int argc, const char **argv){
    
	two_layer two_layer_A;
	two_layer_A.read_file(argv);
	two_layer_A.find_top_gridline_xy();
	two_layer_A.assign_via();
	//two_layer_A.find_bottom_gridline_x();
	//two_layer_A.copy_gird();
	two_layer_A.find_path();
	two_layer_A.merge();
	two_layer_A.gnuplot_top_pic(argv[2]);
	two_layer_A.gnuplot_bottom_pic(argv[3]);
	two_layer_A.gnuplot_all_pic(argv[4]);
    return 0;
}

