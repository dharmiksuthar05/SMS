/*
 * ==============================================================
 *  Project   : Student / Library Management System
 *  Subject   : DSA with C++
 *  Author    : Dharmik Suthar
 *
 *  Description:
 *  A menu-driven program split into three modules.
 *
 *  Module 1 : Inventory Analysis (Arrays & Vectors)
 *      1. Positive / Negative price count
 *      2. Odd / Even ID count
 *      3. Sum, Average, Min, Max price
 *      4. Remove duplicate IDs
 *      5. Palindrome IDs and Leader books
 *
 *  Module 2 : The Sorting Studio
 *      Bubble, Selection, Insertion, Merge and Quick Sort (by ID),
 *      Binary Search on the sorted list and Closest-ID suggestion.
 *
 *  Module 3 : Digital Logbook
 *      Membership List  -> Singly Linked List (insert/update/delete/traverse)
 *      Book Return Desk -> Stack (LIFO)
 *      Issue Counter    -> Queue (FIFO)
 *
 *  Assumptions:
 *  - Book ID is an integer, price is a decimal number.
 *  - A price <= 0 is treated as an invalid (wrong) entry.
 *  - Remove Duplicates keeps the FIRST book of every ID.
 *  - A Leader book has a price strictly greater than every book
 *    after it; the last book is always a leader.
 *  - Sorting is done on Book ID so that Binary Search can work.
 *  - Bad input (letters typed where a number is expected) is caught
 *    and the user is asked again.
 * ==============================================================
 */

#include <iostream>   // cin, cout
#include <string>     // std::string
#include <vector>     // std::vector - the book list
#include <limits>     // numeric_limits - used to clear bad input from cin
#include <iomanip>    // setw, setprecision - table formatting
using namespace std;

/* --------------------------------------------------------------
 *  Input helpers
 * -------------------------------------------------------------- */

// Keeps asking until the user types a valid integer.
int readInt(const string &prompt)
{
    int value;
    while (true)                                                   // repeat until valid input
    {
        cout << prompt;                                            // show the question
        if (cin >> value)                                          // read succeeded (it was a number)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');   // drop the leftover Enter key
            return value;
        }
        cout << "  Invalid input! Please enter a whole number.\n";
        cin.clear();                                               // reset cin's error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');       // throw away the bad text
    }
}

// Keeps asking until the user types a valid decimal number.
double readDouble(const string &prompt)
{
    double value;
    while (true)                                                   // repeat until valid input
    {
        cout << prompt;
        if (cin >> value)                                          // read succeeded
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');   // drop the leftover Enter key
            return value;
        }
        cout << "  Invalid input! Please enter a number.\n";
        cin.clear();                                               // reset cin's error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');       // throw away the bad text
    }
}

// Reads a full line (allows spaces, e.g. "Rahul Patel").
string readLine(const string &prompt)
{
    string text;
    while (true)
    {
        cout << prompt;
        getline(cin, text);          // read the whole line including spaces
        if (!text.empty())           // accept only non-empty text
            return text;
        cout << "  This field cannot be empty.\n";
    }
}

/* ==============================================================
 *  MODULE 1 : Inventory Analysis (Arrays & Vectors)
 * ============================================================== */

/*
 *  Struct : Book
 *    id    -> unique book number
 *    price -> book price (should be positive)
 */
struct Book
{
    int id;         // book ID
    double price;   // book price
};

// Prints the book list as a table.
void displayBooks(const vector<Book> &books)
{
    if (books.empty())                                   // nothing to show
    {
        cout << "  Book list is empty.\n";
        return;
    }
    // table header
    cout << "\n  " << left << setw(6) << "No." << setw(10) << "ID" << "Price\n";
    cout << "  ---------------------------\n";
    for (size_t i = 0; i < books.size(); i++)            // visit every book
    {
        // print serial number, ID and price (2 decimal places)
        cout << "  " << left << setw(6) << i + 1 << setw(10) << books[i].id
             << fixed << setprecision(2) << books[i].price << "\n";
    }
}

// Data Entry : user decides how many books to add.
void enterBooks(vector<Book> &books)
{
    int n = readInt("  How many books do you want to add? ");
    if (n <= 0)                                  // nothing to add
    {
        cout << "  Nothing to add.\n";
        return;
    }
    for (int i = 0; i < n; i++)                  // take details of each book
    {
        cout << "  Book " << i + 1 << ":\n";
        Book b;
        b.id = readInt("    ID    : ");          // read ID
        b.price = readDouble("    Price : ");    // read price
        books.push_back(b);                      // add the book at the end of the vector
    }
    cout << "  " << n << " book(s) added.\n";
}

// Task 1 : count valid (positive) and invalid (zero / negative) prices.
void countPrices(const vector<Book> &books)
{
    int positive = 0, negative = 0;     // counters
    for (const Book &b : books)         // check every book
    {
        if (b.price > 0)
            positive++;                 // valid price
        else
            negative++;                 // 0 or negative = wrong entry
    }
    cout << "  Valid prices (Positive)      : " << positive << "\n";
    cout << "  Wrong prices (Zero/Negative) : " << negative << "\n";
}

// Task 2 : count odd and even IDs.
void countOddEven(const vector<Book> &books)
{
    int odd = 0, even = 0;              // counters
    for (const Book &b : books)
    {
        if (b.id % 2 == 0)              // remainder 0 when divided by 2 -> even
            even++;
        else
            odd++;
    }
    cout << "  Even ID books : " << even << "\n";
    cout << "  Odd ID books  : " << odd << "\n";
}

// Task 3 : total value, average, costliest and cheapest book.
void analytics(const vector<Book> &books)
{
    if (books.empty())                                  // avoid dividing by zero
    {
        cout << "  Book list is empty.\n";
        return;
    }
    double sum = 0;                                     // running total of prices
    int maxIdx = 0, minIdx = 0;                         // start by assuming book 0 is max and min
    for (size_t i = 0; i < books.size(); i++)
    {
        sum += books[i].price;                          // add price to total
        if (books[i].price > books[maxIdx].price)       // found a costlier book
            maxIdx = i;
        if (books[i].price < books[minIdx].price)       // found a cheaper book
            minIdx = i;
    }
    cout << fixed << setprecision(2);                   // show 2 decimal places
    cout << "  Total Value (Sum) : " << sum << "\n";
    cout << "  Average Price     : " << sum / books.size() << "\n";   // average = sum / count
    cout << "  Costliest Book    : ID " << books[maxIdx].id << " (" << books[maxIdx].price << ")\n";
    cout << "  Cheapest Book     : ID " << books[minIdx].id << " (" << books[minIdx].price << ")\n";
}

// Task 4 : remove books whose ID already appeared earlier (keeps first).
void removeDuplicates(vector<Book> &books)
{
    vector<Book> unique;                        // books with no repeated ID
    for (const Book &b : books)                 // take each book one by one
    {
        bool found = false;
        for (const Book &u : unique)            // has this ID been kept already?
        {
            if (u.id == b.id)
            {
                found = true;                   // duplicate found
                break;
            }
        }
        if (!found)
            unique.push_back(b);                // first time we see this ID -> keep it
    }
    int removed = books.size() - unique.size(); // how many were dropped
    books = unique;                             // replace old list with the cleaned list
    cout << "  " << removed << " duplicate book(s) removed.\n";
}

// Returns true if the number reads the same forwards and backwards.
bool isPalindrome(int num)
{
    if (num < 0)                                // negative numbers are not palindromes
        return false;
    int original = num, reversed = 0;
    while (num > 0)
    {
        reversed = reversed * 10 + num % 10;    // append the last digit to reversed
        num /= 10;                              // remove the last digit
    }
    return original == reversed;                // same both ways -> palindrome
}

// Task 5a : list all palindrome IDs.
void palindromeIds(const vector<Book> &books)
{
    bool any = false;                   // did we find at least one?
    cout << "  Palindrome IDs : ";
    for (const Book &b : books)
    {
        if (isPalindrome(b.id))
        {
            cout << b.id << " ";        // print matching ID
            any = true;
        }
    }
    cout << (any ? "\n" : "None\n");    // print "None" if nothing matched
}

// Task 5b : Leader books - scan from right, keep track of max price so far.
void leaderBooks(const vector<Book> &books)
{
    if (books.empty())
    {
        cout << "  Book list is empty.\n";
        return;
    }
    vector<Book> leaders;
    double maxRight = books.back().price;               // highest price seen from the right
    leaders.push_back(books.back());                    // last book is always a leader
    for (int i = (int)books.size() - 2; i >= 0; i--)    // move from right to left
    {
        if (books[i].price > maxRight)                  // greater than all books after it
        {
            leaders.push_back(books[i]);                // it is a leader
            maxRight = books[i].price;                  // update the max
        }
    }
    cout << "  Leader Books (left to right):\n";
    for (int i = (int)leaders.size() - 1; i >= 0; i--)  // print in reverse to get original order
        cout << "    ID " << leaders[i].id << " -> " << fixed << setprecision(2) << leaders[i].price << "\n";
}

// Menu for Module 1.
void inventoryMenu(vector<Book> &books)
{
    while (true)                        // keep showing the menu until user picks 0
    {
        cout << "\n----- Module 1 : Inventory Analysis -----\n"
             << "  1. Add Books\n"
             << "  2. Display Books\n"
             << "  3. Positive / Negative Price Count\n"
             << "  4. Odd / Even ID Count\n"
             << "  5. Analytics (Sum, Average, Min, Max)\n"
             << "  6. Remove Duplicates\n"
             << "  7. Palindrome IDs\n"
             << "  8. Leader Books\n"
             << "  0. Back\n";
        int ch = readInt("  Enter choice: ");
        switch (ch)                     // call the function for the chosen task
        {
        case 1: enterBooks(books); break;
        case 2: displayBooks(books); break;
        case 3: countPrices(books); break;
        case 4: countOddEven(books); break;
        case 5: analytics(books); break;
        case 6: removeDuplicates(books); break;
        case 7: palindromeIds(books); break;
        case 8: leaderBooks(books); break;
        case 0: return;                 // go back to main menu
        default: cout << "  Invalid choice!\n";
        }
    }
}

/* ==============================================================
 *  MODULE 2 : The Sorting Studio (all sorts are by Book ID)
 * ============================================================== */

// Bubble Sort : repeatedly swap neighbours that are in the wrong order.
void bubbleSort(vector<Book> &a)
{
    int n = a.size();
    for (int i = 0; i < n - 1; i++)                 // n-1 passes
    {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++)         // last i elements are already in place
        {
            if (a[j].id > a[j + 1].id)              // neighbours out of order
            {
                swap(a[j], a[j + 1]);               // swap them
                swapped = true;
            }
        }
        if (!swapped)   // already sorted - stop early
            break;
    }
}

// Selection Sort : pick the smallest remaining ID and put it in front.
void selectionSort(vector<Book> &a)
{
    int n = a.size();
    for (int i = 0; i < n - 1; i++)
    {
        int minIdx = i;                         // assume current position is the smallest
        for (int j = i + 1; j < n; j++)         // search the unsorted part
            if (a[j].id < a[minIdx].id)
                minIdx = j;                     // remember the smaller one
        swap(a[i], a[minIdx]);                  // place smallest at position i
    }
}

// Insertion Sort : insert each book into its correct place in the sorted left part.
void insertionSort(vector<Book> &a)
{
    int n = a.size();
    for (int i = 1; i < n; i++)
    {
        Book key = a[i];                        // book to be inserted
        int j = i - 1;
        while (j >= 0 && a[j].id > key.id)      // shift bigger IDs one step right
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;                         // drop the key into the gap
    }
}

// Merges two sorted halves a[low..mid] and a[mid+1..high].
void merge(vector<Book> &a, int low, int mid, int high)
{
    vector<Book> temp;                          // holds the merged result
    int i = low, j = mid + 1;                   // i -> left half, j -> right half
    while (i <= mid && j <= high)               // pick the smaller of the two fronts
    {
        if (a[i].id <= a[j].id)
            temp.push_back(a[i++]);
        else
            temp.push_back(a[j++]);
    }
    while (i <= mid)                            // copy any remaining left items
        temp.push_back(a[i++]);
    while (j <= high)                           // copy any remaining right items
        temp.push_back(a[j++]);
    for (int k = low; k <= high; k++)           // copy merged result back into a
        a[k] = temp[k - low];
}

// Merge Sort : divide the list in half, sort both halves, then merge.
void mergeSort(vector<Book> &a, int low, int high)
{
    if (low >= high)                            // 0 or 1 element -> already sorted
        return;
    int mid = low + (high - low) / 2;           // middle index (avoids overflow)
    mergeSort(a, low, mid);                     // sort left half
    mergeSort(a, mid + 1, high);                // sort right half
    merge(a, low, mid, high);                   // join the two sorted halves
}

// Lomuto partition : last element is the pivot.
int partition(vector<Book> &a, int low, int high)
{
    int pivot = a[high].id;                     // choose last ID as pivot
    int i = low - 1;                            // boundary of "smaller than pivot" area
    for (int j = low; j < high; j++)
    {
        if (a[j].id < pivot)                    // belongs on the left side
        {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[high]);                    // put pivot in its final position
    return i + 1;                               // return pivot index
}

// Quick Sort : partition around a pivot, then sort both sides.
void quickSort(vector<Book> &a, int low, int high)
{
    if (low < high)
    {
        int p = partition(a, low, high);        // pivot is now at index p
        quickSort(a, low, p - 1);               // sort left of pivot
        quickSort(a, p + 1, high);              // sort right of pivot
    }
}

/*
 *  Binary Search on a list sorted by ID.
 *  Returns the index if found, otherwise -1.
 *  When not found, low ends at the insert position, so the closest
 *  ID is either a[low - 1] or a[low].
 */
int binarySearch(const vector<Book> &a, int key, int &closestIdx)
{
    int low = 0, high = (int)a.size() - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;       // middle index
        if (a[mid].id == key)                   // found it
            return mid;
        if (a[mid].id < key)                    // key is in the right half
            low = mid + 1;
        else                                    // key is in the left half
            high = mid - 1;
    }

    // Closest Number : compare the neighbours around the insert position.
    if (low >= (int)a.size())                   // key is bigger than every ID
        closestIdx = a.size() - 1;
    else if (low == 0)                          // key is smaller than every ID
        closestIdx = 0;
    else                                        // pick whichever neighbour is nearer
        closestIdx = (key - a[low - 1].id <= a[low].id - key) ? low - 1 : low;
    return -1;                                  // not found
}

// Asks for an ID, searches it and suggests the closest ID if missing.
void searchBook(const vector<Book> &books)
{
    int key = readInt("  Enter Book ID to search: ");
    int closest = -1;                                   // filled by binarySearch if not found
    int idx = binarySearch(books, key, closest);
    if (idx != -1)                                      // ID found
    {
        cout << "  Found! Position " << idx + 1 << " -> ID " << books[idx].id
             << ", Price " << fixed << setprecision(2) << books[idx].price << "\n";
    }
    else                                                // ID not found -> suggest closest
    {
        cout << "  ID " << key << " not found.\n";
        cout << "  Did you mean ID " << books[closest].id
             << " (Price " << fixed << setprecision(2) << books[closest].price << ")?\n";
    }
}

// Menu for Module 2.
void sortingMenu(vector<Book> &books)
{
    if (books.empty())                                  // need books before sorting
    {
        cout << "  Book list is empty. Add books in Module 1 first.\n";
        return;
    }
    while (true)
    {
        cout << "\n----- Module 2 : The Sorting Studio -----\n"
             << "  1. Bubble Sort\n"
             << "  2. Selection Sort\n"
             << "  3. Insertion Sort\n"
             << "  4. Merge Sort\n"
             << "  5. Quick Sort\n"
             << "  0. Back\n";
        int ch = readInt("  Choose a sorting method: ");
        int n = books.size();
        switch (ch)                                     // run the chosen sorting method
        {
        case 1: bubbleSort(books); break;
        case 2: selectionSort(books); break;
        case 3: insertionSort(books); break;
        case 4: mergeSort(books, 0, n - 1); break;
        case 5: quickSort(books, 0, n - 1); break;
        case 0: return;
        default: cout << "  Invalid choice!\n"; continue;   // skip search on bad choice
        }
        cout << "  Books sorted by ID:";
        displayBooks(books);                            // show sorted list
        searchBook(books);                              // then search in it
    }
}

/* ==============================================================
 *  MODULE 3 : Digital Logbook (Linked List, Stack & Queue)
 * ============================================================== */

/*
 *  Class : Member  (self-referential class)
 *    rollNo -> unique member number
 *    name   -> student name
 *    course -> student course / class
 *    next   -> pointer to the next Member in the list
 */
class Member
{
public:
    int rollNo;
    string name;
    string course;
    Member *next;   // points to an object of the same class (self-referential)

    // Constructor : fills the details, next is empty at first.
    Member(int r, string n, string c) : rollNo(r), name(n), course(c), next(nullptr) {}
};

/*
 *  Class : MemberList  (Singly Linked List)
 */
class MemberList
{
    Member *head;   // first node of the list

    // Returns the node with this roll number, or nullptr if not found.
    Member *find(int rollNo)
    {
        Member *temp = head;
        while (temp != nullptr && temp->rollNo != rollNo)   // walk until match or end
            temp = temp->next;
        return temp;
    }

public:
    MemberList() : head(nullptr) {}     // list starts empty

    // Destructor : free every node to avoid memory leaks.
    ~MemberList()
    {
        while (head != nullptr)
        {
            Member *temp = head;        // remember current node
            head = head->next;          // move head forward
            delete temp;                // free the old node
        }
    }

    // Insert at the end of the list.
    void insert(int rollNo, const string &name, const string &course)
    {
        if (find(rollNo) != nullptr)                    // roll number must be unique
        {
            cout << "  Roll No " << rollNo << " already exists!\n";
            return;
        }
        Member *node = new Member(rollNo, name, course);    // create new node
        if (head == nullptr)                            // empty list -> new node is head
            head = node;
        else
        {
            Member *temp = head;
            while (temp->next != nullptr)               // go to the last node
                temp = temp->next;
            temp->next = node;                          // link new node at the end
        }
        cout << "  Member added.\n";
    }

    // Update name and course of a member.
    void update(int rollNo)
    {
        Member *m = find(rollNo);
        if (m == nullptr)
        {
            cout << "  Member not found!\n";
            return;
        }
        m->name = readLine("  New Name   : ");          // overwrite name
        m->course = readLine("  New Course : ");        // overwrite course
        cout << "  Record updated.\n";
    }

    // Delete a member by roll number.
    void remove(int rollNo)
    {
        Member *temp = head, *prev = nullptr;           // prev trails one step behind temp
        while (temp != nullptr && temp->rollNo != rollNo)
        {
            prev = temp;
            temp = temp->next;
        }
        if (temp == nullptr)                            // reached end without a match
        {
            cout << "  Member not found!\n";
            return;
        }
        if (prev == nullptr)                            // deleting the first node
            head = temp->next;
        else                                            // skip over the node to delete
            prev->next = temp->next;
        delete temp;                                    // free memory
        cout << "  Member deleted.\n";
    }

    // Traverse : print every member from head to end.
    void traverse()
    {
        if (head == nullptr)
        {
            cout << "  No members yet.\n";
            return;
        }
        cout << "\n  " << left << setw(10) << "Roll No" << setw(20) << "Name" << "Course\n";
        cout << "  ----------------------------------------\n";
        for (Member *temp = head; temp != nullptr; temp = temp->next)   // visit each node
            cout << "  " << left << setw(10) << temp->rollNo << setw(20) << temp->name << temp->course << "\n";
    }
};

/*
 *  Class : ReturnStack  (Book Return Desk - LIFO)
 *  Implemented with a linked list; top is the head node.
 */
class ReturnStack
{
    struct Node
    {
        Book book;      // returned book
        Node *next;     // node below this one
    };
    Node *top;          // book on top of the pile

public:
    ReturnStack() : top(nullptr) {}     // pile starts empty

    // Destructor : free all remaining nodes.
    ~ReturnStack()
    {
        while (top != nullptr)
        {
            Node *temp = top;
            top = top->next;
            delete temp;
        }
    }

    bool isEmpty() { return top == nullptr; }

    // Push : new book goes on top of the pile.
    void push(Book b)
    {
        top = new Node{b, top};         // new node points to old top, then becomes top
        cout << "  Book ID " << b.id << " placed on the return pile.\n";
    }

    // Pop : remove the top book (last in, first out).
    void pop()
    {
        if (isEmpty())
        {
            cout << "  Return pile is empty (Stack Underflow).\n";
            return;
        }
        Node *temp = top;
        cout << "  Book ID " << temp->book.id << " moved to the shelf.\n";
        top = top->next;                // next book becomes top
        delete temp;                    // free old top
    }

    // Peek : look at the top book without removing it.
    void peek()
    {
        if (isEmpty())
            cout << "  Return pile is empty.\n";
        else
            cout << "  Top book : ID " << top->book.id << "\n";
    }

    // Display the pile from top to bottom.
    void display()
    {
        if (isEmpty())
        {
            cout << "  Return pile is empty.\n";
            return;
        }
        cout << "  Pile (top -> bottom): ";
        for (Node *temp = top; temp != nullptr; temp = temp->next)
            cout << "[" << temp->book.id << "] ";
        cout << "\n";
    }
};

/*
 *  Class : IssueQueue  (Issue Counter - FIFO)
 *  Implemented with a linked list; front = dequeue end, rear = enqueue end.
 */
class IssueQueue
{
    struct Node
    {
        string student;     // student waiting in line
        int bookId;         // book the student wants
        Node *next;         // student behind this one
    };
    Node *front, *rear;     // front = served next, rear = last to join

public:
    IssueQueue() : front(nullptr), rear(nullptr) {}     // line starts empty

    // Destructor : free all remaining nodes.
    ~IssueQueue()
    {
        while (front != nullptr)
        {
            Node *temp = front;
            front = front->next;
            delete temp;
        }
    }

    bool isEmpty() { return front == nullptr; }

    // Enqueue : student joins at the rear of the line.
    void enqueue(const string &student, int bookId)
    {
        Node *node = new Node{student, bookId, nullptr};
        if (rear == nullptr)            // empty line -> node is both front and rear
            front = rear = node;
        else
        {
            rear->next = node;          // link after the current last student
            rear = node;                // new student is now the rear
        }
        cout << "  " << student << " joined the line.\n";
    }

    // Dequeue : serve the student at the front (first in, first out).
    void dequeue()
    {
        if (isEmpty())
        {
            cout << "  No one is in the line (Queue Underflow).\n";
            return;
        }
        Node *temp = front;
        cout << "  Book ID " << temp->bookId << " issued to " << temp->student << ".\n";
        front = front->next;            // next student moves to front
        if (front == nullptr)           // line became empty
            rear = nullptr;
        delete temp;                    // free served node
    }

    // Display the line from front to rear.
    void display()
    {
        if (isEmpty())
        {
            cout << "  Line is empty.\n";
            return;
        }
        cout << "  Line (front -> rear):\n";
        int pos = 1;                    // position number in the line
        for (Node *temp = front; temp != nullptr; temp = temp->next)
            cout << "    " << pos++ << ". " << temp->student << " (wants Book ID " << temp->bookId << ")\n";
    }
};

// Menu for the Membership List (Linked List).
void membershipMenu(MemberList &members)
{
    while (true)
    {
        cout << "\n----- Membership List (Linked List) -----\n"
             << "  1. Add Member\n"
             << "  2. Update Member\n"
             << "  3. Delete Member\n"
             << "  4. Show All Members\n"
             << "  0. Back\n";
        int ch = readInt("  Enter choice: ");
        switch (ch)
        {
        case 1:
        {
            int r = readInt("  Roll No : ");            // read new member details
            string n = readLine("  Name    : ");
            string c = readLine("  Course  : ");
            members.insert(r, n, c);                    // add to linked list
            break;
        }
        case 2: members.update(readInt("  Roll No to update: ")); break;
        case 3: members.remove(readInt("  Roll No to delete: ")); break;
        case 4: members.traverse(); break;
        case 0: return;
        default: cout << "  Invalid choice!\n";
        }
    }
}

// Menu for the Book Return Desk (Stack).
void returnDeskMenu(ReturnStack &desk)
{
    while (true)
    {
        cout << "\n----- Book Return Desk (Stack - LIFO) -----\n"
             << "  1. Return a Book (Push)\n"
             << "  2. Shelve Top Book (Pop)\n"
             << "  3. Peek Top Book\n"
             << "  4. Show Pile\n"
             << "  0. Back\n";
        int ch = readInt("  Enter choice: ");
        switch (ch)
        {
        case 1:
        {
            Book b;
            b.id = readInt("  Book ID : ");             // only the ID is needed at the desk
            b.price = 0;
            desk.push(b);                               // put on top of the pile
            break;
        }
        case 2: desk.pop(); break;
        case 3: desk.peek(); break;
        case 4: desk.display(); break;
        case 0: return;
        default: cout << "  Invalid choice!\n";
        }
    }
}

// Menu for the Issue Counter (Queue).
void issueCounterMenu(IssueQueue &counter)
{
    while (true)
    {
        cout << "\n----- Issue Counter (Queue - FIFO) -----\n"
             << "  1. Join Line (Enqueue)\n"
             << "  2. Issue Book to Next Student (Dequeue)\n"
             << "  3. Show Line\n"
             << "  0. Back\n";
        int ch = readInt("  Enter choice: ");
        switch (ch)
        {
        case 1:
        {
            string s = readLine("  Student Name : ");   // who is joining
            int id = readInt("  Book ID      : ");      // which book they want
            counter.enqueue(s, id);                     // add to the end of the line
            break;
        }
        case 2: counter.dequeue(); break;
        case 3: counter.display(); break;
        case 0: return;
        default: cout << "  Invalid choice!\n";
        }
    }
}

// Menu for Module 3.
void logbookMenu(MemberList &members, ReturnStack &desk, IssueQueue &counter)
{
    while (true)
    {
        cout << "\n----- Module 3 : Digital Logbook -----\n"
             << "  1. Membership List (Linked List)\n"
             << "  2. Book Return Desk (Stack)\n"
             << "  3. Issue Counter (Queue)\n"
             << "  0. Back\n";
        int ch = readInt("  Enter choice: ");
        switch (ch)
        {
        case 1: membershipMenu(members); break;
        case 2: returnDeskMenu(desk); break;
        case 3: issueCounterMenu(counter); break;
        case 0: return;
        default: cout << "  Invalid choice!\n";
        }
    }
}

/* ==============================================================
 *  main
 * ============================================================== */
int main()
{
    vector<Book> books;     // book list shared by Module 1 and Module 2
    MemberList members;     // linked list of students
    ReturnStack desk;       // stack of returned books
    IssueQueue counter;     // queue of students waiting for books

    while (true)            // main menu loop
    {
        cout << "\n========== Student Management System ==========\n"
             << "  1. Inventory Analysis (Arrays & Vectors)\n"
             << "  2. The Sorting Studio\n"
             << "  3. Digital Logbook (Linked List, Stack, Queue)\n"
             << "  0. Exit\n";
        int ch = readInt("  Enter choice: ");
        switch (ch)
        {
        case 1: inventoryMenu(books); break;
        case 2: sortingMenu(books); break;
        case 3: logbookMenu(members, desk, counter); break;
        case 0:
            cout << "  Thank you! Goodbye.\n";
            return 0;       // end the program
        default: cout << "  Invalid choice!\n";
        }
    }
}
