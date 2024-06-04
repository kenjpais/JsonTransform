#include "json_pack.hpp"
#include <string>
#include <unordered_map>
#include <iostream>

// Function to evaluate JSON path
std::string evaluateJSONPath(JsonPack& jsonPack, const std::string& path) {
    size_t pos = 0;
    JsonPack* currentPack = &jsonPack;
    std::string token;
    while ((pos = path.find('.')) != std::string::npos || (pos = path.find('[')) != std::string::npos) {
        token = path.substr(0, pos);
        if (currentPack->ReadObject()) {
            while (currentPack->ReadMember()) {
                if (std::string(currentPack->Key(), currentPack->KeyLength()) == token) {
                    currentPack = currentPack;  // Proceed to the next nested object
                    break;
                }
            }
        }
        path.erase(0, pos + 1);
    }

    token = path;
    if (currentPack->ReadObject()) {
        while (currentPack->ReadMember()) {
            if (std::string(currentPack->Key(), currentPack->KeyLength()) == token) {
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
                    case JSON_ARRAY:
                    case JSON_OBJECT:
                        // Handle arrays and objects as needed
                        break;
                    default:
                        return "";
                }
            }
        }
    }
    return "";
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
