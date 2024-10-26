#include <bits/stdc++.h>
#include "netlist_parser.hpp"
#include "netlist.hpp"
#define _DEBUG 1
const string fp = "./simple_circuit.txt";
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
    cout<<"Assignment 2 of VLSI CAD 2024! \n Combinational Automatic Test Vector Generator using PODEM. \nGive filepath for the netlist.txt"<<endl;
    string filepath;
    cin>>filepath;
    if (filepath.size()<2)
        filepath = fp;
    cout<<"Parsing the netlist"<<endl;

    netlist_parser my_parser;
    my_parser.parse(filepath);

#if _DEBUG
    cout<<"Printing parsed data"<<endl;
    my_parser.printParsedData();
#endif

    netlist my_netlist(my_parser.netlist);
    cout<<"Came here"<<endl;
    while (1) {
        cout<<"For exiting the loop enter Q\n";
        cout<<"Give input vector in the given foWWrmat: for input A=0 B=1 C=0 write 010 in the command line"<<endl;
        cout<<"Inputs for our netlist are:\n ";
        for (auto inp: my_netlist.myNetlist.inputs) 
            cout<<inp<<", ";
        cout<<endl;
        string ss;
        cin>>ss;
        if ((ss[0] == 'Q') or (ss[0] == 'q')) break;
        if (ss[0] == 'F') {
            auto test_vectors = my_netlist.generate_test_vectors();
            // for (auto entry: test_vectors) {
            //     cout<<"Fault: "<<entry.first<<endl;
            //     for (auto entry2: entry.second) {
            //         cout<<entry2.first<<" <= "<<entry2.second<<endl;
            //     }
            //     cout<<"xxxxxxxxxxxxxxxxxxxx"<<endl;
            // }    
            print_grouped_test_vectors(my_netlist.group_test_vectors(test_vectors));
        }
        if(ss.size() != my_netlist.myNetlist.inputs.size()){
            cout<<"give corrent amount of inputs\n";
            continue;
        }
        for(long unsigned int i = 0; i < ss.size(); i++) {
            my_netlist.setPI(my_netlist.myNetlist.inputs[i],((ss[i]=='0')?0:1));
        }
        my_netlist.simulate();
        my_netlist.display_output();
    }
    return 0;
}