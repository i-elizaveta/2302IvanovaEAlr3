#include <iostream>
#include <queue>
#include <time.h>
#include <algorithm>
#include <chrono>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    Node* parent;
    int height;
    int color;

    Node(int k) :
        key(k),
        left(nullptr),
        right(nullptr),
        parent(nullptr),
        height(1),
        color(1) {}
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getHeight(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

int getBalance(Node* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

Node* insertBST(Node* root, int key) {
    if (root == nullptr)
        return new Node(key);

    if (key < root->key)
        root->left = insertBST(root->left, key);
    else if (key > root->key)
        root->right = insertBST(root->right, key);

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    return root;
}

Node* insertAVL(Node* root, int key) {
    if (root == nullptr)
        return new Node(key);

    if (key < root->key)
        root->left = insertAVL(root->left, key);
    else if (key > root->key)
        root->right = insertAVL(root->right, key);
    else
        return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && key < root->left->key)
        return rotateRight(root);

    if (balance < -1 && key > root->right->key)
        return rotateLeft(root);

    if (balance > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

Node* insertRBT(Node* root, int key);

void fixRBT(Node* node, Node* parent);

Node* createRBTNode(int key, int color) {
    Node* newNode = new Node(key);
    newNode->color = color;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

Node* insertRBT(Node* root, int key) {
    if (root == nullptr)
        return createRBTNode(key, 0);

    if (key < root->key)
        root->left = insertRBT(root->left, key);
    else if (key > root->key)
        root->right = insertRBT(root->right, key);
    else
        return root;

    fixRBT(root, nullptr);

    return root;
}

void fixRBT(Node* node, Node* parent) {
    if (node == nullptr)
        return;

    if (parent == nullptr)
        node->color = 1;
    else if (parent->color == 1)
        return;
    else {
        Node* grandparent = nullptr;
        Node* uncle = nullptr;

        if (parent == parent->left) {
            grandparent = parent->left;
            uncle = parent->right;
        }
        else {
            grandparent = parent->right;
            uncle = parent->left;
        }

        if (uncle != nullptr && uncle->color == 0) {
            parent->color = 1;
            uncle->color = 1;
            grandparent->color = 0;
            fixRBT(grandparent, grandparent->parent);
        }
        else {
            if (node == parent->right && parent == grandparent->left) {
                rotateLeft(parent);
                node = node->left;
            }
            else if (node == parent->left && parent == grandparent->right) {
                rotateRight(parent);
                node = node->right;
            }

            parent->color = 1;
            grandparent->color = 0;

            if (node == parent->left)
                rotateRight(grandparent);
            else
                rotateLeft(grandparent);
        }
    }
}

Node* minValueNode(Node* node) {
    Node* current = node;

    while (current->left != nullptr)
        current = current->left;

    return current;
}

Node* deleteNodeBST(Node* root, int key) {
    if (root == nullptr)
        return root;

    if (key < root->key)
        root->left = deleteNodeBST(root->left, key);
    else if (key > root->key)
        root->right = deleteNodeBST(root->right, key);
    else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNodeBST(root->right, temp->key);
    }

    return root;
}

Node* deleteNodeAVL(Node* root, int key) {
    if (root == nullptr)
        return root;

    if (key < root->key)
        root->left = deleteNodeAVL(root->left, key);
    else if (key > root->key)
        root->right = deleteNodeAVL(root->right, key);
    else {
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            else {
                *root = *temp;
            }

            delete temp;
        }
        else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNodeAVL(root->right, temp->key);
        }
    }

    if (root == nullptr)
        return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

Node* deleteNodeRBT(Node* root, int key) {
    if (root == nullptr)
        return root;

    if (key < root->key)
        root->left = deleteNodeRBT(root->left, key);
    else if (key > root->key)
        root->right = deleteNodeRBT(root->right, key);
    else {
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            else {
                *root = *temp;
            }

            delete temp;
        }
        else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNodeRBT(root->right, temp->key);
        }
    }

    fixRBT(root, nullptr);

    return root;
}

void inorderTraversal(Node* root) {
    if (root != nullptr) {
        inorderTraversal(root->left);
        cout << root->key << " ";
        inorderTraversal(root->right);
    }
}

void preorderTraversal(Node* root) {
    if (root != nullptr) {
        cout << root->key << " ";
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

void postorderTraversal(Node* root) {
    if (root != nullptr) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        cout << root->key << " ";
    }
}

void levelorderTraversal(Node* root) {
    if (root == nullptr)
        return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        cout << current->key << " ";
        q.pop();

        if (current->left != nullptr)
            q.push(current->left);

        if (current->right != nullptr)
            q.push(current->right);
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    Node* bstRoot = nullptr;
    Node* avlRoot = nullptr;
    Node* rbtRoot = nullptr;

    int n;
    cout << "Введите количество элементов: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        int key;
        key = rand() % 200000;
        bstRoot = insertBST(bstRoot, key);
        avlRoot = insertAVL(avlRoot, key);
        rbtRoot = insertRBT(rbtRoot, key);
    }
   

  
    cout << "Двоичное дерево поиска (inorder): ";
    inorderTraversal(bstRoot);
    cout << endl;

    cout << "Красно-черное дерево (inorder): ";
    inorderTraversal(rbtRoot);
    cout << endl;

    cout << "АВЛ-дерево (inorder): ";
    inorderTraversal(avlRoot);
    cout << endl;

    cout << "Высота двоичного дерева поиска: " << getHeight(bstRoot) << endl;
    cout << "Высота красно-черного дерева: " << getHeight(rbtRoot) << endl;
    cout << "Высота АВЛ-дерева: " << getHeight(avlRoot) << endl;

    int deleteKey;
    cout << "Введите элемент для удаления: ";
    cin >> deleteKey;

    bstRoot = deleteNodeBST(bstRoot, deleteKey);
    avlRoot = deleteNodeAVL(avlRoot, deleteKey);
    rbtRoot = deleteNodeRBT(rbtRoot, deleteKey);

    cout << "Двоичное дерево поиска после удаления (inorder): ";
    inorderTraversal(bstRoot);
    cout << endl;

    cout << "Красно-черное дерево после удаления (inorder): ";
    inorderTraversal(rbtRoot);
    cout << endl;

    cout << "АВЛ-дерево после удаления (inorder): ";
    inorderTraversal(avlRoot);
    cout << endl;

    cout << "\nБалансировка красно-черного дерева при удалении элементов\n " << endl;
   auto start = chrono::high_resolution_clock::now(); // Начало замера времени
    inorderTraversal(rbtRoot);
     auto end = chrono::high_resolution_clock::now(); // Конец замера времени
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    cout << " \n Время затрат на балансировку красно-черного дерева при удалении элементов: " << duration.count() << " наносек" << endl;

    cout << "\nБалансировка АВЛ-дерева при удалении элементов\n " << endl;
    start = chrono::high_resolution_clock::now(); // Начало замера времени
    inorderTraversal(avlRoot);
    end = chrono::high_resolution_clock::now(); // Конец замера времени
    duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    cout << "\n Время затрат на балансировку АВЛ-дерева при удалении элементов: " << duration.count() << " наносек" << endl;


    int insertKey;
    cout << "Введите элемент для вставки: ";
    cin >> insertKey;

    bstRoot = insertBST(bstRoot, insertKey);
    avlRoot = insertAVL(avlRoot, insertKey);
    rbtRoot = insertRBT(rbtRoot, insertKey);

    cout << "Двоичное дерево поиска после вставки (inorder): ";
    inorderTraversal(bstRoot);
    cout << endl;

    cout << "Красно-черное дерево после вставки (inorder): ";
    inorderTraversal(rbtRoot);
    cout << endl;

    cout << "АВЛ-дерево после вставки (inorder): ";
    inorderTraversal(avlRoot);
    cout << endl;
    
    cout << "\nБалансировка красно-черного дерева при вставке элементов\n " << endl;
    start = chrono::high_resolution_clock::now(); // Начало замера времени
    inorderTraversal(rbtRoot);
     end = chrono::high_resolution_clock::now(); // Конец замера времени
    duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    cout << "\nВремя затрат на балансировку красно-черного дерева при вставке элементов: " << duration.count() << " наносек" << endl;

    cout << "\nБалансировка АВЛ-дерева при вставке элементов\n " << endl;
    start = chrono::high_resolution_clock::now(); // Начало замера времени
    inorderTraversal(avlRoot);
    end = chrono::high_resolution_clock::now(); // Конец замера времени
    duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    cout << "\nВремя затрат на балансировку АВЛ-дерева при вставке элементов: " << duration.count() << " наносек" << endl;
    return 0;
}