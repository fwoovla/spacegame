#include "utils.hpp"



void Signal::Connect(std::function<void()> const& callback) {
    callbacks.push_back(callback);
}

void Signal::DisconnectAll() {
    callbacks.clear();
}

void Signal::EmitSignal() {
    for(int i = 0; i < callbacks.size(); i++) {
        callbacks[i]();
    }
}   
