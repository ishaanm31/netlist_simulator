#include <bits/stdc++.h>
#include "netlist_parser.hpp"
#include "netlist.hpp"
#define _DEBUG 0

using namespace std;
// Written by Ishaan, Harshraj, Atharva and Avaneesh

int main(){
    cout<<"Assignment 2 of VLSI CAD 2024! \n Combinational Automatic Test Vector Generator using PODEM. \nGive filepath for the netlist.txt"<<endl;
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
    while (1) {
        cout<<"For exiting the loop enter Q\n";
        cout<<"Give input vector in the given format: for input A=0 B=1 C=0 write 010 in the command line"<<endl;
        cout<<"Inputs for our netlist are:\n ";
        for (auto inp: my_parser.netlist.inputs) 
            cout<<inp<<", ";
        cout<<endl;
        string ss;
        cin>>ss;
        if ((ss[0] == 'Q') or (ss[0] == 'q')) break;
        my_netlist.refresh();
        my_netlist.simulate(ss);
        my_netlist.display_output();
    }
    return 0;
}