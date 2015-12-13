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
#include <map>
#include <math.h>

#include <vector>
#include <bitset>
#include <iostream>
#include <ale_interface.hpp>

#include "Point.h"
#include "Decision.h"

#ifdef __USE_SDL
#include <SDL.h>
#endif

using namespace std;
using namespace ale;


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
    ALEInterface ale;
    
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
    
    
    // Load the ROM file. (Also resets the system for new settings to
    // take effect.)
    ale.loadROM("gravitar.bin");
    
    // Get the vector of minimal actions
    const ActionVect minimal_actions = ale.getMinimalActionSet();
     
    // Erase actions that move, but don't fire
    //minimal_actions.erase(minimal_actions.begin() + 2, minimal_actions.begin() + 10);
    
    // Store all rewards earned in all episodes
    float allRewards = 0;
    
    // Play 10 episodes
    int episodes = 10;
    int number = 0;
    int count = 0;
    int lastLives = ale.lives();
    bool reset = false;

    Decision decision = Decision(ale.getMinimalActionSet(), ale.getScreen());
        

    for (int episode=0; episode<episodes; episode++) {
        float totalReward = 0;
        while (!ale.game_over()) {
            if (ale.lives() < lastLives){
                lastLives = ale.lives();
                number = 0;
                count = 0;
                reset = true;
                //cout << " DIE " << endl;
            } else{
            	reset = false;
            }

            // Apply the action and get the resulting reward
            float reward = ale.act(decision.getDecision(ale.getScreen(), ale.lives(), reset));
			//decision.print();
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