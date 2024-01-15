#include <string>

#include "../ServiceCoreSDK.h"
#include "./controllers/TestSuitesController.h"
#include "./controllers/TestSuiteRunsController.h"
#include "./controllers/TestCasesController.h"

using json = nlohmann::json;

TestSuitesController *testSuitesController;
TestSuiteRunsController *testSuiteRunsController;
TestCasesController *testCasesController;

void AnnounceAPIs();

void AnnounceTestSuitesAPIS();

// Test Suites request handlers
json getAllTestSuitesAnalytics(Request Req);

json getTestSuite(Request Req);

json getAllTestSuite(Request Req);

json postTestSuite(Request Req);

json editTestSuite(Request Req);

json deleteTestSuite(Request Req);

json parseTestSuite(Request Req);

json getTestSuiteRuns(Request Req);

json getTestCases(Request Req);

json runTestSuite(Request Req);

void PreTerminateFunction()
{
    std::cout << "PreTerminateFunction " << std::endl;
}

int main(int argc, const char *argv[])
{
    testSuitesController = new TestSuitesController();
    testSuiteRunsController = new TestSuiteRunsController();
    testCasesController = new TestCasesController();

    ServiceCoreSDK::SetServiceType(ServiceCoreSDK::PARENT_DEPENDENT_SERVICE);
    AnnounceAPIs();
    ServiceCoreSDK::RegisterSIGINTCallbackFunction(PreTerminateFunction);
    return ServiceCoreSDK::RunService(argc, argv);
}

void AnnounceAPIs()
{
    // Announcing Get all test suites api
    AnnounceTestSuitesAPIS();
}

void AnnounceTestSuitesAPIS()
{

    std::string getTestSuites_strAPIName = "getAllTestSuitesAnalytics";
    json getTestSuites_jsonParams;
    ServiceCoreSDK::RegisterFunctionCallback(getTestSuites_strAPIName, getTestSuites_jsonParams,
                                             getAllTestSuitesAnalytics);
    std::string getTestSuite_strAPIName = "getTestSuite";
    json getTestSuite_jsonParams = {{"id", "int"}};
    ServiceCoreSDK::RegisterFunctionCallback(getTestSuite_strAPIName, getTestSuite_jsonParams, getTestSuite);

    std::string postTestSuite_strAPIName = "postTestSuite";
    json postTestSuite_jsonParams = {{"testSuite", "object"}};
    ServiceCoreSDK::RegisterFunctionCallback(postTestSuite_strAPIName, postTestSuite_jsonParams, postTestSuite);

    std::string editTestSuite_strAPIName = "editTestSuite";
    json editTestSuite_jsonParams = {{"testSuite", "object"}};
    ServiceCoreSDK::RegisterFunctionCallback(editTestSuite_strAPIName, editTestSuite_jsonParams, editTestSuite);

    std::string deleteTestSuite_strAPIName = "deleteTestSuite";
    json deleteTestSuite_jsonParams = {{"testSuite", "object"}};
    ServiceCoreSDK::RegisterFunctionCallback(deleteTestSuite_strAPIName, deleteTestSuite_jsonParams, deleteTestSuite);

    std::string getAllTestSuite_strAPIName = "getAllTestSuite";
    json getAllTestSuite_jsonParams;
    ServiceCoreSDK::RegisterFunctionCallback(getAllTestSuite_strAPIName, getAllTestSuite_jsonParams, getAllTestSuite);

    std::string parseTestSuite_strAPIName = "parseTestSuite";
    json parseTestSuite_jsonParams = {{"id", "int"},
                                      {"userId", "int"}};
    ServiceCoreSDK::RegisterFunctionCallback(parseTestSuite_strAPIName, parseTestSuite_jsonParams, parseTestSuite);

    std::string getTestSuiteRuns_strAPIName = "getTestSuiteRuns";
    json getTestSuiteRuns_jsonParams = {{"id", "int"}};
    ServiceCoreSDK::RegisterFunctionCallback(getTestSuiteRuns_strAPIName, getTestSuiteRuns_jsonParams, getTestSuiteRuns);

    std::string getTestCases_strAPIName = "getTestCases";
    json getTestCases_jsonParams = {{"id", "int"}};
    ServiceCoreSDK::RegisterFunctionCallback(getTestCases_strAPIName, getTestCases_jsonParams, getTestCases);

    std::string runTestSuite_strAPIName = "runTestSuite";
    json runTestSuite_jsonParams = {{"id", "int"},
                                    {"testCases", "object"}};
    ServiceCoreSDK::RegisterFunctionCallback(runTestSuite_strAPIName, runTestSuite_jsonParams, runTestSuite);
}

json getAllTestSuitesAnalytics(Request Req)
{
    json response = testSuitesController->getAllTestSuitesAnalytics(Req.Parameters["userId"]);
    return response;
}

json getTestSuite(Request Req)
{
    json response = testSuitesController->getTestSuiteById(Req.Parameters["id"]);
    return response;
}

json postTestSuite(Request Req)
{
    json response = testSuitesController->createTestSuite(Req.Parameters["testSuite"], Req.Parameters["userId"]);
    return response;
}

json editTestSuite(Request Req)
{
    json response = testSuitesController->editTestSuite(Req.Parameters["testSuite"]);
    return response;
}

json deleteTestSuite(Request Req)
{
    json response = testSuitesController->deleteTestSuite(Req.Parameters["id"]);
    return response;
}

json getAllTestSuite(Request Req)
{
    json response = testSuitesController->getAllTestSuites(Req.Parameters["userId"]);
    return response;
}

json parseTestSuite(Request Req)
{
    json response = testSuitesController->parseTestSuite(Req.Parameters["id"]);
    return response;
}

json getTestSuiteRuns(Request Req)
{
    json response = testSuiteRunsController->getTestSuiteRuns(Req.Parameters["id"]);
    return response;
}

json getTestCases(Request Req)
{
    json response = testCasesController->getTestCases(Req.Parameters["id"]);
    return response;
}

json runTestSuite(Request Req)
{
    json response = testSuiteRunsController->runTestSuite(Req.Parameters["id"], Req.Parameters["testCases"]);
    return response;
}
