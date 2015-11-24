// Decision.h
//
// Jack Murdock
#ifndef __Decision_H_
#define __Decision_H_


#include <ale_interface.hpp>
#include <iostream>
#include <vector>
#include <sstream>


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
	bool isPointInsideTriangle(Point p, Point one, Point two, Point three){
		bool b1 = sign(p, one, two) < 0.0;
		bool b2 = sign(p,two, three) < 0.0;
		bool b3 = sign(p, three, one) < 0.0;

		return ((b1 == b2) && (b2==b3));
	}

	int nearShip(vector<string> listInts){
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
			cout << "Might Die " << Count << " P1: " << triangleOne << " P2: " << triangleTwo << " P3: " << triangleThree << endl;
		}
		return Count;
	}

	float sign(Point one, Point two, Point three){
		return (one.x() - three.x()) * (two.y() -three.y()) - (two.x() - three.x()) * (one.y() - three.y());
	}
public:
	

	Decision(ActionVect minimal_actions, ALEScreen s) : screen(10, 10){
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
	Action getDecision(ALEScreen screen, int lives){
		//this->screen = screen;
		this->lives = lives;
		shipCenter = centerOfShip(screen);
		slope = calculateSlope();
		defineTrianglePoints(shipCenter, slope.x(), slope.y());


		vector<string> badPoints;
		badPoints.push_back("244");
		badPoints.push_back("94");
		badPoints.push_back("44");

		nearShip(badPoints);


		return minimal_actions[rand() % minimal_actions.size()];
	}

	friend ostream &operator<<(ostream &output, const Decision &D){
		output << endl;
		output << "================================================" <<endl;
		map<string, int> first;
    //output << screen << endl;
		ostringstream os;
		output << "Size: (" << D.screen.width() << ", " << D.screen.height() << ")" << endl;
		output << "Ship: ";
		//Point p = centerOfShip(screen);
		//p.print();
		output << D.shipCenter;
		output << endl;
		output << "Slope: ";
		//Point slope = calculateSlope();
		output << D.slope;
		output << endl;
		//defineTrianglePoints(p, slope.x(), slope.y());
		for (int y = 31; y < D.screen.height(); y++) {
			for (int x = 0; x < D.screen.width(); x++) {
            //output << "Point: (" << x << ", " << y << ")" << endl;
				string thing = to_string(D.screen.get(y, x));
				first[thing] += 1;
				// if (D.shipCenter.x() == x && D.shipCenter.y() == y) {
				// 	output << "&";
				// 	continue;
				// }
				// if (D.triangleOne.x() == x && D.triangleOne.y() == y) {
				// 	output << "$";
				// }
				// if (D.triangleTwo.x() == x && D.triangleTwo.y() == y) {
				// 	output << "$";
				// }
				// if (D.triangleThree.x() == x && D.triangleThree.y() == y) {
				// 	output << "$";
				// }
				if (thing == "0"){
					output << " ";
				} else if (thing == "36"){
					output << "a";
				} else if (thing == "38"){
					output << "%";
            } else if (thing == "44"){      // Land on a Planet
            	output << "%";
            } else if (thing == "54"){      // Turret/Bunker
            	output << "I";
            } else if (thing == "52"){
            	output << "*";
            } else if (thing == "56"){
            	output << "R";
            } else if (thing == "60"){
            	output << "T";
            } else if (thing == "66"){
            	output << "Q";
            } else if (thing == "74"){
            	output << "b";
            } else if (thing == "78"){
            	output << "Y";
            } else if (thing == "86"){
            	output << "X";
            } else if (thing == "92"){
            	output << "c";
            } else if (thing == "94"){
            	output << "~";
            } else if (thing == "116"){
            	output << "P";
            } else if (thing == "118"){     // Fuel
            	output << "E";
            } else if (thing == "120"){
            	output << "F";
            } else if (thing == "122"){     // Planet in Upper Left Hand Corner
            	output << "d";
            } else if (thing == "136"){
            	output << "e";
            } else if (thing == "138"){
            	output << "f";
            } else if (thing =="140"){
            	output << "g";
            } else if (thing == "156"){     // Starting Circle
            	output << "h";
            } else if (thing == "166"){
            	output << "S";
            } else if (thing == "170"){     // Ship Doing Nothing
            	output << "|";
            } else if (thing == "175"){     // Ship With Shiled On
            	output << "@";
            } else if (thing == "188"){
            	output << "j";
            } else if (thing == "194"){
            	output << "Z";
            } else if (thing == "196"){
            	output << "]";
			}  else if (thing == "198"){
				output << "W";
            }  else if (thing == "216"){    // Planet in Left Upper Corner
            	output << "k";
            } else if (thing == "244"){
            	output << "O";
            }  else if (thing == "250"){
            	output << "^";
            } else{
            	output << "U";
            }
            os << D.screen.get(y, x);
            string str = os.str(); // str is what you want.
            output << str << " ";
            
        }
        output << endl;
    }
    output << "================================================" <<endl;
    output << endl;
    
    for(auto elem : first)
    {
    	output << elem.first << " " << elem.second << "\n";
    }
    return output;
}

};

#endif