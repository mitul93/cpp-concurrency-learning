/*
 * This is not a complete, standalone example.
 * The code is intended only to illustrate the concept.
 * This code will compile, but it will do noting!
 */

#include <future>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using payload_type = std::string;

// dummy incoming packet
struct data_packet {
    int id;
    payload_type payload;
};

// dummy outgoing packet
struct outgoing_packet {
    payload_type payload;
    std::promise<bool> promise;
};

struct connection {
    bool has_incoming_data() const {
        return true;
    }

    data_packet incoming() {
        return {42, "Reply from server"};
    }

    std::promise<payload_type>& get_promise(int id) {
        return incoming_promises[id];
    }

    std::future<payload_type> get_future(int id) {
        return incoming_promises[id].get_future();
    }

    bool has_outgoing_data() const {
        return !outgoing.empty();
    }

    outgoing_packet top_of_outgoing_queue() {
        outgoing_packet p = std::move(outgoing.front());
        outgoing.pop();
        return p;
    }

    void push_outgoing(outgoing_packet p) {
        outgoing.push(std::move(p));
    }

    void send(const payload_type& payload) {
        std::cout << "Sending: " << payload << '\n';
    }

  private:
    std::unordered_map<int, std::promise<payload_type>> incoming_promises;
    std::queue<outgoing_packet> outgoing;
};

std::vector<connection> connection_set;

void process_connections(std::vector<connection>& connections) {
    for (auto& conn : connections) {
        // Handle outgoing requests
        if (conn.has_outgoing_data()) {
            outgoing_packet data = conn.top_of_outgoing_queue();
            conn.send(data.payload);
            data.promise.set_value(true);
        }

        // Handle incoming requests
        if (conn.has_incoming_data()) {
            data_packet data = conn.incoming();
            std::promise<payload_type>& p = conn.get_promise(data.id);
            p.set_value(data.payload);
        }
    }
}

int main() {
    std::vector<connection> connections(1);

    // Wait for an incoming reply for id=42
    auto reply = connections[0].get_future(42);

    // Queue an outgoing message
    outgoing_packet pkt;
    pkt.payload = "Hello server";

    auto sent = pkt.promise.get_future();

    // moving pkt here is okay. future is read already
    connections[0].push_outgoing(std::move(pkt));

    process_connections(connections);

    std::cout << "Reply: " << reply.get() << '\n';
    std::cout << "Sent: " << std::boolalpha << sent.get() << '\n';
}