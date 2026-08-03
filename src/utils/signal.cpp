#include "utils.hpp"



int Signal::Connect(std::function<void()> const& callback) {
    connections.push_back({next_id++, callback});
    return next_id++;
}

void Signal::DisconnectAll() {
    connections.clear();
}

void Signal::EmitSignal() {
    for(int i = 0; i < connections.size(); i++) {
        connections[i].callback();
    }
}   


void Signal::Disconnect(int id) {
    std::erase_if(connections,
        [id](const Connection &c)
        {
            return c.id == id;
        });
}
