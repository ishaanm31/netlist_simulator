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
    original_Netlist = n;
    if (!n.ff.empty()){
        cout << "flip-flops in the netlist" << endl;
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
            s += (x);
            port_map[s] = PI_port;
        }

        // Populating wire map and psuedo primary input signals
        for (string x: PPI) {
            wire *w = new wire(x);
            wire_map[x] = w;
            p_primary_input_port* PPI_port = w->createDriverPort_();
            string s ="PPI:";
            s += (x);
            port_map[s] = PPI_port;
        }

        // Populating wire map and primary output signals (should use outputs, not inputs)
        for (string x: myNetlist.outputs) {
            wire *w = new wire(x);
            wire_map[x] = w;
            primary_output_port* PO_port = w->createDrivenPort();
            output_signals[x] = PO_port;
            string s ="PO:";
            s += (x);
            port_map[s] = PO_port;
        }

        // Populating wire map and psuedo primary output signals
        for (string x: PPO) {
            wire *w = new wire(x);
            wire_map[x] = w;
            p_primary_output_port* PPO_port = w->createDrivenPort_();
            string s ="PPO:";
            s += (x);
            port_map[s] = PPO_port;
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
            int id = 0;
            for (auto inp_port: inp_ports) {
                string s = "GID:";
                string p = s;
                s += (to_string(gate->getIndex()));
                p += to_string(gate->getIndex() % (original_Netlist.gates.size()));
                s += ("ip:");
                p += "ip:";
                p += to_string(id);
                s += (to_string(id++));
                port_map[s] = inp_port;
                equivalent_port_maping[p].insert(s);
            }
            string s = "GID:";
            string p = s;
            s += (to_string(gate->getIndex()));
            p += to_string(gate->getIndex() % (original_Netlist.gates.size()));
            p += "op";
            s += ("op");
            port_map[s] = gate->getOutputPort();
            equivalent_port_maping[p].insert(s);
        }
        }
    else {

        // Populating wire map for internal signals
        seq_depth = 0;
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
            s += (x);
            port_map[s] = PI_port;
        }

        // Populating wire map and primary output signals (should use outputs, not inputs)
        for (string x: myNetlist.outputs) {
            wire *w = new wire(x);
            wire_map[x] = w;
            primary_output_port* PO_port = w->createDrivenPort();
            output_signals[x] = PO_port;
            string s ="PO:";
            s += (x);
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
                s += (to_string(gate->getIndex()));
                s += ("ip:");
                s += (to_string(idx++));
                port_map[s] = inp_port;
            }
            string s = "GID:";
            s += (to_string(gate->getIndex()));
            s += ("op");
            port_map[s] = gate->getOutputPort();
        }
    }
    print_netlist(myNetlist);
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
    for (auto ppi_pair: ppi_signals) {
        ppi_pair.second->refresh();
    }
    for (auto ppo_pair: ppo_signals) {
        ppo_pair.second->refresh();
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

// Function to check if two test vectors are compatible and can be merged
bool are_compatible(const map<string, int>& tv1, const map<string, int>& tv2) {
    for (const auto& [input, value1] : tv1) {
        int value2 = tv2.at(input);
        if (value1 != -1 && value2 != -1 && value1 != value2) {
            return false; // Incompatible if both values are defined and different
        }
    }
    return true;
}

// Function to merge two compatible test vectors
map<string, int> merge_test_vectors(const map<string, int>& tv1, const map<string, int>& tv2) {
    map<string, int> merged;
    for (const auto& [input, value1] : tv1) {
        int value2 = tv2.at(input);
        merged[input] = (value1 != -1) ? value1 : value2;
    }
    return merged;
}

map<set<string>, map<string, int>> netlist::group_test_vectors(const map<string, map<string, int>>& fault_map) {
    map<set<string>, map<string, int>> grouped_map;
    vector<bool> used(fault_map.size(), false);
    auto fault_it = fault_map.begin();

    for (size_t i = 0; i < fault_map.size(); ++i, ++fault_it) {
        if (used[i]) continue;

        set<string> compatible_faults = { fault_it->first };
        map<string, int> merged_test_vector = fault_it->second;
        used[i] = true;

        auto inner_it = fault_map.begin();
        for (size_t j = 0; j < fault_map.size(); ++j, ++inner_it) {
            if (i != j && !used[j] && are_compatible(merged_test_vector, inner_it->second)) {
                merged_test_vector = merge_test_vectors(merged_test_vector, inner_it->second);
                compatible_faults.insert(inner_it->first);
                used[j] = true;
            }
        }
        
        grouped_map[compatible_faults] = merged_test_vector;
    }

    return grouped_map;
}


map<string, map<string, int>> netlist::generate_test_vectors(){        
//        return comb_atpg();
        if(seq_depth == 0) return comb_atpg();
        else return seq_atpg();
    }

map<string, map<string, int>> netlist::seq_atpg() {
    map<string, map<string, int>> TestVectors ;
    for (auto stuck_port_entry: equivalent_port_maping) {
        string port_name = stuck_port_entry.first;
        set<port*> equivalent_ports;
        for (auto eq_port: stuck_port_entry.second) {
            equivalent_ports.insert(port_map[eq_port]);
        }

        for (int i = 0; i < 2; i++) {
            // Stucking all the equivalent ports at i
            string s = port_name + " | SA:"+ to_string(i);
            bool testable = false;
            for(auto stuck_port: equivalent_ports) {
                refresh();
                for (auto p: equivalent_ports) {
                    p->setStuckAtFault(i);
                }
                simulate();
                testable = podem_recursion(stuck_port);
                if (testable) break; 
            }
            map<string, int> TestVector;
            for(auto pi: input_signals) {
                if (testable)
                    TestVector[pi.first] = pi.second->getFaultFreeValue();
                else
                    TestVector[pi.first] = -1;
            }
            TestVectors[s] = TestVector;
        }
        
    }
    return TestVectors;
}

map<string, map<string, int>> netlist::comb_atpg() {
    // Populating all ports for which we will be calculating 
map<string, map<string, int>> TestVectors;
    for (auto p: port_map) {
        for (int i = 0; i < 2; i++){
            string s = p.first;
            s += (" | SA") + to_string(i);
            this->refresh();
            p.second->setStuckAtFault(0);
            simulate();
            map<string, int> TestVector;
            bool testable = podem_recursion(p.second);
            for(auto pi: input_signals) {
                if (testable)
                    TestVector[pi.first] = pi.second->getFaultFreeValue();
                else
                    TestVector[pi.first] = -1;
            }
            TestVectors[s] = TestVector;
            
        }  
    }
    return TestVectors;
}

// Function to backtrace and assign PIs
pair<primary_input_port*, int> netlist::backtrace(pair<port*, int> objective) {
    output_port* current_port;
    if(!(current_port = dynamic_cast<output_port*> (objective.first)))
        current_port = (dynamic_cast<input_port*> (objective.first))->getDriverWire()->getDriverPort();
        // if current_port is PI, we are done
    if(primary_input_port* p = dynamic_cast<primary_input_port*> (current_port)){
        return {p, objective.second};
    }
    else if(p_primary_input_port* p = dynamic_cast<p_primary_input_port*> (current_port)){
        return {NULL, objective.second};
    }
    else {
        for(auto gate_ip: current_port->getDriverGate()->getInputPorts()) {
            if(gate_ip->getFaultFreeValue() == -1) {
                int inv_parity = (current_port->getDriverGate()->getInversionParity() + objective.second )% 2;
                auto result = backtrace({gate_ip->getDriverWire()->getDriverPort(), inv_parity});
                if (result.first != NULL) return result;
            }
        }
    }
    return {NULL, objective.second};
}

bool netlist::podem_recursion(port* stuck_port) {
    for(auto po: output_signals) {
        if(po.second->getDvalue() == D || po.second->getDvalue() == D_bar) {
            return true;
        }
    }
    pair<port*, int> obj = getObjective(stuck_port);
    if (obj.first == NULL) return false; // D frontier is empty;
    pair<primary_input_port*, int> pi_value = backtrace(obj);
    if (pi_value.first == NULL) return false;
    pi_value.first->setFaultFreeValue(pi_value.second);
    pi_value.first->setFaultValue(pi_value.second);
    simulate();
    bool result = podem_recursion(stuck_port);
    if(result) {
        return true;
    }
    pi_value.second = (pi_value.second+1)%2;
    pi_value.first->setFaultFreeValue(pi_value.second);
    pi_value.first->setFaultValue(pi_value.second);
    simulate();
    result = podem_recursion(stuck_port);
    if (result) {
        return true;
    }
    pi_value.second = -1;
    pi_value.first->setFaultFreeValue(pi_value.second);
    pi_value.first->setFaultValue(pi_value.second);
    simulate();
    return false;
}


pair<port*, int> netlist::getObjective(port* stuck_port){
    if (stuck_port->getStuckAtFault()) {
        if (stuck_port->getFaultFreeValue() == -1) {
            int comp = (stuck_port->getFaultValue() + 1) % 2;
            return {stuck_port, comp};
        }
        if (stuck_port->getFaultFreeValue() == stuck_port->getFaultValue()) {
            return {NULL, -1};
        }
    }
    node *gate = *D_frontier.begin();
    if (D_frontier.size() <= 0) {
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
                this->PPI.push_back(j+(string("_")+(to_string(i))));        
            }    
        }
        if (i == seq_depth){
            for(auto j : PPO){
                string str_ = string("_")+(to_string(i));
                this->PPO.push_back(j+(str_));
            }
        }
        for(auto j : myNetlist.inputs){
            string str_ = string("_")+(to_string(i));
            unrolled_netlist.inputs.push_back(j+(str_));
            equivalent_port_maping["PI:"+j].insert("PI:"+j+(str_));
        }
        for(auto j : myNetlist.outputs){
            string str_ = string("_")+(to_string(i));
            unrolled_netlist.outputs.push_back(j+(str_));
            equivalent_port_maping["PO:"+j].insert("PO:"+j+(str_));
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

void netlist::print_node_values(){
    for(auto p : port_map){
        cout << "Port " << p.first << " has D: " << p.second->getDvalue() << " f: " << p.second->getFaultValue() << " f_free: " << p.second->getFaultFreeValue() << endl;
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

