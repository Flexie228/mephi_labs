#include <iostream>
#include <vector>
#include <cassert>
#include "BST.hpp"

void TestInsertAndSize() {
    BST<int> tree;
    assert(tree.size() == 0);

    tree.Insert(10);
    assert(tree.size() == 1);
    tree.Insert(5);
    tree.Insert(15);
    assert(tree.size() == 3);

    tree.Insert(10);
    assert(tree.size() == 3);

    std::cout << "TestInsertAndSize passed.\n";
}

void TestContains() {
    BST<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);

    assert(tree.IsContains(10));
    assert(tree.IsContains(5));
    assert(tree.IsContains(15));
    assert(!tree.IsContains(20));

    std::cout << "TestContains passed.\n";
}

void TestRemove() {
    BST<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    tree.Insert(3);
    tree.Insert(7);

    assert(tree.size() == 5);

    tree.Remove(5);
    assert(tree.size() == 4);
    assert(!tree.IsContains(5));

    tree.Remove(10);
    assert(tree.size() == 3);
    assert(!tree.IsContains(10));

    tree.Remove(100);
    assert(tree.size() == 3);

    std::cout << "TestRemove passed.\n";
}

void TestFindByPath() {
    BST<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    tree.Insert(3);
    tree.Insert(7);

    // Путь к корню — пустая строка
    assert(tree.FindByPath("")->data == 10);

    // Путь "l" - левый сынок корня (5)
    assert(tree.FindByPath("l")->data == 5);

    // Путь "r" - правый сынок корня (15)
    assert(tree.FindByPath("r")->data == 15);

    // Путь "ll" - левый сынок 5 (3)
    assert(tree.FindByPath("ll")->data == 3);

    // Путь "lr" - правый сынок 5 (7)
    assert(tree.FindByPath("lr")->data == 7);

    try {
        tree.FindByPath("rr");
        assert(false);
    } catch (...) {

    }

    std::cout << "TestFindByPath passed.\n";
}

void TestGetSubtree() {
    BST<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    tree.Insert(3);
    tree.Insert(7);

    BST<int> subtree = tree.GetSubtree(5);
    assert(subtree.size() == 3);
    assert(subtree.IsContains(5));
    assert(subtree.IsContains(3));
    assert(subtree.IsContains(7));
    assert(!subtree.IsContains(10));

    BST<int> emptySubtree = tree.GetSubtree(100);
    assert(emptySubtree.size() == 0);

    std::cout << "TestGetSubtree passed.\n";
}

void TestTraversals() {
    BST<int> tree;
    std::vector<int> values = {10, 5, 15, 3, 7};
    for (int v : values) tree.Insert(v);

    std::cout << "      Inorder (expected 3 5 7 10 15): ";
    tree.printInorder();

    std::cout << "      Preorder (expected 10 5 3 7 15): ";
    tree.printPreorder();

    std::cout << "      Postorder (expected 3 7 5 15 10): ";
    tree.printPostorder();

    std::cout << "      TestOrders passed.\n";
}

void TestBigData() {
    BST<int> tree;
    const int N = 10000;

    for (int i = 0; i < N; ++i) {
        tree.Insert(i);
    }
    assert(tree.size() == N);

    for (int i = 0; i < N; i += 1000) {
        assert(tree.IsContains(i));
    }
    assert(!tree.IsContains(-1));
    assert(!tree.IsContains(N + 1));


    for (int i = 0; i < N; i += 2) {
        tree.Remove(i);
    }
    assert(tree.size() == N / 2);

    for (int i = 1; i < N; i += 2) {
        assert(tree.IsContains(i));
    }
    for (int i = 0; i < N; i += 2) {
        assert(!tree.IsContains(i));
    }

    std::cout << "TestBigData passed.\n";
}