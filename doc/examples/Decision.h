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

#include <SDL.h>


#include "Point.h"



using namespace ale;
using namespace std;
   // Two Turns to make 45 degrees
class Decision{
private:
	ALEScreen screen;
	Point shipCenter;
	int lives;
	int direction; // 0 -> Up, 4 -> Right, 8 -> Down, 12 -> Left, 
	ActionVect minimal_actions;

	Action noop, shoot, thrust, turnRight, turnLeft, thrustLeft, shieldRight, shieldLeft, shootThrust,shieldTractor,thrustRight, shootRight, shootLeft, shootShield, shootThrustRight, shootThrustLeft, shootShieldRight, shootShieldLeft;
	Point lastOne, lastTwo, lastThree, lastFour;
	Point triangleOne, triangleTwo, triangleThree;
	Point slope;

	void updatePoints(Point p){
		lastFour = lastThree;
		lastThree = lastTwo;
		lastTwo = lastOne;
		lastOne = p;
	}
	Point calculateSlope(){
		int x = 0;
		int y = 0;
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
		Point second = Point(front.x() + xSlope, front.y() - ySlope);
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

	void updateDirectionRight(){
		if (direction == 15){
			direction = 0;
		} else{
			direction++;
		}
	}

	void updateDirectionLeft(){
		if (direction == 0){
			direction = 15;
		} else{
			direction--;
		}
	}

	void moveDirection(Action action){
		if (action == turnLeft || action == thrustLeft || action == shootLeft || action == shootThrustLeft || action == shootShieldLeft){
			updateDirectionLeft();
		} else if (action == turnRight || action == thrustRight || action == shootRight || action == shootThrustRight || action == shootShieldRight){
			updateDirectionRight();
		}
	}

	void resetDirection(){
		direction = 0;
	}

	void printDirection(){
		if (direction < 2 || direction >= 14){
			cout << "North " << direction << endl;
		} else if (direction >= 2 && direction < 6){
			cout << "East " << direction << endl;
		}else if (direction >= 6 && direction < 10){
			cout << "South " << direction << endl;
		}else if (direction >= 10 && direction < 14){
			cout << "West " << direction << endl;
		}
	}

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
	}
	Action getDecision(ALEScreen screen, int lives, bool reset){
		this->screen = screen;
		this->lives = lives;
		shipCenter = centerOfShip(screen);
		updatePoints(shipCenter);
		slope = calculateSlope();
		defineTrianglePoints(shipCenter, slope.x(), slope.y());

		if (reset){
			resetDirection();
		}


		vector<string> badPoints;
		badPoints.push_back("244");
		badPoints.push_back("94");
		badPoints.push_back("44");
		badPoints.push_back("54");

		nearShip(badPoints);

		Action action; //= minimal_actions[rand() % minimal_actions.size()];

      
       if(number < 10){
           action = noop;
           number++;
          // cout << "NOOP: " << number << " " << to_string(noop) << endl;
       } else {
           action = turnRight;
           number = 0;
           // cout << "Turn " << count << " " << to_string(turnRight) << endl;
       }

		moveDirection(action);
		printDirection();


		return action;
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
            } else if (thing == "122"){     // Planet in Upper Left Hand Corner
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
    
    for(auto elem : first)
    {
    	cout << elem.first << " " << elem.second << "\n";
    }
}

};

#endif