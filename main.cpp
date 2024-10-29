#include <bits/stdc++.h>
#include "netlist_parser.hpp"
#include "netlist.hpp"
#define _DEBUG 1
using namespace std;
// Written by Ishaan, Harshraj, Atharva and Avaneesh

void print_grouped_test_vectors(const map<set<string>, map<string, int>>& grouped_map) {
    for (const auto& [faults, test_vector] : grouped_map) {
        cout << "Faults: ";
        for (const auto& fault : faults) {
            cout << fault << " ";
        }
        cout << "\nTest Vector:\n";
        for (const auto& [input, value] : test_vector) {
            cout << "  " << input << ": " << value << "\n";
        }
        cout << "------------------\n";
    }
}

int main(){
    cout<<"Assignment 2 of VLSI CAD 2024! \nSequential Automatic Test Vector Generator using PODEM. \nGive filepath for the netlist (.txt)"<<endl;
    string filepath;
    cin>>filepath;
    cout<<"Parsing the netlist"<<endl;

    netlist_parser my_parser;
    my_parser.parse(filepath);

#if _DEBUG
    cout<<"Printing parsed data"<<endl;
    my_parser.printParsedData();
#endif

    netlist my_netlist(my_parser.netlist);
    auto test_vectors = my_netlist.generate_test_vectors();
    print_grouped_test_vectors(my_netlist.group_test_vectors(test_vectors));

    return 0;
}