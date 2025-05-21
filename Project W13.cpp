# include <iostream>
# include <cstdlib>
# include <ctime>
# include <string>
# include <limits>
#include <chrono>
using namespace std;

const int MAX_LEVEL = 6;
const float P = 0.5;

struct Node {
    string stockSymbol;
    float price;
    Node** forward;

    Node(string key, float value, int level) {
        stockSymbol = key;
        price = value;
        forward = new Node*[level + 1];
        for (int i = 0; i <= level; i++)
            forward[i] = nullptr;
    }
};

class SkipList {
    private:
        int level;
        Node* header;

    public:
        SkipList() {
            level = 0;
            header = new Node(" ", 0.0, MAX_LEVEL);
            srand(time(0));
        }
        int randomlevel() {
            int lvl = 0;
            while (((float)rand() / RAND_MAX) < P && lvl < MAX_LEVEL)
                lvl++;
                return lvl;
        }
        void insert(string symbol, float price) {
            Node* update[MAX_LEVEL + 1];
            Node* current = header;

            for (int i = level; i >= 0; i--) {
                while (current->forward[i] != nullptr && current->forward[i]->stockSymbol < symbol)
                current = current-> forward[i];
                update[i] = current;
            }
            current = current->forward[0];

            if(current != nullptr && current ->stockSymbol == symbol) {
                cout << "Updating price for " << symbol << endl;
                current->price = price;
            }else {
                int newLevel = randomlevel();
                if (newLevel > level) {
                    for (int i = level + 1; i <= newLevel; i++)
                        update[i] = header;
                    level = newLevel;
                }
                Node* newNode = new Node(symbol, price, newLevel);
                for (int i = 0; i <= newLevel; i++) {
                    newNode->forward[i] = update[i]->forward[i];
                    update[i]-> forward[i] = newNode;
                }
                cout << "Inserted: " << symbol << " at $" << price << endl;
            }
        }
        void search(string symbol) {
            Node* current = header;
            for (int i = level; i >= 0; i--) {
                while (current -> forward[i] != nullptr && current->forward[i]->stockSymbol < symbol)
                current = current->forward[i];

            }
            current = current->forward[0];
            if (current != nullptr && current-> stockSymbol == symbol) {
                cout << symbol << " found with price: $" << current->price << endl;

            }else {
                cout << symbol << "not found." << endl;
            }
        }
        void display() {
            cout << "\nStock Prices (level 0):\n";
            Node* node = header-> forward[0];
            while(node != nullptr) {
                cout << node->stockSymbol << " $" << node -> price << endl;
                node = node->forward[0];
            }
        }
        void printLevels() {
            for (int i = level; i >= 0; --i) {
                cout << "Level " << i << ": ";
                Node* current = header->forward[i];
                while (current != nullptr) {
                    cout << current->stockSymbol << " ";
                    current = current->forward[i];
                }
                cout << endl;
            }
        }
};

void benchmarkInsert(int numEntries) {
    SkipList skipList;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        skipList.insert("Stock" + to_string(i), static_cast<float>(rand() % 1000) / 10);
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken to insert " << numEntries << " entries: " << elapsed.count() << " seconds\n";
}

int main () {
    SkipList stockPrices;
    int choice;
    string symbol;
    float price;
    //benchmarkInsert(10000); // Benchmarking with 10,000 entries

    do { 
        cout << "\n--- Stock Ticker Price Index ---\n";
        cout << "1. Insert or Update Stock Price\n";
        cout << "2. Search for Stock\n";
        cout << "3. Display All Stocks\n";
        cout << "4. Exit\n";
        cout << "5. Print Levels\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter stock symbol (e.g., AAPL): ";
                cin >> symbol;
                cout << "Enter price: ";
                cin >> price;
                stockPrices.insert(symbol, price);
                break;
            case 2:
                cout << "Enter stock symbol to search: ";
                cin >> symbol;
                stockPrices.search(symbol);
                break;
            case 3:
                stockPrices.display();
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            case 5:
                stockPrices.printLevels();
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }while (choice != 4);
    return 0;
}

