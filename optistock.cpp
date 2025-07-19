#include <bits/stdc++.h>
using namespace std;

const int INF = numeric_limits<int>::max();

struct Item {
    std::string particulars;
    std::string quantity;
};

vector<vector<int>> readGraph(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    int V;
    inputFile >> V;
    vector<vector<int>> graph(V, vector<int>(V));

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            inputFile >> graph[i][j];
            if (graph[i][j] == 0 && i != j) {
                graph[i][j] = INF;
            }
        }
    }

    inputFile.close();

    return graph;
}

vector<vector<int>> floydWarshall(const vector<vector<int>>& graph) {
    int V = graph.size();
    vector<vector<int>> dist = graph;

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    return dist;
}

const int prime = 101;

vector<int> searchRabinKarp(const string &text, const string &pattern) {
    vector<int> occurrences;
    int m = pattern.length();
    int n = text.length();
    int patternHash = 0;
    int textHash = 0;

    for (int i = 0; i < m; ++i) {
        patternHash = (patternHash + pattern[i]) % prime;
        textHash = (textHash + text[i]) % prime;
    }

    for (int i = 0; i <= n - m; ++i) {
        if (patternHash == textHash) {
            int j;
            for (j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j])
                    break;
            }
            if (j == m)
                occurrences.push_back(i);
        }
        if (i < n - m) {
            textHash = (textHash - text[i] + text[i + m]) % prime;
            if (textHash < 0)
                textHash += prime;
        }
    }
    return occurrences;
}

void searchInventory(const string &item) {
    ifstream inventoryFile("inventory.txt");
    if (!inventoryFile.is_open()) {
        cout << "Error: Unable to open inventory file." << endl;
        return;
    }

    string line;
    vector<pair<string, string>> foundItems;
    string itemLower = item;
    transform(itemLower.begin(), itemLower.end(), itemLower.begin(), ::tolower);

    while (getline(inventoryFile, line)) {
        string lineLower = line;
        transform(lineLower.begin(), lineLower.end(), lineLower.begin(), ::tolower);
        vector<int> occurrences = searchRabinKarp(lineLower, itemLower);
        if (!occurrences.empty()) {
            size_t pos = line.find(',');
            if (pos != string::npos) {
                string particulars = line.substr(0, pos);
                string quantity = line.substr(pos + 1);
                foundItems.push_back(make_pair(particulars, quantity));
            }
        }
    }

    if (!foundItems.empty()) {
        cout << "\033[36m";
        cout << "+----------------------+----------+" << endl;
        cout << "| Item Particulars    | Quantity  |" << endl;
        cout << "+----------------------+----------+" << endl;
        for (const auto &item : foundItems) {
            cout << "| " << setw(20) << left << item.first << " | " << setw(8) << right << item.second << " |" << endl;
        }
        cout << "+----------------------+----------+" << endl;
    } else {
        cout << "Item not found in inventory." << endl;
    }
    cout << "\033[0m";
}
vector<Item> readInventory() {
    vector<Item> inventory;
    ifstream inFile("inventory.txt");
    if (!inFile.is_open()) {
        cout << "Error: Unable to open inventory file." << endl;
        return inventory;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string particulars, quantity;
        getline(ss, particulars, ',');
        getline(ss, quantity);
        inventory.push_back({particulars, quantity});
    }

    inFile.close();
    return inventory;
}

void writeInventory(const vector<Item>& inventory) {
    ofstream outFile("inventory.txt");
    if (!outFile.is_open()) {
        cout << "Error: Unable to open inventory file for writing." << endl;
        return;
    }

    for (const auto& item : inventory) {
        outFile << item.particulars << "," << item.quantity << endl;
    }

    outFile.close();
}

void addItemToInventory(const string& particulars, const string& quantity) {
    vector<Item> inventory = readInventory();
    inventory.push_back({particulars, quantity});
    writeInventory(inventory);
    cout << "Item added to inventory." << endl;
}

void deleteItemFromInventory(const string& particulars) {
    vector<Item> inventory = readInventory();
    auto it = find_if(inventory.begin(), inventory.end(), [&](const Item& item) {
        return item.particulars == particulars;
    });
    if (it != inventory.end()) {
        inventory.erase(it);
        writeInventory(inventory);
        cout << "Item deleted from inventory." << endl;
    } else {
        cout << "Item not found in inventory." << endl;
    }
}

void upgradeItemInInventory(const string& particulars, const string& newQuantity) {
    vector<Item> inventory = readInventory();
    auto it = find_if(inventory.begin(), inventory.end(), [&](const Item& item) {
        return item.particulars == particulars;
    });
    if (it != inventory.end()) {
        it->quantity = newQuantity;
        writeInventory(inventory);
        cout << "Item upgraded in inventory." << endl;
    } else {
        cout << "Item not found in inventory." << endl;
    }
}

void showFullInventory() {
    vector<Item> inventory = readInventory();
    if (inventory.empty()) {
        cout << "Inventory is empty." << endl;
        return;
    }

    cout << "+----------------------+----------+" << endl;
    cout << "| Item Particulars    | Quantity  |" << endl;
    cout << "+----------------------+----------+" << endl;
    for (const auto& item : inventory) {
        cout << "| " << setw(20) << left << item.particulars << " | " << setw(8) << right << item.quantity << " |" << endl;
    }
    cout << "+----------------------+----------+" << endl;
    cout << "\033[0m";
}



void processOrder() {

    string particularName, quantityStr, city, fileName;

    cout << "Enter name of the item to be delivered: ";
    getline(cin, particularName);
    cout << "Enter quantity: ";
    getline(cin, quantityStr);
    cout << "Enter city to be delivered: ";
    getline(cin, city);
    cout << "Enter file name for the order (without extension): ";
    getline(cin, fileName);

    int quantity = stoi(quantityStr);
    int currentOrderNum;
    ifstream orderNumFile("order_number.txt");
    if (orderNumFile.is_open()) {
        orderNumFile >> currentOrderNum;
        orderNumFile.close();
    } else {
        currentOrderNum = 1;
    }

    string filename = fileName + "_" + to_string(currentOrderNum) + ".txt";
    ofstream orderFile(filename);
    if (!orderFile.is_open()) {
        cout << "Error: Unable to create order file." << endl;
        return;
    }

    orderFile << "Order Number: " << currentOrderNum << endl;
    orderFile << "Particular Name: " << particularName << endl;
    orderFile << "Quantity: " << quantity << endl;
    orderFile << "City to be Delivered: " << city << endl;

    orderFile.close();

    ofstream updateOrderNumFile("order_number.txt");
    if (updateOrderNumFile.is_open()) {
        updateOrderNumFile << currentOrderNum + 1;
        updateOrderNumFile.close();
    } else {
        cout << "Error: Unable to update order number." << endl;
    }

    // Update inventory
    vector<Item> inventory = readInventory();
    auto it = find_if(inventory.begin(), inventory.end(), [&](const Item& item) {
        return item.particulars == particularName;
    });
    if (it != inventory.end()) {
        int currentQuantity = stoi(it->quantity);
        if (currentQuantity >= quantity) {
            currentQuantity -= quantity;
            it->quantity = to_string(currentQuantity);
            writeInventory(inventory);
            cout << "Order processed successfully. Order number: " << currentOrderNum << endl;
        } else {
            cout << "Not enough inventory to process the order." << endl;
        }
    } else {
        cout << "Item not found in inventory." << endl;
    }
}



void greedyRestocking() {
    vector<Item> inventory = readInventory();
    int threshold;
    cout << "Enter the restocking threshold: ";
    cin >> threshold;

    vector<Item> restockItems;
    for (const auto& item : inventory) {
        int quantity = stoi(item.quantity);
        if (quantity < threshold) {
            restockItems.push_back(item);
        }
    }

    if (!restockItems.empty()) {
        cout << "\033[36m";
        cout << "+----------------------+----------+" << endl;
        cout << "| Item Particulars    | Quantity  |" << endl;
        cout << "+----------------------+----------+" << endl;
        for (const auto& item : restockItems) {
            cout << "| " << setw(20) << left << item.particulars << " | " << setw(8) << right << item.quantity << " |" << endl;
        }
        cout << "+----------------------+----------+" << endl;
    } else {
        cout << "No items need restocking." << endl;
    }
    cout << "\033[0m";
}
void backtrackingOptimization() {
    vector<Item> inventory = readInventory();
    int maxItems;
    cout << "Enter the maximum number of items to keep in stock: ";
    cin >> maxItems;

    vector<Item> optimalItems;
    vector<bool> included(inventory.size(), false);
    long long maxTotalQty = 0; // Track the maximum total quantity

    // Backtracking function to explore subsets
    function<void(int, int, long long)> backtrack = [&](int start, int count, long long totalQty) {
        // Base case: if we've selected maxItems items
        if (count == maxItems) {
            vector<Item> currentItems;
            for (int i = 0; i < inventory.size(); ++i) {
                if (included[i]) {
                    currentItems.push_back(inventory[i]);
                }
            }
            // Update optimalItems if the current subset has a higher total quantity
            if (totalQty > maxTotalQty) {
                maxTotalQty = totalQty;
                optimalItems = currentItems;
            }
            return;
        }

        // Explore including/excluding each item
        for (int i = start; i < inventory.size(); ++i) {
            included[i] = true;
            backtrack(i + 1, count + 1, totalQty + stoll(inventory[i].quantity));
            included[i] = false;
        }
    };

    // Start backtracking with initial count and total quantity as 0
    backtrack(0, 0, 0);

    // Display the optimal subset
    if (!optimalItems.empty()) {
        cout << "\033[36m";
        cout << "+----------------------+----------+" << endl;
        cout << "| Item Particulars    | Quantity  |" << endl;
        cout << "+----------------------+----------+" << endl;
        for (const auto& item : optimalItems) {
            cout << "| " << setw(20) << left << item.particulars << " | " << setw(8) << right << item.quantity << " |" << endl;
        }
        cout << "+----------------------+----------+" << endl;
        cout << "Total Quantity: " << maxTotalQty << endl;
    } else {
        cout << "No optimal items found." << endl;
    }
    cout << "\033[0m";
}

void divideAndConquerAnalysis() {
    vector<Item> inventory = readInventory();

    std::function<Item(const vector<Item>&, int, int)> maxItem = [&](const vector<Item>& items, int low, int high) -> Item {
    if (low == high) {
        return items[low];
    }

        int mid = (low + high) / 2;
        Item leftMax = maxItem(items, low, mid);
        Item rightMax = maxItem(items, mid + 1, high);

        return stoi(leftMax.quantity) > stoi(rightMax.quantity) ? leftMax : rightMax;
    };

    Item maxQuantityItem = maxItem(inventory, 0, inventory.size() - 1);

    cout << "Item with the highest quantity:" << endl;
    cout << "+----------------------+----------+" << endl;
    cout << "| Item Particulars    | Quantity  |" << endl;
    cout << "+----------------------+----------+" << endl;
    cout << "| " << setw(20) << left << maxQuantityItem.particulars << " | " << setw(8) << right << maxQuantityItem.quantity << " |" << endl;
    cout << "+----------------------+----------+" << endl;
    cout << "\033[0m";
}
void manageInventory() {
    cout << "\033[32m"<<endl;
    cout << "1. Add Item" << endl;
    cout << "\033[31m";
    cout << "2. Delete Item" << endl;
    cout << "\033[33m";
    cout << "3. Upgrade Item" << endl;
    cout << "4. Show Full Inventory" << endl;
    cout << "5. Greedy Algorithm for Inventory Restocking" << endl;
    cout << "6. Backtracking for Inventory Optimization" << endl;
    cout << "7. Divide-and-Conquer for Inventory Analysis" << endl;
    cout << "\033[0m";
    cout << "Enter option: ";
    int option;
    cin >> option;
    cin.ignore();

    switch (option) {
        case 1:
            {
                string particulars, quantity;
                cout << "Enter item particulars: ";
                getline(cin, particulars);
                cout << "Enter item quantity: ";
                getline(cin, quantity);
                addItemToInventory(particulars, quantity);
                break;
            }
        case 2:
            {
                string particulars;
                cout << "Enter item particulars to delete: ";
                getline(cin, particulars);
                deleteItemFromInventory(particulars);
                break;
            }
        case 3:
            {
                string particulars, newQuantity;
                cout << "Enter item particulars to upgrade: ";
                getline(cin, particulars);
                cout << "Enter new quantity: ";
                getline(cin, newQuantity);
                upgradeItemInInventory(particulars, newQuantity);
                break;
            }
        case 4:
            showFullInventory();
            break;
        case 5:
            greedyRestocking();
            break;
        case 6:
            backtrackingOptimization();
            break;
        case 7:
            divideAndConquerAnalysis();
            break;
        default:
            cout << "Invalid option." << endl;
    }
}

int main() {
    char ch;
    do {
        cout << "\n\n\n";
        cout << "\033[32m";
        cout << "\t\t\t MAIN MENU" << endl;
        cout << "\033[0m";
        cout << "------------------------------------------------------------------------" << endl;
        cout << "1. \t\t\tMANAGE INVENTORY\t[M]\t" << endl;
        cout << "2. \t\t\tPROCESS ORDERS\t\t[P]\t" << endl;
        cout << "3. \t\t\tOPTIMIZE ROUTES\t\t[O]\t" << endl;
        cout << "4. \t\t\tSEARCH INVENTORY\t[S]\t" << endl;
        cout << "5. \t\t\tQUIT\t\t\t[Q]\t" << endl;
        cout << "-------------------------------------------------------------------------" << endl;

        cout << "\033[32m";
        cout << "CHOOSE A SUITABLE OPTION" << endl;
        cout << "\033[0m";

        cin >> ch;

        string searchItem;

        switch (ch) {

        case 'M':
            cin.ignore();
            manageInventory();
            break;

        case 'P':
            cin.ignore();
            processOrder();
            break;

        case 'S':
            cin.ignore();
            cout << "Enter the item you want to search: ";
            getline(cin, searchItem);
            searchInventory(searchItem);
            break;

        case 'O':
            {
                                 vector<vector<int>> graph = readGraph("graph.txt");
                vector<vector<int>> shortestPaths = floydWarshall(graph);

                // City names mapped to vertices
                vector<string> cities = {
                    "Itanagar(0)", "Dispur(1)", "Patna(2)", "Raipur(3)", "Panaji(4)", "Gandhinagar(5)",
                    "Chandigarh(6)", "Shimla(7)", "Ranchi(8)", "Bengaluru(9)", "Thiruvananthapuram(10)",
                    "Bhopal(11)", "Mumbai(12)", "Imphal(13)", "Shillong(14)", "Aizawl(15)", "Kohima(16)",
                    "Bhubaneswar(17)", "Jaipur(18)", "Gangtok(19)", "Chennai(20)", "Hyderabad(21)",
                    "Agartala(22)", "Lucknow(23)", "Dehradun(24)", "Kolkata(25)", "New Delhi(26)"
                };

                cout << "Cities: " << endl;
                for (size_t i = 0; i < cities.size(); ++i) {
                    cout << i << ": " << cities[i] << endl;
                }

                int src, dest;
                cout << "Enter source city index: ";
                cin >> src;
                cout << "Enter destination city index: ";
                cin >> dest;

                if (src < 0 || src >= cities.size() || dest < 0 || dest >= cities.size()) {
                    cout << "Invalid city index. Please enter indices in the range [0, " << cities.size() - 1 << "]." << endl;
                    break;
                }

                if (shortestPaths[src][dest] == INF) {
                    cout << "No path found from " << cities[src] << " to " << cities[dest] << endl;
                } else {
                    cout << "Shortest distance from " << cities[src] << " to " << cities[dest] << ": " << shortestPaths[src][dest] << " km" << endl;
                }

                break;

            }

        case 'Q':
            cout << "Exiting program." << endl;
            break;

        default:
            cout << "\033[31m";
            cout << "Enter Right Option" << endl;
            cout << "\033[0m";
        }
    } while (ch != 'Q');

    return 0;
}
