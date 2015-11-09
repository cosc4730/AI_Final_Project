/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare,
 *  Matthew Hausknecht, and the Reinforcement Learning and Artificial Intelligence
 *  Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  sharedLibraryInterfaceExample.cpp
 *
 *  Sample code for running an agent with the shared library interface.
 **************************************************************************** */

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <math.h>

#include <iostream>
#include <ale_interface.hpp>

#ifdef __USE_SDL
#include <SDL.h>
#endif

using namespace std;
using namespace ale;


class Point {
private:
    int xval, yval;
public:
    // Constructor uses default arguments to allow calling with zero, one,
    // or two values.
    Point(int x = -1, int y = -1) {
        xval = x;
        yval = y;
    }
    
    // Extractors.
    int x() { return xval; }
    int y() { return yval; }
    
    // Distance to another point.  Pythagorean thm.
    int dist(Point other) {
        int xd = xval - other.xval;
        int yd = yval - other.yval;
        return sqrt(xd*xd + yd*yd);
    }
    
    // Add or subtract two points.
    Point add(Point b)
    {
        return Point(xval + b.xval, yval + b.yval);
    }
    Point sub(Point b)
    {
        return Point(xval - b.xval, yval - b.yval);
    }
    
    // Move the existing point.
    void move(int a, int b)
    {
        xval += a;
        yval += b;
    }
    
    // Print the point on the stream.  The class ostream is a base class
    // for output streams of various types.
    void print()
    {
        cout << "(" << xval << "," << yval << ")";
    }
};





void printScreen(ALEScreen screen);
Point centerOfShip(ALEScreen screen);
void updatePoints(Point p);
Point calculateSlope();

Point lastOne, lastTwo, lastThree, lastFour;


Action getAction(ActionVect av, ALEState state, ALEInterface& ale) {
    float bestReward = 0;
    Action bestAction = av[rand() % av.size()];
    //	for(int i = 0; i < av.size(); i++) {
    //		float reward = ale.act(av[i]);
    //		if(reward > bestReward) {
    //			bestAction = av[i];
    //			bestReward = reward;
    //		}
    //		ale.restoreState(state);
    //	}
    return bestAction;
}

int main(int argc, char** argv) {
    // if (argc < 2) {
    //     std::cerr << "Usage: " << argv[0] << " rom_file" << std::endl;
    //     return 1;
    // }
    
    ALEInterface ale;
    
    
    lastOne = Point();
    lastTwo = Point();
    lastThree = Point();
    lastFour = Point();
    
    
    // Get & Set the desired settings
    ale.setInt("random_seed", 123);
    //The default is now 0 because we don't want stochasity
    ale.setFloat("repeat_action_probability", 0);
    
#ifdef __USE_SDL
    ale.setBool("display_screen", true);
    ale.setBool("sound", false);
#endif
    
    
    /// Uncomment to Record
    
//    std::string recordPath = "record";
//    std::cout << std::endl;
//    
//    // Set record flags
//    ale.setString("record_screen_dir", recordPath.c_str());
//    ale.setString("record_sound_filename", (recordPath + "/sound.wav").c_str());
//    // We set fragsize to 64 to ensure proper sound sync
//    ale.setInt("fragsize", 64);
//    
//    // Not completely portable, but will work in most cases
//    std::string cmd = "mkdir ";
//    cmd += recordPath;
//    system(cmd.c_str());

    
    
    
    
    // Two Turns to make 45 degreesls
    
    
    
    
    
    // Load the ROM file. (Also resets the system for new settings to
    // take effect.)
    ale.loadROM("gravitar.bin");
    
    // Get the vector of minimal actions
    ActionVect minimal_actions = ale.getMinimalActionSet();
    
    // Actions for all minimal actions
    Action noop = minimal_actions[0];
    Action shoot = minimal_actions[1];
    Action thrust = minimal_actions[2];
    Action turnRight = minimal_actions[3];
    Action turnLeft = minimal_actions[4];
    Action shieldTractor = minimal_actions[50];
    Action thrustRight = minimal_actions[6];
    Action thrustLeft = minimal_actions[7];
    Action shieldRight = minimal_actions[8];
    Action shieldLeft = minimal_actions[9];
    Action shootThrust = minimal_actions[10];
    Action shootRight = minimal_actions[11];
    Action shootLeft = minimal_actions[12];
    Action shootShield = minimal_actions[13];
    Action shootThrustRight = minimal_actions[14];
    Action shootThrustLeft = minimal_actions[15];
    Action shootShieldRight = minimal_actions[16];
    Action shootShieldLeft = minimal_actions[17];
    
    // Erase actions that move, but don't fire
    //minimal_actions.erase(minimal_actions.begin() + 2, minimal_actions.begin() + 10);
    
    // Store all rewards earned in all episodes
    float allRewards = 0;
    
    // Play 10 episodes
    int episodes = 10;
    int number = 0;
    int count = 0;
    int lastLives = ale.lives();
    for (int episode=0; episode<episodes; episode++) {
        float totalReward = 0;
        while (!ale.game_over()) {
            if (ale.lives() < lastLives){
                lastLives = ale.lives();
                number = 0;
                count = 0;
                cout << " DIE " << endl;
            }
//            Point p = centerOfShip(ale.getScreen());
//             updatePoints(p);
//            //cout << "Lives: " << ale.lives();
            printScreen(ale.getScreen());
//            //ALEState curState = ale.cloneState();
//            //Action a = getAction(minimal_actions, curState, ale);
//            Action a;
//            if(number < 10){
//                a = noop;
//                number++;
//                cout << "NOOP: " << number << " " << to_string(noop) << endl;
//            } else {
//                a = turnRight;
//                number = 0;
//                count ++;
//                cout << "Turn " << count << " " << to_string(turnRight) << endl;
//            }
//            cout << endl << to_string(a) << endl;
            //= minimal_actions[rand() % minimal_actions.size()];
            // Apply the action and get the resulting reward
            float reward = ale.act(turnRight);
            totalReward += reward;
        }
        count = 0;
        number = 0;
        allRewards += totalReward;
        cout << "Episode " << episode << " ended with score: " << totalReward << endl;
        ale.reset_game();
    }
    
    // Display average reward per game
    cout << "Average Reward: " << (allRewards / episodes) << endl;
    
    return 0;
}


// 0    -a
// 36   -b -> Tractor Beam/ Shiled     and map on other world
// 38   -D -> Bad Guy Dieing
// 44   -%  -> Land and Bad Guy
// 52   -* -> Bullut
// 54   -I  -> Orangish Bunker
// 56   -R
// 60   -T  -> Bad Guy Ship ?
// 66   -Q
// 74   -c  -> Planet in Lower Right Corner
// 78   -Y  -> Bad Guy Ship ?
// 86   -X  -> Bad Guy Bullets
// 92   -d  -> Planet in upper Right Corner
// 94   -~  -> Bad Planet
// 116  -P  -> Bad bullet?
// 118  -E  -> Fuel
// 120  -F  -> Bad Guy ???
// 122  -f
// 136  -g
// 138  -h -> Ship
// 138  -j
// 140  -k  -> Planet in upper left corner
// 156  -l
// 166  -S  -> Ship and Ship Scatering
// 170  -| -> Lives Image and Ship just turning
// 175  -@ -> Ship When Shild is on
// 188  -o
// 194  -Z  -> Bad Guy???
// 196  -]  -> Bad Guy Bullet
// 198  -W -> Green Bunker
// 216  -p
// 244  -O  -> Bady Guy
// 250  -^ -> Bad Guy Bullet ?

// 5 + 7 + 5 + 7 +
// 7

void printScreen(ALEScreen screen){
    cout << endl;
    cout << "================================================" <<endl;
    map<string, int> first;
    //cout << screen << endl;
    ostringstream os;
    cout << "Size: (" << screen.width() << ", " << screen.height() << ")" << endl;
    cout << "Ship: ";
    Point p = centerOfShip(screen);
    p.print();
    cout << endl;
    cout << "Slope: ";
    calculateSlope().print();
    cout << endl;
    
    for (int y = 31; y < screen.height(); y++) {
        for (int x = 0; x < screen.width(); x++) {
            //cout << "Point: (" << x << ", " << y << ")" << endl;
            string thing = to_string(screen.get(y, x));
            first[thing] += 1;
            if (p.x() == x && p.y() == y) {
                cout << "&";
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
            //            os << screen.get(y, x);
            //            string str = os.str(); // str is what you want.
            //            cout << str << " ";
            
        }
        cout << endl;
    }
    cout << "================================================" <<endl;
    cout << endl;
    
    for(auto elem : first)
    {
        std::cout << elem.first << " " << elem.second << "\n";
    }
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
