/*

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <stdexcept>

// Define the ServiceInterface class.
class ServiceInterface {
public:
    virtual ~ServiceInterface() = default;
    virtual std::string execute(const std::string& operationName, const std::string& request, const std::string& url) = 0;
};

// Define the OeService class and its related structs.
struct OeMsg {
    std::string data;
};

class OeService {
public:
    std::string create(const std::string& url, const OeMsg& oeMsg) {
        return "OEPY created at " + url + ": " + oeMsg.data;
    }

    std::string read(const std::string& url, const OeMsg& oeMsg) {
        return "OEPY read at " + url + ": " + oeMsg.data;
    }

    std::string newFunction(const std::string& url, const OeMsg& oeMsg) {
        return "OEPY new function at " + url + ": " + oeMsg.data;
    }
};

// Define the OeAdapter class.
class OeAdapter : public ServiceInterface {
private:
    OeService oeService;
    std::unordered_map<std::string, std::function<std::string(const std::string&, const OeMsg&)>> functionMap;

    OeMsg convertToOeMsg(const std::string& request) {
        return { request };
    }

public:
    OeAdapter() {
        functionMap["create"] = [this](const std::string& url, const OeMsg& oeMsg) {
            return this->oeService.create(url, oeMsg);
        };
        functionMap["read"] = [this](const std::string& url, const OeMsg& oeMsg) {
            return this->oeService.read(url, oeMsg);
        };
        functionMap["newFunction"] = [this](const std::string& url, const OeMsg& oeMsg) {
            return this->oeService.newFunction(url, oeMsg);
        };
    }

    std::string execute(const std::string& operationName, const std::string& request, const std::string& url) override {
        OeMsg oeMsg = convertToOeMsg(request);
        auto it = functionMap.find(operationName);
        if (it != functionMap.end()) {
            return it->second(url, oeMsg);
        } else {
            throw std::invalid_argument("Unsupported operation: " + operationName);
        }
    }
};

// Define the XyzService and XyzAdapter classes similarly.
struct XyzMsg {
    std::string data;
};

class XyzService {
public:
    std::string create(const std::string& url, const XyzMsg& xyzMsg) {
        return "XYZ created at " + url + ": " + xyzMsg.data;
    }

    std::string read(const std::string& url, const XyzMsg& xyzMsg) {
        return "XYZ read at " + url + ": " + xyzMsg.data;
    }

    std::string newFunction(const std::string& url, const XyzMsg& xyzMsg) {
        return "XYZ new function at " + url + ": " + xyzMsg.data;
    }
};

class XyzAdapter : public ServiceInterface {
private:
    XyzService xyzService;
    std::unordered_map<std::string, std::function<std::string(const std::string&, const XyzMsg&)>> functionMap;

    XyzMsg convertToXyzMsg(const std::string& request) {
        return { request };
    }

public:
    XyzAdapter() {
        functionMap["create"] = [this](const std::string& url, const XyzMsg& xyzMsg) {
            return this->xyzService.create(url, xyzMsg);
        };
        functionMap["read"] = [this](const std::string& url, const XyzMsg& xyzMsg) {
            return this->xyzService.read(url, xyzMsg);
        };
        functionMap["newFunction"] = [this](const std::string& url, const XyzMsg& xyzMsg) {
            return this->xyzService.newFunction(url, xyzMsg);
        };
    }

    std::string execute(const std::string& operationName, const std::string& request, const std::string& url) override {
        XyzMsg xyzMsg = convertToXyzMsg(request);
        auto it = functionMap.find(operationName);
        if (it != functionMap.end()) {
            return it->second(url, xyzMsg);
        } else {
            throw std::invalid_argument("Unsupported operation: " + operationName);
        }
    }
};

// Define the OperationFactory class.
class OperationFactory {
private:
    std::unordered_map<std::string, std::unique_ptr<ServiceInterface>> serviceMap;

public:
    OperationFactory() {
        serviceMap["OEPY"] = std::make_unique<OeAdapter>();
        serviceMap["XYZ"] = std::make_unique<XyzAdapter>();
    }

    void performOperation(const std::string& serviceName, const std::string& operationName, const std::string& url, const std::string& requestData) {
        auto it = serviceMap.find(serviceName);
        if (it != serviceMap.end()) {
            std::string response = it->second->execute(operationName, requestData, url);
            std::cout << operationName << " Response: " << response << std::endl;
        } else {
            throw std::invalid_argument("Unsupported service: " + serviceName);
        }
    }
};

// Main function to test the code.
int main() {
    try {
        OperationFactory operationFactory;

        std::cout << "Performing operations using OEPY Service:" << std::endl;
        operationFactory.performOperation("OEPY", "create", "http://oe.host/oe/api/create", "Create Request Data");
        operationFactory.performOperation("OEPY", "read", "http://oe.host/oe/api/read", "Read Request Data");
        operationFactory.performOperation("OEPY", "newFunction", "http://oe.host/oe/api/newFunction", "New Function Request Data");

        std::cout << "\nPerforming operations using XYZ Service:" << std::endl;
        operationFactory.performOperation("XYZ", "create", "http://xyz.host/xyz/api/create", "Create Request Data");
        operationFactory.performOperation("XYZ", "read", "http://xyz.host/xyz/api/read", "Read Request Data");
        operationFactory.performOperation("XYZ", "newFunction", "http://xyz.host/xyz/api/newFunction", "New Function Request Data");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
Explanation
ServiceInterface: Defines the interface for all service adapters.
OeService: Implements the actual OEPY service functions.
OeAdapter: Adapts OeService to the ServiceInterface, providing request conversion and function mapping.
XyzService and XyzAdapter: Similarly implement and adapt the XYZ service.
OperationFactory: Combines the functionality of Operation and ServiceFactory. It stores service adapters and performs operations based on the provided service name and operation details.


*//*1. Define the Operation Class
cpp
Copy code
#include <string>

class Operation {
public:
    std::string serviceName;
    std::string operationName;
    std::string url;

    Operation(const std::string& serviceName, const std::string& operationName, const std::string& url)
        : serviceName(serviceName), operationName(operationName), url(url) {}
};
2. Define a Base Service Interface
cpp
Copy code
#include <string>

class ServiceInterface {
public:
    virtual ~ServiceInterface() = default;
    virtual std::string execute(const Operation& operation, const std::string& request) = 0;
};
3. Implement Adapters for Each Third-Party Application
OEPY Adapter
cpp
Copy code
#include <string>
#include <unordered_map>
#include <functional>
#include <stdexcept>

struct OeMsg {
    std::string data;
};

class OeService {
public:
    std::string create(const Operation& operation, const OeMsg& oeMsg) {
        return "OEPY created at " + operation.url + ": " + oeMsg.data;
    }

    std::string read(const Operation& operation, const OeMsg& oeMsg) {
        return "OEPY read at " + operation.url + ": " + oeMsg.data;
    }

    std::string newFunction(const Operation& operation, const OeMsg& oeMsg) {
        return "OEPY new function at " + operation.url + ": " + oeMsg.data;
    }
};

class OeAdapter : public ServiceInterface {
private:
    OeService oeService;
    std::unordered_map<std::string, std::function<std::string(const Operation&, const OeMsg&)>> functionMap;

    OeMsg convertToOeMsg(const std::string& request) {
        return { request };
    }

public:
    OeAdapter() {
        functionMap["create"] = [this](const Operation& operation, const OeMsg& oeMsg) {
            return this->oeService.create(operation, oeMsg);
        };
        functionMap["read"] = [this](const Operation& operation, const OeMsg& oeMsg) {
            return this->oeService.read(operation, oeMsg);
        };
        functionMap["newFunction"] = [this](const Operation& operation, const OeMsg& oeMsg) {
            return this->oeService.newFunction(operation, oeMsg);
        };
    }

    std::string execute(const Operation& operation, const std::string& request) override {
        OeMsg oeMsg = convertToOeMsg(request);
        if (functionMap.find(operation.operationName) != functionMap.end()) {
            return functionMap[operation.operationName](operation, oeMsg);
        } else {
            throw std::invalid_argument("Unsupported operation: " + operation.operationName);
        }
    }
};
XYZ Adapter
cpp
Copy code
#include <string>
#include <unordered_map>
#include <functional>
#include <stdexcept>

struct XyzMsg {
    std::string data;
};

class XyzService {
public:
    std::string create(const Operation& operation, const XyzMsg& xyzMsg) {
        return "XYZ created at " + operation.url + ": " + xyzMsg.data;
    }

    std::string read(const Operation& operation, const XyzMsg& xyzMsg) {
        return "XYZ read at " + operation.url + ": " + xyzMsg.data;
    }

    std::string newFunction(const Operation& operation, const XyzMsg& xyzMsg) {
        return "XYZ new function at " + operation.url + ": " + xyzMsg.data;
    }
};

class XyzAdapter : public ServiceInterface {
private:
    XyzService xyzService;
    std::unordered_map<std::string, std::function<std::string(const Operation&, const XyzMsg&)>> functionMap;

    XyzMsg convertToXyzMsg(const std::string& request) {
        return { request };
    }

public:
    XyzAdapter() {
        functionMap["create"] = [this](const Operation& operation, const XyzMsg& xyzMsg) {
            return this->xyzService.create(operation, xyzMsg);
        };
        functionMap["read"] = [this](const Operation& operation, const XyzMsg& xyzMsg) {
            return this->xyzService.read(operation, xyzMsg);
        };
        functionMap["newFunction"] = [this](const Operation& operation, const XyzMsg& xyzMsg) {
            return this->xyzService.newFunction(operation, xyzMsg);
        };
    }

    std::string execute(const Operation& operation, const std::string& request) override {
        XyzMsg xyzMsg = convertToXyzMsg(request);
        if (functionMap.find(operation.operationName) != functionMap.end()) {
            return functionMap[operation.operationName](operation, xyzMsg);
        } else {
            throw std::invalid_argument("Unsupported operation: " + operation.operationName);
        }
    }
};
4. Factory to Create Adapters
cpp
Copy code
#include <memory>
#include <stdexcept>

class ServiceFactory {
public:
    static std::unique_ptr<ServiceInterface> createService(const std::string& serviceName) {
        if (serviceName == "OEPY") {
            return std::make_unique<OeAdapter>();
        } else if (serviceName == "XYZ") {
            return std::make_unique<XyzAdapter>();
        } else {
            throw std::invalid_argument("Unsupported service: " + serviceName);
        }
    }
};
5. Main Application
cpp
Copy code
#include <iostream>

void performOperation(ServiceInterface* service, const Operation& operation, const std::string& requestData) {
    std::string response = service->execute(operation, requestData);
    std::cout << operation.operationName << " Response: " << response << std::endl;
}

int main() {
    try {
        Operation oeCreateOperation("OEPY", "create", "http://oe.host/oe/api/create");
        Operation oeReadOperation("OEPY", "read", "http://oe.host/oe/api/read");
        Operation oeNewFunctionOperation("OEPY", "newFunction", "http://oe.host/oe/api/newFunction");

        auto oeService = ServiceFactory::createService(oeCreateOperation.serviceName);
        std::cout << "Performing operations using OEPY Service:" << std::endl;
        performOperation(oeService.get(), oeCreateOperation, "Create Request Data");
        performOperation(oeService.get(), oeReadOperation, "Read Request Data");
        performOperation(oeService.get(), oeNewFunctionOperation, "New Function Request Data");

        Operation xyzCreateOperation("XYZ", "create", "http://xyz.host/xyz/api/create");
        Operation xyzReadOperation("XYZ", "read", "http://xyz.host/xyz/api/read");
        Operation xyzNewFunctionOperation("XYZ", "newFunction", "http://xyz.host/xyz/api/newFunction");

        auto xyzService = ServiceFactory::createService(xyzCreateOperation.serviceName);
        std::cout << "\nPerforming operations using XYZ Service:" << std::endl;
        performOperation(xyzService.get(), xyzCreateOperation, "Create Request Data");
        performOperation(xyzService.get(), xyzReadOperation, "Read Request Data");
        performOperation(xyzService.get(), xyzNewFunctionOperation, "New Function Request Data");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}*/*#include <functional>
#include <unordered_map>
#include <string>

class ThirdPartyService {
public:
    using ServiceFunction = std::function<std::string(const std::string&)>;
    virtual ~ThirdPartyService() = default;
    
    std::unordered_map<std::string, ServiceFunction> functionMap;

    virtual std::string execute(const std::string& functionName, const std::string& request) {
        if (functionMap.find(functionName) != functionMap.end()) {
            return functionMap[functionName](request);
        }
        return "Function not found";
    }
};
2. Implement Adapters for Each Third-Party Application
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

    // New function
    std::string newFunction(const OeMsg& oeMsg) {
        return "OEPY new function: " + oeMsg.data;
    }
};

class OeServiceAdapter : public ThirdPartyService {
private:
    OeService oeService;

    OeMsg convertToOeMsg(const std::string& request) {
        return { request };
    }

public:
    OeServiceAdapter() {
        functionMap["create"] = [this](const std::string& request) {
            return this->create(request);
        };
        functionMap["read"] = [this](const std::string& request) {
            return this->read(request);
        };
        functionMap["newFunction"] = [this](const std::string& request) {
            return this->newFunction(request);
        };
    }

    std::string create(const std::string& request) {
        OeMsg oeMsg = convertToOeMsg(request);
        return oeService.create(oeMsg);
    }

    std::string read(const std::string& request) {
        OeMsg oeMsg = convertToOeMsg(request);
        return oeService.read(oeMsg);
    }

    // New function
    std::string newFunction(const std::string& request) {
        OeMsg oeMsg = convertToOeMsg(request);
        return oeService.newFunction(oeMsg);
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

    // New function
    std::string newFunction(const XyzMsg& xyzMsg) {
        return "XYZ new function: " + xyzMsg.data;
    }
};

class XyzServiceAdapter : public ThirdPartyService {
private:
    XyzService xyzService;

    XyzMsg convertToXyzMsg(const std::string& request) {
        return { request };
    }

public:
    XyzServiceAdapter() {
        functionMap["create"] = [this](const std::string& request) {
            return this->create(request);
        };
        functionMap["read"] = [this](const std::string& request) {
            return this->read(request);
        };
        functionMap["newFunction"] = [this](const std::string& request) {
            return this->newFunction(request);
        };
    }

    std::string create(const std::string& request) {
        XyzMsg xyzMsg = convertToXyzMsg(request);
        return xyzService.create(xyzMsg);
    }

    std::string read(const std::string& request) {
        XyzMsg xyzMsg = convertToXyzMsg(request);
        return xyzService.read(xyzMsg);
    }

    // New function
    std::string newFunction(const std::string& request) {
        XyzMsg xyzMsg = convertToXyzMsg(request);
        return xyzService.newFunction(xyzMsg);
    }
};
3. Factory to Create Adapters
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
4. Main Application
cpp
Copy code
void performOperation(ThirdPartyService* service, const std::string& functionName, const std::string& requestData) {
    std::string response = service->execute(functionName, requestData);
    std::cout << functionName << " Response: " << response << std::endl;
}

int main() {
    // Example operations
    Operation oeOperation("OEPY", "http://oe.host", "/oe/api");
    Operation xyzOperation("XYZ", "http://xyz.host", "/xyz/api");

    try {
        auto oeService = ServiceFactory::createService(oeOperation);
        std::cout << "Performing operations using OEPY Service:" << std::endl;
        performOperation(oeService.get(), "create", "Create Request Data");
        performOperation(oeService.get(), "read", "Read Request Data");
        performOperation(oeService.get(), "newFunction", "New Function Request Data");

        auto xyzService = ServiceFactory::createService(xyzOperation);
        std::cout << "\nPerforming operations using XYZ Service:" << std::endl;
        performOperation(xyzService.get(), "create", "Create Request Data");
        performOperation(xyzService.get(), "read", "Read Request Data");
        performOperation(xyzService.get(), "newFunction", "New Function Request Data");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}*/
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
