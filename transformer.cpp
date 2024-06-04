#include "json_pack.hpp"
#include <string>
#include <unordered_map>
#include <iostream>

std::string evaluateJSONPath(JsonPack& jsonPack, const std::string& path) {
    size_t pos = 0;
    JsonPack* currentPack = &jsonPack;
    std::string currentPath = path;
    
    while (true) {
        pos = currentPath.find_first_of(".[");
        std::string token = currentPath.substr(0, pos);

        if (currentPack->ReadObject()) {
            bool found = false;
            while (currentPack->ReadMember()) {
                if (std::string(currentPack->Key(), currentPack->KeyLength()) == token) {
                    found = true;
                    break;
                }
            }
            if (!found) return ""; // Path not found
        }

        if (pos == std::string::npos) break; // End of path
        currentPath = currentPath.substr(pos);

        if (currentPath[0] == '[') {
            size_t endPos = currentPath.find(']');
            if (endPos == std::string::npos) return ""; // Malformed path
            int index = std::stoi(currentPath.substr(1, endPos - 1));
            if (currentPack->ValueType() == JSON_ARRAY) {
                for (int i = 0; i <= index; ++i) {
                    currentPack->ReadValue();
                    if (i < index) currentPack->ReadValue();
                }
            }
            currentPath = currentPath.substr(endPos + 1);
        } else {
            currentPath = currentPath.substr(1);
        }
    }

    switch (currentPack->ValueType()) {
        case JSON_STRING:
            return std::string(currentPack->Value(), currentPack->ValueLength());
        case JSON_INTEGER:
            return std::to_string(currentPack->Quantity());
        case JSON_DECIMAL:
            return std::to_string(currentPack->Number());
        case JSON_BOOLEAN:
            return currentPack->Flag() ? "true" : "false";
        case JSON_NULL:
            return "null";
        default:
            return "";
    }
}


// Function to parse the transformation JSON
std::unordered_map<std::string, std::string> parseTransformationJson(char* transformationData, int len) {
    JsonPack transPack(transformationData, len);
    std::unordered_map<std::string, std::string> mapping;

    if (transPack.ReadObject()) {
        while (transPack.ReadMember()) {
            std::string key(transPack.Key(), transPack.KeyLength());
            std::string value(transPack.Value(), transPack.ValueLength());
            mapping[key] = value;
        }
    }

    return mapping;
}

// Function to transform the input JSON based on the mappings
void transformJson(const std::unordered_map<std::string, std::string>& mapping, char* inputData, int inputLen, std::ostream& output) {
    JsonPack inputPack(inputData, inputLen);

    output << "{";
    if (inputPack.ReadObject()) {
        bool first = true;
        for (const auto& pair : mapping) {
            if (!first) {
                output << ",";
            }
            first = false;

            std::string value = evaluateJSONPath(inputPack, pair.second);

            output << "\"" << pair.first << "\": \"" << value << "\"";
        }
    }
    output << "}";
}

int main() {
    char inputData[] = R"({
        "exasSITypeDtls": {
            "externalRefNum": "ke113n"
        },
        "exasSIAmtAndFreqDtls": {
            "amounts": [100, 200]
        }
    })";

    char transformationData[] = R"({
        "siRefNum": "exasSITypeDtls.externalRefNum",
        "siAmount": "exasSIAmtAndFreqDtls.amounts[1]"
    })";

    auto mapping = parseTransformationJson(transformationData, sizeof(transformationData) - 1);

    std::cout << "Transformed JSON: ";
    transformJson(mapping, inputData, sizeof(inputData) - 1, std::cout);

    return 0;
}
