#include <iostream>
#include <fstream>
#include <nlohmann\json.hpp>

using json = nlohmann::json;

int main() {
    json data;

    data["name"] = "HeavyDriver";
    data["score"] = 100;
    data["tasks"] = {"Eat", "Code", "Sleep"};

    std::ofstream file("data.json");

    if (file.is_open()) {
        file << data.dump(4); // pretty print with indent
        file.close();
        std::cout << "JSON saved\n";
    }

    return 0;
}