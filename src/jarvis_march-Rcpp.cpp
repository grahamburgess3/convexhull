#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <stdlib.h> 
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include <Rcpp.h>
using namespace Rcpp;

struct point
{  
    double x;
    double y;
 
    point(double _x,double _y)
    {
        x = _x;
        y = _y;   
    }
};

struct triplet_of_points
{  
    bool right_turn {};
    bool collinearity {};
    double determinent {};
    double dot_product {};
    
    triplet_of_points(point p1, point p2, point p3)  
    {
        point a(p1.x - p2.x, p1.y - p2.y);
        point b(p3.x - p2.x, p3.y - p2.y);
        determinent = a.x * b.y - b.x * a.y;
        dot_product = a.x * b.x + a.y * b.y ; 
    }
    
    void find_orientation()
    {        
        if (determinent > 0) // right turn
        {
            right_turn = true;
            collinearity = false;
        }
        else if (determinent == 0 && dot_product < 0) // straight line, categorised as right turn
        {
            right_turn = true;
            collinearity = true;
        }
        else if (determinent == 0 && dot_product > 0) // back on itselft, cateogorised as left turn
        {
            right_turn = false;
            collinearity = true;
        }
        else if (determinent < 0) // left turn
        {
            right_turn = false;
            collinearity = false;
        }
    }
};

int find_leftmost_point(double leftmost_val, std::vector<point> points)
{    
    double leftmost_val_update {leftmost_val};
    int leftmost_index_update {};
    for(int p = 0; p < points.size(); p++)
    {
        if (points[p].x < leftmost_val_update)
        {
            leftmost_val_update = points[p].x;
            leftmost_index_update = p;            
        }
    }
    return leftmost_index_update;
};

int find_new_point(std::vector<point> points, std::vector<int> exceptions)
{
    bool looking_for_new_point {true};
    int new_point {};
    while (looking_for_new_point == true)
    {
        new_point = rand() % points.size();
        if (std::find(exceptions.begin(), exceptions.end(), new_point) == exceptions.end()) // new point not exception
        {
            looking_for_new_point = false;
        }
    }
    return new_point;
};

std::vector<point> find_convex_hull(std::vector<point> points)
{
    // set up attributes
    std::vector<int> convex_hull {}; // list of indices indicating list-position of the points on the hull
    std::vector<point> convex_hull_points {}; // list of points (from the points class) on the convex hull
    bool all_points_collinear {true}; // change this if we find non-collinear points
    int leftmost_index;
    int rightmost_index;
    double leftmost_val {std::numeric_limits<double>::infinity()};
    
    // First deal with special cases of small sets of points
    if (points.size() == 0){
        std::cout << "No data points to analyse" << std::endl;
    }
    else if (points.size() == 1){
        std::cout << "Only one data point to analyse" << std::endl;
        convex_hull_points.push_back(points[0]);
    }
    else if (points.size() == 2){
        std::cout << "only two data points to analyse" << std::endl;
        // add leftmost point
        leftmost_index = find_leftmost_point(leftmost_val, points);
        convex_hull.push_back(leftmost_index);
        convex_hull_points.push_back(points[leftmost_index]);
        // add rightmost point
        if (leftmost_index == 0)
        {
            rightmost_index = 1;
        }
        else {
            rightmost_index = 0;
        }
        convex_hull.push_back(rightmost_index);
        convex_hull_points.push_back(points[rightmost_index]);
    }
    else {
        leftmost_index = find_leftmost_point(leftmost_val, points); // find leftmost point
        convex_hull.push_back(leftmost_index);
        bool not_complete_hull {true}; // this will change to False once the convex hull reaches its starting point
        
        // main while loop
        while (not_complete_hull == true)
        {
            // identify end of the current hull
            int end_of_hull_index {convex_hull.back()};
            
            // select candidate
            std::vector<int> exceptions {end_of_hull_index};
            int candidate {};
            candidate = find_new_point(points, exceptions);
            
            // test the candidate with test points
            for(int test_point = 0; test_point < points.size(); test_point++)
            {
                if (test_point != candidate && test_point != end_of_hull_index)
                {
                    // create new instance of triplet class from the end of the hull -> test point -> candidate
                    triplet_of_points triplet(points[end_of_hull_index], points[test_point], points[candidate]);
                    triplet.find_orientation();
                    
                    // update candidate if angle theta in the triplet from a -> b counterclockwise satisfies 0 < theta <= 180
                    if (triplet.right_turn == true)
                    {
                        candidate = test_point;
                    }
                    
                    // update candidate
                    if (triplet.collinearity == true && std::find(convex_hull.begin(), convex_hull.end(), candidate) != convex_hull.end() && find(convex_hull.begin()+1, convex_hull.end(), test_point) == convex_hull.end())
                    {
                        candidate = test_point;
                    }
                    
                    // Update instance attribute if a non-collinear triplet has been found. 
                    if (triplet.collinearity == false)
                    {
                        all_points_collinear = false;
                    }
                }
            }
            
            // update hull
            convex_hull.push_back(candidate);

            // is the hull complete?
            if (std::find(convex_hull.begin(), convex_hull.end()-1, convex_hull.back()) != convex_hull.end()-1)
            {
                not_complete_hull = false;
                if (convex_hull.front() == convex_hull.back())
                {
                    convex_hull.pop_back();
                }
            }
        }
            
        // If not all points are collinear, then simply add all point objects the hull, based on their index. 
        if (all_points_collinear == false)
        {
            for(int point_to_add = 0; point_to_add < convex_hull.size(); point_to_add++)
            {
                convex_hull_points.push_back(points[convex_hull[point_to_add]]);
            }
        }

        // If all the points collinear, ignore self.convex_hull and create self.convex_hull_points from scratch
        else
        {
            for (int i = 0; i < convex_hull.size(); i++)
            {
                // add first point
                if (i == 0)
                {
                    convex_hull_points.push_back(points[convex_hull[i]]);
                }

                // go out to the end of the hull
                else if (std::find(convex_hull.begin(), convex_hull.begin()+(i-1), convex_hull[i]) == convex_hull.begin()+(i-1))
                {
                    convex_hull_points.push_back(points[convex_hull[i]]);
                }

                else
                {
                    break;
                }          
            }
        }
    }
    return convex_hull_points;
};

std::list<std::vector<double>> jarvis_march_old(std::vector<double> xy)
{
    // read points
    // std::list<std::vector<double> > xy_pairs {readcsv(filename)};
    std::vector<point> points {};
    for(int i = 0; i < xy.size(); i=i+2)
    {
        point new_point(xy[i],xy[i+1]);
        points.push_back(new_point);
    }

    // find hull
    srand(10);
    std::vector<point> hull_points {};
    hull_points = find_convex_hull(points);
    
    // output
    std::list<std::vector<double>> hull {};
    for(int hull_index = 0; hull_index < hull_points.size(); hull_index++)
    {
	std::vector<double> line_segment {};
	if(hull_points.size() - hull_index > 1)
	{
		line_segment.push_back(hull_points[hull_index].x);
		line_segment.push_back(hull_points[hull_index].y);
		line_segment.push_back(hull_points[hull_index+1].x);
		line_segment.push_back(hull_points[hull_index+1].y);
		hull.push_back(line_segment);
	} else {
		line_segment.push_back(hull_points[hull_index].x);
		line_segment.push_back(hull_points[hull_index].y);
		line_segment.push_back(hull_points[0].x);
		line_segment.push_back(hull_points[0].y);
		hull.push_back(line_segment);
	}
    }
    return hull;
};

//[[Rcpp::export]]
std::vector<std::vector<double>> jarvis_march_cpp(const std::vector<std::vector<double>>& xy)
{
    // read points
    //std::list<std::vector<double> > xy_pairs {readcsv(filename)};
    std::vector<point> points {};
    for(int i = 0; i < xy.size(); i++)
    {
        point new_point(xy[i][0],xy[i][1]);
        points.push_back(new_point);
    }
    
    // find hull
    srand(10);
    std::vector<point> hull_points {};
    hull_points = find_convex_hull(points);
    
    // output
    std::vector<std::vector<double>> hull(0,std::vector<double>(4));
    for(int hull_index = 0; hull_index < hull_points.size(); hull_index++)
    {
	std::vector<double> line_segment {};
	if(hull_points.size() - hull_index > 1)
	{
		line_segment.push_back(hull_points[hull_index].x);
		line_segment.push_back(hull_points[hull_index].y);
		line_segment.push_back(hull_points[hull_index+1].x);
		line_segment.push_back(hull_points[hull_index+1].y);
		hull.push_back(line_segment);
	} else {
		line_segment.push_back(hull_points[hull_index].x);
		line_segment.push_back(hull_points[hull_index].y);
		line_segment.push_back(hull_points[0].x);
		line_segment.push_back(hull_points[0].y);
		hull.push_back(line_segment);
	}
    }
    return hull;
};
