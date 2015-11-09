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
//
//#include <iostream>
//#include <ale_interface.hpp>
//
//#ifdef __USE_SDL
//  #include <SDL.h>
//#endif
//
//using namespace std;
//
//int main(int argc, char** argv) {
//    if (argc < 2) {
//        std::cerr << "Usage: " << argv[0] << " rom_file" << std::endl;
//        return 1;
//    }
//
//    ALEInterface ale;
//
//    // Get & Set the desired settings
//    ale.setInt("random_seed", 123);
//    //The default is already 0.25, this is just an example
//    ale.setFloat("repeat_action_probability", 0.25);
//
//#ifdef __USE_SDL
//    ale.setBool("display_screen", true);
//    ale.setBool("sound", true);
//#endif
//
//    // Load the ROM file. (Also resets the system for new settings to
//    // take effect.)
//    ale.loadROM(argv[1]);
//
//    // Get the vector of legal actions
//    ActionVect legal_actions = ale.getLegalActionSet();
//
//    // Play 10 episodes
//    for (int episode=0; episode<10; episode++) {
//        float totalReward = 0;
//        while (!ale.game_over()) {
//            Action a = legal_actions[rand() % legal_actions.size()];
//            // Apply the action and get the resulting reward
//            float reward = ale.act(a);
//            totalReward += reward;
//        }
//        cout << "Episode " << episode << " ended with score: " << totalReward << endl;
//        ale.reset_game();
//    }
//
//    return 0;
//}


#include <iostream>
#include <ale_interface.hpp>

//#include "MyAgent.hpp"

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
    // if (argc < 2) {
    //     std::cerr << "Usage: " << argv[0] << " rom_file" << std::endl;
    //     return 1;
    // }
    
    ALEInterface ale;
    
    // Get & Set the desired settings
    ale.setInt("random_seed", 123);
    //The default is now 0 because we don't want stochasity
    ale.setFloat("repeat_action_probability", 0);
    
#ifdef __USE_SDL
    ale.setBool("display_screen", true);
    ale.setBool("sound", false);
#endif
    
    // Load the ROM file. (Also resets the system for new settings to
    // take effect.)
    ale.loadROM("gravitar.bin");
    
    // Get the vector of legal actions
    ActionVect legal_actions = ale.getLegalActionSet();
    
    // Erase actions that move, but don't fire
    //minimal_actions.erase(minimal_actions.begin() + 2, minimal_actions.begin() + 10);
    
    // Store all rewards earned in all episodes
    float allRewards = 0;
    
    // Play 10 episodes
    int episodes = 10;
    for (int episode=0; episode<episodes; episode++) {
        float totalReward = 0;
        while (!ale.game_over()) {
            //ALEState curState = ale.cloneState();
            //Action a = getAction(minimal_actions, curState, ale);
            Action a = legal_actions[rand() % legal_actions.size()];
            // Apply the action and get the resulting reward
            float reward = ale.act(a);
            totalReward += reward;
        }
        allRewards += totalReward;
        cout << "Episode " << episode << " ended with score: " << totalReward << endl;
        ale.reset_game();
    }
    
    // Display average reward per game
    cout << "Average Reward: " << (allRewards / episodes) << endl;
    
    return 0;
}
