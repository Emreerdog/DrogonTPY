#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <drogon/drogon.h>
#include <drogon/WebSocketController.h>
#include <XE/FileStream.h>
#include <XE/Memory.h>
#include <XE/System.h>
#include <XE/CLI.h>
#include <XE/Threading.h>
#include <XE/Stack.h>
#include <XE/Logger.h>
#include <XE/SubSystemMacros.h>
#include <XE/PyWrapper.h>
#include <XE/StringUtils.h>
#include <stdio.h>
#include <stdarg.h>

#pragma comment(lib,"WS2_32")
using namespace drogon;

//XESYSTEM FUNC
PyObject* xeCallToCLI(PyObject* self, PyObject* command)
{
    char* typeCheck = (char*)xepToString(command);
    PyObject* returnedResult = NULL;
    if(typeCheck == NULL)
    {
        const char* err = "Call to CLI, failed with type conversion error, make sure you call the function with string argument";
        returnedResult = XE_PY_STR(err);
        return returnedResult;
    }
    
    I32 commandResult = xeProcessArguments(typeCheck);
    returnedResult = XE_PY_LONG(commandResult);
    return returnedResult;
}

// XEHTTP FUNC
PyObject* xeReqParamCount(PyObject* self, PyObject* capsulatedInfo)
{
    HttpRequest *req = (HttpRequest*)PyCapsule_GetPointer(capsulatedInfo, NULL);
    PyObject* resultToPy;
    if(req == NULL)
    {
        resultToPy = XE_PY_LONG(-1);
        return resultToPy;
    }
    resultToPy = XE_PY_LONG(req->getParameters().size());
    return resultToPy;
}

// XEHTTP FUNC
PyObject* xeReqPeerIp(PyObject* self, PyObject* capsulatedInfo)
{
    HttpRequest *req = (HttpRequest*)PyCapsule_GetPointer(capsulatedInfo, NULL);
    PyObject* resultToPy;
    if(req == NULL)
    {
        printf("Request is empty \n");
        resultToPy = XE_PY_STR(NULL);
    }
    resultToPy = XE_PY_STR(req->getPeerAddr().toIp().c_str());
    return resultToPy;
}

// XEWS FUNC
PyObject* xeWSReqPeerIp(PyObject* self, PyObject* capsulatedInfo)
{
    WebSocketConnection* wCon = (WebSocketConnection*)PyCapsule_GetPointer(capsulatedInfo, NULL);
    PyObject* resultToPy;
    if(wCon == NULL)
    {
        resultToPy = XE_PY_STR(NULL);
    }
    resultToPy = XE_PY_STR(wCon->peerAddr().toIp().c_str());
    return resultToPy;
}

// XEWS FUNC
PyObject* xeWSSend(PyObject* self, PyObject* args)
{
    PyObject* capsulatedInfo = PyTuple_GetItem(args, 0);
    WebSocketConnection* wCon = (WebSocketConnection*)PyCapsule_GetPointer(capsulatedInfo, NULL);
    PyObject* resultToPy;
    if(wCon == NULL)
    {
        wCon->send("Failed to send message, unable to pass socket info");
        resultToPy = XE_PY_LONG(-1);
        return resultToPy;
    }
    else
    {
        PyObject* message = PyTuple_GetItem(args, 1);
        const char* convertedString = xepToString(message);
        if(convertedString == NULL)
        {
            wCon->send("Failed to send message, type is incorrect");
        }
        wCon->send(convertedString);
        resultToPy = XE_PY_LONG(-1);
        return resultToPy;
    }
}

PyObject* xeWSConnected(PyObject* self, PyObject* args)
{
    PyObject* capsulatedInfo = PyTuple_GetItem(args, 0);
    WebSocketConnection* wCon = (WebSocketConnection*)PyCapsule_GetPointer(capsulatedInfo, NULL);
    PyObject* resultToPy;
    if(wCon == NULL)
    {
        resultToPy = XE_PY_LONG(-1);
        return resultToPy;
    }
    else
    {
        PyObject* message = PyTuple_GetItem(args, 1);
        const char* convertedString = xepToString(message);
        if(convertedString == NULL)
        {
            wCon->send("Failed to send message, type is incorrect");
        }
        wCon->send(convertedString);
        resultToPy = XE_PY_LONG(-1);
        return resultToPy;
    }
}

HttpResponsePtr xeMatchTupleParams(PyObject* tuple)
{
    I32 responseType = 0;
    const char* resultContent;
    HttpResponsePtr resp;
    if(!PyArg_ParseTuple(tuple, "is", &responseType, &resultContent))
    {
        resp = HttpResponse::newNotFoundResponse();
        XE_ENV_LOG(XE_OUT_FILE, kError, xeGetNetworkLogFile(), "Failed passing tuple arguments, make sure your tuple is in form (int, string)");
        return resp;
    }
    else
    {
        if(responseType == 0)
        {
            resp = HttpResponse::newHttpResponse();
            resp->setBody(resultContent);
            return resp;
        }
        else if(responseType == 1)
        {
            std::stringstream ss(resultContent);
            Json::Value JSVL;
            ss >> JSVL;
            resp = HttpResponse::newHttpJsonResponse(JSVL);
            return resp;
        }
    }
}

class requests:public drogon::HttpSimpleController<requests>
{
public:
    virtual void asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)override;
    PATH_LIST_BEGIN
    //list path definitions here;
    PATH_ADD("/requests");
    PATH_LIST_END
};

void requests::asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    std::optional<I32>& mop = req->getOptionalParameter<I32>("ssid");

    I32 myParam = mop.value();
    PyObject* myFunction = xepLoadFunction(myParam, "responseFunction", XE_PY_FUNCTION_LOAD_BROADCASTED);
    if(myFunction == NULL)
    {
        // Do nothing
        XE_ENV_LOG(XE_OUT_FILE, kError, xeGetNetworkLogFile(), "Expecting responseFunction on handle id: %d", myParam);
        auto resp = HttpResponse::newNotFoundResponse();
        callback(resp);
    }
    else{
        I32 paramCount = req->getParameters().size();

        if(paramCount == 1)
        {
            // Which means only ssid param is given
            // Call function with no arguments
            PyObject* ssid = XE_PY_LONG(myParam);
            PyObject* capsulatedInfo = xepCapsulateCPtr((PTRGENERIC)req.get());
            PyObject* newTuple = XE_PY_MAKE_TUPLE(paramCount + 1);
            XE_PY_ADD_TUPLE(newTuple, 0, capsulatedInfo);
            XE_PY_ADD_TUPLE(newTuple, 1, ssid);
            PyObject* noArgResult = XE_PY_FUNC_ARGS(myFunction, newTuple);
            if(noArgResult == NULL)
            {
                auto resp = HttpResponse::newNotFoundResponse();
                callback(resp);
                XE_ENV_LOG(XE_OUT_FILE, kError, xeGetNetworkLogFile(), "Call to responseFunction returned NULL, arguments doesn't match or function don't return anything!");
                Py_CLEAR(newTuple);
            }
            
            else if(PyTuple_Check(noArgResult))
            {
                HttpResponsePtr resp = xeMatchTupleParams(noArgResult);
                Py_CLEAR(noArgResult);
                Py_CLEAR(newTuple);
                callback(resp);
            }

            else if(PyUnicode_Check(noArgResult))
            {
                const char* _myResult = xepToString(noArgResult);
                auto resp = HttpResponse::newHttpResponse();
                resp->setContentTypeCode(CT_TEXT_HTML);
                resp->setBody(_myResult);
                Py_CLEAR(newTuple);
                Py_CLEAR(noArgResult);
                callback(resp);
            }
            else
            {
                auto resp = HttpResponse::newNotFoundResponse();
                XE_ENV_LOG(XE_OUT_FILE, kError, xeGetNetworkLogFile(), "Invalid return value type, it should be either tuple or string");
                Py_CLEAR(newTuple);
                Py_CLEAR(noArgResult);
                callback(resp);
            }
        }

        else{
            PyObject* newTuple = XE_PY_MAKE_TUPLE(paramCount + 1);
            
            I32 i = 1;

            for(auto &n : req->getParameters())
            {
                const char* paramString = n.second.c_str();
                PyObject* sentParam = xepConvertToType(paramString);
                
                XE_PY_ADD_TUPLE(newTuple, i, sentParam);
                i++;
            }
            
            PyObject* capsulatedInfo = xepCapsulateCPtr((PTRGENERIC)req.get());
            XE_PY_ADD_TUPLE(newTuple, 0, capsulatedInfo);
            PyObject* myResult = XE_PY_FUNC_ARGS(myFunction, newTuple);
            if(myResult == NULL)
            {
                Py_CLEAR(newTuple);
                auto resp = HttpResponse::newNotFoundResponse();
                callback(resp);
                XE_ENV_LOG(XE_OUT_FILE, kError, xeGetNetworkLogFile(), "Call to responseFunction returned NULL, arguments doesn't match or function don't return anything!");
            }
            else
            {
                if(PyTuple_Check(myResult))
                {
                    HttpResponsePtr resp = xeMatchTupleParams(myResult);
                    Py_CLEAR(myResult);
                    Py_CLEAR(newTuple);
                    callback(resp);
                }

                else if(PyUnicode_Check(myResult))
                {
                    const char* _myResult = xepToString(myResult);
                    auto resp = HttpResponse::newHttpResponse();
                    resp->setContentTypeCode(CT_TEXT_HTML);
                    resp->setBody(_myResult);
                    Py_CLEAR(newTuple);
                    Py_CLEAR(myResult);
                    callback(resp);
                }
                else
                {
                    auto resp = HttpResponse::newNotFoundResponse();
                    XE_ENV_LOG(XE_OUT_FILE, kError, xeGetNetworkLogFile(), "Invalid return value type, it should be either tuple or string");
                    Py_CLEAR(newTuple);
                    Py_CLEAR(myResult);
                    callback(resp);
                }
            }
        }
    }
}

class XEWebSocketComm:public drogon::WebSocketController<XEWebSocketComm>
{
  public:
    GENERIC handleNewMessage(const WebSocketConnectionPtr&, std::string &&, const WebSocketMessageType &) override;
    GENERIC handleNewConnection(const HttpRequestPtr &, const WebSocketConnectionPtr&) override;
    GENERIC handleConnectionClosed(const WebSocketConnectionPtr&) override;
    WS_PATH_LIST_BEGIN
    //list path definitions here;
    WS_PATH_ADD("/xewscom");
    WS_PATH_LIST_END
};

GENERIC XEWebSocketComm::handleNewMessage(const WebSocketConnectionPtr& cnPtr, std::string &&message, const WebSocketMessageType &msgType)
{
    if(message.c_str() == NULL)
    {
        cnPtr->send("No message sent, make sure you send your message in correct order");
    }
    
    std::set<std::string> seperatedStrings = utils::splitStringToSet(message, ",");
    I32 sentArraySize = seperatedStrings.size();
    if(sentArraySize <= 1)
    {
        // Which means given message is incorrect
        cnPtr->send("Message should be send as an array object, [scriptId, message]");
    }
    else
    {
        const char* firstElement = seperatedStrings.begin()->c_str();
        if(!xeIsNumber(firstElement))
        {
            cnPtr->send("Request failed, script id should be integer type");
        }
        else
        {
            I32 scriptHandle = atoi(firstElement);
            PyObject* messageHandler = xepLoadFunction(scriptHandle, "HandleMessage", XE_PY_FUNCTION_LOAD_BROADCASTED);
            if(messageHandler == NULL)
            {
                cnPtr->send("Unable to find message handler, script id or function name may be incorrect");
            }
            else
            {
                PyObject* messageContent = XE_PY_MAKE_TUPLE(sentArraySize);
                I32 tupleStepper = 0;
                for(std::set<std::string>::iterator It = seperatedStrings.begin(); It != seperatedStrings.end(); It++)
                {
                    PyObject* strResult = XE_PY_STR(It->c_str());
                    XE_PY_ADD_TUPLE(messageContent, tupleStepper, strResult);
                    tupleStepper++;
                }
                PyObject* funcParams = XE_PY_MAKE_TUPLE(2);
                PyObject* capsulatedInfo = xepCapsulateCPtr((PTRGENERIC)cnPtr.get());
                XE_PY_ADD_TUPLE(funcParams, 0, capsulatedInfo);
                XE_PY_ADD_TUPLE(funcParams, 1, messageContent);
                PyObject* resultArgs = XE_PY_FUNC_ARGS(messageHandler, funcParams);
                if(resultArgs == NULL)
                {
                    Py_CLEAR(funcParams);
                }
                else
                {
                    Py_CLEAR(funcParams);
                    Py_CLEAR(resultArgs);
                }
            }
        }
    }
}

GENERIC XEWebSocketComm::handleNewConnection(const HttpRequestPtr &reqPtr, const WebSocketConnectionPtr &cnPtr)
{
    if(cnPtr->hasContext())
    {
        printf("Given connection has context \n");
    }
    printf("Established connection \n");
}

GENERIC XEWebSocketComm::handleConnectionClosed(const WebSocketConnectionPtr &cnPtr)
{
    // PyObject* capsulatedPtr = xepCapsulateCPtr((PTRGENERIC)cnPtr.get());
    printf("Closed connection \n");
}

I32 xMain(I32 threadHandle)
{
    I32 XESYSTEMhandleValue = xepCreateCustomModule("XESYSTEM");
    I32 XEHTTPhandleValue = xepCreateCustomModule("XEHTTP");
    I32 XEWShandleValue = xepCreateCustomModule("XEWS");

    xepAddCustomFunction(XESYSTEMhandleValue, "CliCall", xeCallToCLI, XE_PY_METHOD_ONEARG);
    xepAddCustomFunction(XEHTTPhandleValue, "GetParamCount", xeReqParamCount, XE_PY_METHOD_ONEARG);
    xepAddCustomFunction(XEHTTPhandleValue, "PeerIp", xeReqPeerIp, XE_PY_METHOD_ONEARG);
    xepAddCustomFunction(XEWShandleValue, "GetPeerIp", xeWSReqPeerIp, XE_PY_METHOD_ONEARG);
    xepAddCustomFunction(XEWShandleValue, "Send", xeWSSend, XE_PY_METHOD_VARARGS);

    xepRegisterCustomModule(XESYSTEMhandleValue);
    xepRegisterCustomModule(XEHTTPhandleValue);
    xepRegisterCustomModule(XEWShandleValue);

    xepInitializeInterpreter();

    app().addListener("127.0.0.1", 80);
    app().run();
    return 0;
}

int main()
{
    xeInitProgram(xMain);

    getchar();
    return 0;
}