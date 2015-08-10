//
//  layer.cpp
//  layer
//

#include "layer.h"

bool compare (interval a ,interval b){
	if (a.xy != b.xy){
		return a.xy < b.xy;
	}
	else {
		return a.state > b.state;
	}
}
bool compare2 (double a ,double b){
	return a < b;
}

bool compare3 (shape a,shape b){
	return a.center.first < b.center.first;
}

void two_layer::read_file(const char **test){
	ifstream fin (test[1]);
	string temp,layer_name,net_name;
	double x,y,num;
	int vertice_num;
	layer temp_layer;
	net temp_net;
	shape temp_shape;
	pair < double , double > temp_xy;
	interval vase_x;
	interval vase_y;
	while (!fin.eof()){
		fin >> temp;
		if (temp == "Dsgn"){
			fin >> x >> y;
			Dsgn_ll.first = x;
			Dsgn_ll.second = y;
			fin >> x >> y;
			Dsgn_ru.first = x;
			Dsgn_ru.second = y;
		}
		else if (temp == "Lay"){
			temp_layer.num = layers.size(); 
			fin >> temp;
			temp_layer.name = temp;
		}
		else if (temp == "Width"){
			fin >> num;
			temp_layer.width = num;
		}
		else if (temp == "Spacing"){
			fin >> num;
			temp_layer.spacing = num;
			layers.push_back(temp_layer);
		}
		else if (temp == "Net"){
			fin >> net_name;
			int a = -1; //未存有net
			
			if ( net_name != "NULL"){
				for (int i = 0; i < nets.size(); i++){
					if (net_name == nets[i].name){
						a = i; //已存有net
					}
				}
			}
			else{ //NULL
				a = -2;
			}
			
			if (a == -1){
				temp_net.name = net_name;
				temp_net.num = nets.size();
				nets.push_back(temp_net);
			}
			
			while(!fin.eof()){
				fin >> temp;
				if (temp == "Shp"){
					fin >> temp;
					temp_shape.net_num = temp_net.num;//
					temp_shape.num = shapes.size();
					temp_shape.name = temp;
					temp_shape.type = temp[0];
					
					if ( a >= 0){ //已存有net
						nets[a].Shps.second = temp_shape.num;
					}
					else if ( a == -1){
						nets[nets.size()-1].Shps.first = temp_shape.num;
					}
				}
				else if (temp == "Lay"){
					fin >> layer_name;
				}
				else if (temp == "#Pt"){
					fin >> vertice_num;
					for (int i = 0 ; i < vertice_num; i++){
						fin >> x >> y;
						temp_xy.first = x;
						temp_xy.second = y;
						temp_shape.vertices.push_back(temp_xy);
						//////
						if( i == 0 ){
							temp_shape.coordinate_ld.first = x;
							temp_shape.coordinate_ld.second = y;
						}
						else if ( i == 2){
							temp_shape.coordinate_ru.first = x;
							temp_shape.coordinate_ru.second = y;
						}
						//////
					}
					///
					temp_shape.center.first = (temp_shape.vertices[0].first+temp_shape.vertices[2].first)/2 ; //x
					temp_shape.center.second = (temp_shape.vertices[0].second+temp_shape.vertices[2].second)/2; //y
					////
					shapes.push_back(temp_shape);
					
					if (temp_shape.type == 'v'){
						for (int i = 0 ; i < layers.size() ; i++){
							layers[i].shapes.push_back(temp_shape.num);
						}
						////
						vias.push_back(temp_shape);
						//////
					}
					else{
						if (layer_name == layers[0].name){
							layers[0].shapes.push_back(temp_shape.num);
						}
						else{
							layers[1].shapes.push_back(temp_shape.num);
						}
						//////
						vase_x.state = 'b';
						vase_x.type = temp_shape.type;
						vase_x.xy = temp_shape.coordinate_ld.first;
						vase_x.num = temp_shape.num;
						vases_x.push_back(vase_x);
						vase_x.state = 'e';
						vase_x.type = temp_shape.type;
						vase_x.xy = temp_shape.coordinate_ru.first;
						vase_x.num = temp_shape.num;
						vases_x.push_back(vase_x);
						
						vase_y.state = 'b';
						vase_y.type = temp_shape.type;
						vase_y.xy = temp_shape.coordinate_ld.second;
						vase_y.num = temp_shape.num;
						vases_y.push_back(vase_y);
						vase_y.state = 'e';
						vase_y.type = temp_shape.type;
						vase_y.xy = temp_shape.coordinate_ru.second;
						vase_y.num = temp_shape.num;
						vases_y.push_back(vase_y);
						/////
					}
					temp_shape.vertices.clear();
					break;
				}
			}
		}
	}
	////////0803
	/*for (int i = 0; i < vases_v_y.size(); i++){
		vases_y.push_back(vases_v_y[i]);
	}*/
	//////
	sort (vias.begin(),vias.end(),compare3);
	sort (vases_x.begin(),vases_x.end(),compare);
	sort (vases_y.begin(),vases_y.end(),compare);
	
	cout << "Dsgn " << Dsgn_ll.first << " " << Dsgn_ll.second << " " ;
	cout << Dsgn_ru.first << " " << Dsgn_ru.second << endl ;
	for (int i = 0; i < nets.size(); i++){
		cout << "Net " << nets[i].name << " Shp " << nets[i].Shps.first << " " << nets[i].Shps.second << endl;
	}
	
	for (int i = 0; i < shapes.size(); i++){
		cout << "shape_" << shapes[i].num << " " << shapes[i].name << " ";
		for(int j = 0; j < shapes[i].vertices.size(); j++){
			cout << "(" << shapes[i].vertices[j].first << "," << shapes[i].vertices[j].second <<") ";
		}
		cout << endl;
		cout << "(lx,ly) = (" << shapes[i].coordinate_ld.first  << "," << shapes[i].coordinate_ld.second << ") (rx,ry) = (" << shapes[i].coordinate_ru.first  << "," << shapes[i].coordinate_ru.second << ") center = (" << shapes[i].center.first << "," << shapes[i].center.second << ")" << endl;
		cout << endl;
	}
	
	for (int i = 0; i < layers.size(); i++){
		cout << "Lay " << layers[i].name << " " << layers[i].width << " " << layers[i].spacing << endl;
		cout << "Shapes: " << endl;
		for (int j = 0 ; j < layers[i].shapes.size(); j++){
			cout << layers[i].shapes[j] << " ";
		}
		cout << endl;
	}

	/*cout << "vases_x" << endl;
	for (int i = 0 ; i < vases_x.size() ; i++){
		cout << vases_x[i].state << vases_x[i].num << " ";
	}
	cout << endl;
	cout << "vases_y" << endl;
	for (int i = 0 ; i < vases_y.size() ; i++){
		cout << vases_y[i].state << vases_y[i].num << " ";
	}
	cout << endl;*/
}

///// 0810 //////
void two_layer::find_top_gridline_xy(){
	pair < double , double > temp_xy;
	temp_xy.first = -1;
	temp_xy.second = -1;
	vector < pair < double , double > > x_obs;
	vector < pair < double , double > > y_obs;
	double temp_line;
	for (int i = 0 ; i < vases_x.size() ; ){ // i++
		if (vases_x[i].state == 'b'){
			temp_xy.first = vases_x[i].xy;
			i++;
		}
		else{ //e
			temp_xy.second = vases_x[i].xy;
			
			if ( i == vases_x.size()-1){
				x_obs.push_back(temp_xy);
				break;
			}
			else if (vases_x[i+1].state == 'b'){
				x_obs.push_back(temp_xy);
			}
			i++;
		}
	}
	
	for (int i = 0 ; i < vases_y.size() ; ){ // i++
		if (vases_y[i].state == 'b'){
			temp_xy.first = vases_y[i].xy;
			i++;
		}
		else{ //e
			temp_xy.second = vases_y[i].xy;
			
			if ( i == vases_y.size()-1){
				y_obs.push_back(temp_xy);
				break;
			}
			else if (vases_y[i+1].state == 'b'){
				y_obs.push_back(temp_xy);
			}
			i++;
		}
	}
	/////
	cout << "x_top_obs" << endl;
	for (int i = 0; i < x_obs.size() ; i++){
		cout << "( " << x_obs[i].first << "," << x_obs[i].second << ") "; 
	}
	cout << endl;
	cout << "y_top_obs" << endl;
	for (int i = 0; i < y_obs.size() ; i++){
		cout << "( " << y_obs[i].first << "," << y_obs[i].second << ") "; 
		///////////////
		temp_line = (y_obs[i].first+y_obs[i].second)/2;
		y_line.push_back(temp_line);
		/////////////////
	}
	cout << endl;
	////
	double distance,delta;
	int n;
	for (int i = 0; i < x_obs.size() ; i++ ){
		//temp_line = x_obs[i].second + layers[0].spacing + 0.5*layers[0].width;
		if ( i != x_obs.size()-1){	
			distance = 0;
			n = 0;
			distance = x_obs[i+1].first - x_obs[i].second;
			n = (distance - 2 * layers[0].spacing - layers[0].width) / (layers[0].spacing + layers[0].width);
			delta = (distance - n *(layers[0].spacing + layers[0].width) )/2 ; // - (layers[0].spacing + 0.5*layers[0].width);
			temp_line = x_obs[i].second + delta;
			while( temp_line <= x_obs[i+1].first-layers[0].spacing-0.5*layers[0].width ){
				x_line.push_back(temp_line);
				temp_line = temp_line+layers[0].spacing + layers[0].width;
			}
			
			if ( i == 0){
				temp_line = x_obs[i].first - layers[0].spacing - 0.5*layers[0].width;
				while( temp_line >= Dsgn_ll.first + 0.5*layers[0].width ){ //x_obs[i+1].first-layers[0].spacing-0.5*layers[0].width
					x_line.push_back(temp_line);
					temp_line = temp_line - layers[0].spacing - layers[0].width;				
				}
			} 
		}
		else{
			temp_line = x_obs[i].second + layers[0].spacing + 0.5*layers[0].width;
			while( temp_line <= Dsgn_ru.first - 0.5*layers[0].width){
				x_line.push_back(temp_line);
				temp_line = temp_line + layers[0].spacing + layers[0].width;
			}
		}
	}
	/////
	for (int i = 0; i < y_obs.size() ; i++ ){
		temp_line = y_obs[i].second + layers[0].spacing + 0.5*layers[0].width;
		if ( i != y_obs.size()-1){
			while( temp_line <= y_obs[i+1].first-layers[0].spacing-0.5*layers[0].width ){
				y_line.push_back(temp_line);
				temp_line = temp_line+layers[0].spacing + layers[0].width;
			}
			
			if ( i == 0){
				temp_line = y_obs[i].first - layers[0].spacing - 0.5*layers[0].width;
				while( temp_line >= Dsgn_ll.second + 0.5*layers[0].width ){ 
					y_line.push_back(temp_line);
					temp_line = temp_line - layers[0].spacing - layers[0].width;
				}
			} 
		}
		else{
			while( temp_line <= Dsgn_ru.second - 0.5*layers[0].width){
				y_line.push_back(temp_line);
				temp_line = temp_line + layers[0].spacing + layers[0].width;
			}
		}
	}
	sort (x_line.begin(),x_line.end(),compare2);
	sort (y_line.begin(),y_line.end(),compare2);
	
	//x_bottom_line
	for (int i = 0; i < x_line.size();i++ ){
		//temp_line = x_line[i] + layers[0].spacing + layers[0].width;
		if ( i != x_line.size()-1){
			distance = 0;
			n = 0;
			distance = x_line[i+1] - x_line[i];
			n = (distance - layers[0].spacing - layers[0].width) / (layers[0].spacing + layers[0].width) ;
			delta = (distance - (n-1) *(layers[0].spacing + layers[0].width) )/2 ; // - (layers[0].spacing + 0.5*layers[0].width);
			temp_line = x_line[i] + delta;
			while( temp_line <= x_line[i+1]-layers[0].spacing-layers[0].width ){
				x_bottom_line.push_back(temp_line);
				temp_line = temp_line+layers[0].spacing + layers[0].width;
			}
			
			if ( i == 0){
				temp_line = x_line[i] - layers[0].spacing - layers[0].width;
				while( temp_line >= Dsgn_ll.second + layers[0].width ){ 
					x_bottom_line.push_back(temp_line);
					temp_line = temp_line - layers[0].spacing - layers[0].width;
				}
			} 
		}
		else{
			temp_line = x_line[i] + layers[0].spacing + layers[0].width;
			while( temp_line <= Dsgn_ru.second - layers[0].width){
				x_bottom_line.push_back(temp_line);
				temp_line = temp_line + layers[0].spacing + layers[0].width;
			}
		}
	}
	/////
	for (int i = 0; i < x_obs.size() ; i++){
		//cout << "( " << x_obs[i].first << "," << x_obs[i].second << ") "; 
		///////////////
		temp_line = (x_obs[i].first+x_obs[i].second)/2;
		x_line.push_back(temp_line);
		/////////////////
	}
	sort (x_line.begin(),x_line.end(),compare2);
	sort (x_bottom_line.begin(),x_bottom_line.end(),compare2);
	/////
	cout << "x_line" << endl;
	for (int i = 0; i < x_line.size();i++){
		cout << x_line[i] << " ";
	}
	cout << "\ny_line" << endl;
	for (int i = 0; i < y_line.size();i++){
		cout << y_line[i] << " ";
	}
	cout << endl;
	cout << "\nx_bottom_line" << endl;
	for (int i = 0; i < x_bottom_line.size();i++){
		cout << x_bottom_line[i] << " ";
	}
	cout << endl;
}
//// 0810 test ////
void two_layer::assign_via(){
	int temp_x,start,end;
	record_via.resize(x_bottom_line.size(),0);
	for (int i = 0; i < vias.size(); i++){
		for (int j = 0; j < x_bottom_line.size();j++){
			if (x_bottom_line[j] > vias[i].center.first){
				if (record_via[j-1] != 1){
					record_via[j-1] = 1;
					vias[i].modified_center.first = x_bottom_line[j-1];
				}
				else{
					temp_x = j-1;
					while ( record_via[temp_x] == 1){
						temp_x++;
					}
					record_via[temp_x] = 1;
					vias[i].modified_center.first = x_bottom_line[temp_x];
				}
				
				if (shapes[nets[vias[i].net_num].Shps.first].type == 'v'){
					start = nets[vias[i].net_num].Shps.first;
					end = nets[vias[i].net_num].Shps.second;
				}
				else{
					start = nets[vias[i].net_num].Shps.second;
					end = nets[vias[i].net_num].Shps.first;
				}
				
				
				if(vias[i].center.second < shapes[end].center.second){
					for (int k = 0; k < y_line.size();k++){
						if (y_line[k] > vias[i].center.second){
							vias[i].modified_center.second = y_line[k];
							break;
						}
					}
				}
				else{
					for (int k = y_line.size()-1; k >= 0 ;k--){
						if (y_line[k] < vias[i].center.second){
							vias[i].modified_center.second = y_line[k];
							break;
						}
					}
				}
			
				break;
			}
		}
	}
	
	for(int i = 0; i < shapes.size(); i++){
		if (shapes[i].type == 'v'){
			for (int j = 0; j < vias.size() ; j++){
				if (shapes[i].num == vias[j].num){
					shapes[i].modified_center.first = vias[j].modified_center.first;
					shapes[i].modified_center.second = vias[j].modified_center.second;
				}
			}
		}
	}
}


/////0810 test
void two_layer::find_path(){
	int start,end,temp_x,temp_y;
	net_segment temp_segment;
	horizontal_intervals.resize(y_line.size());
	vertical_top_intervals.resize(x_line.size());
	vertical_bottom_intervals.resize(x_bottom_line.size());
	int check = -1;
	int counter = 0;
	for (int i = 0; i < nets.size();i++){
		if (shapes[nets[i].Shps.first].type == 'v'){
			start = nets[i].Shps.first;
			end = nets[i].Shps.second;
		}
		else{
			start = nets[i].Shps.second;
			end = nets[i].Shps.first;
		}
		
		for (int j = 0; j < y_line.size();j++){
			if (shapes[end].center.second == y_line[j]){//
				temp_y = j-1;
				break;
			}
		}

		if (shapes[start].modified_center.first < shapes[end].center.first){
			for (int j = 0; j < x_line.size();j++){
				if (shapes[end].center.first == x_line[j]){
					if ( shapes[start].modified_center.first <= x_line[j-1]){
						temp_x = j-1;
					}
					else{
						temp_x = j;
						temp_y++;
					}
					///
					if (shapes[start].center.second >= shapes[end].coordinate_ld.second){
						for (int k = 0; k < y_line.size();k++){
							if (y_line[k]==shapes[end].center.second){
								temp_y = k;
								break;
							}
						}
					}
					///
					break;
				}
			}
			////
			check = check_path(1, temp_y-counter, &shapes[start].modified_center.first, &x_line[temp_x]);
			while ( ! check ){
				counter++;
				check = check_path(1, temp_y-counter, &shapes[start].modified_center.first, &x_line[temp_x]);
			}
			temp_segment.segment.first = shapes[start].modified_center.first;
			temp_segment.segment.second = x_line[temp_x];
		}
		else {
			for (int j = x_line.size()-1; j >= 0;j--){
				if (shapes[end].center.first == x_line[j]){
					if ( shapes[start].modified_center.first >= x_line[j+1]){
						temp_x = j+1;
					}
					else{
						temp_x = j;
						temp_y++;
					}
					///
					if (shapes[start].center.second >= shapes[end].coordinate_ld.second){
						for (int k = 0; k < y_line.size();k++){
							if (y_line[k]==shapes[end].center.second){
								temp_y = k;
								break;
							}
						}
					}
					///
					break;
				}
			}
			////
			check = check_path(1, temp_y-counter, &x_line[temp_x], &shapes[start].modified_center.first);
			while ( ! check ){
				counter++;
				check = check_path(1, temp_y-counter, &x_line[temp_x], &shapes[start].modified_center.first);
			}
			temp_segment.segment.first = x_line[temp_x];
			temp_segment.segment.second = shapes[start].modified_center.first;
		}
		temp_segment.net_num = i;
		temp_segment.grid_num = temp_y-counter;
		temp_segment.HV = 'H';
		temp_segment.layer_num = 0;
		temp_segment.num = horizontal_intervals[temp_y-counter].size();
		horizontal_intervals[temp_y-counter].push_back(temp_segment);
		nets[i].path.push_back(temp_segment);
		counter = 0;
		/////////////
		
		for (int j = 0; j < x_bottom_line.size();j++){
			if ( x_bottom_line[j] == shapes[start].modified_center.first){
				temp_x = j;
			}
		}
		temp_segment.segment.first = shapes[start].modified_center.second;
		temp_segment.segment.second = y_line[nets[i].path[0].grid_num];//nets[i].path.size()-1
		temp_segment.net_num = i;
		temp_segment.grid_num = temp_x;
		temp_segment.HV = 'V';
		temp_segment.layer_num = 1;
		temp_segment.num = vertical_bottom_intervals[temp_x].size();
		vertical_bottom_intervals[temp_x].push_back(temp_segment); //vertical
		nets[i].path.push_back(temp_segment);
		
		//////////
		if (shapes[start].modified_center.first < shapes[end].center.first){
			for (int j = 0; j < x_line.size();j++){
				if ( x_line[j] == nets[i].path[0].segment.second){
					temp_x = j;
				}
			}
			temp_segment.segment.first = y_line[nets[i].path[0].grid_num];
			temp_segment.segment.second = shapes[end].center.second;//nets[i].path.size()-1
			temp_segment.net_num = i;
			temp_segment.grid_num = temp_x;
			temp_segment.HV = 'V';
			temp_segment.layer_num = 1;
			temp_segment.num = vertical_top_intervals[temp_x].size();
			vertical_top_intervals[temp_x].push_back(temp_segment); //vertical
			nets[i].path.push_back(temp_segment);
			
			for (int j = 0; j < y_line.size();j++){
				if ( y_line[j] == nets[i].path[nets[i].path.size()-1].segment.second){
					temp_y = j;
				}
			}
			temp_segment.segment.first = x_line[nets[i].path[nets[i].path.size()-1].grid_num];
			temp_segment.segment.second = shapes[end].center.first;//nets[i].path.size()-1
			temp_segment.net_num = i;
			temp_segment.grid_num = temp_y;
			temp_segment.HV = 'H';
			temp_segment.layer_num = 0;
			temp_segment.num = horizontal_intervals[temp_y].size();
			horizontal_intervals[temp_y].push_back(temp_segment);
			nets[i].path.push_back(temp_segment);
		}
		else{
			for (int j = 0; j < x_line.size();j++){
				if ( x_line[j] == nets[i].path[0].segment.first){
					temp_x = j;
				}
			}
			temp_segment.segment.first = y_line[nets[i].path[0].grid_num];
			temp_segment.segment.second = shapes[end].center.second;//nets[i].path.size()-1
			temp_segment.net_num = i;
			temp_segment.grid_num = temp_x;
			temp_segment.HV = 'V';
			temp_segment.layer_num = 1;
			temp_segment.num = vertical_top_intervals[temp_x].size();
			vertical_top_intervals[temp_x].push_back(temp_segment); //vertical
			nets[i].path.push_back(temp_segment);
			
			for (int j = 0; j < y_line.size();j++){
				if ( y_line[j] == nets[i].path[nets[i].path.size()-1].segment.second){
					temp_y = j;
				}
			}
			temp_segment.segment.first = shapes[end].center.first;
			temp_segment.segment.second = x_line[nets[i].path[nets[i].path.size()-1].grid_num];//nets[i].path.size()-1
			temp_segment.net_num = i;
			temp_segment.grid_num = temp_y;
			temp_segment.HV = 'H';
			temp_segment.layer_num = 0;
			temp_segment.num = horizontal_intervals[temp_y].size();
			horizontal_intervals[temp_y].push_back(temp_segment);
			nets[i].path.push_back(temp_segment);
		}
		
		
		
		
	}
	/*for (int i = 0 ;i < horizontal_intervals.size();i++){
		cout << "y" << i << " ";
		for (int j = 0 ; j < horizontal_intervals[i].size(); j++){
			cout  << "(" << horizontal_intervals[i][j].segment.first << "," << horizontal_intervals[i][j].segment.second << ") ";
		}
		cout << endl;
	}*/
}

bool two_layer::check_path(int xy, int num, double *begin, double *end){ //0:x 1:y ; 0:top 1:bottom ; , int layer
	vector <interval> vases;
	interval vase;
	
	vase.state = 'b';
	vase.xy = (*begin);
	vase.num = 0;
	vases.push_back(vase);
	vase.state = 'e';
	vase.xy = (*end);
	vase.num = 0;
	vases.push_back(vase);
	if ( xy == 0){		
		for (int i = 0 ; i < vertical_top_intervals[(num)].size();i++){
			vase.state = 'b';
			vase.xy = vertical_top_intervals[(num)][i].segment.first;
			vase.num = i+1;
			vases.push_back(vase);
			vase.state = 'e';
			vase.xy = vertical_top_intervals[(num)][i].segment.second;
			vase.num = i+1;
			vases.push_back(vase);
		}
	}
	else {
		for (int i = 0 ; i < horizontal_intervals[(num)].size();i++){
			vase.state = 'b';
			vase.xy = horizontal_intervals[(num)][i].segment.first;
			vase.num = i+1;
			vases.push_back(vase);
			vase.state = 'e';
			vase.xy = horizontal_intervals[(num)][i].segment.second;
			vase.num = i+1;
			vases.push_back(vase);
		}
	}
	sort (vases.begin(),vases.end(),compare);
	for (int i = 0 ; i < vases.size();i++){
		if(vases[i].num == 0 && vases[i].state == 'b'){
			if (vases[i+1].num == 0 && vases[i+1].state == 'e'){
				return true;
			}
			else{
				return false;
			}
		}
	}
}


void two_layer::merge(){
	int overlap;
	for (int i = 0; i < nets.size();i++){
		overlap = 0;
		if( nets[i].path[nets[i].path.size()-1].grid_num - nets[i].path[0].grid_num > 1 ){
			for (int j = nets[i].path[0].grid_num + 1; j < nets[i].path[nets[i].path.size()-1].grid_num ; j++){
				for ( int k = 0; k < horizontal_intervals[j].size(); k++){
					if (horizontal_intervals[j][k].segment.first <= x_line[nets[i].path[2].grid_num] && horizontal_intervals[j][k].segment.second >= x_line[nets[i].path[2].grid_num]){
						overlap = 1;
						break;
					}
				}
				if( overlap == 1 ){
					break;
				}
			}
		}
		
		if ( overlap == 0 ){
			nets[i].path[2].layer_num = 0;
			vertical_top_intervals[nets[i].path[2].grid_num][nets[i].path[2].num].layer_num = 0;
		}
	}
}











void two_layer::gnuplot_top_pic(const char *output){
	ofstream gnuData_out("data.txt", ios::out);
    ofstream gnuCmd_out("gp", ios::out);
	
	for ( int i=0;i<shapes.size();i++){
		for (int j=0;j<shapes[i].vertices.size();j++){
			if (shapes[i].type == 'v'){
				if ( j != shapes[i].vertices.size()-1){
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[j+1].first<<","<<shapes[i].vertices[j+1].second<<" nohead lc 6 lw 2"<<endl;
				}
				else{
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[0].first<<","<<shapes[i].vertices[0].second<<" nohead lc 6 lw 2"<<endl;
				}
			}
			else {
				if ( j != shapes[i].vertices.size()-1){
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[j+1].first<<","<<shapes[i].vertices[j+1].second<<" nohead lc 3 lw 2"<<endl;
				}
				else{
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[0].first<<","<<shapes[i].vertices[0].second<<" nohead lc 3 lw 2"<<endl;
				}
			}			
		}
		//cout<<shapes[i].name<<endl;
		gnuData_out<<shapes[i].vertices[0].first<<" "<<shapes[i].vertices[0].second<<endl<<shapes[i].vertices[2].first<<" "<<shapes[i].vertices[2].second<<endl;
		gnuCmd_out<<"set label \""<< shapes[i].name <<"\" at "<<shapes[i].center.first<<","<<shapes[i].center.second<<" front center font \",36\""<<endl;
	}
	
	for (int i=0;i<nets.size();i++){
		gnuCmd_out << "set arrow from "<<shapes[nets[i].Shps.first].center.first<<","<<shapes[nets[i].Shps.first].center.second<<" to "<<shapes[nets[i].Shps.second].center.first<<","<<shapes[nets[i].Shps.second].center.second<<" nohead lc 6 lw 1"<<endl;
	}
	
	for (int i =0; i < x_line.size(); i++){
		gnuCmd_out << "set arrow from "<<x_line[i]<<","<<Dsgn_ll.second<<" to "<<x_line[i]<<","<<Dsgn_ru.second<<" nohead lc 4 lw 1"<<endl;
	}
	
	for (int i =0; i < y_line.size(); i++){
		gnuCmd_out << "set arrow from "<<Dsgn_ll.first<<","<<y_line[i]<<" to "<<Dsgn_ru.first<<","<<y_line[i]<<" nohead lc 4 lw 1"<<endl;
	}
	
	for(int i = 0 ; i < horizontal_intervals.size(); i++){
		for (int j = 0; j < horizontal_intervals[i].size();j++){
			gnuCmd_out << "set arrow from "<<horizontal_intervals[i][j].segment.first<<","<<y_line[i]<<" to "<<horizontal_intervals[i][j].segment.second<<","<<y_line[i]<<" nohead lc 10 lw 3"<<endl;
		}
	}	
	
	for(int i = 0 ; i < vertical_top_intervals.size(); i++){
		for (int j = 0; j < vertical_top_intervals[i].size();j++){
			gnuCmd_out << "set arrow from "<<x_line[i]<<","<<vertical_top_intervals[i][j].segment.first<<" to "<<x_line[i]<<","<<vertical_top_intervals[i][j].segment.second<<" nohead lc 8 lw 3"<<endl;
		}
	}
	
    gnuData_out.close();
	//gnuCmd_out << "set term png size 1000,1000" <<endl;
	//gnuCmd_out << "set term png size 15000,3000" <<endl;
	gnuCmd_out << "set term png size 40000,3000" <<endl;
    gnuCmd_out << "set output '"<<output<<".png'"<<endl;
    gnuCmd_out << "plot 'data.txt' using 1:2 with points"<<endl;
    gnuCmd_out.close();
    system("gnuplot gp");
}

/////////////
void two_layer::gnuplot_bottom_pic(const char *output){
	ofstream gnuData_out("data2.txt", ios::out);
    ofstream gnuCmd_out("gp2", ios::out);
	
	for ( int i=0;i<shapes.size();i++){
		for (int j=0;j<shapes[i].vertices.size();j++){
			if (shapes[i].type == 'v'){
				if ( j != shapes[i].vertices.size()-1){
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[j+1].first<<","<<shapes[i].vertices[j+1].second<<" nohead lc 6 lw 2"<<endl;
				}
				else{
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[0].first<<","<<shapes[i].vertices[0].second<<" nohead lc 6 lw 2"<<endl;
				}
			}
			else {
				if ( j != shapes[i].vertices.size()-1){
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[j+1].first<<","<<shapes[i].vertices[j+1].second<<" nohead lc 3 lw 2"<<endl;
				}
				else{
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[0].first<<","<<shapes[i].vertices[0].second<<" nohead lc 3 lw 2"<<endl;
				}
			}			
		}
		//cout<<shapes[i].name<<endl;
		gnuData_out<<shapes[i].vertices[0].first<<" "<<shapes[i].vertices[0].second<<endl<<shapes[i].vertices[2].first<<" "<<shapes[i].vertices[2].second<<endl;
		gnuCmd_out<<"set label \""<< shapes[i].name <<"\" at "<<shapes[i].center.first<<","<<shapes[i].center.second<<" front center font \",36\""<<endl;
	}
	for (int i=0;i<nets.size();i++){
		gnuCmd_out << "set arrow from "<<shapes[nets[i].Shps.first].center.first<<","<<shapes[nets[i].Shps.first].center.second<<" to "<<shapes[nets[i].Shps.second].center.first<<","<<shapes[nets[i].Shps.second].center.second<<" nohead lc 6 lw 1"<<endl;
	}
	
	for (int i =0; i < x_bottom_line.size(); i++){
		gnuCmd_out << "set arrow from "<<x_bottom_line[i]<<","<<Dsgn_ll.second<<" to "<<x_bottom_line[i]<<","<<Dsgn_ru.second<<" nohead lc 5 lw 1"<<endl;
	}
	
	for (int i =0; i < x_line.size(); i++){
		gnuCmd_out << "set arrow from "<<x_line[i]<<","<<Dsgn_ll.second<<" to "<<x_line[i]<<","<<Dsgn_ru.second<<" nohead lc 4 lw 1"<<endl;
	}
	
	for (int i =0; i < y_line.size(); i++){
		gnuCmd_out << "set arrow from "<<Dsgn_ll.first<<","<<y_line[i]<<" to "<<Dsgn_ru.first<<","<<y_line[i]<<" nohead lc 4 lw 1"<<endl;
	}
	///
	for (int i = 0; i < vias.size();i++){
		gnuCmd_out << "set arrow from "<<vias[i].center.first<<","<<vias[i].center.second<<" to "<<vias[i].modified_center.first<<","<<vias[i].modified_center.second<<" nohead lc 11 lw 2"<<endl;
	}
	
	for(int i = 0 ; i < vertical_bottom_intervals.size(); i++){
		for (int j = 0; j < vertical_bottom_intervals[i].size();j++){
			gnuCmd_out << "set arrow from "<<x_bottom_line[i]<<","<<vertical_bottom_intervals[i][j].segment.first<<" to "<<x_bottom_line[i]<<","<<vertical_bottom_intervals[i][j].segment.second<<" nohead lc 8 lw 3"<<endl;
		}
	}
	
    gnuData_out.close();
	//gnuCmd_out << "set term png size 1000,1000" <<endl;
	//gnuCmd_out << "set term png size 15000,3000" <<endl;
	gnuCmd_out << "set term png size 40000,3000" <<endl;
    gnuCmd_out << "set output '"<<output<<".png'"<<endl;
    gnuCmd_out << "plot 'data2.txt' using 1:2 with points"<<endl;
    gnuCmd_out.close();
    system("gnuplot gp2");
}

void two_layer::gnuplot_all_pic(const char *output){
	ofstream gnuData_out("data3.txt", ios::out);
    ofstream gnuCmd_out("gp3", ios::out);
	
	for ( int i=0;i<shapes.size();i++){
		for (int j=0;j<shapes[i].vertices.size();j++){
			if (shapes[i].type == 'v'){
				if ( j != shapes[i].vertices.size()-1){
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[j+1].first<<","<<shapes[i].vertices[j+1].second<<" nohead lc 6 lw 2"<<endl;
				}
				else{
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[0].first<<","<<shapes[i].vertices[0].second<<" nohead lc 6 lw 2"<<endl;
				}
			}
			else {
				if ( j != shapes[i].vertices.size()-1){
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[j+1].first<<","<<shapes[i].vertices[j+1].second<<" nohead lc 3 lw 2"<<endl;
				}
				else{
					gnuCmd_out << "set arrow from "<<shapes[i].vertices[j].first<<","<<shapes[i].vertices[j].second<<" to "<<shapes[i].vertices[0].first<<","<<shapes[i].vertices[0].second<<" nohead lc 3 lw 2"<<endl;
				}
			}			
		}
		//cout<<shapes[i].name<<endl;
		gnuData_out<<shapes[i].vertices[0].first<<" "<<shapes[i].vertices[0].second<<endl<<shapes[i].vertices[2].first<<" "<<shapes[i].vertices[2].second<<endl;
		gnuCmd_out<<"set label \""<< shapes[i].name <<"\" at "<<shapes[i].center.first<<","<<shapes[i].center.second<<" front center font \",36\""<<endl;
	}
	
	for (int i=0;i<nets.size();i++){
		gnuCmd_out << "set arrow from "<<shapes[nets[i].Shps.first].center.first<<","<<shapes[nets[i].Shps.first].center.second<<" to "<<shapes[nets[i].Shps.second].center.first<<","<<shapes[nets[i].Shps.second].center.second<<" nohead lc 6 lw 1"<<endl;
	}
	
	for(int i = 0 ; i < horizontal_intervals.size(); i++){
		for (int j = 0; j < horizontal_intervals[i].size();j++){
			gnuCmd_out << "set arrow from "<<horizontal_intervals[i][j].segment.first<<","<<y_line[i]<<" to "<<horizontal_intervals[i][j].segment.second<<","<<y_line[i]<<" nohead lc 10 lw 3"<<endl;
		}
	}	
	
	for(int i = 0 ; i < vertical_top_intervals.size(); i++){
		for (int j = 0; j < vertical_top_intervals[i].size();j++){
			if (vertical_top_intervals[i][j].layer_num == 1){
				gnuCmd_out << "set arrow from "<<x_line[i]<<","<<vertical_top_intervals[i][j].segment.first<<" to "<<x_line[i]<<","<<vertical_top_intervals[i][j].segment.second<<" nohead lc 8 lw 3"<<endl;
			}
			else{
				gnuCmd_out << "set arrow from "<<x_line[i]<<","<<vertical_top_intervals[i][j].segment.first<<" to "<<x_line[i]<<","<<vertical_top_intervals[i][j].segment.second<<" nohead lc 10 lw 3"<<endl;
			}
		}
	}
	
	for(int i = 0 ; i < vertical_bottom_intervals.size(); i++){
		for (int j = 0; j < vertical_bottom_intervals[i].size();j++){
			if (vertical_bottom_intervals[i][j].layer_num == 1){
				gnuCmd_out << "set arrow from "<<x_bottom_line[i]<<","<<vertical_bottom_intervals[i][j].segment.first<<" to "<<x_bottom_line[i]<<","<<vertical_bottom_intervals[i][j].segment.second<<" nohead lc 8 lw 3"<<endl;
			}
			else{
				gnuCmd_out << "set arrow from "<<x_bottom_line[i]<<","<<vertical_bottom_intervals[i][j].segment.first<<" to "<<x_bottom_line[i]<<","<<vertical_bottom_intervals[i][j].segment.second<<" nohead lc 10 lw 3"<<endl;
			}
		}
	}
	
	for (int i = 0; i < vias.size();i++){
		gnuCmd_out << "set arrow from "<<vias[i].center.first<<","<<vias[i].center.second<<" to "<<vias[i].modified_center.first<<","<<vias[i].modified_center.second<<" nohead lc 11 lw 3"<<endl;
	}
	
    gnuData_out.close();
	//gnuCmd_out << "set term png size 1000,1000" <<endl;
	//gnuCmd_out << "set term png size 15000,3000" <<endl;
	gnuCmd_out << "set term png size 40000,3000" <<endl;
    gnuCmd_out << "set output '"<<output<<".png'"<<endl;
    gnuCmd_out << "plot 'data3.txt' using 1:2 with points"<<endl;
    gnuCmd_out.close();
    system("gnuplot gp3");
}
