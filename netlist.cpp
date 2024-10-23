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

map<string, vector<int>> netlist::comb_atpg() {
    // Populating all ports for which we will be calculating 
    map<string, vector<int>> TestVectors;
    for (auto p: port_map) {
        string s = p.first;
        s.append("|SA0");
        this->refresh();
        p.second->setStuckAtFault(0);
        TestVectors[s] = generateTestVector(p.second);
        s = p.first;
        s.append("|SA1");
        this->refresh();
        p.second->setStuckAtFault(1);
        TestVectors[s] = generateTestVector(p.second);
    }
    return TestVectors;
}

bool netlist::X_path_check(port* stuck_port) {
    if(dynamic_cast<primary_output_port*>(stuck_port)) {
        return stuck_port->getFaultFreeValue() == -1;
    }
    else if(input_port* p = dynamic_cast<input_port*>(stuck_port)) {
        if(p->getInputGate()->getOutputPort()->getFaultFreeValue() == -1){
            return X_path_check(p->getInputGate()->getOutputPort());
        }
        else return false;
    }
    else if(primary_input_port* p = dynamic_cast<primary_input_port*>(stuck_port)) {
        bool x_path_exists = false;
        for(auto next_gates: p->getDependentGates()) {
            x_path_exists |= X_path_check(next_gates->getOutputPort());
        }
        return x_path_exists;
    }
    else if(output_port* p = dynamic_cast<output_port*>(stuck_port)) {
        bool x_path_exists = false;
        if(p->getFaultFreeValue() != -1) return false;
        for(auto next_gates: p->getDependentGates()) {
            x_path_exists |= X_path_check(next_gates->getOutputPort());
        }
        return x_path_exists;
    }
}

// Function to backtrace and assign PIs
pair<primary_input_port*, int> netlist::backtrace(pair<port*, int> objective) {
    while(1){
        if(primary_input_port* p = dynamic_cast<primary_input_port*> (objective.first)){
            p->setFaultFreeValue(objective.second);
            // TODO: update stack
            return {p, objective.second};
        }
        else {
            
        }
    }
}

// TODO: Maintain a D-frontier
// PODEM algorithm to generate a test vector
vector<int> netlist::generateTestVector(port* stuck_port) {
    while(1){
    if (X_path_check(stuck_port)) {   // return true if following
        /*
            All gate output of the chosen path must have X values  Called X-PATH
             If more than one X-path to choose,  chose shortest X-path to PO
             If X-path disappear,  backtrack
        */
        if (input_port* p = dynamic_cast<input_port*> (stuck_port)){
            pair<port*, int> objective = getObjective(p); //  getObjective() can return null
            // call backtrace(objective)
            backtrace(objective);
        }
        else if (output_port* p = dynamic_cast<output_port*> (stuck_port)){
            
        }

    }
    else {
        // todo: make a stack for decisons at PI: PI name, value, bool is_flipped
        // call bool all_assignmnents_tried(stack). ... i.e. is_flipped of all is 1
        // if true return untestable
        // if false flip PI which has the first is_flipped=0 from the top of the stack
        //          is_flipped of that PI=1; others above it = 0
    }
    // call bool imply() to find PIs, and it returns true if fault is tested
    // if false continue

    }

}

pair<input_port*, int> netlist::getObjective(input_port* stuck_port){
    //fault propagation
    if (stuck_port->getDvalue() == D || stuck_port->getDvalue() == D_bar){
        for (auto x : stuck_port->getInputGate()->getInputPorts()){
            if (x != stuck_port)
                if ((stuck_port->getInputGate()->getGateType() == "AND2") || (stuck_port->getInputGate()->getGateType() == "NAND2"))
                    return {x,_1};
                else if ((stuck_port->getInputGate()->getGateType() == "OR2") || (stuck_port->getInputGate()->getGateType() == "NOR2") || (stuck_port->getInputGate()->getGateType() == "XOR2") || (stuck_port->getInputGate()->getGateType() == "XNOR2"))
                    return {x,_0};
                
        }
    }
    //fault activation
    else if (stuck_port->getFaultValue() == 1 && stuck_port->getStuckAtFault()) return{stuck_port,0};
    else return{stuck_port, 1};

    

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
