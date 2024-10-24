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
        s.append(x);
        port_map[s] = PI_port;
    }

    // Populating wire map and primary output signals (should use outputs, not inputs)
    for (string x: myNetlist.outputs) {
        wire *w = new wire(x);
        wire_map[x] = w;
        primary_output_port* PO_port = w->createDrivenPort();
        output_signals[x] = PO_port;
        string s ="PO:";
        s.append(x);
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
            s.append(to_string(gate->getIndex()));
            s.append("ip:");
            s.append(to_string(idx++));
            port_map[s] = inp_port;
        }
        string s = "GID:";
        s.append(to_string(gate->getIndex()));
        s.append("op");
        port_map[s] = gate->getOutputPort();
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
        s.append("|SA0");
        this->refresh();
        p.second->setStuckAtFault(0);
        map<string, int> TestVector;
        if (podem_recursion(p.second)){
            for(auto pi: input_signals) {
                TestVector[pi.first] = pi.second->getFaultFreeValue();
            }
            TestVectors[s] = TestVector;
        }
        s = p.first;
        s.append("|SA1");
        this->refresh();
        p.second->setStuckAtFault(1);
        TestVector.clear();
        if (podem_recursion(p.second)){
            for(auto pi: input_signals) {
                TestVector[pi.first] = pi.second->getFaultFreeValue();
            }
            TestVectors[s] = TestVector;
        }    
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
    current_port = dynamic_cast<output_port*> (objective.first);
    while(1){
        // if current_port is PI, we are done
        if(primary_input_port* p = dynamic_cast<primary_input_port*> (current_port)){
            p->setFaultFreeValue((objective.second +num_inversions%2)%2);
            return {p, objective.second};
        }
        else {
            num_inversions += current_port->getDriverGate()->getInversionParity();
            for(auto gate_ip: current_port->getDriverGate()->getInputPorts()) {
                if(gate_ip->getFaultFreeValue() == -1) {
                    current_port = gate_ip->getDriverWire()->getDriverPort();
                    break;
                }
            }
        }
    }
}

bool netlist::podem_recursion(port* stuck_port) {
    for(auto po: output_signals) {
        if(po.second->getDvalue() == D || po.second->getDvalue() == D_bar) {
            return true;
        }
    }
    pair<port*, int> obj = getObjective(stuck_port);
    pair<primary_input_port*, int> pi_value = backtrace(obj);
    simulate();
    bool result = podem_recursion(stuck_port);
    if(result) return true;
    pi_value.second = (pi_value.second+1)%2;
    simulate();
    result = podem_recursion(stuck_port);
    pi_value.second = -1;
    simulate();
    return false;
}


pair<port*, int> netlist::getObjective(port* stuck_port){
    //fault propagation
    if (stuck_port->getDvalue() == D || stuck_port->getDvalue() == D_bar){
        if (input_port* p = dynamic_cast<input_port*> (stuck_port)){
            for (auto x : p->getInputGate()->getInputPorts()){
                if (x != p && x->getFaultFreeValue() == -1)
                    if ((p->getInputGate()->getGateType() == "AND2") || (p->getInputGate()->getGateType() == "NAND2"))
                        return {x,1};
                    else if ((p->getInputGate()->getGateType() == "OR2") || (p->getInputGate()->getGateType() == "NOR2") || (p->getInputGate()->getGateType() == "XOR2") || (p->getInputGate()->getGateType() == "XNOR2"))
                        return {x,0};
            }        
                
        }
        else {
            cerr<<"Error get objective"<<endl;
            exit(1);
            return {NULL, 0};
        }
    }
    //fault activation
    else if (stuck_port->getFaultValue() == 1 && stuck_port->getStuckAtFault()) return{stuck_port,0};
    else return {stuck_port, 1};
}

netlist::~netlist() {
    // Destructor: Clean up dynamically allocated memory
    for (auto pair : wire_map) {
        delete pair.second;
    }
    for (auto g : gates) {
        delete g;
    }
}
