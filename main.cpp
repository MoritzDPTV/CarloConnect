//
// Created by Momo on 22.07.2019.
//


#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include "node.h"
#include "tree.h"

using namespace std;


// the to be used approach, "1" for MCTS, "2" for MCS
short int ai = 1;

// time limit for the MCS/MCTS to define how long a search can take
double limit_millis = 1000;

// height of the game field, the array
short int static const row = 6;

// width of the game field, the array
short int static const column = 7;

// two dimensional array representing the board and its state
short int array[row][column];

// array to store possible moves and total amount of possible moves from a state
short int possible_moves[column + 1];

// player identification, player 1 = human, player 2 = computer
short int player;



// initialize starting values, erase the field and set player to 1
void initGame() {
    // iterate through the whole array using a pointer to set the value (for higher performance)
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            array[i][j] = 0;
        }
    }

    // set player to 1
    player = 1;
}


// print the game field
void printBoard() {
    cout << endl << endl << "  C O N N E C T 4" << endl << endl;
    for (short int i = row - 1; i >= 0; i--) {
        for (short int j = 0; j < column; j++) {
            cout << array[i][j] << "  ";
        }
        cout << endl;
    }
    cout << endl;
}


// create a list of possible moves and the total amount of them and return randomly one of them
short int getPossibleMoves(short int array[row][column]) {
    short int remaining_moves[column];
    short int amount_moves = 0;
    short int random_move;

    // list available columns
    for (short int j = 0; j < column; j++) {
        if (array[row - 1][j] == 0) {
            possible_moves[j] = 1;
            remaining_moves[amount_moves] = j;
            amount_moves++;
        } else {
            possible_moves[j] = 0;
        }
    }

    // set the total amount of possible moves
    possible_moves[7] = amount_moves;

    // chose a random number from from the list of available columns
    if (amount_moves > 0) {
        random_move = remaining_moves[rand() % amount_moves];
    }

    return random_move;
}


// set the move in the next available slot
short int setMove(short int array[row][column], short int player, short int move) {
    short int position_row;

    // check for free slot within the selected column
    for (short int i = 0; i < row; i++) {
        if (array[i][move] == 0) {
            array[i][move] = player;
            position_row = i;
            break;
        }
    }

    // return the row in which the move has been set
    return position_row;
}


// check for game ending
short int checkWin(short int array[row][column], short int player, short int move, short int position_row) {
    short int temp_column;
    short int temp_position_row;

    // vertical check
    for (short int i = 0; i < row - 3; i++) {
        if (array[i][move] == player && array[i + 1][move] == player && array[i + 2][move] == player &&
            array[i + 3][move] == player) {
            return player;
        }
    }

    // horizontal check
    for (short int j = 0; j < column - 3; j++) {
        if (array[position_row][j] == player && array[position_row][j + 1] == player &&
            array[position_row][j + 2] == player &&
            array[position_row][j + 3] == player) {
            return player;
        }
    }

    // diagonal down right check
    if (move > position_row) {
        temp_column = move - position_row;
        temp_position_row = 0;
    } else {
        temp_column = 0;
        temp_position_row = position_row - move;
    }
    for (; temp_position_row < row - 3 && temp_column < column - 3; temp_position_row++, temp_column++) {
        if (array[temp_position_row][temp_column] == player &&
            array[temp_position_row + 1][temp_column + 1] == player &&
            array[temp_position_row + 2][temp_column + 2] == player &&
            array[temp_position_row + 3][temp_column + 3] == player) {
            return player;
        }
    }

    // diagonal down left check
    if (((column - 1) - move) > position_row) {
        temp_column = position_row + move;
        temp_position_row = 0;
    } else {
        temp_column = (column - 1);
        temp_position_row = position_row - ((column - 1) - move);
    }
    for (; temp_position_row < row - 3 && temp_column > 2; temp_position_row++, temp_column--) {
        if (array[temp_position_row][temp_column] == player &&
            array[temp_position_row + 1][temp_column - 1] == player &&
            array[temp_position_row + 2][temp_column - 2] == player &&
            array[temp_position_row + 3][temp_column - 3] == player) {
            return player;
        }
    }

    // tie
    if (array[row - 1][0] != 0 && array[row - 1][1] != 0 && array[row - 1][2] != 0 &&
        array[row - 1][3] != 0 && array[row - 1][4] != 0 && array[row - 1][5] != 0 &&
        array[row - 1][6] != 0) {
        return 3;
    }

    // return 0 if no final game state found
    return 0;
}


// switch the actual player to the other player
short int switchPlayer(short int player) {
    if (player == 1) {
        player = 2;
    } else {
        player = 1;
    }

    // return the switched player
    return player;
}


// simulate the game with random moves until someone wins
short int simulate(short int array[row][column], short int player, short int move, short int position_row) {
    // loop to set moves till a final game state
    while (true) {
        // check if game ending occurred
        if (checkWin(array, player, move, position_row) != 0) {
            // return result of the final game state
            return checkWin(array, player, move, position_row);
        }

        // switch the actual player
        player = switchPlayer(player);

        // make a random possible move
        move = getPossibleMoves(array);
        position_row = setMove(array, player, move);
    }
}


// select the best node calculated with the uct formula
short int uctSelect(Node *node_pt, Node *root_pt) {
    double uct_result;
    double best_value = -1000;
    short int selected_node;

    // iterate through the child nodes of the node
    for (short int i = 0; i < column; i++) {

        // check if child node exists
        if (node_pt->child_nodes[i] != 0) {

            // calculate ucb1 result
            uct_result = (double) node_pt->child_nodes[i]->wins / (double) node_pt->child_nodes[i]->visits +
                         sqrt(2) * sqrt(log((double) root_pt->visits) / (double) node_pt->child_nodes[i]->visits);

            // save the best
            if (uct_result > best_value) {
                best_value = uct_result;
                selected_node = i;
            }
        }
    }

    // return the selected node
    return selected_node;
}


// select the node with the most visits
short int visitsSelect(Node *node_pt) {
    double best_value = -1000;
    short int selected_node;

    // iterate through the child nodes of the node
    for (short int i = 0; i < column; i++) {

        // check if child node exists
        if (node_pt->child_nodes[i] != 0) {

            // save the best
            if (node_pt->child_nodes[i]->visits > best_value) {
                best_value = node_pt->child_nodes[i]->visits;
                selected_node = i;
            }
        }
    }

    // return the selected node
    return selected_node;
}


// update the nodes of the tree with the result of the simulation
void update(short int win, Node *node_pt) {
    // iterate through all nodes of the used branch
    while (true) {
        // update the node either with a win or no win, depending on whose move it would be
        if (win == node_pt->player) {
            node_pt->wins = node_pt->wins + 1;
        } else if (win != 0) {
            node_pt->wins = node_pt->wins - 1;
        }

        // increase visit for nodes of all nodes, of both player
        node_pt->visits++;

        // select parent node
        if (node_pt->parent_node != 0) {
            node_pt = node_pt->parent_node;
        } else {
            break;
        }
    }
}


// start the mcts algorithm to build a search tree to find a good move
short int runMCTS() {
    short int temp_array[row][column];
    short int temp_player;
    short int move;
    short int position_row;
    short int result;
    double delay;

    // counter for the time limit
    chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now();

    // copy the actual array and player to a temporary array for the simulations, get a list of all available moves
    temp_player = player;
    copy(&array[0][0], &array[0][0] + row * column, &temp_array[0][0]);
    getPossibleMoves(array);

    // create a root node and a pointer for it representing the actual state
    Node root;
    root.setPossibleMoves(possible_moves);
    root.player = player;
    Node *root_pt = &root;

    // create a pointer to manage the nodes within the tree building
    Node *node_pt = &root;

    // create tree to keep track of all nodes to erase them at the end of all simulations
    Tree tree;

    // iterate through the search tree to select, expand, simulate and update the nodes of it
    while (true) {

        // if there is a move which is not represented as child node left, expand it as child node
        if (node_pt->possible_moves[7] > 0) {

            // get a random move
            move = node_pt->getRandomMove();

            // set the move inside the copy of the field
            position_row = setMove(temp_array, temp_player, move);

            // get the possible moves for the state of the new child node
            getPossibleMoves(temp_array);

            // expand the node by the child node appropriate to the possible move
            node_pt->addChild(possible_moves, temp_player, move);

            // add this node to the tree
            tree.addNode(node_pt->child_nodes[move]);

            // start the simulation upon the actual move
            result = simulate(temp_array, temp_player, move, position_row);

            // update the node and all parent nodes by the result of the simulation
            update(result, node_pt->child_nodes[move]);

            // copy again the original state and set the node back to root
            temp_player = player;
            copy(&array[0][0], &array[0][0] + row * column, &temp_array[0][0]);
            node_pt = root_pt;

        } else {
            // check if it is a final state, a leaf node that can not have any child nodes to select next
            if (checkWin(temp_array, switchPlayer(temp_player), move, position_row) != 0) {

                // update the node and all parent nodes by the result of this final state
                result = checkWin(temp_array, switchPlayer(temp_player), move, position_row);
                update(result, node_pt);

                // copy again the original state and set the node back to root
                temp_player = player;
                copy(&array[0][0], &array[0][0] + row * column, &temp_array[0][0]);
                node_pt = root_pt;

            } else {
                // select the child node with the best ratio calculated with the uct formula
                move = uctSelect(node_pt, root_pt);

                // set the move inside the copy of the field and switch player
                position_row = setMove(temp_array, temp_player, move);
                temp_player = switchPlayer(temp_player);

                // select child node as next node to be processed
                node_pt = node_pt->child_nodes[move];
            }
        }

        // calculate the time spent for simulations to ensure it stops if the time is up
        chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
        delay = (chrono::duration_cast<chrono::milliseconds>(end - begin).count());
        if (delay > limit_millis) {
            break;
        }
    }

    // print the total amount of simulations from this search
    //cout << "Total amount of simulations MCTS: " << root_pt->visits << endl;

    // get the best move of the root node, according to the final action selection criteria
    move = visitsSelect(root_pt);

    // delete all nodes that were created during the simulation
    tree.deleteNodes();

    // return best move
    return move;
}


// start the mcs algorithm to find a good move
short int runMCS() {
    short int temp_array[row][column];
    short int move;
    short int position_row;
    short int result;
    int simulations;
    int total_simulations = 0;
    int wins;
    double limit_millis_mcs;
    double delay;
    double ratio;
    double best_ratio = -1000;

    // calculate the time limit that every move has
    getPossibleMoves(array);
    limit_millis_mcs = limit_millis / (double) possible_moves[7];

    // iteration through all moves
    for (short int i = 0; i < column; i++) {

        // skip if column is already full
        if (array[row - 1][i] != 0) {
            continue;
        }

        // counter for the time limit
        chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now();

        // (re)set the simulations and wins count
        simulations = 0;
        wins = 0;

        // iteration for each possible move including the simulations
        while (true) {

            // copy of the original field into the temporary
            copy(&array[0][0], &array[0][0] + row * column, &temp_array[0][0]);

            // set the available move chosen from the iteration of i
            position_row = setMove(temp_array, player, i);

            // check if win occurred with this move to avoid further simulations
            if (checkWin(temp_array, player, i, position_row) == player) {
                return i;
            }

            // start the simulation upon the actual move
            result = simulate(temp_array, player, i, position_row);

            // safe the result of the simulation
            if (result == player) {
                wins++;
            } else if (result != 3) {
                wins--;
            }

            // increment the simulation counter
            simulations++;

            // calculate the time spent for simulations to ensure it stops if the time is up
            chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
            delay = (chrono::duration_cast<chrono::milliseconds>(end - begin).count());
            if (delay > limit_millis_mcs) {
                break;
            }
        }

        // calculate the win rate
        ratio = (double) wins / (double) simulations;
        if (ratio > best_ratio) {
            best_ratio = ratio;
            move = i;
        }

        // count the total amount of simulations from this search
        total_simulations += simulations;
    }

    // print the total amount of simulations from this search
    //cout << "Total amount of simulations MCS: " << total_simulations << endl;

    // return best move
    return move;
}


// start the game
void playGame() {
    short int move;
    short int move_print;
    short int position_row;
    short int win;
    char game_restart;

    // erase the game field
    initGame();

    // print the board at the beginning of the game
    printBoard();

    // game play
    while (true) {

        // text to show whose players turn it is
        cout << "Player " << player << " select your column:" << endl;

        // player selection
        if (player == 1) {
            while (true) {
                // get human move
                cin >> move;
                // check for legal move - if column number exist or if it is full already
                if (move < 1 || move > column || array[row - 1][move - 1] != 0) {
                    cout << "Invalid column, please select a valid column:" << endl;
                } else {
                    move--;
                    break;
                }
            }
        } else {
            // run mcs/mcts algorithm to approximate the best move for the computer player
            if (ai == 1) {
                move = runMCTS();
            } else {
                move = runMCS();
            }

            // print move
            move_print = move + 1;
            cout << move_print << endl;
        }

        // set the move in the next available slot
        position_row = setMove(array, player, move);

        // print board
        printBoard();

        // check for game ending
        win = checkWin(array, player, move, position_row);

        // end game if final game state reached, otherwise switch player
        if (win != 0) {
            if (win == 3) {
                cout << "Tie. Nobody won." << endl;
            } else {
                cout << "Player " << player << " won." << endl;
            }

            // game restart
            cout << endl << "New game? [Y/N]" << endl;
            cin >> game_restart;
            if (game_restart == 'y' || game_restart == 'Y') {
                // erase the game field and print the empty board
                initGame();
				// new game message
				cout << endl << endl << "NEW GAME";
                printBoard();
            } else {
                cout << "Take care, bye." << endl;
                break;
            }
        } else {
            // switch the actual player
            player = switchPlayer(player);
        }
    }
}



// main function
int main() {
    // start the game
    playGame();

    return 0;
}