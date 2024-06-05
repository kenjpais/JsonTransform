#include "json_pack.hpp"
#include <string>
#include <unordered_map>
#include <iostream>

std::pair<std::string, size_t> parseArrayToken(const std::string& token) {
    size_t startPos = token.find('[');
    size_t endPos = token.find(']');
    if (startPos == std::string::npos || endPos == std::string::npos || endPos <= startPos + 1) {
        throw std::runtime_error("Invalid array token: " + token);
    }
    std::string key = token.substr(0, startPos);
    size_t index = std::stoi(token.substr(startPos + 1, endPos - startPos - 1));
    return {key, index};
}

JSONValue extractValue(const JSONValue& data, const std::string& path) {
    const JSONValue* current = &data;
    std::istringstream ss(path);
    std::string token;

    while (std::getline(ss, token, '.')) {
        // Check if the token has array indexing
        if (token.find('[') != std::string::npos) {
            try {
                auto [key, index] = parseArrayToken(token);
                // Ensure current is an object and contains the key
                if (!current->isObject() || current->getObject().find(key) == current->getObject().end()) {
                    throw std::runtime_error("Invalid path: key not found");
                }
                // Ensure the key maps to an array and the index is valid
                const JSONValue& arrayValue = current->getObject().at(key);
                if (!arrayValue.isArray() || index >= arrayValue.getArray().size()) {
                    throw std::runtime_error("Invalid path: array index out of bounds");
                }
                current = &arrayValue.getArray().at(index);
            } catch (const std::exception& e) {
                throw std::runtime_error("Error parsing array token: " + std::string(e.what()));
            }
        } else {
            // Ensure current is an object and contains the key
            if (!current->isObject() || current->getObject().find(token) == current->getObject().end()) {
                throw std::runtime_error("Invalid path: key not found");
            }
            current = &current->getObject().at(token);
        }
    }

    return *current;
}

// Split a string by a delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to evaluate JSON path
std::string evaluateJSONPath(JsonPack& jsonPack, const std::string& path) {
    std::vector<std::string> tokens = split(path, '.');
    JsonPack* currentPack = &jsonPack;

    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string token = tokens[i];
        size_t arrayIndexPos = token.find('[');
        size_t arrayIndex = std::string::npos;
        
        // Check if the token includes an array index
        if (arrayIndexPos != std::string::npos) {
            arrayIndex = std::stoi(token.substr(arrayIndexPos + 1, token.find(']') - arrayIndexPos - 1));
            token = token.substr(0, arrayIndexPos);
        }

        // Traverse the object to find the token
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

        // If array index exists, traverse the array
        if (arrayIndex != std::string::npos) {
            if (currentPack->ValueType() == JSON_ARRAY) {
                for (size_t j = 0; j <= arrayIndex; ++j) {
                    if (!currentPack->ReadValue()) return ""; // Array index out of bounds
                    if (j < arrayIndex) currentPack->ReadValue();
                }
            }
        }
    }

    // Extract the value based on its type
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
