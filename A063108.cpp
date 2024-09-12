#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <ctime>
#include <set>
#include <cstring>
#include <cmath>

using namespace std;

// Question 2
unsigned long long radix = 10;
unsigned long long terms = 200000;

map<unsigned long long, unsigned long long> digitProductMemo;
map<unsigned long long, set<unsigned long long>> preimages;

/**
 * Helper method to identify the product of a given term's digits. Start from the right and go left.
 * Precondition: Current term identified.
 * Postcondition: Identifies next term.
 */
unsigned long long productOfNonzeroDigits(unsigned long long num) {
    unsigned long long product = 1;
    while (num > 0) {
        unsigned long long digit = num % radix;
        if (digit != 0) {
            product *= digit;
        }
        num /= radix;
    }

    digitProductMemo[num] = product;
    return product;
}

/**
 * Identifies the start and end bases to work with and then answers questions found in the Loomis paper.
 * Precondition: Base 1 sequence generated.
 * Postcondition: Answers questions 1 (unattainable size), 2 (radices/bases), and 4 (preimages) found from Loomis paper. 
 */
void calcBaseN(set<unsigned long long> base1Elts) {
    ofstream outFile("convergences.txt");
    unsigned long long startBase;
    unsigned long long endBase;
    cout << "Start base? ";
    cin >> startBase;
    cout << "End base? ";
    cin >> endBase;

    unsigned long long newBaseTerms[terms];
    set<unsigned long long> newBaseSet;
    set<unsigned long long> unattainables;

    for (unsigned long long i = 1; i < endBase + 1; i++) {
        unattainables.insert(i);
    }

    unsigned long long base = startBase;

    // Map to store the minimum base for each number of preimages
    map<int, unsigned long long> minBaseForPreimages;

    while (base <= endBase) {
        memset(newBaseTerms, 0, sizeof(newBaseTerms));
        newBaseSet = base1Elts;
        newBaseTerms[0] = base;

        bool diverged = false;

        for (unsigned long long i = 1; i < terms; i++) {
            newBaseTerms[i] = newBaseTerms[i - 1] + productOfNonzeroDigits(newBaseTerms[i - 1]);
            unattainables.erase(newBaseTerms[i - 1] + productOfNonzeroDigits(newBaseTerms[i - 1]));

            preimages[newBaseTerms[i]].insert(newBaseTerms[i - 1]);

            if (base1Elts.count(base) > 0) {
                outFile << base << " " << base << endl;
                break;
            }

            if (find(newBaseSet.begin(), newBaseSet.end(), newBaseTerms[i]) != newBaseSet.end()) {
                outFile << base << " " << newBaseTerms[i] << endl;
                break;
            }

            if (newBaseTerms[i] > *base1Elts.rbegin()) {
                diverged = true;
                outFile << base << " " << "D" << endl;
                break;
            }

            newBaseSet.insert(newBaseTerms[i]);
        }

        base++;
    }

    // Question 1
    cout << unattainables.size() << endl;

    // Question 4
    for (const auto& entry : preimages) {
        if (base1Elts.count(entry.first) > 0) {
            int numPreimages = entry.second.size();
            if (minBaseForPreimages.find(numPreimages) == minBaseForPreimages.end() || entry.first < minBaseForPreimages[numPreimages]) {
                minBaseForPreimages[numPreimages] = entry.first;
                cout << entry.first << ": ";
                for (const auto& preimage : entry.second) {
                    cout << preimage << " ";
                }
                cout << endl;
            }
        }
    }

    outFile.close();
}

/**
* Helper function to generate elements that originated from digit product sequence from 1.
* Precondition: None
* Postcondition: Generates set of base 1 elements.
*/
void precomputeBase1() {
    ofstream outFile("modCounts2600000.txt");
    unsigned long long a063108Terms[terms];
    set<unsigned long long> a063108;
    a063108Terms[0] = 89498376717984;
    // a063108.insert(531230547362); // initially 1
    unsigned long long mod0Count = 505253;
    unsigned long long mod1Count = 291;
    unsigned long long mod2Count = 530778;
    unsigned long long mod3Count = 665;
    unsigned long long mod4Count = 314521;
    unsigned long long mod5Count = 363690;
    unsigned long long mod6Count = 359408;
    unsigned long long mod7Count = 417;
    unsigned long long mod8Count = 324142;
    unsigned long long mod9Count = 835;
    unsigned long long lowToHigh = 836;
    unsigned long long highToLow = 835;
    unsigned long long highToHigh = 2396956;
    unsigned long long lowToLow = 1373;

    for (unsigned long long i = 1; i <= terms; i++) {
        a063108Terms[i] = a063108Terms[i - 1] + productOfNonzeroDigits(a063108Terms[i - 1]);
        a063108.insert(a063108Terms[i]);
        if (a063108Terms[i] % 10 == 1)
        {
            mod1Count++;
        }
        if (a063108Terms[i] % 10 == 3)
        {
            mod3Count++;
        }
        if (a063108Terms[i] % 10 == 5)
        {
            mod5Count++;
        }
        if (a063108Terms[i] % 10 == 7)
        {
            mod7Count++;
        }
        if (a063108Terms[i] % 10 == 9)
        {
            mod9Count++;
        }
        if (a063108Terms[i] % 10 == 0)
        {
            mod0Count++;
        }
        if (a063108Terms[i] % 10 == 2)
        {
            mod2Count++;
        }
        if (a063108Terms[i] % 10 == 4)
        {
            mod4Count++;
        }
        if (a063108Terms[i] % 10 == 6)
        {
            mod6Count++;
        }
        if (a063108Terms[i] % 10 == 8)
        {
            mod8Count++;
        }

        if ((a063108Terms[i - 1] % 2 == 1 && a063108Terms[i - 1] % 10 != 5) && (a063108Terms[i] % 2 == 1 && a063108Terms[i] % 10 != 5)) 
        {
            lowToLow++;
        }
        if ((a063108Terms[i - 1] % 2 == 1 && a063108Terms[i - 1] % 10 != 5) && (a063108Terms[i] % 2 == 0 || a063108Terms[i] % 10 == 5)) 
        {
            lowToHigh++;
        } 
        if ((a063108Terms[i - 1] % 2 == 0 || a063108Terms[i - 1] % 10 == 5) && (a063108Terms[i] % 2 == 1 && a063108Terms[i] % 10 != 5)) 
        {
            highToLow++;
        } 
        if ((a063108Terms[i - 1] % 2 == 0 || a063108Terms[i - 1] % 10 == 5) && (a063108Terms[i] % 2 == 0 || a063108Terms[i] % 10 == 5)) 
        {
            highToHigh++;
        }
    }
    outFile << setprecision((int)log10(terms));
    outFile << "Mod 0 count: " << mod0Count << endl;
    outFile << "Mod 1 count: " << mod1Count << endl;
    outFile << "Mod 2 count: " << mod2Count << endl;
    outFile << "Mod 3 count: " << mod3Count << endl;
    outFile << "Mod 4 count: " << mod4Count << endl;
    outFile << "Mod 5 count: " << mod5Count << endl;
    outFile << "Mod 6 count: " << mod6Count << endl;
    outFile << "Mod 7 count: " << mod7Count << endl;
    outFile << "Mod 8 count: " << mod8Count << endl;
    outFile << "Mod 9 count: " << mod9Count << endl;
    outFile << endl;
    outFile << endl;
    outFile << "Proportion of mod 0 count: " << (double)mod0Count / (2400000 + terms) << endl;
    outFile << "Proportion of mod 1 count: " << (double)mod1Count / (2400000 + terms) << endl;
    outFile << "Proportion of mod 2 count: " << (double)mod2Count / (2400000 + terms) << endl;
    outFile << "Proportion of mod 3 count: " << (double)mod3Count / (2400000 + terms) << endl;
    outFile << "Proportion of mod 4 count: " << (double)mod4Count / (2400000 + terms) << endl;
    outFile << "Proportion of mod 5 count: " << (double)mod5Count / (2400000 + terms) << endl;
    outFile << "Proportion of mod 6 count: " << (double)mod6Count / (2400000 + terms) << endl;
    outFile << "Proportion of mod 7 count: " << (double)mod7Count / (2400000 + terms) << endl;
    outFile << "Proportion of mod 8 count: " << (double)mod8Count / (2400000 + terms) << endl;
    outFile << "Proportion of mod 9 count: " << (double)mod9Count / (2400000 + terms) << endl;
    outFile << "Maximum term: " << a063108Terms[terms-1] << endl;
    outFile << setprecision((int)log10(terms) + 1);
    outFile << "Proportion of non-common mods: " << ((double)mod1Count + (double)mod3Count + (double)mod7Count + (double)mod9Count) / (2400000 + terms) << endl;
    outFile << "Proportion of common mods: " << 1 - ((double)mod1Count + (double)mod3Count + (double)mod7Count + (double)mod9Count) / (2400000 + terms) << endl;
    outFile << "High to high: " << highToHigh << endl;
    outFile << "Low to high: " << lowToHigh << endl;
    outFile << "Low to low: " << lowToLow << endl;
    outFile << "High to low: " << highToLow << endl;
    outFile << "Proportion of low to high: " << (double)lowToHigh / (2400000 + terms) << endl;
    outFile << "Proportion of high to low: " << (double)highToLow / (2400000 + terms) << endl;
    outFile << "Proportion of high to high: " << (double)highToHigh / (2400000 + terms) << endl;
    outFile << "Proportion of low to low: " << (double)lowToLow / (2400000 + terms) << endl;
    // calcBaseN(a063108);
}

/**
* Generates digit product sequences that start from different numbers, that take the previous term 
and add the product of its non-zero digits to obtain the next term.
*/
int main() {
    clock_t start = clock();
    precomputeBase1();
    clock_t end = clock();

    double elapsed_secs = double(end - start) / CLOCKS_PER_SEC;
    int hours = static_cast<int>(elapsed_secs) / 3600;
    int minutes = (static_cast<int>(elapsed_secs) % 3600) / 60;
    double seconds = elapsed_secs - (hours * 3600) - (minutes * 60);

    std::cout << "Elapsed time: " << hours << " hours, "
              << minutes << " minutes, and "
              << std::fixed << std::setprecision(3) << seconds << " seconds\n";

    return 0;
}