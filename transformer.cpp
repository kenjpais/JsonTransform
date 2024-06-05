#include "json_pack.hpp"
#include <string>
#include <unordered_map>
#include <iostream>
JSONValue resolveExpression(const JSONValue& input, const std::string& expression) {
    std::string result = expression;
    size_t pos = 0;
    while ((pos = result.find("${", pos)) != std::string::npos) {
        size_t endPos = result.find('}', pos);
        if (endPos == std::string::npos) {
            // Invalid placeholder, continue searching from the next character
            ++pos;
            continue;
        }
        std::string placeholder = result.substr(pos + 2, endPos - pos - 2);
        JSONValue placeholderValue = resolvePath(input, placeholder);
        std::string replacement;
        if (std::holds_alternative<std::string>(placeholderValue.value)) {
            replacement = std::get<std::string>(placeholderValue.value);
        } else {
            replacement = std::to_string(placeholderValue);
        }
        result.replace(pos, endPos - pos + 1, replacement);
        pos += replacement.length(); // Move to the next position after the replacement
    }
    return JSONValue(result);
}

JSONValue resolveExpression(const JSONValue& input, const std::string& expression) {
    try {
        std::regex placeholderRegex("\\$\\{([^}]*)\\}");
        std::string result = expression;
        std::smatch match;
        while (std::regex_search(result, match, placeholderRegex)) {
            if (match.size() != 2) {
                throw std::runtime_error("Invalid placeholder match");
            }
            std::string placeholder = match[1];
            JSONValue placeholderValue = resolvePath(input, placeholder);
            std::string replacement;
            if (std::holds_alternative<std::string>(placeholderValue.value)) {
                replacement = std::get<std::string>(placeholderValue.value);
            } else {
                replacement = std::to_string(placeholderValue);
            }
            result.replace(match.position(), match.length(), replacement);
        }
        return result;
    } catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
        return expression; // Return the original expression in case of regex error
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return expression; // Return the original expression in case of other errors
    }
}

JSONValue resolveExpression(const JSONValue& input, const std::string& expression) {
        static std::regex regex("\\$\\{([^}]*)\\}");
        std::smatch match;
        std::string expr = expression;
        while (std::regex_search(expr, match, regex)) {
            std::string placeholder = match[0];
            std::string path = match[1];
            JSONValue result = resolvePath(input, path);
            std::string value;
            if (std::holds_alternative<std::string>(result.value)) {
                value = std::get<std::string>(result.value);
            } else {
                std::ostringstream oss;
                oss << result;
                value = oss.str();
            }
            expr.replace(match.position(), placeholder.length(), value);
        }
        return expr;
    }
JSONValue extractValue(const JSONValue& data, const std::string& path) {
    const JSONValue* current = &data;
    size_t start = 0;
    size_t end = 0;

    while (start < path.length()) {
        end = path.find_first_of(".[", start);

        std::string key = (end == std::string::npos) ? path.substr(start) : path.substr(start, end - start);

        if (!key.empty()) {
            if (!current->isObject() || current->getObject().find(key) == current->getObject().end()) {
                throw std::runtime_error("Invalid path: key '" + key + "' not found");
            }
            current = &current->getObject().at(key);
        }

        if (end != std::string::npos && path[end] == '[') {
            start = end + 1;
            end = path.find(']', start);
            if (end == std::string::npos) {
                throw std::runtime_error("Invalid path: unmatched '['");
            }
            size_t index = std::stoi(path.substr(start, end - start));
            start = end + 1;

            if (!current->isArray() || index >= current->getArray().size()) {
                throw std::runtime_error("Invalid path: array index out of bounds");
            }
            current = &current->getArray().at(index);
        } else {
            start = (end == std::string::npos) ? path.length() : end + 1;
        }
    }

    return *current;
}
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
