//
// Created by Momo on 02.09.2019.
//


class Tree {

public:

    // amount of nodes that are created and added to the tree with one mcts run
    int amount_nodes = 0;

    // array of pointers to keep track of each created node object
    Node *nodes[400000] = {};


    // add a pointer to the tree of nodes
    void addNode(Node *node) {
        nodes[amount_nodes] = node;
        amount_nodes++;
    }


    // delete all objects the pointers are pointing on
    void deleteNodes() {
        for (int i = 0; i < amount_nodes; i++) {
            delete nodes[i];
            nodes[i] = 0;
        }
    }

};