#include <App.h>
#include <iostream>
#include <fstream>
const int  _PORT_ = 3000;

std::string fileToString(std::string);

int main() {
    struct PerSocketData {};
    uWS::App app = uWS::App();

    app.get("/chat",[](auto *res, auto *req){
        std::string t = fileToString("client/index.html");
        res->end(t);
    });

    app.ws<PerSocketData>("/*", {
        .compression = uWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024,
        .idleTimeout = 0,
        .maxBackpressure = 1 * 1024 * 1204,

        .open = [](auto *ws, auto *req) {
			std::cout << "A client has connected" << std::endl;
			ws->subscribe("broadcast");
            ws->publish("broadcast", "A new user has joined");
        },
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
			std::cout << "Message: " << message << std::endl;
            ws->publish("broadcast", message, opCode);
        },       
    });

    app.listen(_PORT_, [](auto *token) {
        if (token) {
            std::cout << "Listening on port " << _PORT_ << std::endl;
        }
    });

    app.run(); 
}

std::string fileToString(std::string filePath){
    std::string htmlText;
    std::string line;
    std::ifstream file(filePath);

    while(getline(file,line)){
        std::string t = line + "\n";
        htmlText.append(t);
    } 
        
    file.close();

    return htmlText;
} 