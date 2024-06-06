/**/
/*#include <string>

class Operation {
public:
    std::string serviceName;
    std::string host;
    std::string url;

    Operation(const std::string& serviceName, const std::string& host, const std::string& url)
        : serviceName(serviceName), host(host), url(url) {}
};
2. Define Common Interfaces
cpp
Copy code
class ThirdPartyService {
public:
    virtual ~ThirdPartyService() = default;
    virtual std::string create(const std::string& request) = 0;
    virtual std::string read(const std::string& request) = 0;
    virtual std::string update(const std::string& request) = 0;
    virtual std::string deleteOp(const std::string& request) = 0;
};
3. Create Adapters for Each Third-Party Application
OEPY Adapter
cpp
Copy code
struct OeMsg {
    std::string data;
};

class OeService {
public:
    std::string create(const OeMsg& oeMsg) {
        return "OEPY created: " + oeMsg.data;
    }

    std::string read(const OeMsg& oeMsg) {
        return "OEPY read: " + oeMsg.data;
    }

    std::string update(const OeMsg& oeMsg) {
        return "OEPY updated: " + oeMsg.data;
    }

    std::string deleteOp(const OeMsg& oeMsg) {
        return "OEPY deleted: " + oeMsg.data;
    }
};

class OeServiceAdapter : public ThirdPartyService {
private:
    OeService oeService;

    OeMsg convertToOeMsg(const std::string& request) {
        return { request };
    }

public:
    std::string create(const std::string& request) override {
        OeMsg oeMsg = convertToOeMsg(request);
        return oeService.create(oeMsg);
    }

    std::string read(const std::string& request) override {
        OeMsg oeMsg = convertToOeMsg(request);
        return oeService.read(oeMsg);
    }

    std::string update(const std::string& request) override {
        OeMsg oeMsg = convertToOeMsg(request);
        return oeService.update(oeMsg);
    }

    std::string deleteOp(const std::string& request) override {
        OeMsg oeMsg = convertToOeMsg(request);
        return oeService.deleteOp(oeMsg);
    }
};
XYZ Adapter
cpp
Copy code
struct XyzMsg {
    std::string data;
};

class XyzService {
public:
    std::string create(const XyzMsg& xyzMsg) {
        return "XYZ created: " + xyzMsg.data;
    }

    std::string read(const XyzMsg& xyzMsg) {
        return "XYZ read: " + xyzMsg.data;
    }

    std::string update(const XyzMsg& xyzMsg) {
        return "XYZ updated: " + xyzMsg.data;
    }

    std::string deleteOp(const XyzMsg& xyzMsg) {
        return "XYZ deleted: " + xyzMsg.data;
    }
};

class XyzServiceAdapter : public ThirdPartyService {
private:
    XyzService xyzService;

    XyzMsg convertToXyzMsg(const std::string& request) {
        return { request };
    }

public:
    std::string create(const std::string& request) override {
        XyzMsg xyzMsg = convertToXyzMsg(request);
        return xyzService.create(xyzMsg);
    }

    std::string read(const std::string& request) override {
        XyzMsg xyzMsg = convertToXyzMsg(request);
        return xyzService.read(xyzMsg);
    }

    std::string update(const std::string& request) override {
        XyzMsg xyzMsg = convertToXyzMsg(request);
        return xyzService.update(xyzMsg);
    }

    std::string deleteOp(const std::string& request) override {
        XyzMsg xyzMsg = convertToXyzMsg(request);
        return xyzService.deleteOp(xyzMsg);
    }
};
4. Factory to Create Adapters
cpp
Copy code
#include <memory>
#include <stdexcept>

class ServiceFactory {
public:
    static std::unique_ptr<ThirdPartyService> createService(const Operation& operation) {
        if (operation.serviceName == "OEPY") {
            return std::make_unique<OeServiceAdapter>();
        } else if (operation.serviceName == "XYZ") {
            return std::make_unique<XyzServiceAdapter>();
        } else {
            throw std::invalid_argument("Unsupported service: " + operation.serviceName);
        }
    }
};
5. Main Application
cpp
Copy code
void performCRUD(ThirdPartyService* service) {
    std::string createResponse = service->create("Create Request Data");
    std::cout << "Create Response: " << createResponse << std::endl;

    std::string readResponse = service->read("Read Request Data");
    std::cout << "Read Response: " << readResponse << std::endl;

    std::string updateResponse = service->update("Update Request Data");
    std::cout << "Update Response: " << updateResponse << std::endl;

    std::string deleteResponse = service->deleteOp("Delete Request Data");
    std::cout << "Delete Response: " << deleteResponse << std::endl;
}

int main() {
    // Example operations
    Operation oeOperation("OEPY", "http://oe.host", "/oe/api");
    Operation xyzOperation("XYZ", "http://xyz.host", "/xyz/api");

    try {
        auto oeService = ServiceFactory::createService(oeOperation);
        std::cout << "Performing CRUD operations using OEPY Service:" << std::endl;
        performCRUD(oeService.get());

        auto xyzService = ServiceFactory::createService(xyzOperation);
        std::cout << "\nPerforming CRUD operations using XYZ Service:" << std::endl;
        performCRUD(xyzService.get());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
Define the Target Interface for the Services:

cpp
Copy code
// Target.h
#ifndef TARGET_H
#define TARGET_H

#include <string>

class Target {
public:
    virtual ~Target() = default;
    virtual std::string createStandingInstruction(const std::string& data) const = 0;
    virtual std::string createMandate(const std::string& data) const = 0;
    virtual std::string createPayment(const std::string& data) const = 0;
};

#endif // TARGET_H
Define the Adaptees for ABC and XYZ Services:

cpp
Copy code
// ABCService.h
#ifndef ABC_SERVICE_H
#define ABC_SERVICE_H

#include <string>

class ABCService {
public:
    std::string createABCStandingInstruction(const std::string& data) const {
        return "ABC Standing Instruction: " + data;
    }
    std::string createABCMandate(const std::string& data) const {
        return "ABC Mandate: " + data;
    }
    std::string createABCPayment(const std::string& data) const {
        return "ABC Payment: " + data;
    }
};

#endif // ABC_SERVICE_H
cpp
Copy code
// XYZService.h
#ifndef XYZ_SERVICE_H
#define XYZ_SERVICE_H

#include <string>

class XYZService {
public:
    std::string createXYZStandingInstruction(const std::string& data) const {
        return "XYZ Standing Instruction: " + data;
    }
    std::string createXYZMandate(const std::string& data) const {
        return "XYZ Mandate: " + data;
    }
    std::string createXYZPayment(const std::string& data) const {
        return "XYZ Payment: " + data;
    }
};

#endif // XYZ_SERVICE_H
Implement the Adapters:

cpp
Copy code
// ABCAdapter.h
#ifndef ABC_ADAPTER_H
#define ABC_ADAPTER_H

#include "Target.h"
#include "ABCService.h"

class ABCAdapter : public Target {
private:
    ABCService *abcService;

public:
    ABCAdapter(ABCService *service) : abcService(service) {}
    
    std::string createStandingInstruction(const std::string& data) const override {
        return abcService->createABCStandingInstruction(data);
    }

    std::string createMandate(const std::string& data) const override {
        return abcService->createABCMandate(data);
    }

    std::string createPayment(const std::string& data) const override {
        return abcService->createABCPayment(data);
    }
};

#endif // ABC_ADAPTER_H
cpp
Copy code
// XYZAdapter.h
#ifndef XYZ_ADAPTER_H
#define XYZ_ADAPTER_H

#include "Target.h"
#include "XYZService.h"

class XYZAdapter : public Target {
private:
    XYZService *xyzService;

public:
    XYZAdapter(XYZService *service) : xyzService(service) {}

    std::string createStandingInstruction(const std::string& data) const override {
        return xyzService->createXYZStandingInstruction(data);
    }

    std::string createMandate(const std::string& data) const override {
        return xyzService->createXYZMandate(data);
    }

    std::string createPayment(const std::string& data) const override {
        return xyzService->createXYZPayment(data);
    }
};

#endif // XYZ_ADAPTER_H
Main Application:

cpp
Copy code
// main.cpp
#include "Target.h"
#include "ABCAdapter.h"
#include "XYZAdapter.h"
#include <iostream>

void ClientCode(const Target *service) {
    std::cout << service->createStandingInstruction("Standing Instruction Data") << std::endl;
    std::cout << service->createMandate("Mandate Data") << std::endl;
    std::cout << service->createPayment("Payment Data") << std::endl;
}

int main() {
    ABCService abcService;
    XYZService xyzService;

    ABCAdapter abcAdapter(&abcService);
    XYZAdapter xyzAdapter(&xyzService);

    std::cout << "Client: Using ABC service via ABCAdapter:\n";
    ClientCode(&abcAdapter);
    std::cout << "\n";

    std::cout << "Client: Using XYZ service via XYZAdapter:\n";
    ClientCode(&xyzAdapter);

    return 0;
}*/#include "json_pack.hpp"
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
