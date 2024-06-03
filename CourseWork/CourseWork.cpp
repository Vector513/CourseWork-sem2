#include <iostream>
#include <Windows.h>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <string>
#include <queue>
#include <vector>

using std::chrono::time_point;
using std::chrono::steady_clock;
using std::chrono::duration;
using std::ratio;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using namespace std;

/*start = steady_clock::now();
					res = getValue(list, index);
					end = steady_clock::now();
					result = duration_cast<nanoseconds>(end - start);

					if (res == 0.5) {
						cout << "Введённый индекс выходит за рамки списка!!!\n";
					}
					else {
						cout << "На позиции " << index << " находится значение " << res << '\n';
					}

					cout << "Время на получение элемента: " << (result.count() / 1000000.0) << "ms(" << result.count() << "ns)\n";
*/

time_point<steady_clock, duration<__int64, ratio<1, 1000000000>>> start, finish;
nanoseconds result;

const char separator[] = "------------------------------------------------------------------------------------------------------------------------";

class Tree 
{
private:
    int size = 0;
    struct TreeNode {
        int data = 0;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        TreeNode* parent = nullptr;
    };
    TreeNode* mainRoot = nullptr;

public:
    int getSize() {
        return size;
    }

    void show() 
    {
        printTree(mainRoot);
    }
    
    void showFile(ofstream& file) 
    {
        printTreeFile(mainRoot, 0, false, file);
    }
    
    void printTreeFile(TreeNode* tree, int level, bool isRight, ofstream& file)
    {
        if (tree == nullptr) return;
        printTreeFile(tree->right, level + 1, true, file);

        if (!level) file << "-->"; // марафет для корня дерева
        else file << "   ";

        for (int i = 0; i < level; i++) {
            if (i + 1 == level) isRight ? file << ".-->" : file << "`-->"; // отступ для уровня
            else file << "    ";
        }

        file << tree->data << "\n";

        printTreeFile(tree->left, level + 1, false, file);
    }
    
    void printTree(TreeNode* tree, int level = 0, bool isRight = false) {
        if (tree == nullptr) return;
        printTree(tree->right, level + 1, true);

        if (!level) cout << "-->"; // марафет для корня дерева
        else cout << "   ";

        for (int i = 0; i < level; i++) {
            if (i + 1 == level) isRight ? cout << ".-->" : cout << "`-->"; // отступ для уровня
            else cout << "    ";
        }

        cout << tree->data << "\n";

        printTree(tree->left, level + 1);
    }
    
    void insert(int data) 
    {
        addNode(data, mainRoot);
    }

    void addNode(int data, TreeNode*& root) 
    {
        if (root == nullptr) {
            root = new TreeNode;
            root->data = data;
        }

        else {
            if (data < root->data) {
                if (root->left != nullptr) {
                    addNode(data, root->left);
                }
                else {
                    root->left = new TreeNode;
                    root->left->data = data;
                    root->left->parent = root;
                }
            }
            else if (data >= root->data) {
                if (root->right != nullptr) {
                    addNode(data, root->right);
                }
                else {
                    root->right = new TreeNode;
                    root->right->data = data;
                    root->right->parent = root;
                }
            }
        }
    }

    TreeNode* findElem(int data) 
    {
        return findNode(data, mainRoot);
    }

    TreeNode* findNode(int data, TreeNode* root) 
    {
        if (root == nullptr) {
            return nullptr;
        }
        else {
            if (data == root->data) {
                return root;
            }
            else if (data < root->data) {
                return findNode(data, root->left);
            }
            else if (data > root->data) {
                return findNode(data, root->right);
            }
        }
    }

    TreeNode* leftMost(TreeNode* root) {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->left != nullptr) {
            return leftMost(root->left);
        }
        return root;
    }

    TreeNode* rightMost(TreeNode* root) {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->right != nullptr) {
            return rightMost(root->right);
        }
        return root;
    }

    void deleteElem(int data) {
        deleteNode(mainRoot, data);
    }

    TreeNode* maxData(TreeNode*& root) {
        if (root) {
            if (root->right)
                return maxData(root->right);
            return root;
        }
        else return nullptr;
    }

    void deleteNode(TreeNode*& root, int num) {
        TreeNode* del = findElem(num);
        if (!del) return;
        if (!del->left && !del->right) {
            if (del->parent) {
                if (del == del->parent->left)
                    del->parent->left = nullptr;
                else
                    del->parent->right = nullptr;
            }
            delete del;
            return;
        }
        if (del->left && !del->right) {
            if (del->parent) {
                if (del == del->parent->left)
                    del->parent->left = del->left;
                else
                    del->parent->right = del->left;
            }
            delete del;
            return;
        }
        if (!del->left && del->right) {
            if (del->parent) {
                if (del == del->parent->left)
                    del->parent->left = del->right;
                else
                    del->parent->right = del->right;
            }
            delete del;
            return;
        }
        if (del->left && del->right) {
            swap(del->data, maxData(del->left)->data);
            del = maxData(del->left);
            if (del->parent) {
                if (del == del->parent->left)
                    del->parent->left = del->left;
                else
                    del->parent->right = del->left;
            }
            delete del;
            return;
        }
    }
    /*
    void deleteNode(int data, TreeNode* root) 
    {
        if (root == nullptr) {
            return;
        }
        else if (data < root->data) {
            return deleteNode(data, root->left);
        }
        else if (data > root->data) {
            return deleteNode(data, root->right);
        }
        else {
            if (root->left == nullptr && root->right == nullptr) {
                if (root->parent->left == root) {
                    root->parent->left = nullptr;
                }
                else {
                    root->parent->right = nullptr;
                }
                delete root;
            }
            else {
                TreeNode* newNode = nullptr;
                if (root->left != nullptr) {
                    newNode = rightMost(root->left);
                }
                else {
                    newNode = leftMost(root->right);
                }

                if (root->parent->left == root) {
                    root->parent->left == newNode;
                }
                else {
                    root->parent->right = newNode;
                }

                newNode->parent = root->parent;
                newNode->right = root->right;
                newNode->left = root->left;

                delete root;
            }
        }
    }
    */

    void preOrderTravers() 
    {
        preOrderTraversRec(mainRoot);
        cout << '\n';
    }

    void preOrderTraversRec(TreeNode* root) {
        if (root) {
            std::cout << root->data << " ";
            preOrderTraversRec(root->left);
            preOrderTraversRec(root->right);
        }
    }

    void postOrderTravers() 
    {
        postOrderTraversRec(mainRoot);
        cout << '\n';
    }

    void postOrderTraversRec(TreeNode* root) {
        if (root) {
            postOrderTraversRec(root->left);
            postOrderTraversRec(root->right);
            std::cout << root->data << " ";
        }
    }

    void wideBypassOut() 
    {
        wideBypass(mainRoot);
        cout << '\n';
    }

    void wideBypass(TreeNode* root) 
    {
        queue<TreeNode*> elements;
        elements.push(root);
        while (!elements.empty()) {
            root = elements.front();
            elements.pop();
            cout << root->data << " ";
            if (root->left)
                elements.push(root->left);
            if (root->right)
                elements.push(root->right);
        }
    }

    void fill(vector<int>& tokens) 
    {
        for (int i = 0; i < tokens.size(); i++) {
            addNode(tokens[i], mainRoot);
            size++;
        }
    }

    void destroyTree() 
    {
        destroyTreeRec(mainRoot);
    }

    void destroyTreeRec(TreeNode* root)
    {
        if (root != nullptr)	    	        // если узел дерева существует
        {					// функция вызовет сама себя
            destroyTreeRec(root->left);	// сначала для левого потомка,
            destroyTreeRec(root->right);  	// после для правого потомка.
            delete root;			// если потомков нет, она удалит
        }
    }
};

int getNumber() 
{
    return rand() % 199 - 99;
}

void checkForNumbers()
{
    char a = cin.get();
    while (a < '0' || a > '9') {
        if (a == '-') {
            a = cin.get();
            if (a >= '0' && a <= '9') {
                cin.unget();
                break;
            }
        }
        //cout << "Следует вводить числа: ";
        cin >> a;
    }
    cin.unget();
}

void readFile(ifstream& file, vector<int>& tokens) 
{
    while (file.peek() != EOF) {
        string tmp;
        file >> tmp;
        tokens.push_back(stoi(tmp));
    }
}

// 7 -2 12 -5 3 0 -6 4 8 -216 643 54 -13

void launch()
{
    Tree tree;
    vector<int> tokens = {};

    ifstream fromFile;
    ofstream toFile;
    string pathFrom = "file.txt";
    string pathTo = "tree.txt";


    char commands[] = "1a) Создание бинарного дерева, где N - кол-во элементов\n1b) Создание бинарного дерева, где элементы вводит пользователь\n1c) Создание бинарного дерева из элементов в файле\n2) Записать дерево в файл\n3i) Вставка элемента в дерево\n3d) Удаление элемента из дерева\n3g) Поиск элемента в дереве\n4) Вывод прямого, обратного обходов и обхода в ширину\ns) Вывести дерево\nd) Уничтожить дерево\nc) Вывод списка комманд\ne) Выход из программы\n";

    char command = 'c';

    do {
        if (command == 'e') {
            cout << "Программа была завершена по воле пользователя";
            break;
        }

        else if (command == 'c') {
            cout << commands;
        }

        else if (command == 's') {
            tree.show();
        }

        else if (command == 'd') {
            tree.destroyTree();
            cout << "Дерево было уничтожено\n";
        }

        else if (command == '1') {
            cin >> command;
            if (command == 'a') {
                int N;
                cout << "Введите количество элементов: ";
                cin >> N;

                for (int i = 0; i < N; i++) {
                    tokens.push_back(getNumber());
                }

                tree.fill(tokens);
            }
            else if (command == 'b') {
                cout << "Введите элементы дерева: ";
                checkForNumbers();

                int a;
                while (cin.get() != '\n') {
                    cin.unget();
                    cin >> a;
                    tokens.push_back(a);
                }

                tree.fill(tokens);
            }
            else if (command == 'c') {
                //cout << "Введите путь к файлу: ";
                //cin >> path;
                fromFile.open(pathFrom, ios::in);
                if (fromFile.is_open()) {
                    readFile(fromFile, tokens);
                    tree.fill(tokens);
                    fromFile.close();
                    cout << "Дерево было создано из элементов в файле\n";
                }
                else {
                    cout << "Файл не был открыт\n";
                }
            }
        }

        else if (command == '2') {
            toFile.open(pathTo, ios::out | ios::trunc);
            if (toFile.is_open()) {
                tree.showFile(toFile);
                toFile.close();
                cout << "Дерево было записано в файл\n";
            }
            else {
                cout << "Файл не был открыт\n";
            }
        }

        else if (command == '3') {
            cin >> command;
            if (command == 'i') {
                cout << "Введите число которое следует вставить: ";
                int N;
                cin >> N;
                start = steady_clock::now();
                tree.insert(N);
                finish = steady_clock::now();
                result = duration_cast<nanoseconds>(finish - start);
                cout << "Число было добавлено в дерево за время " << (result.count() / 1000000.0) << " ms\n";
            }
            else if (command == 'd') {
                cout << "Введите число которое следует удалить: ";
                int N;
                cin >> N;
                start = steady_clock::now();
                tree.deleteElem(N);
                finish = steady_clock::now();
                result = duration_cast<nanoseconds>(finish - start);
                cout << "Число было удалено из дерева за время " << (result.count() / 1000000.0) << " ms\n";
            }

            else if (command == 'g') {
                cout << "Введите число которое следует найти: ";
                int N;
                cin >> N;
                start = steady_clock::now();
                tree.findElem(N);
                finish = steady_clock::now();
                result = duration_cast<nanoseconds>(finish - start);
                cout << "Число " << N << " было найдено за время " << (result.count() / 1000000.0) << " ms\n";
            }
        }

        else if (command == '4') {
            cout << "Прямой обход: ";
            tree.preOrderTravers();
            cout << "Обратный обход: ";
            tree.postOrderTravers();
            cout << "Обход в ширину: ";
            tree.wideBypassOut();
        }

        else if (command == '5') {

        }

        cout << separator << '\n';
        cout << "Введите комманду: ";
        cin >> command;
        cout << '\n';

    } while (true);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");


    launch();


    return 0;
}