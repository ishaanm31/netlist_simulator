#include "netlist.hpp"
#include "wire.hpp"
#include "node.hpp"
#include "port.hpp"
#include "operation.hpp"
#include <iostream>
#include <queue>
#include <cstdlib>  // For exit()
#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


netlist::netlist(Netlist n) : myNetlist(n) {
    if (!n.ff.empty()){
        cout<<"flip-flops in the netlist"<<endl;
        seq_depth = getSeqDepth(n);
        unroll();
        // Populating wire map for internal signals
    for (string x: myNetlist.wires){
        wire *w = new wire(x);
        wire_map[x] = w;
    }

    // Populating wire map and primary input signals
    for (string x: myNetlist.inputs) {
        wire *w = new wire(x);
        wire_map[x] = w;
        primary_input_port* PI_port = w->createDriverPort();
        input_signals[x] = PI_port;
        string s ="PI:";
        s+(x);
        port_map[s] = PI_port;
    }

    // Populating wire map and primary output signals (should use outputs, not inputs)
    for (string x: myNetlist.outputs) {
        wire *w = new wire(x);
        wire_map[x] = w;
        primary_output_port* PO_port = w->createDrivenPort();
        output_signals[x] = PO_port;
        string s ="PO:";
        s+(x);
        port_map[s] = PO_port;
    }
    // Creating gates and connecting wires
    int idx = 0;
    for (Gate g: myNetlist.gates) {
        vector<wire*> g_inp;
        for (string inp: g.inputs) {
            g_inp.push_back(wire_map[inp]);
        }
        node *gate = new node(idx++, g_inp, wire_map[g.output], g.type);
        gates.push_back(gate);

        vector<input_port*> inp_ports = gate->getInputPorts();
        int idx = 0;
        for (auto inp_port: inp_ports) {
            string s = "GID:";
            s+(to_string(gate->getIndex()));
            s+("ip:");
            s+(to_string(idx++));
            port_map[s] = inp_port;
        }
        string s = "GID:";
        s+(to_string(gate->getIndex()));
        s+("op");
        port_map[s] = gate->getOutputPort();
    }
    }
    else {

    // Populating wire map for internal signals
    for (string x: myNetlist.wires) {
        wire *w = new wire(x);
        wire_map[x] = w;
    }

    // Populating wire map and primary input signals
    for (string x: myNetlist.inputs) {
        wire *w = new wire(x);
        wire_map[x] = w;
        primary_input_port* PI_port = w->createDriverPort();
        input_signals[x] = PI_port;
        string s ="PI:";
        s+(x);
        port_map[s] = PI_port;
    }

    // Populating wire map and primary output signals (should use outputs, not inputs)
    for (string x: myNetlist.outputs) {
        wire *w = new wire(x);
        wire_map[x] = w;
        primary_output_port* PO_port = w->createDrivenPort();
        output_signals[x] = PO_port;
        string s ="PO:";
        s+(x);
        port_map[s] = PO_port;
    }
    // Creating gates and connecting wires
    int idx = 0;
    for (Gate g: myNetlist.gates) {
        vector<wire*> g_inp;
        for (string inp: g.inputs) {
            g_inp.push_back(wire_map[inp]);
        }
        node *gate = new node(idx++, g_inp, wire_map[g.output], g.type);
        gates.push_back(gate);

        vector<input_port*> inp_ports = gate->getInputPorts();
        int idx = 0;
        for (auto inp_port: inp_ports) {
            string s = "GID:";
            s+(to_string(gate->getIndex()));
            s+("ip:");
            s+(to_string(idx++));
            port_map[s] = inp_port;
        }
        string s = "GID:";
        s+(to_string(gate->getIndex()));
        s+("op");
        port_map[s] = gate->getOutputPort();
    }
    }

    levelize();  // Levelize the circuit (evaluate levels)
}

void netlist::levelize() {
    // Set input signal levels to 0
    for (auto inp_pair: input_signals)
        inp_pair.second->setLevel(0);

    // Process each gate
    queue<node*> worklist;
    for (auto x: gates) {
        worklist.push(x);
    }
    while (!worklist.empty()) {
        node *curr_gate = worklist.front();
        worklist.pop();
        vector<node*> affected_gates = curr_gate->evalLevel();
        if (affected_gates.empty()) continue;
        for (auto g: affected_gates){
            worklist.push(g);
        }
    }
}

void netlist::refresh() {
    // Refresh each primary input signal
    for (auto inp_pair: input_signals) {
        inp_pair.second->refresh();
    }
    for (auto out_pair: output_signals) {
        out_pair.second->refresh();
    }
    for (auto port_pair: port_map) {
        port_pair.second->refresh();
    }
    // Resimulate after refresh
    simulate();
}

void netlist::simulate() {
    // Evaluate each gate using worklist
    queue<node*> worklist;
    for (auto x: gates)
        worklist.push(x);

    while (!worklist.empty()) {
        node *curr_gate = worklist.front();
        worklist.pop();
        vector<node*> affected_gates = curr_gate->eval();
        for (auto g: affected_gates)
            worklist.push(g);
    }
    D_frontier.clear();
    for (auto x: gates) {
        if (x->belong_to_D_frontier()) {
            D_frontier.insert(x);
        }
    }
        
}

void netlist::setPI(string PI_name, int val) {
    // Set primary input signal fault-free value
    input_signals[PI_name]->setFaultFreeValue(val);
    input_signals[PI_name]->setFaultValue(val);
}

void netlist::display_output() const {
    // Display output values
    for (auto out_pair: output_signals) {
        cout << out_pair.first << " --> " << out_pair.second->getFaultFreeValue() 
             << "/" << out_pair.second->getFaultValue() << endl;
    }
    for (auto g: gates) {
        cout<<"gate: "<<g->getIndex()<<"| level: "<<g->getLevel()<<endl;
    }
}

map<string, map<string, int>> netlist::comb_atpg() {
    // Populating all ports for which we will be calculating 
    map<string, map<string, int>> TestVectors;
    for (auto p: port_map) {
        string s = p.first;
        s+("|SA0");
        this->refresh();
        p.second->setStuckAtFault(0);
        simulate();
        map<string, int> TestVector;
        cout<<"Finding Test vector for fault: "<<s<<endl;
        bool testable = podem_recursion(p.second);
        for(auto pi: input_signals) {
            if (testable)
                TestVector[pi.first] = pi.second->getFaultFreeValue();
            else
               TestVector[pi.first] = -1;

        }
        TestVectors[s] = TestVector;
        cout<<"Found TV: "<<s<<" -> ";
        for(auto t: TestVector){
            cout<<t.second<<", ";
        }
        cout<<endl;
        s = p.first;
        s+("|SA1");
        this->refresh();
        p.second->setStuckAtFault(1);
        simulate();
        TestVector.clear();
        cout<<"Finding Test vector for fault: "<<s<<endl;
        testable = podem_recursion(p.second);
        for(auto pi: input_signals) {
            if (testable)
                TestVector[pi.first] = pi.second->getFaultFreeValue();
            else
               TestVector[pi.first] = -1;

        }
        TestVectors[s] = TestVector;
        cout<<"Found TV: "<<s<<" -> ";
        for(auto t: TestVector){
            cout<<t.second<<", ";
        }
        cout<<endl;    
    }
    return TestVectors;
}

// bool netlist::X_path_check(port* stuck_port) {
//     if(dynamic_cast<primary_output_port*>(stuck_port)) {
//         return stuck_port->getFaultFreeValue() == -1;
//     }
//     else if(input_port* p = dynamic_cast<input_port*>(stuck_port)) {
//         if(p->getInputGate()->getOutputPort()->getFaultFreeValue() == -1){
//             return X_path_check(p->getInputGate()->getOutputPort());
//         }
//         else return false;
//     }
//     else if(primary_input_port* p = dynamic_cast<primary_input_port*>(stuck_port)) {
//         bool x_path_exists = false;
//         for(auto next_gates: p->getDependentGates()) {
//             x_path_exists |= X_path_check(next_gates->getOutputPort());
//         }
//         return x_path_exists;
//     }
//     else if(output_port* p = dynamic_cast<output_port*>(stuck_port)) {
//         bool x_path_exists = false;
//         if(p->getFaultFreeValue() != -1) return false;
//         for(auto next_gates: p->getDependentGates()) {
//             x_path_exists |= X_path_check(next_gates->getOutputPort());
//         }
//         return x_path_exists;
//     }
// }

// Function to backtrace and assign PIs
pair<primary_input_port*, int> netlist::backtrace(pair<port*, int> objective) {
    int num_inversions = 0;
    output_port* current_port;
    cout<<"backtrace"<<endl;
    if(!(current_port = dynamic_cast<output_port*> (objective.first)))
        current_port = (dynamic_cast<input_port*> (objective.first))->getDriverWire()->getDriverPort();
    while(1){
        // if current_port is PI, we are done
        if(primary_input_port* p = dynamic_cast<primary_input_port*> (current_port)){
            for( auto x: input_signals) {
                if (x.second == p)
                    cout<<"Backtraced PI: "<<x.first<<" Value: "<<(objective.second + num_inversions) %2<<endl;
            }
            cout<<"Backtraced to PI"<<endl;
            return {p, (objective.second + num_inversions) % 2};
        }
        else {
            for(auto gate_ip: current_port->getDriverGate()->getInputPorts()) {
                if(gate_ip->getFaultFreeValue() == -1) {
                    num_inversions += current_port->getDriverGate()->getInversionParity();
                    cout<<"back back with num_inv: "<<num_inversions<<endl;
                    current_port = gate_ip->getDriverWire()->getDriverPort();
                    break;
                }
            }
        }
    }
}

bool netlist::podem_recursion(port* stuck_port) {
    // cout<<"hi1"<<endl;
    for(auto po: output_signals) {
        if(po.second->getDvalue() == D || po.second->getDvalue() == D_bar) {
            return true;
        }
    }
    // cout<<"hi2"<<endl;
    pair<port*, int> obj = getObjective(stuck_port);
    if (obj.first == NULL) return false; // D frontier is empty;
    // cout<<"hi3"<<endl;
    pair<primary_input_port*, int> pi_value = backtrace(obj);
    // cout<<"hi4"<<endl;
    pi_value.first->setFaultFreeValue(pi_value.second);
    pi_value.first->setFaultValue(pi_value.second);
    // cout<<"hi5"<<endl;
    simulate();
    bool result = podem_recursion(stuck_port);
    if(result) {
        // cout<<"Returned true once bhai"<<endl;
        return true;
    }
    pi_value.second = (pi_value.second+1)%2;
    pi_value.first->setFaultFreeValue(pi_value.second);
    pi_value.first->setFaultValue(pi_value.second);
    simulate();
    result = podem_recursion(stuck_port);
    pi_value.second = -1;
    pi_value.first->setFaultFreeValue(pi_value.second);
    pi_value.first->setFaultValue(pi_value.second);
    simulate();
    return false;
}


pair<port*, int> netlist::getObjective(port* stuck_port){
    cout<<"Get objective"<<endl;
    if (stuck_port->getStuckAtFault()) {
        if (stuck_port->getFaultFreeValue() == -1) {
            int comp = (stuck_port->getFaultValue() + 1) % 2;
            cout<<"Returning activation of :"<<comp<<endl;
            return {stuck_port, comp};
        }
        if (stuck_port->getFaultFreeValue() == stuck_port->getFaultValue()) {
            return {NULL, -1};
        }
    }
    node *gate = *D_frontier.begin();
    cout<<"Dfrontier size: "<<D_frontier.size()<<endl;
    if (gate == NULL) {
        return {NULL, -1};
    }
    for (auto x : gate->getInputPorts()){
        if ((x != stuck_port) && (x->getFaultFreeValue() == -1)){
            if ((gate->getGateType() == "AND2") || (gate->getGateType() == "NAND2"))
                return {x,1};
            else 
                return {x,0};
        }
    }
    cerr<<"D frontier is corrupted"<<endl;
    exit(1); 
}

// pair<port*, int> netlist::getObjective(port* stuck_port){
//     //fault propagation
//     if(stuck_port == NULL) {
//         cerr<<"Stuck port NULL"<<endl;
//         exit(1);
//     }
//     if (stuck_port->getDvalue() == D || stuck_port->getDvalue() == D_bar){
//         cout<<"Hi1"<<endl;
//         if (input_port* p = dynamic_cast<input_port*> (stuck_port)){
//             for (auto x : p->getInputGate()->getInputPorts()){
//                 if ((x != p) && (x->getFaultFreeValue() == -1)){
//                     if ((p->getInputGate()->getGateType() == "AND2") || (p->getInputGate()->getGateType() == "NAND2"))
//                         return {x,1};
//                     else if ((p->getInputGate()->getGateType() == "OR2") || (p->getInputGate()->getGateType() == "NOR2") || (p->getInputGate()->getGateType() == "XOR2") || (p->getInputGate()->getGateType() == "XNOR2"))
//                         return {x,0};
//                 }
//             }        
                
//         }
//         else {
//             cerr<<"Error get objective"<<endl;
//             exit(1);
//             return {stuck_port, 0};
//         }
//     }
//     //fault activation
//     else if ((stuck_port->getFaultValue() == 1) && (stuck_port->getStuckAtFault())) {
//         cout<<"Hi2"<<endl;
//         return{stuck_port,0};
//     }
//     cout<<"Hi3"<<endl;
//     return {stuck_port, 1};
// }

netlist::~netlist() {
    // Destructor: Clean up dynamically allocated memory
    for (auto pair : wire_map) {
        delete pair.second;
    }
    for (auto g : gates) {
        delete g;
    }
}
void netlist::print_netlist(Netlist n){
    cout<<"Inputs: ";
    for(auto i : n.inputs){
        cout<<i<<" ";
    }
    cout<<endl;
    cout<<"Outputs: ";
    for(auto i : n.outputs){
        cout<<i<<" ";
    }
    cout<<endl;
    cout<<"Wires: ";
    for(auto i : n.wires){
        cout<<i<<" ";
    }
    cout<<endl;
    cout<<"Gates: " << endl;
    for(auto i : n.gates){
        cout<<i.output<<" "<<i.type<<" ";
        for(auto j : i.inputs){
            cout<<j<<" ";
        }
        cout<<endl;
    }
    cout<<"FF: ";
    for(auto i : n.ff){
        cout<<i.D<<" "<<i.Q<<endl;
    }
}
