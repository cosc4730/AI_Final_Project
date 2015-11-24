// // Decision.cpp
// //
// // Jack Murdock

// #include "Decision.h"

// Decision::Decision(){
// 	shipCenter = Point(-1, -1);
// 	screen = NULL;
// }

// Decision::Decision(ActionVect minimal_actions)


// Action Decision::getDecision(ALEScreen &screen, int lives){
// 	this.screen = screen;
// 	this.screen = lives;

// 	return minimal_actions[rand() % minimal_actions.size()];
// }

// float Decision::sign(Point one, Point two, Point three){
//     return (one.x() - three.x()) * (two.y() -three.y()) - (two.x() - three.x()) * (one.y() - three.y());
// }
// bool Decision::isPointInsideTriangle(Point p, Point one, Point two, Point three){
//     bool b1 = sign(p, one, two) < 0.0;
//     bool b2 = sign(p,two, three) < 0.0;
//     bool b3 = sign(p, three, one) < 0.0;
    
//     return ((b1 == b2) && (b2==b3));
// }

// int Decision::nearShip(vector<string> listInts){
//     if (triangleOne.x() == triangleTwo.x() && triangleTwo.x() == triangleThree.x()) {
//         if (triangleOne.y() == triangleTwo.y() && triangleTwo.y() == triangleThree.y()){
//             return 0;
//         }
//     }
//     int Count = 0;
//     for (int y = 31; y < screen.height(); y++) {
//         for (int x = 0; x < screen.width(); x++) {
//             string thing = to_string(screen.get(y, x));
//             if (find(listInts.begin(), listInts.end(), thing) != listInts.end()) {
//                 Point p = Point(x, y);
//                 if (isPointInsideTriangle(p, triangleOne, triangleTwo, triangleThree)) {
//                     Count++;

//                 }
//             }
//         }
//     }
//     if (Count > 1) {
//         cout << "Might Die " << Count << " P1: ";
//         triangleOne.print();
//         cout << " P2: ";
//         triangleTwo.print();
//         cout << " P3: ";
//         triangleThree.print();
//         cout << endl;
//     }
//     return Count;
// }

// Point Decision::centerOfShip(ALEScreen screen){
//     int count = 0;
//     for (int y = 31; y < screen.height(); y++) {
//         for (int x = 0; x < screen.width(); x++) {
//             string thing = to_string(screen.get(y, x));
//             if (thing == "170" || thing == "175" || thing == "166"){
//                 for (int dx = -1; dx < 2; dx++) {
//                     for (int dy= -1; dy < 2; dy++) {
//                         if (y+dy > 0 && y+dy < screen.height() && x+dx > 0 && x+dx < screen.width()) {
                            
//                             string other = to_string(screen.get(y+dy, x+dx));
//                             if (other == "170" || other == "175" || other == "166"){
//                                 count++;
//                             }
//                         }
//                     }
//                 }
//                 if (count >= 5) {
//                     return Point(x, y);
//                 }else{
//                     count = 0;
//                 }
//             }
            
//         }
//     }
//     return Point();
//     //    if (xCount == 0 || yCount == 0 ){
//     //        return Point(-1, -1);
//     //    }else{
//     ////        cout << xTotal << "  " << yTotal << " "<< xCount <<" " << yCount<< endl;
//     ////        cout << (int) floor(xTotal/xCount) << " " << (int) floor(yTotal/yCount);
//     //        return Point((int) floor(xTotal/xCount), (int) floor(yTotal/yCount));
//     //    }
    
// }

// void Decision::updatePoints(Point p){
//     lastFour = lastThree;
//     lastThree = lastTwo;
//     lastTwo = lastOne;
//     lastOne = p;
// }

// Point Decision::calculateSlope(){
//     int x = 0;
//     int y = 0;
//     if(lastFour.x() > 0 && lastFour.y() > 0) {
//         x = lastOne.x() - lastFour.x();
//         y = lastOne.y() - lastFour.y();
//     }
//     return Point(x,y);
// }

// void Decision::defineTrianglePoints(Point ship, int xSlope, int ySlope){
//     Point first = ship;
//     Point front = Point(ship.x() + 2*xSlope, ship.y() + 2*ySlope);
//     Point second = Point(front.x() + xSlope, front.y() - ySlope);
//     Point third = Point(front.x() - xSlope, front.y() + ySlope);
    
//     triangleOne = front;
//     triangleTwo = second;
//     triangleThree = third;
// }


// friend ostream &operator<<(ostream &output, const Decision &D){
//  output << endl;
//     output << "================================================" <<endl;
//     map<string, int> first;
//     //output << screen << endl;
//     ostringstream os;
//     output << "Size: (" << screen.width() << ", " << screen.height() << ")" << endl;
//     output << "Ship: ";
//     Point p = centerOfShip(screen);
//     p.print();
//     output << endl;
//     output << "Slope: ";
//     Point slope = calculateSlope();
//     slope.print();
//     output << endl;
//     defineTrianglePoints(p, slope.x(), slope.y());
//     for (int y = 31; y < screen.height(); y++) {
//         for (int x = 0; x < screen.width(); x++) {
//             //output << "Point: (" << x << ", " << y << ")" << endl;
//             string thing = to_string(screen.get(y, x));
//             first[thing] += 1;
//             if (p.x() == x && p.y() == y) {
//                 output << "&";
//                 continue;
//             }
//             if (triangleOne.x() == x && triangleOne.y() == y) {
//                 output << "$";
//             }
//             if (triangleTwo.x() == x && triangleTwo.y() == y) {
//                 output << "$";
//             }
//             if (triangleThree.x() == x && triangleThree.y() == y) {
//                 output << "$";
//             }
//             if (thing == "0"){
//                 output << " ";
//             } else if (thing == "36"){
//                 output << "a";
//             } else if (thing == "38"){
//                 output << "%";
//             } else if (thing == "44"){      // Land on a Planet
//                 output << "%";
//             } else if (thing == "54"){      // Turret/Bunker
//                 output << "I";
//             } else if (thing == "52"){
//                 output << "*";
//             } else if (thing == "56"){
//                 output << "R";
//             } else if (thing == "60"){
//                 output << "T";
//             } else if (thing == "66"){
//                 output << "Q";
//             } else if (thing == "74"){
//                 output << "b";
//             } else if (thing == "78"){
//                 output << "Y";
//             } else if (thing == "86"){
//                 output << "X";
//             } else if (thing == "92"){
//                 output << "c";
//             } else if (thing == "94"){
//                 output << "~";
//             } else if (thing == "116"){
//                 output << "P";
//             } else if (thing == "118"){     // Fuel
//                 output << "E";
//             } else if (thing == "120"){
//                 output << "F";
//             } else if (thing == "122"){     // Planet in Upper Left Hand Corner
//                 output << "d";
//             } else if (thing == "136"){
//                 output << "e";
//             } else if (thing == "138"){
//                 output << "f";
//             } else if (thing =="140"){
//                 output << "g";
//             } else if (thing == "156"){     // Starting Circle
//                 output << "h";
//             } else if (thing == "166"){
//                 output << "S";
//             } else if (thing == "170"){     // Ship Doing Nothing
//                 output << "|";
//             } else if (thing == "175"){     // Ship With Shiled On
//                 output << "@";
//             } else if (thing == "188"){
//                 output << "j";
//             } else if (thing == "194"){
//                 output << "Z";
//             } else if (thing == "196"){
//                 output << "]";
//             }  else if (thing == "198"){
//                 output << "W";
//             }  else if (thing == "216"){    // Planet in Left Upper Corner
//                 output << "k";
//             } else if (thing == "244"){
//                 output << "O";
//             }  else if (thing == "250"){
//                 output << "^";
//             } else{
//                 output << "U";
//             }
//             os << screen.get(y, x);
//             string str = os.str(); // str is what you want.
//             output << str << " ";
            
//         }
//         output << endl;
//     }
//     output << "================================================" <<endl;
//     output << endl;
    
//     for(auto elem : first)
//     {
//         std::output << elem.first << " " << elem.second << "\n";
//     }
//     return output;
// }
