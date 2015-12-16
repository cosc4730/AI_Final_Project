// Decision.h
//
// Jack Murdock
#ifndef __Decision_H_
#define __Decision_H_


#include <ale_interface.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <SDL.h>


#include "Point.h"

using namespace ale;
using namespace std;
   // Two Turns to make 45 degrees
#ifndef to_string
template <typename T>
    std::string to_string(T value)
    {
      //create an output string stream
      std::ostringstream os ;

      //throw the value into the string stream
      os << value ;

      //convert the string stream into a string and return
      return os.str() ;
    }
#endif

class Decision{
private:
	ALEScreen screen;
	Point shipCenter;
	int lives;
	int countTurns = 0;
	int actionTurns = 0;
	int timeToUpdatePoints = 5;
	int timePoints = 0;
	int direction; // 0 -> Up, 4 -> Right, 8 -> Down, 12 -> Left,
	bool flag_top_left, flag_top, flag_top_right, flag_right, flag_bottom_right, flag_bottom, flag_bottom_left, flag_left = false;
	bool flag_top_left_R, flag_top_R, flag_top_right_R, flag_right_R, flag_bottom_right_R, flag_bottom_R, flag_bottom_left_R, flag_left_R = false;
	bool flag_front_All, flag_right_All, flag_back_All, flag_left_All = false;
	bool flag_front_Any, flag_right_Any, flag_back_Any, flag_left_Any = false;
	bool flag_north, flag_east, flag_south, flag_west;
	ActionVect minimal_actions;
	ActionVect no_thrust_actions;

	Action noop, shoot, thrust, turnRight, turnLeft, thrustLeft, shieldRight, shieldLeft, shootThrust,shieldTractor,thrustRight, shootRight, shootLeft, shootShield, shootThrustRight, shootThrustLeft, shootShieldRight, shootShieldLeft;
	Point lastOne, lastTwo, lastThree, lastFour;
	Point triangleOne, triangleTwo, triangleThree;
	Point slope;
	vector<string> badPoints;

	void updatePoints(Point p){
		if (timePoints == timeToUpdatePoints){
			lastFour = lastThree;
			lastThree = lastTwo;
			lastTwo = lastOne;
			lastOne = p;
			timePoints = 0;
		}else{
			timePoints++;
		}

	}
	Point calculateSlope(){
		float x = 0;
		float y = 0;
		if(lastFour.x() > 0 && lastFour.y() > 0) {
			x = lastOne.x() - lastFour.x();
			y = lastOne.y() - lastFour.y();
		}
		return Point(x,y);
	}
	Point centerOfShip(ALEScreen screen){
		int count = 0;
		for (int y = 31; y < screen.height(); y++) {
			for (int x = 0; x < screen.width(); x++) {
				string thing = to_string(screen.get(y, x));
				if (thing == "170" || thing == "175" || thing == "166"){
					for (int dx = -1; dx < 2; dx++) {
						for (int dy= -1; dy < 2; dy++) {
							if (y+dy > 0 && y+dy < screen.height() && x+dx > 0 && x+dx < screen.width()) {

								string other = to_string(screen.get(y+dy, x+dx));
								if (other == "170" || other == "175" || other == "166"){
									count++;
								}
							}
						}
					}
					if (count >= 5) {
						return Point(x, y);
					}else{
						count = 0;
					}
				}

			}
		}
		return Point();
	//    if (xCount == 0 || yCount == 0 ){
	//        return Point(-1, -1);
	//    }else{
	////        cout << xTotal << "  " << yTotal << " "<< xCount <<" " << yCount<< endl;
	////        cout << (int) floor(xTotal/xCount) << " " << (int) floor(yTotal/yCount);
	//        return Point((int) floor(xTotal/xCount), (int) floor(yTotal/yCount));
	//    }

	}
	void defineTrianglePoints(Point ship, int xSlope, int ySlope){

		Point first = ship;
		Point front = Point(ship.x() + 2*xSlope, ship.y() + 2*ySlope);
		Point second = Point(front.x() + (xSlope + 10), front.y() - ySlope);
		Point third = Point(front.x() - xSlope, front.y() + ySlope);

		triangleOne = front;
		triangleTwo = second;
		triangleThree = third;
	}
	float sign(Point one, Point two, Point three){
		return (one.x() - three.x()) * (two.y() -three.y()) - (two.x() - three.x()) * (one.y() - three.y());
	}

	bool isPointInsideTriangle(Point p, Point one, Point two, Point three){
		bool b1 = sign(p, one, two) < 0.0;
		bool b2 = sign(p,two, three) < 0.0;
		bool b3 = sign(p, three, one) < 0.0;

		return ((b1 == b2) && (b2==b3));
	}

	void setFlags(vector<string> listInts, Point center, int size, int threshold){
		int topLeftCount = 0;
		int topCount = 0;
		int topRightCount = 0;
		int leftCount = 0;
		int rightCount = 0;
		int bottomLeftCount = 0;
		int bottomCount = 0;
		int bottomRightCount = 0;

		// This might look weird but (0, 0) is the top left
		int leftBoarder = center.x() - (size/3);
		int rightBoarder = center.x() + (size/3);
		int topBoarder = center.y() - (size/3);
		int bottomBoarder = center.y() + (size/3);

		for(int x= center.x()-size; x < center.x()+size;x++){
			if (x < 0 || x >= screen.width()){
				continue;
			}
			for (int y = center.y()-size; y < center.y() + size; y++){
				if ( y < 31 || y >= screen.height()){
					continue;
				}
				string screenPoint = to_string(screen.get(y, x));
				if (!isShip(screenPoint) && screenPoint != "0"){
					if(x <= leftBoarder && y <= topBoarder){  // Top Left
						topLeftCount++;
					} else if (x >= rightBoarder && y <= topBoarder){ // Top Right
						topRightCount++;
					} else if (x >= rightBoarder && y >= bottomBoarder){  // Bottom Right
						bottomRightCount++;
					} else if (x <= leftBoarder && y >= bottomBoarder){  // Bottom Left
						bottomLeftCount++;
					} else if (x >= leftBoarder && x <= rightBoarder && y <= topBoarder){ // Top
						topCount++;
					} else if (x >= rightBoarder && y >= topBoarder && y <= bottomBoarder){ // Right
						rightCount++;
					} else if (x >= leftBoarder && x <= rightBoarder && y >= bottomBoarder){ // Bottom
						bottomCount++;
					} else if (x <= leftBoarder && y >= topBoarder && y <= bottomBoarder){ // Left
						leftCount++;
					}

				}
			}
		}
		if (topLeftCount >= threshold){
			flag_top_left = true;
		} else {
			flag_top_left = false;
		}

		if (topCount >= threshold){
			flag_top = true;
		} else {
			flag_top = false;
		}

		if (topRightCount >= threshold){
			flag_top_right = true;
		} else {
			flag_top_right = false;
		}

		if (rightCount >= threshold){
			flag_right = true;
		} else {
			flag_right = false;
		}

		if (bottomRightCount >= threshold){
			flag_bottom_right = true;
		} else {
			flag_bottom_right = false;
		}

		if (bottomCount >= threshold){
			flag_bottom = true;
		} else {
			flag_bottom = false;
		}

		if (bottomLeftCount >= threshold){
			flag_bottom_left = true;
		} else {
			flag_bottom_left = false;
		}

		if (leftCount >= threshold){
			flag_left = true;
		} else {
			flag_left = false;
		}
		// cout << endl;
		// cout << topLeftCount << " " << topCount << " " << topRightCount << endl;
		// cout << leftCount << " # " << rightCount << endl;
		// cout << bottomLeftCount << " " << bottomCount << " " << bottomRightCount << endl;
	}

	bool isShip(string character){
		return (character == "170" || character == "175" || character == "166");
	}


	void setDirectionFlags(){
		// The full direction looks like the followig
		//
		//  %
		//  %
		//  %
		//  %
		//  % <- Ship Center
		// %%%
		// %%%

		//thing == "170" || thing == "175" || thing == "166"

		int x = shipCenter.x();
		int y = shipCenter.y();
		if (x < 4 || x > screen.width() - 4){
			return;
		}
		if (y < 4 || y > screen.height() - 4){
			return;
		}


		string one = to_string(screen.get(y - 4, x));
		string two = to_string(screen.get(y - 3, x));
		string three = to_string(screen.get(y - 2, x));
		string four = to_string(screen.get(y - 1, x));
		string five = to_string(screen.get(y + 1, x - 1));
		string six = to_string(screen.get(y + 1, x));
		string seven = to_string(screen.get(y + 1, x + 1));
		string eight = to_string(screen.get(y + 2, x - 1));
		string nine = to_string(screen.get(y + 2, x));
		string ten = to_string(screen.get(y + 2, x + 1));
		string eleven = to_string(screen.get(y, x));
		if (isShip(one) && isShip(two) &&isShip(three) &&isShip(four) &&isShip(five) &&isShip(six) &&isShip(seven) &&isShip(eight) &&isShip(nine) &&isShip(ten)){ // North
				flag_north = true;
				flag_south = false;
				flag_west = false;
				flag_east = false;
		}

				// EASt  ** TODO **
		one = to_string(screen.get(y - 2, x));
		two = to_string(screen.get(y - 1, x));
		three = to_string(screen.get(y, x));
		four = to_string(screen.get(y + 1, x));
		five = to_string(screen.get(y + 2, x));
		six = to_string(screen.get(y + 3, x));
		seven = to_string(screen.get(y + 1, x + 1));
		eight = to_string(screen.get(y, x + 2));
		nine = to_string(screen.get(y + 1, x + 2));
		ten = to_string(screen.get(y, x + 3));
		eleven = to_string(screen.get(y + 1, x + 3));
		if(isShip(one) && isShip(two) &&isShip(three) &&isShip(four) &&isShip(five) &&isShip(six) &&isShip(seven) &&isShip(eight) &&isShip(nine) &&isShip(ten) &&isShip(eleven) ){
				flag_north = false;
				flag_south = false;
				flag_west = false;
				flag_east = true;
		}


				// South TODO
		one = to_string(screen.get(y, x - 1));
		two = to_string(screen.get(y, x + 1));
		three = to_string(screen.get(y + 1, x - 1));
		four = to_string(screen.get(y + 1, x));
		five = to_string(screen.get(y + 1, x + 1));
		six = to_string(screen.get(y + 2, x));
		seven = to_string(screen.get(y + 3, x));
		eight = to_string(screen.get(y + 4, x));
		nine = to_string(screen.get(y + 5, x));
		ten = to_string(screen.get(y, x));
		eleven = to_string(screen.get(y + 1, x));
		if(isShip(one) && isShip(two) &&isShip(three) &&isShip(four) &&isShip(five) &&isShip(six) &&isShip(seven) &&isShip(eight) &&isShip(nine) &&isShip(ten)){
				flag_north = false;
				flag_south = true;
				flag_west = false;
				flag_east = false;
		}

				// West
		one = to_string(screen.get(y - 2, x + 2));
		two = to_string(screen.get(y - 1, x + 2));
		three = to_string(screen.get(y, x + 2));
		four = to_string(screen.get(y + 1, x + 2));
		five = to_string(screen.get(y + 2, x + 2));
		six = to_string(screen.get(y + 3, x + 2));
		seven = to_string(screen.get(y, x + 1));
		eight = to_string(screen.get(y + 1, x + 1));
		nine = to_string(screen.get(y + 1, x));
		ten = to_string(screen.get(y, x - 1));
		eleven = to_string(screen.get(y + 1, x - 1));
		if(isShip(one) && isShip(two) &&isShip(three) &&isShip(four) &&isShip(five) &&isShip(six) &&isShip(seven) &&isShip(eight) &&isShip(nine) &&isShip(ten) &&isShip(eleven) ){
				flag_north = false;
				flag_south = false;
				flag_west = true;
				flag_east = false;
		}

	}

	void convertFlagsToRelative(){
		if (actionTurns == 0){  // Cardinal Direction
			if (flag_north){
				flag_top_R = flag_top;
				flag_top_right_R = flag_top_right;
				flag_right_R = flag_right;
				flag_bottom_right_R = flag_bottom_right;
				flag_bottom_R = flag_bottom;
				flag_bottom_left_R = flag_bottom_left;
				flag_left_R = flag_left;
				flag_top_left_R = flag_top_left;

			} else if (flag_east){
				flag_top_R = flag_right;
				flag_top_right_R = flag_bottom_right;
				flag_right_R = flag_bottom;
				flag_bottom_right_R = flag_bottom_left;
				flag_bottom_R = flag_left;
				flag_bottom_left_R = flag_top_left;
				flag_left_R = flag_top;
				flag_top_left_R = flag_top_right;

			} else if (flag_south){
				flag_top_R = flag_bottom;
				flag_top_right_R = flag_bottom_left;
				flag_right_R = flag_left;
				flag_bottom_right_R = flag_top_left;
				flag_bottom_R = flag_top;
				flag_bottom_left_R = flag_top_right;
				flag_left_R = flag_right;
				flag_top_left_R = flag_bottom_right;
			} else if (flag_west){
				flag_top_R = flag_left;
				flag_top_right_R = flag_top_left;
				flag_right_R = flag_top;
				flag_bottom_right_R = flag_top_right;
				flag_bottom_R = flag_right;
				flag_bottom_left_R = flag_bottom_right;
				flag_left_R = flag_bottom;
				flag_top_left_R = flag_bottom_left;
			}
		} else if (actionTurns > 0){ // Toward Right
			if (flag_north){ // NE
				flag_top_R = flag_top_right;
				flag_top_right_R = flag_right;
				flag_right_R = flag_bottom_right;
				flag_bottom_right_R = flag_bottom;
				flag_bottom_R = flag_bottom_left;
				flag_bottom_left_R = flag_left;
				flag_left_R = flag_top_left;
				flag_top_left_R = flag_top;

			} else if (flag_east){ // SE
				flag_top_R = flag_bottom_right;
				flag_top_right_R = flag_bottom;
				flag_right_R = flag_bottom_left;
				flag_bottom_right_R = flag_left;
				flag_bottom_R = flag_top_left;
				flag_bottom_left_R = flag_top;
				flag_left_R = flag_top_right;
				flag_top_left_R = flag_right;

			} else if (flag_south){ // SW
				flag_top_R = flag_bottom_left;
				flag_top_right_R = flag_left;
				flag_right_R = flag_top_left;
				flag_bottom_right_R = flag_top;
				flag_bottom_R = flag_top_right;
				flag_bottom_left_R = flag_right;
				flag_left_R = flag_bottom_right;
				flag_top_left_R = flag_bottom;
			} else if (flag_west){ //NW
				flag_top_R = flag_top_left;
				flag_top_right_R = flag_top;
				flag_right_R = flag_top_right;
				flag_bottom_right_R = flag_right;
				flag_bottom_R = flag_bottom_right;
				flag_bottom_left_R = flag_bottom;
				flag_left_R = flag_bottom_left;
				flag_top_left_R = flag_left;
			}
		} else if (actionTurns < 0){ // Toward Left
			if (flag_north){ // NW
				flag_top_R = flag_top_left;
				flag_top_right_R = flag_top;
				flag_right_R = flag_top_right;
				flag_bottom_right_R = flag_right;
				flag_bottom_R = flag_bottom_right;
				flag_bottom_left_R = flag_bottom;
				flag_left_R = flag_bottom_left;
				flag_top_left_R = flag_left;

			} else if (flag_east){ // NE
				flag_top_R = flag_top_right;
				flag_top_right_R = flag_right;
				flag_right_R = flag_bottom_right;
				flag_bottom_right_R = flag_bottom;
				flag_bottom_R = flag_bottom_left;
				flag_bottom_left_R = flag_left;
				flag_left_R = flag_top_left;
				flag_top_left_R = flag_top;

			} else if (flag_south){ // SE
				flag_top_R = flag_bottom_right;
				flag_top_right_R = flag_bottom;
				flag_right_R = flag_bottom_left;
				flag_bottom_right_R = flag_left;
				flag_bottom_R = flag_top_left;
				flag_bottom_left_R = flag_top;
				flag_left_R = flag_top_right;
				flag_top_left_R = flag_right;
			} else if (flag_west){ // SW
				flag_top_R = flag_bottom_left;
				flag_top_right_R = flag_left;
				flag_right_R = flag_top_left;
				flag_bottom_right_R = flag_top;
				flag_bottom_R = flag_top_right;
				flag_bottom_left_R = flag_right;
				flag_left_R = flag_bottom_right;
				flag_top_left_R = flag_bottom;
			}
		}

		if (flag_top_left_R	&& flag_top_R && flag_top_right_R){
			flag_front_All = true;
		} else{
			flag_front_All = false;
		}

		if (flag_top_right_R && flag_right_R && flag_bottom_right_R){
			flag_right_All = true;
		} else{
			flag_right_All = false;
		}

		if (flag_bottom_right_R	&& flag_bottom_R && flag_bottom_left_R){
			flag_back_All = true;
		} else{
			flag_back_All = false;
		}

		if (flag_top_left_R	&& flag_left_R && flag_bottom_left_R){
			flag_left_All = true;
		} else{
			flag_left_All = false;
		}

		if (flag_top_left_R	|| flag_top_R || flag_top_right_R){
			flag_front_Any = true;
		} else{
			flag_front_Any = false;
		}

		if (flag_top_right_R || flag_right_R || flag_bottom_right_R){
			flag_right_Any = true;
		} else{
			flag_right_Any = false;
		}

		if (flag_bottom_right_R	|| flag_bottom_R || flag_bottom_left_R){
			flag_back_Any = true;
		} else{
			flag_back_Any = false;
		}

		if (flag_top_left_R	|| flag_left_R || flag_bottom_left_R){
			flag_left_Any = true;
		} else{
			flag_left_Any = false;
		}
	}

	void printFlags(){
		cout << flag_top_left << " " << flag_top << " " << flag_top_right << endl;
		cout << flag_left << " X " << flag_right << endl;
		cout << flag_bottom_left << " " << flag_bottom << " " << flag_bottom_right << endl;
	}

	int nearShip(vector<string> listInts){
		//cout << "Slope: " << slope << " P1: " << triangleOne << " P2: " << triangleTwo << " P3: " << triangleThree << endl;
		if (triangleOne.x() == triangleTwo.x() && triangleOne.y() == triangleTwo.y()) {
			return 0;
		}
		if (triangleTwo.x() == triangleThree.x() && triangleTwo.y() == triangleThree.y()) {
			return 0;
		}
		if (triangleThree.x() == triangleOne.x() && triangleThree.y() == triangleOne.y()) {
			return 0;
		}




		int Count = 0;
		for (int y = 31; y < screen.height(); y++) {
			for (int x = 0; x < screen.width(); x++) {
				string thing = to_string(screen.get(y, x));
				if (find(listInts.begin(), listInts.end(), thing) != listInts.end()) {
					Point p = Point(x, y);
					if (isPointInsideTriangle(p, triangleOne, triangleTwo, triangleThree)) {
						Count++;

					}
				}
			}
		}
		if (Count > 1) {
			//cout << "Might Die " << Count << " P1: " << triangleOne << " P2: " << triangleTwo << " P3: " << triangleThree << endl;
		}
		//cout << "nope " << Count;
		return Count;
	}


	void moveDirection(Action action){
		if (action == turnLeft || action == thrustLeft || action == shootLeft || action == shootThrustLeft || action == shootShieldLeft){
			actionTurns--;
		} else if (action == turnRight || action == thrustRight || action == shootRight || action == shootThrustRight || action == shootShieldRight){
			actionTurns++;
		}

		bool n = flag_north;
		bool e = flag_east;
		bool s = flag_south;
		bool w = flag_west;
		setDirectionFlags();

		if (n != flag_north || e != flag_east || s != flag_south || w != flag_west){
			resetTurns();
		}
	}

	void resetTurns(){
		actionTurns = 0;
	}

	void printDirection(){
		if (flag_north){
			cout << "North T:" << actionTurns << endl;
		} else if (flag_east){
			cout << "East T:" << actionTurns << endl;
		} else if (flag_south){
			cout << "South T:" << actionTurns << endl;
		} else if (flag_west){
			cout << "West T:" << actionTurns <<endl;
		}
	}
	// cout << slope << endl;
	// cout << " " << flag_north << " " << endl;
	// cout << flag_west << " " << flag_east << endl;
	// cout << " " << flag_south << " " << endl << endl;
	int number = 0;

public:


	Decision(ActionVect minimal_actions, ALEScreen s):screen(s){
		shipCenter = Point(-1, -1);
		screen = s;

		this->minimal_actions = minimal_actions;
		noop = minimal_actions[0];
		shoot = minimal_actions[1];
		thrust = minimal_actions[2];
		turnRight = minimal_actions[3];
		turnLeft = minimal_actions[4];
		shieldTractor = minimal_actions[50];
		thrustRight = minimal_actions[6];
		thrustLeft = minimal_actions[7];
		shieldRight = minimal_actions[8];
		shieldLeft = minimal_actions[9];
		shootThrust = minimal_actions[10];
		shootRight = minimal_actions[11];
		shootLeft = minimal_actions[12];
		shootShield = minimal_actions[13];
		shootThrustRight = minimal_actions[14];
		shootThrustLeft = minimal_actions[15];
		shootShieldRight = minimal_actions[16];
		shootShieldLeft = minimal_actions[17];

		no_thrust_actions.push_back(shoot);
		no_thrust_actions.push_back(turnLeft);
		no_thrust_actions.push_back(thrustRight);
		no_thrust_actions.push_back(shieldTractor);
		no_thrust_actions.push_back(shieldLeft);
		no_thrust_actions.push_back(shieldRight);
		no_thrust_actions.push_back(shootLeft);
		no_thrust_actions.push_back(shootRight);
		no_thrust_actions.push_back(shootShield);
		no_thrust_actions.push_back(shootShieldLeft);
		no_thrust_actions.push_back(shootShieldRight);

		badPoints.push_back("244");
		badPoints.push_back("94");
		badPoints.push_back("44");
		badPoints.push_back("54");
		badPoints.push_back("38");
		//badPoints.push_back("122");

		badPoints.push_back("36"); // bad planet left and bad guys
		badPoints.push_back("92"); // bottom right planet
		badPoints.push_back("188"); // bottom left planet
		badPoints.push_back("216"); // top left Planet

		badPoints.push_back("184"); // Somtimes the black hole
		badPoints.push_back("186"); // Somtimes the black hole
		badPoints.push_back("190"); // Somtimes the black hole
		badPoints.push_back("192"); // Somtimes the black hole
		badPoints.push_back("194"); // Somtimes the black hole
		badPoints.push_back("196"); // Somtimes the black hole
		badPoints.push_back("198"); // Somtimes the black hole
		badPoints.push_back("200");

	}
	Action getDecision(ALEScreen screen, int lives, bool reset){
		this->screen = screen;
		this->lives = lives;
		shipCenter = centerOfShip(screen);
		updatePoints(shipCenter);
		slope = calculateSlope();
		defineTrianglePoints(shipCenter, slope.x(), slope.y());

		if (reset){
			resetTurns();
		}

		setFlags(badPoints, shipCenter, 30, 6);
		convertFlagsToRelative();

		//cout << "=====" << endl;
		//printFlags();

		Action action = minimal_actions[rand() % no_thrust_actions.size()];//= avoidShit();
		if (action == noop){
			action = no_thrust_actions[rand() % no_thrust_actions.size()];
		}

	   // if(number < 10){
	   //     action = noop;
	   //     number++;
	   //    // cout << "NOOP: " << number << " " << to_string(noop) << endl;
	   // } else {
	   //     action = turnRight;
	   //     if (countTurns < 2){
	   //     	countTurns++;
	   //     }else{
	   //     	countTurns = 0;
	   //     	number = 0;
	   //     }
	   //     // cout << "Turn " << count << " " << to_string(turnRight) << endl;
	   // }

		moveDirection(action);
		setDirectionFlags();

		//print();
		//printDirection();
		// cout << action << endl;

		return action;
	}

	Action avoidShit(){
		// if (slope_R )
		if(flag_front_All){
			return turnRight;
		}

		if (flag_left_All || (flag_bottom && flag_left_Any)){
			return thrustRight;
		}

		if (flag_right_All || (flag_bottom && flag_right_Any)){
			return thrustLeft;
		}



		if (flag_back_All || flag_bottom){
			return thrust;
		}

		if (flag_top_R){
			return turnRight;
		} else if (flag_top_left_R){
			return thrustRight;
		} else if (flag_top_right_R){
			return thrustLeft;
		} else if (flag_left_R){
			return thrustRight;
		} else if (flag_right_R){
			return thrustLeft;
		} else if (flag_bottom_left_R){
			return thrustRight;
		} else if (flag_bottom_right_R){
			return thrustLeft;
		}
		if ( abs(slope.x()) < 2 && abs(slope.y())< 2){
			return shootThrustRight;
		}
		return noop;
	}

	void print(){
		cout << endl;
		cout << "================================================" <<endl;
		map<string, int> first;
		//cout << screen << endl;
		ostringstream os;
		cout << "Size: (" << screen.width() << ", " << screen.height() << ")" << endl;
		cout << "Ship: ";
		//Point p = centerOfShip(screen);
		//p.print();
		cout << shipCenter;
		cout << endl;
		cout << "Slope: ";
		//Point slope = calculateSlope();
		cout << slope;
		cout << endl;
		//defineTrianglePoints(p, slope.x(), slope.y());
		for (int y = 31; y < screen.height(); y++) {
			for (int x = 0; x < screen.width(); x++) {
			//cout << "Point: (" << x << ", " << y << ")" << endl;
				string thing = to_string(screen.get(y, x));
				first[thing] += 1;
				Point p = shipCenter;
				if (p.x() == x && p.y() == y) {
					cout << "&";
					continue;
				}
				p = triangleOne;
				if (p.x() == x && p.y() == y) {
					cout << "$";
					continue;
				}
				p = triangleTwo;

				if (p.x() == x && p.y() == y) {
					cout << "$";
					continue;
				}
				p = triangleThree;

				if (p.x() == x && p.y() == y) {
					cout << "$";
					continue;
				}

				if (thing == "0"){
					cout << " ";
				} else if (thing == "36"){
					cout << "a";
				} else if (thing == "38"){
					cout << "%";
				} else if (thing == "44"){      // Land on a Planet
					cout << "%";
				} else if (thing == "54"){      // Turret/Bunker
					cout << "I";
				} else if (thing == "52"){
					cout << "*";
				} else if (thing == "56"){
					cout << "R";
				} else if (thing == "60"){
					cout << "T";
				} else if (thing == "66"){
					cout << "Q";
				} else if (thing == "74"){
					cout << "b";
				} else if (thing == "78"){
					cout << "Y";
				} else if (thing == "86"){
					cout << "X";
				} else if (thing == "92"){
					cout << "c";
				} else if (thing == "94"){
					cout << "~";
				} else if (thing == "116"){
					cout << "P";
				} else if (thing == "118"){     // Fuel
					cout << "E";
				} else if (thing == "120"){
					cout << "F";
				} else if (thing == "122"){     // Planet in Upper Right Hand Corner
					cout << "d";
				} else if (thing == "136"){
					cout << "e";
				} else if (thing == "138"){
					cout << "f";
				} else if (thing =="140"){
					cout << "g";
				} else if (thing == "156"){     // Starting Circle
					cout << "h";
				} else if (thing == "166"){
					cout << "S";
				} else if (thing == "170"){     // Ship Doing Nothing
					cout << "|";
				} else if (thing == "175"){     // Ship With Shiled On
					cout << "@";
				} else if (thing == "188"){
					cout << "j";
				} else if (thing == "194"){
					cout << "Z";
				} else if (thing == "196"){
					cout << "]";
				}  else if (thing == "198"){
					cout << "W";
				}  else if (thing == "216"){    // Planet in Left Upper Corner
					cout << "k";
				} else if (thing == "244"){
					cout << "O";
				}  else if (thing == "250"){
					cout << "^";
				} else{
					cout << "U";
				}
				// os << screen.get(y, x);
				// string str = os.str(); // str is what you want.
				// cout << str << " ";

			}
			cout << endl;
		}
		cout << "================================================" <<endl;
		cout << endl;

		// for(auto elem : first)
		//{
		//	cout << elem.first << " " << elem.second << "\n";
		//}
	}

};

#endif
