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
        s+("|SA0");
        this->refresh();
        p.second->setStuckAtFault(0);
        TestVectors[s] = generateTestVector(p.second);
        s = p.first;
        s+("|SA1");
        this->refresh();
        p.second->setStuckAtFault(1);
        TestVectors[s] = generateTestVector(p.second);
    }
    return TestVectors;
}

// PODEM algorithm to generate a test vector
vector<int> netlist::generateTestVector(port* stuck_port) {
 return  {};

    // while(1){
    // if (X_path_check()) {   // return true if following
    //     /*
    //         All gate output of the chosen path must have X values  Called X-PATH
    //          If more than one X-path to choose,  chose shortest X-path to PO
    //          If X-path disappear,  backtrack
    //     */
    //     pair<port*, int> objective = getObjective(); //  getObjective() can return null
    //     // call backtrace()

    // }
    // else {
    //     // todo: make a stack for decisons at PI: PI name, value, bool is_flipped
    //     // call bool all_assignmnents_tried(stack). ... i.e. is_flipped of all is 1
    //     // if true return untestable
    //     // if false flip PI which has the first is_flipped=0 from the top of the stack
    //     //          is_flipped of that PI=1; others above it = 0
    // }
    // // call bool imply() to find PIs, and it returns true if fault is tested
    // // if false continue

    // }
}

int netlist::getSeqDepth(Netlist n) {
    // Calculate the sequential depth of the netlist
    CircuitGraph circuit;

    // Add edges from logic gates to the graph
    for (const Gate& gate : n.gates) {
        for (const string& input : gate.inputs) {
            circuit.addEdge(input, gate.output); // Connect gate inputs to its output
        }
    }

    // Add edges from flip-flop outputs to gate inputs
    for (const FF& ff : n.ff) {
        circuit.addEdge(ff.D, ff.Q); // Connect flip-flop input to output
        circuit.addFlipFlop(ff.Q);  // Add flip-flop output to the graph
    }

    // Calculate and display the sequential depth of the circuit
    int depth = circuit.calculateSequentialDepth();
    cout << "Sequential Depth: " << depth << endl;
    return depth;
}
void netlist::unroll() {
    // Unroll the netlist
    // Implement your logic here
    vector<string> PPI;
    vector<string> PPO;
    unordered_map<string, string> PPO_PPI_map;
    for(auto i : myNetlist.ff){
        PPI.push_back(i.Q);
        PPO.push_back(i.D);
    }
    for(auto i : myNetlist.ff){
        PPO_PPI_map[i.Q] = i.D;
    }

    Netlist unrolled_netlist;
    for(int i = 0; i <= seq_depth; i++){
        if(i == 0){
            for(auto j: PPI){
                unrolled_netlist.inputs.push_back(j+(string("_")+(to_string(i))));        
            }    
        }
        if (i == seq_depth){
            for(auto j : PPO){
                string str_ = string("_")+(to_string(i));
                unrolled_netlist.outputs.push_back(j+(str_));
            }
        }
        for(auto j : myNetlist.inputs){
            string str_ = string("_")+(to_string(i));
            unrolled_netlist.inputs.push_back(j+(str_));
        }
        for(auto j : myNetlist.outputs){
            string str_ = string("_")+(to_string(i));
            unrolled_netlist.outputs.push_back(j+(str_));
        }
        for(auto j : myNetlist.gates){
            Gate g;
            g.output = j.output+(string("_")+(to_string(i)));
            g.type = j.type;
            for(auto k : j.inputs){
                if (PPO_PPI_map.find(k) != PPO_PPI_map.end()){
                    if(i != 0){
                         g.inputs.push_back(PPO_PPI_map.find(k)->second+(string("_")+(to_string(i-1))));
                    }
                    else{
                        g.inputs.push_back(k+(string("_")+(to_string(i))));
                    }
                }
                else g.inputs.push_back(k+(string("_")+(to_string(i))));
            }
            unrolled_netlist.gates.push_back(g);
        }
        
        for(auto j : unrolled_netlist.gates){
            for(auto k : j.inputs){
                if(find(unrolled_netlist.inputs.begin(), unrolled_netlist.inputs.end(), k) == unrolled_netlist.inputs.end()){
                    if(find(unrolled_netlist.wires.begin(), unrolled_netlist.wires.end(), k) == unrolled_netlist.wires.end()) unrolled_netlist.wires.push_back(k);
                }
            }
            auto k = j.output;
            if(find(unrolled_netlist.outputs.begin(), unrolled_netlist.outputs.end(), k) == unrolled_netlist.outputs.end()){
                if(find(unrolled_netlist.wires.begin(), unrolled_netlist.wires.end(), k) == unrolled_netlist.wires.end()) unrolled_netlist.wires.push_back(k);
            }
            
        }
    }
    myNetlist = unrolled_netlist;

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
