#include <iostream>
#include <stack>

#include "Node.h"

using char_node_t = node<char>;

std::vector<char_node_t*> getDFS(char_node_t* root, char target);

int main() {
	//      A
	//    B   C
	//  D   E   F
	// Create nodes with char data
	char_node_t* nodeA = new char_node_t('A');
	char_node_t* nodeB = new char_node_t('B');
	char_node_t* nodeC = new char_node_t('C');
	char_node_t* nodeD = new char_node_t('D');
	char_node_t* nodeE = new char_node_t('E');
	char_node_t* nodeF = new char_node_t('F');
	// Create tree from nodes (children)
	nodeA->children.push_back(nodeB); // A->B
	nodeA->children.push_back(nodeC); // A->C
	nodeB->children.push_back(nodeD); // B->D
	nodeB->children.push_back(nodeE); // B->E
	nodeC->children.push_back(nodeE); // C->E
	nodeC->children.push_back(nodeF); // C->F

    // get search path to 'F' from node 'A'
    auto path = getDFS(nodeA, 'F');
    // display path result
    std::cout << "path: ";
    for(auto path_node : path) {
        std::cout << "->" << path_node->data;
    }
    std::cout << std::endl;

	return 0;
}

std::vector<char_node_t*> getDFS(char_node_t* root, char target) {
    // check for valid root node
    if(root == nullptr) return std::vector<char_node_t*>();

    // create nodes stack and push root onto stack
    std::stack<char_node_t*> node_stack;
    root->visited = true;
    node_stack.push(root);

    while(!node_stack.empty()) {
        // get current node from the top of stack
        char_node_t* current = node_stack.top();
        // check if the current node data is target value
        if(current->data == target) {
            std::cout << "found: " << current->data << std::endl;
            break;
        } else {
            std::cout << "visited: " << current->data << std::endl;
        }

        // if target not found then go down tree (child of current node)
        // push unvisited child node onto stack
        bool popNode = true; // pop node if there are no unvisited children
        for(auto child : current->children) {
            // if child node is unvisited then mark visited and push onto stack
            if(!child->visited) {
                child->visited = true;
                node_stack.push(child);
                popNode = false;
                break;
            }
        }
        // if no unvisited children of the current node then pop the stack to go back up the tree
        if(popNode) {
            node_stack.pop();
        }
    }

    // convert stack nodes to vector of nodes (path)
    // stack nodes are in reverse order with the target at the top
    // add stack nodes to front of path vector
    std::vector<char_node_t*> path;
    while(!node_stack.empty()) {
        // add top node and then pop node off of stack
        path.insert(path.begin(), node_stack.top());
        node_stack.pop();
    }

    return path;
}