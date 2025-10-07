#ifndef UI_HPP
#define UI_HPP

#include "BST.hpp"
#include "Complex.hpp"
#include "Errors.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>

template <typename T>
struct TreeContainer {
    BST<T> tree;
    std::string name;
};

class Manager {
    private:
        std::vector<TreeContainer<int>> intTrees;
        std::vector<TreeContainer<Complex>> complexTrees;
        const size_t MAX_TREES = 10;
    
        void clearInput() {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    
        template <typename T>
        void printAvailableTrees(const std::vector<TreeContainer<T>>& trees) {
            if (trees.empty()) {
                std::cout << "No trees available.\n";
                return;
            }
    
            std::cout << "\nAvailable trees:\n";
            for (size_t i = 0; i < trees.size(); ++i) {
                std::cout << i+1 << ". " << trees[i].name << " (size: " << trees[i].tree.size() << ")\n";
            }
        }
    
        template <typename T>
        void printTreeMenu(TreeContainer<T>& container) {
            int choice;
            do {
                std::cout << "\n=== Tree Operations: " << container.name << " ===\n"
                          << "1. Insert element\n"
                          << "2. Remove element\n"
                          << "3. Check if contains\n"
                          << "4. Print inorder\n"
                          << "5. Print preorder\n"
                          << "6. Print postorder\n"
                          << "7. Visualize tree\n"
                          << "8. Get subtree\n"
                          << "9. Find by path\n"
                          << "10. Get traversal sequences\n"
                          << "0. Back to main menu\n"
                          << "Choice: ";
                
                std::cin >> choice;
                clearInput();
    
                try {
                    switch(choice) {
                        case 1: {
                            T value;
                            std::cout << "Enter value to insert: ";
                            std::cin >> value;
                            clearInput();
                            container.tree.Insert(value);
                            std::cout << "Value inserted successfully.\n";
                            break;
                        }
                        case 2: {
                            T value;
                            std::cout << "Enter value to remove: ";
                            std::cin >> value;
                            clearInput();
                            container.tree.Remove(value);
                            std::cout << "Value removed successfully.\n";
                            break;
                        }
                        case 3: {
                            T value;
                            std::cout << "Enter value to check: ";
                            std::cin >> value;
                            clearInput();
                            std::cout << (container.tree.IsContains(value) ? "Found" : "Not found") << "\n";
                            break;
                        }
                        case 4:
                            std::cout << "Inorder traversal: ";
                            container.tree.printInorder();
                            break;
                        case 5:
                            std::cout << "Preorder traversal: ";
                            container.tree.printPreorder();
                            break;
                        case 6:
                            std::cout << "Postorder traversal: ";
                            container.tree.printPostorder();
                            break;
                        case 7:
                            std::cout << "Tree visualization:\n";
                            container.tree.printTree();
                            break;
                        case 8: {
                            T value;
                            std::cout << "Enter subtree root value: ";
                            std::cin >> value;
                            clearInput();
                            BST<T> subtree = container.tree.GetSubtree(value);
                            std::cout << "Subtree:\n";
                            subtree.printTree();
                            break;
                        }
                        case 9: {
                            std::string path;
                            std::cout << "Enter path (e.g. 'lrl'): ";
                            std::cin >> path;
                            clearInput();
                            Node<T>* node = container.tree.FindByPath(path);
                            std::cout << "Found node: " << node->data << "\n";
                            break;
                        }
                        case 10: {
                            std::cout << "Inorder: ";
                            container.tree.printInorder();
                            std::cout << "Preorder: ";
                            container.tree.printPreorder();
                            std::cout << "Postorder: ";
                            container.tree.printPostorder();
                            break;
                        }
                        case 0:
                            return;
                        default:
                            throwError(INVALID_ARGUMENT);
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                } catch (...) {
                    std::cerr << "Unknown error!\n";
                }
            } while (true);
        }
    
        template <typename T>
        void createTreeMenu(std::vector<TreeContainer<T>>& trees) {
            if (trees.size() >= MAX_TREES) {
                std::cout << "Maximum number of trees (" << MAX_TREES << ") reached!\n";
                return;
            }
    
            TreeContainer<T> newTree;
            std::cout << "Enter tree name: ";
            std::getline(std::cin, newTree.name);
    
            int choice;
            std::cout << "Create tree from:\n"
                      << "1. Empty\n"
                      << "2. Inorder sequence\n"
                      << "3. Preorder sequence\n"
                      << "4. Postorder sequence\n"
                      << "5. Inorder + Preorder sequences\n"
                      << "Choice: ";
            std::cin >> choice;
            clearInput();
    
            try {
                switch(choice) {
                    case 1:
                        trees.push_back(newTree);
                        std::cout << "Empty tree created successfully.\n";
                        break;
                        case 2: case 3: case 4: {
                            std::vector<T> elements;
                            std::cout << "Enter elements (space separated, finish with empty line):\n";
                            std::string line;
                            while (std::getline(std::cin, line) && !line.empty()) {
                                std::istringstream iss(line);
                                T value;
                                while (iss >> value) {
                                    elements.push_back(value);
                                }
                            }
                            
                            if (elements.empty()) {
                                trees.push_back(newTree);
                                std::cout << "Empty tree created.\n";
                                break;
                            }
                            
                            try {
                                newTree.tree = BST<T>(elements, choice);
                                trees.push_back(newTree);
                                std::cout << "Tree created successfully with " << elements.size() << " elements.\n";
                            } catch (const std::exception& e) {
                                std::cerr << "Error creating tree: " << e.what() << "\n";
                            }
                            break;
                        }
                    case 5: {
                        std::vector<T> inorder, preorder;
                        T value;
                        
                        std::cout << "Enter INORDER elements (one per line, empty line to finish):\n";
                        while (true) {
                            std::string input;
                            std::getline(std::cin, input);
                            if (input.empty()) break;
    
                            std::istringstream iss(input);
                            if (iss >> value) {
                                inorder.push_back(value);
                            } else {
                                std::cout << "Invalid input, try again.\n";
                            }
                        }
                        
                        std::cout << "Enter PREORDER elements (one per line, empty line to finish):\n";
                        while (true) {
                            std::string input;
                            std::getline(std::cin, input);
                            if (input.empty()) break;
    
                            std::istringstream iss(input);
                            if (iss >> value) {
                                preorder.push_back(value);
                            } else {
                                std::cout << "Invalid input, try again.\n";
                            }
                        }
                        
                        if (inorder.size() != preorder.size()) {
                            throwError(INVALID_ARGUMENT);
                        }
                        
                        std::vector<T> combined = inorder;
                        combined.insert(combined.end(), preorder.begin(), preorder.end());
                        
                        newTree.tree = BST<T>(combined, 4);
                        trees.push_back(newTree);
                        std::cout << "Tree created successfully from inorder and preorder sequences.\n";
                        break;
                    }
                    default:
                        throwError(INVALID_ARGUMENT);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error creating tree: " << e.what() << "\n";
            } catch (...) {
                std::cerr << "Unknown error!\n";
            }
        }
    
        template <typename T>
        void selectTreeMenu(std::vector<TreeContainer<T>>& trees) {
            if (trees.empty()) {
                std::cout << "No trees available. Create one first.\n";
                return;
            }
    
            printAvailableTrees(trees);
    
            size_t choice;
            std::cout << "Select tree (0 to cancel): ";
            std::cin >> choice;
            clearInput();
    
            if (choice > 0 && choice <= trees.size()) {
                printTreeMenu(trees[choice-1]);
            }
        }
    
        template <typename T>
        void typeMenu(std::vector<TreeContainer<T>>& trees, const std::string& typeName) {
            int choice;
            do {
                std::cout << "\n=== " << typeName << " Trees Menu ===\n"
                          << "1. Create new tree\n"
                          << "2. Select existing tree\n"
                          << "3. List all trees\n"
                          << "0. Back to main menu\n"
                          << "Choice: ";
                
                std::cin >> choice;
                clearInput();
    
                try {
                    switch(choice) {
                        case 1:
                            createTreeMenu(trees);
                            break;
                        case 2:
                            selectTreeMenu(trees);
                            break;
                        case 3:
                            printAvailableTrees(trees);
                            break;
                        case 0:
                            return;
                        default:
                            throwError(INVALID_ARGUMENT);
                    }
                } catch (...) {
                    std::cerr << "Error occurred!\n";
                }
            } while (true);
        }
    
    public:
        void runMainMenu() {
            int choice;
            do {
                std::cout << "\n=== Main Menu ===\n"
                          << "1. Work with integer trees\n"
                          << "2. Work with complex trees\n"
                          << "0. Exit\n"
                          << "Choice: ";
                
                std::cin >> choice;
                clearInput();
    
                try {
                    switch(choice) {
                        case 1:
                            typeMenu(intTrees, "Integer");
                            break;
                        case 2:
                            typeMenu(complexTrees, "Complex");
                            break;
                        case 0:
                            std::cout << "Goodbye!\n";
                            return;
                        default:
                            throwError(INVALID_ARGUMENT);
                    }
                } catch (...) {
                    std::cerr << "Error occurred!\n";
                }
            } while (true);
        }
};

#endif