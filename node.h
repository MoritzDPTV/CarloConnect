//
// Author: Moritz Duarte Pinheiro Torres Vogt
// Contact: info@carloconnect.com
// Date: 22.07.2019
//


using namespace std;


class Node {

public:

    short int static const column = 7;

    // parent node of the node
    Node *parent_node = 0;

    // child nodes of the node
    Node *child_nodes[column] = {};

    // amount of times that this node has been used for simulations either as leaf node or as node in the branch of the leaf node
    int visits = 0;

    // amount of wins that occurred by simulations having this node in its branch
    int wins = 0;

    // variable to note whose player's move this node is
    short int player;

    // list of possible moves and total amount of possible moves from the state of this node
    short int possible_moves[column + 1];


    // set the array with possible moves
    void setPossibleMoves(short int possible_moves[column + 1]) {
        copy(&possible_moves[0], &possible_moves[0] + column + 1, &this->possible_moves[0]);
    }


    // expansion of a child node
    void addChild(short int possible_moves[column + 1], short int player, short int move) {
        // create new node with its game state and possible moves
        Node *child;
        child = new Node();
        child->setPossibleMoves(possible_moves);
        child->player = player;

        // assign the new child node as a child node of the actual node
        child_nodes[move] = child;

        // assign actual node as parent node
        child->parent_node = this;
    }


    // return randomly a possible move and thereupon erase it from the list of possible moves
    short int getRandomMove() {
        short int remaining_moves[column];
        short int amount_moves = 0;
        short int random_move;

        // list available columns
        for (short int j = 0; j < column; j++) {
            if (possible_moves[j] == 1) {
                remaining_moves[amount_moves] = j;
                amount_moves++;
            }
        }

        // chose a random number from from the list of available columns
        random_move = remaining_moves[rand() % amount_moves];

        // erase the availability of the chosen move for further children of this node and decrease the total amount of available moves
        possible_moves[random_move] = 0;
        possible_moves[7]--;

        return random_move;
    }

};
