/*
 * Google's Firebase Realtime Database Arduino Library for ESP32, version 3.0.2
 * 
 * May 1, 2019
 * 
 * Feature Added:
 * 
 * 
 * Feature Fixed:
 * - readStream bugs
 *  
 * 
 * This library provides ESP32 to perform REST API by GET PUT, POST, PATCH, DELETE data from/to with Google's Firebase database using get, set, update
 * and delete calls. 
 * 
 * The library was test and work well with ESP32s based module and add support for multiple stream event path.
 * 
 * The MIT License (MIT)
 * Copyright (c) 2019 K. Suwatchai (Mobizt)
 * 
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef FirebaseESP32_CPP
#define FirebaseESP32_CPP

#include "FirebaseESP32.h"

struct FirebaseESP32::FirebaseDataType
{
  static const uint8_t NULL_ = 1;
  static const uint8_t INTEGER = 2;
  static const uint8_t FLOAT = 3;
  static const uint8_t DOUBLE = 4;
  static const uint8_t BOOLEAN = 5;
  static const uint8_t STRING = 6;
  static const uint8_t JSON = 7;
  static const uint8_t BLOB = 8;
  static const uint8_t FILE = 9;
};

struct FirebaseESP32::FirebaseMethod
{
  static const uint8_t PUT = 0;
  static const uint8_t PUT_SILENT = 1;
  static const uint8_t POST = 2;
  static const uint8_t GET = 3;
  static const uint8_t STREAM = 4;
  static const uint8_t PATCH = 5;
  static const uint8_t PATCH_SILENT = 6;
  static const uint8_t DELETE = 7;
  static const uint8_t BACKUP = 8;
  static const uint8_t RESTORE = 9;
  static const uint8_t GET_RULES = 10;
  static const uint8_t SET_RULES = 11;
};

FirebaseESP32::FirebaseESP32()
{
}

FirebaseESP32::~FirebaseESP32()
{
  std::string().swap(_host);
  std::string().swap(_auth);
  std::vector<const char *>().swap(_rootCA);
}

void FirebaseESP32::begin(const String &host, const String &auth)
{
  int p1 = std::string::npos;
  int p2 = 0;
  char *h = new char[host.length() + 1];
  memset(h, 0, host.length() + 1);
  strcpy(h, host.c_str());
  char *_h = new char[host.length() + 1];
  memset(_h, 0, host.length() + 1);

  _host.clear();
  _auth.clear();
  _rootCA.clear();

  _host = host.c_str();

  p1 = _host.find(ESP32_FIREBASE_STR_111);
  if (p1 != std::string::npos)
  {
    if (h[strlen(h) - 1] == '/')
      p2 = 1;

    strncpy(_h, h + p1 + strlen(ESP32_FIREBASE_STR_111), strlen(h) - p1 - p2 - strlen(ESP32_FIREBASE_STR_111));
    _host = _h;
  }

  if (p1 == std::string::npos)
  {
    p1 = _host.find(ESP32_FIREBASE_STR_112);
    if (p1 != std::string::npos)
    {
      if (h[strlen(h) - 1] == '/')
        p2 = 1;

      strncpy(_h, h + p1 + strlen(ESP32_FIREBASE_STR_112), strlen(h) - p1 - p2 - strlen(ESP32_FIREBASE_STR_112));
      _host = _h;
    }
  }

  _auth = auth.c_str();
  _port = FIEBASE_PORT;

  delete[] h;
  delete[] _h;
}

void FirebaseESP32::begin(const String &host, const String &auth, const char *rootCA)
{
  int p1 = std::string::npos;
  int p2 = 0;
  char *h = new char[host.length() + 1];
  memset(h, 0, host.length() + 1);
  strcpy(h, host.c_str());
  char *_h = new char[host.length() + 1];
  memset(_h, 0, host.length() + 1);

  _host.clear();
  _auth.clear();

  _host = host.c_str();

  p1 = _host.find(ESP32_FIREBASE_STR_111);
  if (p1 != std::string::npos)
  {
    if (h[strlen(h) - 1] == '/')
      p2 = 1;

    strncpy(_h, h + p1 + strlen(ESP32_FIREBASE_STR_111), strlen(h) - p1 - p2 - strlen(ESP32_FIREBASE_STR_111));
    _host = _h;
  }

  if (p1 == std::string::npos)
  {
    p1 = _host.find(ESP32_FIREBASE_STR_112);
    if (p1 != std::string::npos)
    {
      if (h[strlen(h) - 1] == '/')
        p2 = 1;

      strncpy(_h, h + p1 + strlen(ESP32_FIREBASE_STR_112), strlen(h) - p1 - p2 - strlen(ESP32_FIREBASE_STR_112));
      _host = _h;
    }
  }

  _auth = auth.c_str();
  _port = FIEBASE_PORT;

  _rootCA.clear();
  if (strlen(rootCA) > 0)
    _rootCA.push_back((char *)rootCA);

  delete[] h;
  delete[] _h;
}

void FirebaseESP32::end(FirebaseData &dataObj)
{

  endStream(dataObj);

  removeStreamCallback(dataObj);

  WiFiClient *netClient = dataObj.http.http_getStreamPtr();
  netClient->~WiFiClient();
  dataObj.clear();
}

void FirebaseESP32::reconnectWiFi(bool reconnect)
{
  _reconnectWiFi = reconnect;
  WiFi.setAutoReconnect(reconnect);
}

bool FirebaseESP32::getRules(FirebaseData &dataObj)
{
  dataObj.queryFilter.clear();
  return sendRequest(dataObj, ESP32_FIREBASE_STR_103, FirebaseMethod::GET_RULES, FirebaseDataType::JSON, "");
}

bool FirebaseESP32::setRules(FirebaseData &dataObj, const String &rules)
{
  dataObj.queryFilter.clear();
  return sendRequest(dataObj, ESP32_FIREBASE_STR_103, FirebaseMethod::SET_RULES, FirebaseDataType::JSON, rules.c_str());
}

bool FirebaseESP32::buildRequest(FirebaseData &dataObj, uint8_t firebaseMethod, uint8_t firebaseDataType, const String &path, const char *buf, bool queue)
{

  bool flag;
  dataObj.queryFilter.clear();

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  unsigned long lastTime = millis();

  if (dataObj._streamCall)
    while (dataObj._streamCall && millis() - lastTime < 1000)
      delay(1);

  if (dataObj._streamCall)
  {
    dataObj._httpCode = HTTPC_ERROR_CONNECTION_INUSED;
    return false;
  }

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, path.c_str(), firebaseMethod, firebaseDataType, buf);
    if (flag)
      break;

    if (dataObj._httpCode == HTTPC_ERROR_CONNECTION_INUSED && dataObj._firebaseCall)
    {
      //Waiting for queue to finish
      while (dataObj._firebaseCall && WiFi.status() != WL_CONNECTED)
        delay(1);
    }
    else
    {

      if (dataObj._maxRetry > 0)
        if (!flag && commError(dataObj))
          errCount++;
    }
  }

  dataObj._qID = 0;

  if (!queue && !flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)

    if (firebaseMethod == FirebaseMethod::PUT || firebaseMethod == FirebaseMethod::PUT_SILENT || firebaseMethod == FirebaseMethod::POST || firebaseMethod == FirebaseMethod::PATCH || firebaseMethod == FirebaseMethod::PATCH_SILENT)

      dataObj.addQueue(firebaseMethod, firebaseDataType, path.c_str(), "", buf, false, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

  if (flag)
    dataObj._firebaseCall = false;
  return flag;
}

bool FirebaseESP32::buildRequestFile(FirebaseData &dataObj, uint8_t firebaseMethod, const String &path, const String &fileName, bool queue)
{

  dataObj.queryFilter.clear();
  dataObj._fileName = fileName.c_str();

  bool flag;

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  unsigned long lastTime = millis();

  if (dataObj._streamCall)
    while (dataObj._streamCall && millis() - lastTime < 1000)
      delay(1);

  if (dataObj._streamCall)
  {
    dataObj._httpCode = HTTPC_ERROR_CONNECTION_INUSED;
    return false;
  }

  for (int i = 0; i < maxRetry; i++)
  {

    flag = sendRequest(dataObj, path.c_str(), firebaseMethod, FirebaseDataType::FILE, "");
    if (flag)
      break;

    if (dataObj._httpCode == HTTPC_ERROR_CONNECTION_INUSED && dataObj._firebaseCall)
    {
      //Waiting for queue to finish
      while (dataObj._firebaseCall && WiFi.status() != WL_CONNECTED)
        delay(1);
    }
    else
    {

      if (dataObj._maxRetry > 0)
        if (!flag && commError(dataObj))
          errCount++;
    }
  }

  dataObj._qID = 0;

  if (!queue && !flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(firebaseMethod, FirebaseDataType::FILE, path.c_str(), fileName.c_str(), "", false, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

  if (flag)
    dataObj._firebaseCall = false;

  return flag;
}

bool FirebaseESP32::pathExist(FirebaseData &dataObj, const String &path)
{
  dataObj.queryFilter.clear();
  if (sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::STRING, ""))
    return !dataObj._pathNotExist;
  else
    return false;
}

bool FirebaseESP32::pushInt(FirebaseData &dataObj, const String &path, int intValue)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  itoa(intValue, buf, 10);

  bool flag = buildRequest(dataObj, FirebaseMethod::POST, FirebaseDataType::INTEGER, path, buf, false);
  delete[] buf;
  return flag;
}

bool FirebaseESP32::pushFloat(FirebaseData &dataObj, const String &path, float floatValue)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  dtostrf(floatValue, 7, 6, buf);

  bool flag = buildRequest(dataObj, FirebaseMethod::POST, FirebaseDataType::FLOAT, path, buf, false);
  delete[] buf;
  return flag;
}

bool FirebaseESP32::pushDouble(FirebaseData &dataObj, const String &path, double doubleValue)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  dtostrf(doubleValue, 12, 9, buf);

  bool flag = buildRequest(dataObj, FirebaseMethod::POST, FirebaseDataType::DOUBLE, path, buf, false);
  delete[] buf;
  return flag;
}

bool FirebaseESP32::pushBool(FirebaseData &dataObj, const String &path, bool boolValue)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);

  if (boolValue)
    strcpy(buf, ESP32_FIREBASE_STR_107);
  else
    strcpy(buf, ESP32_FIREBASE_STR_106);

  bool flag = buildRequest(dataObj, FirebaseMethod::POST, FirebaseDataType::BOOLEAN, path, buf, false);
  delete[] buf;
  return flag;
}

bool FirebaseESP32::pushString(FirebaseData &dataObj, const String &path, const String &stringValue)
{
  bool flag = buildRequest(dataObj, FirebaseMethod::POST, FirebaseDataType::STRING, path, stringValue.c_str(), false);
  return flag;
}

bool FirebaseESP32::pushJSON(FirebaseData &dataObj, const String &path, const String &jsonString)
{
  bool flag = buildRequest(dataObj, FirebaseMethod::POST, FirebaseDataType::JSON, path, jsonString.c_str(), false);
  return flag;
}

bool FirebaseESP32::pushBlob(FirebaseData &dataObj, const String &path, uint8_t *blob, size_t size)
{

  if (dataObj._maxBlobSize < size)
  {
    dataObj._httpCode = HTTP_CODE_PAYLOAD_TOO_LARGE;
    return false;
  }

  size_t tmpSize = strlen(ESP32_FIREBASE_STR_92) + 1;
  char *tmp = new char[tmpSize];
  memset(tmp, 0, tmpSize);

  std::string blobStr = "";
  strcpy_P(tmp, ESP32_FIREBASE_STR_92);
  blobStr = tmp;
  blobStr += base64_encode_string((const unsigned char *)blob, size);
  memset(tmp, 0, tmpSize);
  strcpy_P(tmp, ESP32_FIREBASE_STR_3);
  blobStr += tmp;

  bool flag = buildRequest(dataObj, FirebaseMethod::POST, FirebaseDataType::BLOB, path, blobStr.c_str(), false);

  std::string().swap(blobStr);

  delete[] tmp;
  return flag;
}

bool FirebaseESP32::pushFile(FirebaseData &dataObj, const String &path, const String &fileName)
{
  return buildRequestFile(dataObj, FirebaseMethod::POST, path, fileName, false);
}

bool FirebaseESP32::setInt(FirebaseData &dataObj, const String &path, int intValue)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  itoa(intValue, buf, 10);
  bool flag = buildRequest(dataObj, FirebaseMethod::PUT, FirebaseDataType::INTEGER, path, buf, false);
  delete[] buf;
  return flag;
}

bool FirebaseESP32::setFloat(FirebaseData &dataObj, const String &path, float floatValue)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  dtostrf(floatValue, 7, 6, buf);
  bool flag = buildRequest(dataObj, FirebaseMethod::PUT, FirebaseDataType::FLOAT, path, buf, false);
  delete[] buf;
  return flag;
}

bool FirebaseESP32::setDouble(FirebaseData &dataObj, const String &path, double doubleValue)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  dtostrf(doubleValue, 12, 9, buf);
  bool flag = buildRequest(dataObj, FirebaseMethod::PUT, FirebaseDataType::DOUBLE, path, buf, false);
  delete[] buf;
  return flag;
}

bool FirebaseESP32::setBool(FirebaseData &dataObj, const String &path, bool boolValue)
{
  size_t bufSize = 50;
  char *buf = new char[bufSize];
  memset(buf, 0, bufSize);
  if (boolValue)
    strcpy_P(buf, ESP32_FIREBASE_STR_107);
  else
    strcpy_P(buf, ESP32_FIREBASE_STR_106);

  bool flag = buildRequest(dataObj, FirebaseMethod::PUT, FirebaseDataType::BOOLEAN, path, buf, false);
  delete[] buf;
  return flag;
}

bool FirebaseESP32::setString(FirebaseData &dataObj, const String &path, const String &stringValue)
{
  bool flag = buildRequest(dataObj, FirebaseMethod::PUT, FirebaseDataType::STRING, path, stringValue.c_str(), false);
  return flag;
}

bool FirebaseESP32::setJSON(FirebaseData &dataObj, const String &path, const String &jsonString)
{
  bool flag = buildRequest(dataObj, FirebaseMethod::PUT, FirebaseDataType::JSON, path, jsonString.c_str(), false);
  return flag;
}

bool FirebaseESP32::setBlob(FirebaseData &dataObj, const String &path, uint8_t *blob, size_t size)
{
  if (dataObj._maxBlobSize < size)
  {
    dataObj._httpCode = HTTP_CODE_PAYLOAD_TOO_LARGE;
    return false;
  }

  size_t tmpSize = strlen(ESP32_FIREBASE_STR_92) + 1;
  char *tmp = new char[tmpSize];
  memset(tmp, 0, tmpSize);

  std::string blobStr = "";
  strcpy(tmp, ESP32_FIREBASE_STR_92);
  blobStr = tmp;
  blobStr += base64_encode_string((const unsigned char *)blob, size);
  memset(tmp, 0, tmpSize);
  strcpy(tmp, ESP32_FIREBASE_STR_3);
  blobStr += tmp;

  bool flag = buildRequest(dataObj, FirebaseMethod::PUT_SILENT, FirebaseDataType::BLOB, path, blobStr.c_str(), false);
  delete[] tmp;
  std::string().swap(blobStr);
  return flag;
}

bool FirebaseESP32::setFile(FirebaseData &dataObj, const String &path, const String &fileName)
{
  return buildRequestFile(dataObj, FirebaseMethod::PUT_SILENT, path, fileName, false);
}

bool FirebaseESP32::updateNode(FirebaseData &dataObj, const String &path, const String &jsonString)
{
  bool flag = buildRequest(dataObj, FirebaseMethod::PATCH, FirebaseDataType::JSON, path, jsonString.c_str(), false);
  return flag;
}

bool FirebaseESP32::updateNodeSilent(FirebaseData &dataObj, const String &path, const String &jsonString)
{
  bool flag = buildRequest(dataObj, FirebaseMethod::PATCH_SILENT, FirebaseDataType::JSON, path, jsonString.c_str(), false);
  return flag;
}

bool FirebaseESP32::getInt(FirebaseData &dataObj, const String &path)
{
  return getFloat(dataObj, path);
}

bool FirebaseESP32::getInt(FirebaseData &dataObj, const String &path, int &target)
{

  bool flag;
  dataObj.queryFilter.clear();

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::INTEGER, "");
    target = dataObj.intData();
    if (flag)
      break;

    if (dataObj._maxRetry > 0)
      if (!flag && commError(dataObj))
        errCount++;
  }

  if (!flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(FirebaseMethod::GET, FirebaseDataType::INTEGER, path.c_str(), "", "", false, &target, nullptr, nullptr, nullptr, nullptr, nullptr);

  if (flag && dataObj._dataType != FirebaseDataType::INTEGER && dataObj._dataType != FirebaseDataType::FLOAT && dataObj._dataType != FirebaseDataType::DOUBLE)
    flag = false;

  return flag;
}

bool FirebaseESP32::getFloat(FirebaseData &dataObj, const String &path)
{
  dataObj.queryFilter.clear();
  bool flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::FLOAT, "");
  if (dataObj._dataType != FirebaseDataType::INTEGER && dataObj._dataType != FirebaseDataType::FLOAT && dataObj._dataType != FirebaseDataType::DOUBLE)
    flag = false;
  return flag;
}

bool FirebaseESP32::getFloat(FirebaseData &dataObj, const String &path, float &target)
{

  bool flag;
  dataObj.queryFilter.clear();

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::FLOAT, "");
    target = dataObj.floatData();
    if (flag)
      break;

    if (dataObj._maxRetry > 0)
      if (!flag && commError(dataObj))
        errCount++;
  }

  if (!flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(FirebaseMethod::GET, FirebaseDataType::FLOAT, path.c_str(), "", "", false, nullptr, &target, nullptr, nullptr, nullptr, nullptr);

  if (flag && dataObj._dataType != FirebaseDataType::INTEGER && dataObj._dataType != FirebaseDataType::FLOAT && dataObj._dataType != FirebaseDataType::DOUBLE)
    flag = false;

  return flag;
}

bool FirebaseESP32::getDouble(FirebaseData &dataObj, const String &path)
{
  dataObj.queryFilter.clear();
  bool flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::DOUBLE, "");
  if (dataObj._dataType != FirebaseDataType::INTEGER && dataObj._dataType != FirebaseDataType::FLOAT && dataObj._dataType != FirebaseDataType::DOUBLE)
    flag = false;
  return flag;
}

bool FirebaseESP32::getDouble(FirebaseData &dataObj, const String &path, double &target)
{

  bool flag;
  dataObj.queryFilter.clear();

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::DOUBLE, "");
    target = dataObj.floatData();
    if (flag)
      break;

    if (dataObj._maxRetry > 0)
      if (!flag && commError(dataObj))
        errCount++;
  }

  if (!flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(FirebaseMethod::GET, FirebaseDataType::DOUBLE, path.c_str(), "", "", false, nullptr, nullptr, &target, nullptr, nullptr, nullptr);

  if (flag && dataObj._dataType != FirebaseDataType::INTEGER && dataObj._dataType != FirebaseDataType::FLOAT && dataObj._dataType != FirebaseDataType::DOUBLE)
    flag = false;

  return flag;
}

bool FirebaseESP32::getBool(FirebaseData &dataObj, const String &path)
{
  dataObj.queryFilter.clear();
  bool flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::BOOLEAN, "");
  if (dataObj._dataType != FirebaseDataType::BOOLEAN)
    flag = false;
  return flag;
}

bool FirebaseESP32::getBool(FirebaseData &dataObj, const String &path, bool &target)
{

  bool flag;
  dataObj.queryFilter.clear();

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::BOOLEAN, "");
    target = dataObj.boolData();
    if (flag)
      break;

    if (dataObj._maxRetry > 0)
      if (!flag && commError(dataObj))
        errCount++;
  }

  if (!flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(FirebaseMethod::GET, FirebaseDataType::BOOLEAN, path.c_str(), "", "", false, nullptr, nullptr, nullptr, &target, nullptr, nullptr);

  if (flag && dataObj._dataType != FirebaseDataType::BOOLEAN)
    flag = false;

  return flag;
}

bool FirebaseESP32::getString(FirebaseData &dataObj, const String &path)
{
  dataObj.queryFilter.clear();
  bool flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::STRING, "");
  if (dataObj._dataType != FirebaseDataType::STRING)
    flag = false;
  return flag;
}

bool FirebaseESP32::getString(FirebaseData &dataObj, const String &path, String &target)
{

  bool flag;
  dataObj.queryFilter.clear();

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::STRING, "");
    target = dataObj.stringData();
    if (flag)
      break;

    if (dataObj._maxRetry > 0)
      if (!flag && commError(dataObj))
        errCount++;
  }

  if (!flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(FirebaseMethod::GET, FirebaseDataType::STRING, path.c_str(), "", "", false, nullptr, nullptr, nullptr, nullptr, &target, nullptr);

  if (flag && dataObj._dataType != FirebaseDataType::STRING)
    flag = false;

  return flag;
}

bool FirebaseESP32::getJSON(FirebaseData &dataObj, const String &path)
{
  dataObj.queryFilter.clear();
  bool flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::JSON, "");
  if (dataObj._dataType != FirebaseDataType::JSON)
    flag = false;
  return flag;
}

bool FirebaseESP32::getJSON(FirebaseData &dataObj, const String &path, String &target)
{

  bool flag;
  dataObj.queryFilter.clear();

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::JSON, "");
    target = dataObj.jsonData();
    if (flag)
      break;

    if (dataObj._maxRetry > 0)
      if (!flag && commError(dataObj))
        errCount++;
  }

  if (!flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(FirebaseMethod::GET, FirebaseDataType::JSON, path.c_str(), "", "", false, nullptr, nullptr, nullptr, nullptr, &target, nullptr);

  if (flag && dataObj._dataType != FirebaseDataType::JSON)
    flag = false;

  return flag;
}

bool FirebaseESP32::getJSON(FirebaseData &dataObj, const String &path, QueryFilter &query)
{
  dataObj.queryFilter.clear();
  if (query._orderBy != "")
    dataObj.setQuery(query);

  bool flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::JSON, "");
  if (dataObj._dataType != FirebaseDataType::JSON)
    flag = false;
  return flag;
}

bool FirebaseESP32::getJSON(FirebaseData &dataObj, const String &path, QueryFilter &query, String &target)
{
  dataObj.queryFilter.clear();
  if (query._orderBy != "")
    dataObj.setQuery(query);

  bool flag;

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::JSON, "");
    target = dataObj.jsonData();
    if (flag)
      break;

    if (dataObj._maxRetry > 0)
      if (!flag && commError(dataObj))
        errCount++;
  }

  if (!flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(FirebaseMethod::GET, FirebaseDataType::JSON, path.c_str(), "", "", true, nullptr, nullptr, nullptr, nullptr, &target, nullptr);

  if (flag && dataObj._dataType != FirebaseDataType::JSON)
    flag = false;

  return flag;
}

bool FirebaseESP32::getBlob(FirebaseData &dataObj, const String &path)
{
  dataObj.queryFilter.clear();
  bool flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::BLOB, "");
  if (dataObj._dataType != FirebaseDataType::BLOB)
    flag = false;
  return flag;
}

bool FirebaseESP32::getBlob(FirebaseData &dataObj, const String &path, std::vector<uint8_t> &target)
{

  dataObj.queryFilter.clear();

  bool flag;

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, path.c_str(), FirebaseMethod::GET, FirebaseDataType::BLOB, "");
    target = dataObj.blobData();
    if (flag)
      break;

    if (dataObj._maxRetry > 0)
      if (!flag && commError(dataObj))
        errCount++;
  }

  if (!flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(FirebaseMethod::GET, FirebaseDataType::BLOB, path.c_str(), "", "", false, nullptr, nullptr, nullptr, nullptr, nullptr, &target);

  if (flag && dataObj._dataType != FirebaseDataType::BLOB)
    flag = false;

  return flag;
}

bool FirebaseESP32::getFile(FirebaseData &dataObj, const String &nodePath, const String &fileName)
{
  dataObj.queryFilter.clear();
  dataObj._fileName.clear();
  dataObj._fileName = fileName.c_str();

  bool flag;

  uint8_t errCount = 0;
  uint8_t maxRetry = dataObj._maxRetry;
  if (maxRetry == 0)
    maxRetry = 1;

  for (int i = 0; i < maxRetry; i++)
  {
    flag = sendRequest(dataObj, nodePath.c_str(), FirebaseMethod::GET, FirebaseDataType::FILE, "");
    if (flag)
      break;

    if (dataObj._maxRetry > 0)
      if (!flag && (commError(dataObj) || dataObj._file_transfer_error.length() > 0))
        errCount++;
  }

  if (!flag && errCount == maxRetry && dataObj._qMan._maxQueue > 0)
    dataObj.addQueue(FirebaseMethod::GET, FirebaseDataType::FILE, nodePath.c_str(), fileName.c_str(), "", false, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

  return flag;
}

bool FirebaseESP32::deleteNode(FirebaseData &dataObj, const String &path)
{
  dataObj.queryFilter.clear();
  return sendRequest(dataObj, path.c_str(), FirebaseMethod::DELETE, FirebaseDataType::STRING, "");
}

bool FirebaseESP32::beginStream(FirebaseData &dataObj, const String &path)
{
  dataObj._pause = false;
  return firebaseConnectStream(dataObj, path.c_str());
}

bool FirebaseESP32::readStream(FirebaseData &dataObj)
{
  if (dataObj._streamStop)
    return true;

  if (_reconnectWiFi && WiFi.status() != WL_CONNECTED)
    WiFi.reconnect();

  return getServerStreamResponse(dataObj);
}

bool FirebaseESP32::endStream(FirebaseData &dataObj)
{
  dataObj._pause = true;
  bool flag = false;
  dataObj._streamStop = true;

  if (!apConnected(dataObj))
  {
    dataObj._isStream = false;
    dataObj._streamStop = true;
    return true;
  }

  WiFiClient *netClient = dataObj.http.http_getStreamPtr();
  if (netClient)
  {
    if (netClient->available() > 0)
    {
      netClient->flush();
      delay(50);
      netClient->stop();
    }
  }

  flag = dataObj.http.http_connected();
  if (!flag)
  {
    dataObj._isStream = false;
    dataObj._streamStop = true;
  }
  return !flag;
}

int FirebaseESP32::firebaseConnect(FirebaseData &dataObj, const std::string &path, const uint8_t method, uint8_t dataType, const std::string &payload)
{

  dataObj._firebaseError.clear();

  if (dataObj._pause)
    return 0;

  if (!apConnected(dataObj))
    return HTTPC_ERROR_CONNECTION_LOST;

  if (path.length() == 0 || _host.length() == 0 || _auth.length() == 0)
  {
    dataObj._httpCode = HTTP_CODE_BAD_REQUEST;
    return HTTP_CODE_BAD_REQUEST;
  }

  uint8_t retryCount = 0;
  uint8_t maxRetry = 5;

  size_t bufSize = 32;
  char *buf = new char[bufSize];

  int len = 0;
  size_t toRead = 0;
  bool httpConnected = false;

  int httpCode = HTTPC_ERROR_CONNECTION_REFUSED;

  //build requested uri
  std::string payloadStr = "";
  std::string header = "";
  std::string uri = "";

  if (dataObj._redirectURL == "")
  {
    if (path[0] != '/')
      uri = ESP32_FIREBASE_STR_1;

    uri += path;

    if (method == FirebaseMethod::PATCH || method == FirebaseMethod::PATCH_SILENT || method == FirebaseMethod::RESTORE)
      uri += ESP32_FIREBASE_STR_1;

    uri += ESP32_FIREBASE_STR_2;
    uri += _auth;
  }
  else
  {
    uri = dataObj._redirectURL;
  }

  //init the firebase data
  resetFirebasedataFlag(dataObj);
  dataObj._path.c_str();

  if (method == FirebaseMethod::STREAM)
  {
    //stream path change? reset the current (keep alive) connection
    if (path != dataObj._streamPath)
      dataObj._streamPathChanged = true;
    if (!dataObj._isStream || dataObj._streamPathChanged)
    {
      if (dataObj.http.http_connected())
        forceEndHTTP(dataObj);
    }

    dataObj._streamPath.clear();

    if (path.length() > 0)
      if (path[0] != '/')
        dataObj._streamPath = ESP32_FIREBASE_STR_1;

    dataObj._streamPath += path;
  }
  else
  {
    //last requested method was stream?, reset the connection
    if (dataObj._isStream)
      forceEndHTTP(dataObj);
    if (method != FirebaseMethod::BACKUP && method != FirebaseMethod::RESTORE)
    {
      dataObj._path.clear();
      if (path.length() > 0)
        if (path[0] != '/')
          dataObj._path = ESP32_FIREBASE_STR_1;
      dataObj._path += path;
    }

    dataObj._isStreamTimeout = false;
  }

  if (_rootCA.size() > 0)
    httpConnected = dataObj.http.http_begin(_host.c_str(), _port, uri.c_str(), (const char *)_rootCA.front());
  else
    httpConnected = dataObj.http.http_begin(_host.c_str(), _port, uri.c_str(), (const char *)NULL);

  if (!httpConnected)
  {
    dataObj._httpCode = HTTPC_ERROR_CONNECTION_REFUSED;
    goto EXIT_1;
  }

  //Prepare for string and JSON payloads
  if (method != FirebaseMethod::GET && method != FirebaseMethod::STREAM &&
      method != FirebaseMethod::DELETE && method != FirebaseMethod::RESTORE)
  {
    payloadStr.clear();
    if (dataType == FirebaseDataType::STRING)
      payloadStr = ESP32_FIREBASE_STR_3;
    payloadStr += payload;
    if (dataType == FirebaseDataType::STRING)
      payloadStr += ESP32_FIREBASE_STR_3;
  }

  //Prepare request header

  if (method != FirebaseMethod::BACKUP && method != FirebaseMethod::RESTORE && dataType != FirebaseDataType::FILE)
    buildFirebaseRequest(dataObj, _host, method, path, _auth, payloadStr.length(), header);
  else
  {

    if (_sdInUse)
    {

      dataObj._file_transfer_error.clear();
      dataObj._file_transfer_error = ESP32_FIREBASE_STR_84;
      goto EXIT_1;
    }

    if (!_sdOk)
      _sdOk = sdTest();

    if (!_sdOk)
    {

      dataObj._file_transfer_error.clear();
      dataObj._file_transfer_error = ESP32_FIREBASE_STR_85;

      goto EXIT_1;
    }

    _sdInUse = true;

    if (method == FirebaseMethod::BACKUP || method == FirebaseMethod::RESTORE)
    {

      if (!SD.exists(dataObj._backupDir.c_str()) && method == FirebaseMethod::BACKUP)
        createDirs(dataObj._backupDir.c_str());

      dataObj._backupFilename.clear();

      dataObj._backupFilename = dataObj._backupDir + ESP32_FIREBASE_STR_1;

      for (int i = 1; i < dataObj._backupNodePath.length(); i++)
      {
        if (dataObj._backupNodePath[i] == '/')
          dataObj._backupFilename += ESP32_FIREBASE_STR_4;
        else
          dataObj._backupFilename += dataObj._backupNodePath[i];
      }
      if (dataObj._backupFilename == dataObj._backupDir + ESP32_FIREBASE_STR_1)
        dataObj._backupFilename = ESP32_FIREBASE_STR_90;
      else
        dataObj._backupFilename += ESP32_FIREBASE_STR_89;

      if (method == FirebaseMethod::BACKUP)
      {
        file = SD.open(dataObj._backupFilename.c_str(), FILE_WRITE);
      }
      else
      {

        if (SD.exists(dataObj._backupFilename.c_str()))
          file = SD.open(dataObj._backupFilename.c_str(), FILE_READ);
        else
        {
          dataObj._file_transfer_error.clear();
          dataObj._file_transfer_error = ESP32_FIREBASE_STR_83;
        }
      }

      if (!file)
      {
        dataObj._file_transfer_error.clear();
        dataObj._file_transfer_error = ESP32_FIREBASE_STR_86;
        goto EXIT_1;
      }

      if (method == FirebaseMethod::RESTORE)
        len = file.size();
    }

    if (dataType == FirebaseDataType::FILE)
    {

      if (method == FirebaseMethod::PUT_SILENT || method == FirebaseMethod::POST)
      {

        if (SD.exists(dataObj._fileName.c_str()))
          file = SD.open(dataObj._fileName.c_str(), FILE_READ);
        else
        {
          dataObj._file_transfer_error.clear();
          dataObj._file_transfer_error = ESP32_FIREBASE_STR_83;
          goto EXIT_1;
        }

        len = (4 * ceil(file.size() / 3.0)) + strlen(ESP32_FIREBASE_STR_93) + 1;
      }
      else if (method == FirebaseMethod::GET)
      {

        int p1 = dataObj._fileName.find_last_of(ESP32_FIREBASE_STR_1);
        std::string folder = "/";

        if (p1 != std::string::npos && p1 != 0)
          folder = dataObj._fileName.substr(p1 - 1);

        if (!SD.exists(folder.c_str()))
          createDirs(folder);

        file = SD.open(dataObj._fileName.c_str(), FILE_WRITE);
        std::string().swap(folder);
      }

      if (!file)
      {
        dataObj._file_transfer_error.clear();
        dataObj._file_transfer_error = ESP32_FIREBASE_STR_86;
        goto EXIT_1;
      }
    }

    if (dataType == FirebaseDataType::FILE)
      buildFirebaseRequest(dataObj, _host, method, dataObj._path.c_str(), _auth, len, header);
    else
      buildFirebaseRequest(dataObj, _host, method, dataObj._backupNodePath.c_str(), _auth, len, header);
  }

  if (method == FirebaseMethod::PATCH_SILENT || (method == FirebaseMethod::PUT_SILENT && dataType == FirebaseDataType::BLOB))
    dataObj._isSilentResponse = true;

  if (dataType == FirebaseDataType::BLOB)
    std::vector<uint8_t>().swap(dataObj._blob);

  //Send request w/wo payload

  if (!apConnected(dataObj))
    return HTTPC_ERROR_CONNECTION_LOST;

  httpCode = dataObj.http.http_sendRequest(header.c_str(), payloadStr.c_str());

  retryCount = 0;
  while (httpCode != 0)
  {
    retryCount++;
    if (retryCount > maxRetry)
      break;

    if (!apConnected(dataObj))
      return HTTPC_ERROR_CONNECTION_LOST;

    httpCode = dataObj.http.http_sendRequest(header.c_str(), payloadStr.c_str());
  }

  if (method == FirebaseMethod::RESTORE || (dataType == FirebaseDataType::FILE && (method == FirebaseMethod::PUT_SILENT || method == FirebaseMethod::POST)))
  {

    if (dataType == FirebaseDataType::FILE && (method == FirebaseMethod::PUT_SILENT || method == FirebaseMethod::POST))
    {
      memset(buf, 0, bufSize);
      strcpy_P(buf, ESP32_FIREBASE_STR_93);

      if (!apConnected(dataObj))
        return HTTPC_ERROR_CONNECTION_LOST;

      httpCode = dataObj.http.http_sendRequest("", buf);

      WiFiClient *netClient = dataObj.http.http_getStreamPtr();

      send_base64_encode_file(netClient, dataObj._fileName);

      memset(buf, 0, bufSize);
      buf[0] = '"';
      buf[1] = '\0';

      if (!apConnected(dataObj))
        return HTTPC_ERROR_CONNECTION_LOST;

      httpCode = dataObj.http.http_sendRequest("", buf);
    }
    else
    {

      while (len)
      {
        toRead = len;
        if (toRead > bufSize)
          toRead = bufSize - 1;

        memset(buf, 0, bufSize);
        file.read((uint8_t *)buf, toRead);
        buf[toRead] = '\0';

        if (!apConnected(dataObj))
          return HTTPC_ERROR_CONNECTION_LOST;

        httpCode = dataObj.http.http_sendRequest("", buf);

        len -= toRead;

        if (len <= 0)
          break;
      }
    }

    endFileTransfer(dataObj);
  }

  delete[] buf;
  std::string().swap(payloadStr);
  std::string().swap(header);
  std::string().swap(uri);

  return httpCode;

EXIT_1:

  delete[] buf;
  std::string().swap(payloadStr);
  std::string().swap(header);
  std::string().swap(uri);

  return HTTPC_ERROR_CONNECTION_REFUSED;
}

bool FirebaseESP32::sendRequest(FirebaseData &dataObj, const std::string &path, const uint8_t method, uint8_t dataType, const std::string &payload)
{

  bool flag = false;
  dataObj._firebaseError.clear();

  if (dataObj._pause || dataObj._file_transfering)
    return true;

  if (path.length() == 0 || _host.length() == 0 || _auth.length() == 0)
  {
    dataObj._httpCode = HTTP_CODE_BAD_REQUEST;
    return false;
  }

  if ((method == FirebaseMethod::PUT || method == FirebaseMethod::POST || method == FirebaseMethod::PATCH || method == FirebaseMethod::PATCH_SILENT || method == FirebaseMethod::SET_RULES) && payload.length() == 0 && dataType != FirebaseDataType::STRING && dataType != FirebaseDataType::BLOB && dataType != FirebaseDataType::FILE)
  {
    dataObj._httpCode = HTTP_CODE_BAD_REQUEST;
    return false;
  }

  //Try to reconnect WiFi if lost connection
  if (_reconnectWiFi && WiFi.status() != WL_CONNECTED)
  {
    uint8_t tryCount = 0;
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED)
    {
      tryCount++;
      delay(50);
      if (tryCount > 60)
        break;
    }
  }

  //If WiFi is not connected, return false
  if (!apConnected(dataObj))
    return false;

  if (dataObj._firebaseCall)
  {
    dataObj._httpCode = HTTPC_ERROR_CONNECTION_INUSED;
    return false;
  }

  dataObj._firebaseCall = true;
  dataObj._qID = 0;

  //Get the current WiFi client from current firebase data
  //Check for connection status
  WiFiClient *netClient = dataObj.http.http_getStreamPtr();
  if (netClient)
  {
    if (netClient->connected())
      dataObj._httpConnected = true;
    else
      dataObj._httpConnected = false;
  }

  if (dataObj._httpConnected)
  {

    if (method == FirebaseMethod::STREAM)
    {

      dataObj._streamMillis = millis();
      dataObj._firebaseCall = false;
      return false;
    }
    else
    {
      if (!dataObj._keepAlive)
      {
        dataObj._streamMillis = millis() + 50;
        dataObj._interruptRequest = true;
        delay(20);
        if (dataObj.http.http_connected())
        {
          delay(20);
          forceEndHTTP(dataObj);
          if (dataObj.http.http_connected())
            if (!dataObj._isStream)
            {
              dataObj._firebaseCall = false;
              return false;
            }
        }
        dataObj._httpConnected = false;
      }
    }
  }

  if (!dataObj._keepAlive && dataObj._httpConnected)
  {
    dataObj._firebaseCall = false;
    return false;
  }

  dataObj._httpConnected = true;
  dataObj._interruptRequest = false;
  dataObj._redirectURL = "";
  dataObj._r_method = method;
  dataObj._r_dataType = dataType;

  if (!apConnected(dataObj))
    return false;

  int httpCode = firebaseConnect(dataObj, path, method, dataType, payload);

  if (httpCode == 0)
  {
    if (method == FirebaseMethod::BACKUP)
    {
      dataObj._file_transfering = true;
    }
    else if (method == FirebaseMethod::RESTORE || (dataType == FirebaseDataType::FILE && (method == FirebaseMethod::PUT_SILENT || method == FirebaseMethod::POST)))
    {
      dataObj._file_transfering = true;
    }

    dataObj._dataType2 = dataType;

    if (method == FirebaseMethod::STREAM)
    {

      dataObj._dataMillis = millis();
      flag = getServerStreamResponse(dataObj);
      if (!flag)
        forceEndHTTP(dataObj);
    }
    else if (method == FirebaseMethod::BACKUP || (dataType == FirebaseDataType::FILE && method == FirebaseMethod::GET))
    {
      flag = getDownloadResponse(dataObj);
    }
    else if (method == FirebaseMethod::RESTORE || (dataType == FirebaseDataType::FILE && method == FirebaseMethod::PUT_SILENT))
    {
      flag = getUploadResponse(dataObj);
    }
    else
    {
      dataObj._path = path;
      flag = getServerResponse(dataObj);
      dataObj._dataAvailable = dataObj._data.length() > 0 || dataObj._blob.size() > 0;

      if (dataObj._file_transfering)
        endFileTransfer(dataObj);
    }

    if (!flag)
      forceEndHTTP(dataObj);
  }
  else
  {
    //can't establish connection
    dataObj._httpCode = httpCode;
    dataObj._httpConnected = false;
    delay(100);
    dataObj._firebaseCall = false;
    return false;
  }
  dataObj._firebaseCall = false;
  return flag;
}

bool FirebaseESP32::getServerResponse(FirebaseData &dataObj)
{

  if (dataObj._pause)
    return true;

  if (!apConnected(dataObj))
    return false;

  WiFiClient *netClient = dataObj.http.http_getStreamPtr();
  if (!netClient || dataObj._interruptRequest)
    return cancelCurrentResponse(dataObj);
  if (!handleNetNotConnected(dataObj) || !dataObj._httpConnected)
    return false;

  bool flag = false;
  std::string lineBuf = "";
  std::string eventType = "";
  std::string jsonRes = "";

  char c;
  int p1, p2;
  bool isStream = false;
  dataObj._httpCode = -1000;
  dataObj._contentLength = -1;
  dataObj._pushName.clear();

  bool hasEvent = false;
  bool hasEventData = false;
  bool hasBlob = false;
  bool rulesBegin = false;

  size_t lfCount = 0;
  size_t charPos = 0;

  unsigned long dataTime = millis();

  if (!dataObj._isStream)
    while (netClient->connected() && !netClient->available() && millis() - dataTime < dataObj.http.tcpTimeout)
    {
      if (!apConnected(dataObj))
        return false;
      delay(1);
    }

  dataTime = millis();
  if (netClient->connected() && netClient->available())
  {

    while (netClient->available())
    {
      if (dataObj._interruptRequest)
        return cancelCurrentResponse(dataObj);

      if (!apConnected(dataObj))
        return false;

      c = netClient->read();

      if (!hasBlob)
      {
        if (c != '\r' && c != '\n')
          lineBuf.append(1, c);
      }
      else
      {

        if (c != '"' && c != '\n' && dataObj._httpCode != HTTP_CODE_NO_CONTENT)
          lineBuf.append(1, c);

        continue;
      }

      if (charPos >= strlen(ESP32_FIREBASE_STR_92) - 1 && !hasBlob)
      {

        if (lineBuf.find(ESP32_FIREBASE_STR_92) == 0)
        {
          hasBlob = true;
          if (dataObj._dataType2 != FirebaseDataType::BLOB)
            dataObj._httpCode = FIREBASE_ERROR_DATA_TYPE_MISMATCH;
          lineBuf.clear();
        }
      }
      charPos++;

      if (c == '\n')
      {

        if (rulesBegin)
          jsonRes += lineBuf;

        dataTime = millis();

        p1 = lineBuf.find(ESP32_FIREBASE_STR_5);
        if (p1 != std::string::npos)
        {
          p2 = lineBuf.find(ESP32_FIREBASE_STR_6, p1 + strlen(ESP32_FIREBASE_STR_5));
          if (p2 != std::string::npos)
            dataObj._httpCode = atoi(lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_5), p2 - p1 - strlen(ESP32_FIREBASE_STR_5)).c_str());
        }

        if (dataObj._httpCode == HTTP_CODE_TEMPORARY_REDIRECT)
        {

          p1 = lineBuf.find(ESP32_FIREBASE_STR_95);
          if (p1 != std::string::npos)
          {
            dataObj._redirectURL = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_95));

            int res = firebaseConnect(dataObj, dataObj._redirectURL, dataObj._r_method, dataObj._r_dataType, "");

            if (res == 0)
              goto EXIT_4;

            goto EXIT_3;
          }
        }

        if (dataObj._httpCode == HTTP_CODE_NO_CONTENT)
          continue;

        p1 = lineBuf.find(ESP32_FIREBASE_STR_7);
        if (p1 != std::string::npos)
        {
          p1 = lineBuf.find(ESP32_FIREBASE_STR_102);
          if (p1 != std::string::npos)
          {
            dataObj._firebaseError = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_102) + 1);
            dataObj._firebaseError = dataObj._firebaseError.substr(0, dataObj._firebaseError.length() - 1);
          }

          p1 = lineBuf.find(ESP32_FIREBASE_STR_8);
          if (p1 != std::string::npos)
            if (lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_8)) == ESP32_FIREBASE_STR_9)
              isStream = true;

          p1 = lineBuf.find(ESP32_FIREBASE_STR_10);
          if (p1 != std::string::npos)
          {
            if (lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_10)) == ESP32_FIREBASE_STR_11)
              dataObj._keepAlive = true;
            else
              dataObj._keepAlive = false;
          }

          p1 = lineBuf.find(ESP32_FIREBASE_STR_12);
          if (p1 != std::string::npos)
            dataObj._contentLength = atoi(lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_12)).c_str());

          p1 = lineBuf.find(ESP32_FIREBASE_STR_13);
          if (p1 != std::string::npos)
          {
            eventType = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_13));
            hasEvent = true;
            isStream = true;
            dataObj._httpCode = HTTP_CODE_OK;
          }

          p1 = lineBuf.find(ESP32_FIREBASE_STR_14);
          if (p1 != std::string::npos)
          {
            hasEventData = true;
            isStream = true;
            dataObj._httpCode = HTTP_CODE_OK;
          }
        }

        if (dataObj._httpCode == HTTP_CODE_OK && lfCount > 0 && lineBuf.length() == 0 && dataObj._r_method == FirebaseMethod::GET_RULES)
          rulesBegin = true;

        if (!hasEventData || !hasEvent)
          lineBuf.clear();

        lfCount++;
        charPos = 0;
      }

      if (millis() - dataTime > dataObj.http.tcpTimeout)
      {
        dataObj._httpCode = HTTPC_ERROR_READ_TIMEOUT;
        break;
      }
    }

    if (dataObj._httpCode == HTTP_CODE_OK)
    {
      if (rulesBegin)
      {
        jsonRes += lineBuf;
        lineBuf = jsonRes;
      }

      if (dataObj._r_method == FirebaseMethod::SET_RULES)
      {
        p1 = lineBuf.find(ESP32_FIREBASE_STR_104);
        if (p1 != std::string::npos)
        {
          lineBuf.clear();
        }
      }

      //JSON stream data?
      if (isStream)
      {
        if (hasEventData && hasEvent)
        {
          if (eventType == ESP32_FIREBASE_STR_15 || eventType == ESP32_FIREBASE_STR_16)
          {
            dataObj._eventType = eventType;

            //Parses json response for path
            p1 = lineBuf.find(ESP32_FIREBASE_STR_17);
            if (p1 != std::string::npos)
            {
              p2 = lineBuf.find(ESP32_FIREBASE_STR_3, p1 + strlen(ESP32_FIREBASE_STR_17));
              if (p2 != std::string::npos)
              {
                dataObj._path.clear();
                dataObj._path = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_17), p2 - p1 - strlen(ESP32_FIREBASE_STR_17));
              }
            }

            //Parses json response for data
            p1 = lineBuf.find(ESP32_FIREBASE_STR_18);
            if (p1 != std::string::npos)
            {
              dataObj._data.clear();
              dataObj._data = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_18), lineBuf.length() - p1 - strlen(ESP32_FIREBASE_STR_18) - 1);

              setDataType(dataObj, dataObj._data);
              bool samePath = dataObj._path == dataObj._path2;
              bool rootPath = dataObj._path == ESP32_FIREBASE_STR_1;
              bool emptyPath = dataObj._path2.length() == 0;
              bool sameData = dataObj._data == dataObj._data2;

              if (dataObj._data.length() >= strlen(ESP32_FIREBASE_STR_92) && !hasBlob)
              {
                if (dataObj._data.substr(0, strlen(ESP32_FIREBASE_STR_92)) == ESP32_FIREBASE_STR_92)
                {
                  lineBuf.clear();
                  lineBuf = dataObj._data;
                  dataObj._data = lineBuf.substr(strlen(ESP32_FIREBASE_STR_92), lineBuf.length() - strlen(ESP32_FIREBASE_STR_92) - 1);
                  hasBlob = true;
                  std::vector<uint8_t>().swap(dataObj._blob);
                  dataObj._dataType = FirebaseDataType::BLOB;
                  base64_decode_string(dataObj._data, dataObj._blob);
                  dataObj._data.clear();
                  dataObj._data2.clear();
                }
              }

              //Any stream update?
              if ((!samePath && (!rootPath || emptyPath)) || (samePath && !sameData && !dataObj._streamPathChanged))
              {

                dataObj._streamDataChanged = true;
                dataObj._data2.clear();
                dataObj._data2 = dataObj._data;
                dataObj._path2.clear();
                dataObj._path2 = dataObj._path;
              }
              else
                dataObj._streamDataChanged = false;
              dataObj._dataMillis = millis();
              flag = true;
              dataObj._dataAvailable = flag;
              dataObj._isStreamTimeout = false;
            }

            dataObj._streamPathChanged = false;
          }
          else
          {
            //Firebase keep alive event
            if (eventType == ESP32_FIREBASE_STR_11)
            {
              dataObj._isStreamTimeout = false;
              dataObj._dataMillis = millis();

              if (dataObj._timeoutCallback)
                dataObj._timeoutCallback(false);
            }

            //Firebase cancel and auth_revoked events
            else if (eventType == ESP32_FIREBASE_STR_109 || eventType == ESP32_FIREBASE_STR_110)
            {
              dataObj._isStreamTimeout = false;
              dataObj._dataMillis = millis();
              dataObj._eventType = eventType;
              //make stream available status
              dataObj._streamDataChanged = true;
              dataObj._dataAvailable = true;
            }
          }
        }
        dataObj._streamMillis = millis();
      }
      else
      {
        //Just text payload
        dataObj._data.clear();

        if (hasBlob)
        {
          dataObj._dataType = FirebaseDataType::BLOB;
          base64_decode_string(lineBuf, dataObj._blob);
        }
        else
        {
          dataObj._data = lineBuf;
          setDataType(dataObj, lineBuf);

          //Push (POST) data?
          p1 = lineBuf.find(ESP32_FIREBASE_STR_20);
          if (p1 != std::string::npos)
          {
            p2 = lineBuf.find(ESP32_FIREBASE_STR_3, p1 + strlen(ESP32_FIREBASE_STR_20));
            if (p2 != std::string::npos)
            {
              dataObj._pushName.clear();
              dataObj._pushName = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_20), p2 - p1 - strlen(ESP32_FIREBASE_STR_20));
              dataObj._dataType = -1;
              dataObj._dataType2 = -1;
              dataObj._data.clear();
            }
          }
        }
      }
    }
    else
    {
      if (dataObj._isSilentResponse)
      {
        if (dataObj._httpCode == HTTP_CODE_NO_CONTENT)
        {
          dataObj._httpCode = HTTP_CODE_OK;
          dataObj._path.clear();
          dataObj._data.clear();
          dataObj._pushName.clear();
          dataObj._dataType = -1;
          dataObj._dataType2 = -1;
          dataObj._dataAvailable = false;
        }
      }
      else
        dataObj._contentLength = -1;
    }

    if (dataObj._dataType == FirebaseDataType::NULL_)
    {
      dataObj._pathNotExist = true;
    }
    else
    {
      dataObj._pathNotExist = false;
      bool _n1 = dataObj._dataType == FirebaseDataType::FLOAT || dataObj._dataType == FirebaseDataType::INTEGER;
      bool _n2 = dataObj._dataType2 == FirebaseDataType::FLOAT || dataObj._dataType2 == FirebaseDataType::INTEGER;

      if (dataObj._dataType2 == dataObj._dataType || (_n1 && _n2))
        dataObj._mismatchDataType = false;
      else
        dataObj._mismatchDataType = true;
    }

    if (!dataObj._httpCode)
    {
      dataObj._contentLength = -1;
      dataObj._httpCode = HTTPC_ERROR_NO_HTTP_SERVER;
    }
    dataObj._httpConnected = false;
    dataObj._streamMillis = millis();

    goto EXIT_2;
  }

  if (dataObj._httpCode == -1000)
    flag = true;

  dataObj._httpConnected = false;
  dataObj._streamMillis = millis();

  return flag;

EXIT_2:

  std::string().swap(lineBuf);
  std::string().swap(eventType);
  std::string().swap(jsonRes);

  if (dataObj._httpCode == HTTPC_ERROR_READ_TIMEOUT)
    return false;
  return dataObj._httpCode == HTTP_CODE_OK;

EXIT_3:

  std::string().swap(lineBuf);
  std::string().swap(eventType);
  std::string().swap(jsonRes);
  return true;

EXIT_4:
  std::string().swap(lineBuf);
  std::string().swap(eventType);
  std::string().swap(jsonRes);
  return getServerResponse(dataObj);
}

bool FirebaseESP32::getDownloadResponse(FirebaseData &dataObj)
{

  if (dataObj._pause)
  {
    endFileTransfer(dataObj);
    return true;
  }

  if (!apConnected(dataObj))
    return false;

  WiFiClient *netClient = dataObj.http.http_getStreamPtr();
  if (!netClient)
  {
    endFileTransfer(dataObj);
    return false;
  }

  dataObj._httpCode = -1000;
  char c = 0;
  size_t bufSize = 1024;
  char *buf = new char[bufSize + 1];

  std::string lineBuf = "";
  std::string contentType = "";
  std::string contentDisposition = "";

  size_t contentLength = 0;
  int p1 = 0;
  int p2 = 0;
  bool beginPayload = false;
  int count = 0;
  size_t toRead = count;
  int cnt = 0;

  unsigned long dataTime = millis();

  while (netClient->connected() && !netClient->available() && millis() - dataTime < dataObj.http.tcpTimeout + 5000)
  {

    if (!apConnected(dataObj))
      return false;

    delay(1);
  }

  dataTime = millis();
  if (netClient->connected() && netClient->available())
  {

    while (netClient->available() || count > 0)
    {
      if (dataObj._interruptRequest)
        return cancelCurrentResponse(dataObj);

      if (!apConnected(dataObj))
        return false;

      if (!beginPayload)
      {
        c = netClient->read();
        if (c != '\r' && c != '\n')
          lineBuf.append(1, c);
      }
      else
      {

        if (contentLength <= 4)
        {
          dataObj._httpCode = HTTP_CODE_NOT_FOUND;
          continue;
        }
        else
        {

          toRead = count;
          cnt = 0;
          yield();
          if (toRead > bufSize)
            toRead = bufSize;

          memset(buf, 0, bufSize + 1);
          cnt = 0;

          while (cnt < toRead)
          {

            c = netClient->read();
            if (c >= 0x20)
            {
              if (dataObj._fileName == "" && c < 0xff)
              {
                buf[cnt] = c;
                cnt++;
              }
              else if (dataObj._fileName != "" && c != '"')
              {
                buf[cnt] = c;
                cnt++;
              }
            }
          }

          if (cnt > 0)
          {
            count -= cnt;
            dataTime = millis();
            toRead = cnt;
            buf[cnt] = '\0';
            if (dataObj._fileName == "")
              file.write((uint8_t *)buf, toRead);
            else
              base64_decode_file(file, buf, toRead);
          }

          if (count == 0)
            break;
        }
      }

      if (c == '\n' && !beginPayload)
      {

        dataTime = millis();

        p1 = lineBuf.find(ESP32_FIREBASE_STR_5);
        if (p1 != std::string::npos)
        {
          p2 = lineBuf.find(ESP32_FIREBASE_STR_6, p1 + strlen(ESP32_FIREBASE_STR_5));
          if (p2 != std::string::npos)
          {
            dataObj._httpCode = atoi(lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_5), p2 - p1 - strlen(ESP32_FIREBASE_STR_5)).c_str());
          }
        }

        p1 = lineBuf.find(ESP32_FIREBASE_STR_102);
        if (p1 != std::string::npos)
        {
          dataObj._firebaseError = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_102) + 1);
          dataObj._firebaseError = dataObj._firebaseError.substr(0, dataObj._firebaseError.length() - 1);
        }

        p1 = lineBuf.find(ESP32_FIREBASE_STR_8);
        if (p1 != std::string::npos)
        {
          p2 = lineBuf.find(ESP32_FIREBASE_STR_79, p1 + strlen(ESP32_FIREBASE_STR_8));
          if (p2 != std::string::npos)
          {
            contentType = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_8), p2 - p1 - strlen(ESP32_FIREBASE_STR_8)).c_str();
          }
        }

        p1 = lineBuf.find(ESP32_FIREBASE_STR_12);
        if (p1 != std::string::npos)
        {
          contentLength = atoi(lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_12)).c_str());
          dataObj._backupzFileSize = contentLength;
          count = contentLength;
          if (dataObj._fileName != "")
            count -= strlen(ESP32_FIREBASE_STR_93) - 1;
        }

        p1 = lineBuf.find(ESP32_FIREBASE_STR_80);
        if (p1 != std::string::npos)
        {
          p2 = lineBuf.find(ESP32_FIREBASE_STR_79, p1 + strlen(ESP32_FIREBASE_STR_80));
          if (p2 != std::string::npos)
          {
            contentDisposition = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_80), p2 - p1 - strlen(ESP32_FIREBASE_STR_80)).c_str();
          }
        }

        if (dataObj._httpCode == HTTP_CODE_OK && lineBuf.length() == 0 && contentLength > 0 && contentType == ESP32_FIREBASE_STR_81 && contentDisposition == ESP32_FIREBASE_STR_82)
        {
          beginPayload = true;
          if (dataObj._fileName != "")
          {
            for (int i = 0; i < strlen(ESP32_FIREBASE_STR_93); i++)
              netClient->read();
          }
        }

        lineBuf.clear();
      }

      if (millis() - dataTime > dataObj.http.tcpTimeout)
      {
        dataObj._httpCode = HTTPC_ERROR_READ_TIMEOUT;
        break;
      }
    }
  }

  delete[] buf;

  std::string().swap(lineBuf);
  std::string().swap(contentType);
  std::string().swap(contentDisposition);

  endFileTransfer(dataObj);

  if (dataObj._httpCode != -1000 && dataObj._httpCode != HTTP_CODE_OK)
  {
    dataObj._file_transfer_error.clear();
    if (dataObj._httpCode == HTTP_CODE_NOT_FOUND)
    {
      dataObj._file_transfer_error = ESP32_FIREBASE_STR_88;
    }
    else
    {
      dataObj._file_transfer_error = ESP32_FIREBASE_STR_87;
      dataObj._file_transfer_error += dataObj.errorReason().c_str();
    }
    return false;
  }

  return true;
}

bool FirebaseESP32::getUploadResponse(FirebaseData &dataObj)
{

  if (dataObj._pause)
    return true;

  if (!apConnected(dataObj))
    return false;

  WiFiClient *netClient = dataObj.http.http_getStreamPtr();
  if (!netClient)
    return false;

  int _httpCode = -1000;
  char c = 0;

  std::string lineBuf = "";
  int p1, p2;
  bool beginPayload = false;

  unsigned long dataTime = millis();

  if (!dataObj._isStream)
    while (netClient->connected() && !netClient->available() && millis() - dataTime < dataObj.http.tcpTimeout + 5000)
    {
      if (!apConnected(dataObj))
        return false;
      delay(1);
    }

  dataTime = millis();

  if (netClient->connected() && netClient->available())
  {

    while (netClient->available())
    {
      if (dataObj._interruptRequest)
        return cancelCurrentResponse(dataObj);

      if (!apConnected(dataObj))
        return false;

      c = netClient->read();

      if (c != '\r' && c != '\n')
        lineBuf.append(1, c);

      if (c == '\n' && !beginPayload)
      {
        dataTime = millis();

        p1 = lineBuf.find(ESP32_FIREBASE_STR_102);
        if (p1 != std::string::npos)
        {
          dataObj._firebaseError = lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_102) + 1);
          dataObj._firebaseError = dataObj._firebaseError.substr(0, dataObj._firebaseError.length() - 1);
        }

        p1 = lineBuf.find(ESP32_FIREBASE_STR_5);
        if (p1 != std::string::npos)
        {
          p2 = lineBuf.find(ESP32_FIREBASE_STR_6, p1 + strlen(ESP32_FIREBASE_STR_5));
          if (p2 != std::string::npos)
          {
            _httpCode = atoi(lineBuf.substr(p1 + strlen(ESP32_FIREBASE_STR_5), p2 - p1 - strlen(ESP32_FIREBASE_STR_5)).c_str());
          }
        }

        lineBuf.clear();
      }

      if (millis() - dataTime > dataObj.http.tcpTimeout)
      {
        _httpCode = HTTPC_ERROR_READ_TIMEOUT;
        dataObj._httpCode = HTTPC_ERROR_READ_TIMEOUT;
        break;
      }
    }
  }

  dataObj._file_transfering = false;

  if (_httpCode == HTTP_CODE_NO_CONTENT)
    dataObj._httpCode = HTTP_CODE_OK;
  else
    dataObj._httpCode = _httpCode;

  if (dataObj._httpCode != -1000 && _httpCode != HTTP_CODE_NO_CONTENT)
  {
    dataObj._file_transfer_error.clear();
    dataObj._file_transfer_error = ESP32_FIREBASE_STR_87;
    dataObj._file_transfer_error += dataObj.errorReason().c_str();
  }

  return _httpCode == HTTP_CODE_NO_CONTENT;
}

void FirebaseESP32::endFileTransfer(FirebaseData &dataObj)
{
  _sdInUse = false;
  file.close();

  SD.end();
  _sdOk = false;
  dataObj._file_transfering = false;
}

bool FirebaseESP32::firebaseConnectStream(FirebaseData &dataObj, const std::string &path)
{

  if (dataObj._pause || dataObj._file_transfering)
    return true;

  dataObj._streamStop = false;

  if (dataObj._isStream && path == dataObj._streamPath)
    return true;

  if (path.length() == 0 || _host.length() == 0 || _auth.length() == 0)
  {
    dataObj._httpCode = HTTP_CODE_BAD_REQUEST;
    return false;
  }

  if (!apConnected(dataObj))
    return false;

  if (millis() - dataObj._streamResetMillis > 50)
    delay(50);

  bool flag;
  flag = dataObj._streamPath.length() == 0;
  flag |= firebaseConnect(dataObj, path, FirebaseMethod::STREAM, FirebaseDataType::STRING, "") == 0;
  dataObj._dataMillis = millis();
  return flag;
}

bool FirebaseESP32::getServerStreamResponse(FirebaseData &dataObj)
{

  bool res = false;

  if (dataObj._pause)
    return true;

  if (dataObj._streamStop)
    return true;

  unsigned long ml = millis();
  if (dataObj._streamMillis == 0)
    dataObj._streamMillis = ml;
  if (dataObj._streamResetMillis == 0)
    dataObj._streamResetMillis = ml;

  //Reset firebase data every 50 ms and extend delay for 50 ms before stream response checking
  //to allow other http connection that may happen
  if (ml - dataObj._streamResetMillis > 50)
  {
    dataObj._streamResetMillis = ml;
    dataObj._streamDataChanged = false;
    dataObj._data2.clear();
    dataObj._path2.clear();
    dataObj._dataAvailable = false;
    dataObj._isStreamTimeout = false;
    if (dataObj._index == -1)
      delay(50);
    return true;
  }

  if (ml - dataObj._streamMillis > 50)
  {

    dataObj._streamMillis = ml;
    std::string path = "";

    //Stream timeout
    if (dataObj._dataMillis > 0 && millis() - dataObj._dataMillis > KEEP_ALIVE_TIMEOUT)
    {
      dataObj._dataMillis = millis();
      dataObj._isStreamTimeout = true;
      path = dataObj._streamPath;

      if (_reconnectWiFi && WiFi.status() != WL_CONNECTED)
      {
        uint8_t tryCount = 0;
        WiFi.reconnect();
        while (WiFi.status() != WL_CONNECTED)
        {
          tryCount++;
          delay(50);
          if (tryCount > 60)
            break;
        }
      }

      if (!apConnected(dataObj))
        return false;

      while (dataObj._firebaseCall)
        delay(1);

      dataObj._streamCall = true;

      firebaseConnectStream(dataObj, path);

      res = false;

      res = getServerResponse(dataObj);

      dataObj._streamCall = false;

      return res;
    }

    //last connection was not close
    if (dataObj._httpConnected)
      return true;

    dataObj._httpConnected = true;
    resetFirebasedataFlag(dataObj);

    if (!apConnected(dataObj))
      return false;

    while (dataObj._firebaseCall)
      delay(1);

    dataObj._streamCall = true;

    WiFiClient *netClient = dataObj.http.http_getStreamPtr();
    if (netClient)
    {
      if (netClient->connected() && !dataObj._isStream)
        forceEndHTTP(dataObj);
      if (!netClient->connected())
      {
        path = dataObj._streamPath;
        firebaseConnectStream(dataObj, path);
      }
    }
    else
    {
      path = dataObj._streamPath;
      firebaseConnectStream(dataObj, path);
    }

    std::string().swap(path);

    bool res = false;

    res = getServerResponse(dataObj);

    dataObj._streamCall = false;

    return res;
  }

  return true;
}

bool FirebaseESP32::apConnected(FirebaseData &dataObj)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    dataObj._httpCode = HTTPC_ERROR_CONNECTION_LOST;
    dataObj._firebaseCall = false;
    dataObj._streamCall = false;
    return false;
  }
  return true;
}

void FirebaseESP32::forceEndHTTP(FirebaseData &dataObj)
{

  if (!apConnected(dataObj))
    return;

  WiFiClient *netClient = dataObj.http.http_getStreamPtr();
  if (netClient)
  {
    if (netClient->available() > 0)
    {
      netClient->flush();
      delay(50);
    }
    netClient->stop();
    delay(50);
  }
}

void FirebaseESP32::processErrorQueue(FirebaseData &dataObj, QueueInfoCallback callback)
{
  if (dataObj._firebaseCall)
    return;

  if (!apConnected(dataObj))
    return;

  if (dataObj._qMan._queueCollection.size() > 0)
  {

    for (uint8_t i = 0; i < dataObj._qMan._queueCollection.size(); i++)
    {
      QueueItem item = dataObj._qMan._queueCollection[i];

      if (callback)
      {
        QueueInfo qinfo;
        qinfo._isQueue = true;
        qinfo._dataType = dataObj.getDataType(item.firebaseDataType);
        qinfo._path = item.path;
        qinfo._currentQueueID = item.qID;
        qinfo._method = dataObj.getMethod(item.firebaseMethod);
        qinfo._totalQueue = dataObj._qMan._queueCollection.size();
        qinfo._isQueueFull = dataObj._qMan._queueCollection.size() == dataObj._qMan._maxQueue;
        callback(qinfo);
      }

      if (item.firebaseMethod == FirebaseMethod::GET)
      {

        switch (item.firebaseDataType)
        {

        case FirebaseDataType::INTEGER:

          if (Firebase.getInt(dataObj, item.path.c_str()))
          {

            if (item.intPtr)
              *item.intPtr = dataObj.intData();

            dataObj.clearQueueItem(item);
            dataObj._qMan.remove(i);
          }

          break;

        case FirebaseDataType::FLOAT:

          if (Firebase.getFloat(dataObj, item.path.c_str()))
          {

            if (item.floatPtr)
              *item.floatPtr = dataObj.floatData();

            dataObj.clearQueueItem(item);
            dataObj._qMan.remove(i);
          }

          break;

        case FirebaseDataType::DOUBLE:

          if (Firebase.getDouble(dataObj, item.path.c_str()))
          {

            if (item.doublePtr)
              *item.doublePtr = dataObj.doubleData();

            dataObj.clearQueueItem(item);
            dataObj._qMan.remove(i);
          }

          break;

        case FirebaseDataType::BOOLEAN:

          if (Firebase.getBool(dataObj, item.path.c_str()))
          {

            if (item.boolPtr)
              *item.boolPtr = dataObj.boolData();

            dataObj.clearQueueItem(item);
            dataObj._qMan.remove(i);
          }

          break;

        case FirebaseDataType::STRING:

          if (Firebase.getString(dataObj, item.path.c_str()))
          {

            if (item.stringPtr)
              *item.stringPtr = dataObj.stringData();

            dataObj.clearQueueItem(item);
            dataObj._qMan.remove(i);
          }

          break;

        case FirebaseDataType::JSON:

          if (item.queryFilter._orderBy.length() > 0)
          {
            if (Firebase.getJSON(dataObj, item.path.c_str(), item.queryFilter))
            {

              if (item.stringPtr)
                *item.stringPtr = dataObj.jsonData();

              dataObj.clearQueueItem(item);
              dataObj._qMan.remove(i);
            }
          }
          else
          {
            if (Firebase.getJSON(dataObj, item.path.c_str()))
            {

              if (item.stringPtr)
                *item.stringPtr = dataObj.jsonData();

              dataObj.clearQueueItem(item);
              dataObj._qMan.remove(i);
            }
          }

          break;

        case FirebaseDataType::BLOB:

          if (Firebase.getBlob(dataObj, item.path.c_str()))
          {
            if (item.blobPtr)
              *item.blobPtr = dataObj.blobData();

            dataObj.clearQueueItem(item);
            dataObj._qMan.remove(i);
          }

          break;

        case FirebaseDataType::FILE:

          if (Firebase.getFile(dataObj, item.path.c_str(), item.filename.c_str()))
          {
            dataObj.clearQueueItem(item);
            dataObj._qMan.remove(i);
          }

          break;

        default:
          break;
        }
      }
      else if (item.firebaseMethod == FirebaseMethod::POST || item.firebaseMethod == FirebaseMethod::PUT || item.firebaseMethod == FirebaseMethod::PUT_SILENT || item.firebaseMethod == FirebaseMethod::PATCH || item.firebaseMethod == FirebaseMethod::PATCH_SILENT)
      {

        if (item.firebaseDataType == FirebaseDataType::FILE)
        {
          if (buildRequestFile(dataObj, item.firebaseMethod, item.path.c_str(), item.filename.c_str(), true))
          {
            dataObj.clearQueueItem(item);
            dataObj._qMan.remove(i);
          }
        }
        else
        {
          if (buildRequest(dataObj, item.firebaseMethod, item.firebaseDataType, item.path.c_str(), item.payload.c_str(), true))
          {
            dataObj.clearQueueItem(item);
            dataObj._qMan.remove(i);
          }
        }
      }
    }
  }
}

uint32_t FirebaseESP32::getErrorQueueID(FirebaseData &dataObj)
{
  return dataObj._qID;
}

bool FirebaseESP32::isErrorQueueExisted(FirebaseData &dataObj, uint32_t errorQueueID)
{

  for (uint8_t i = 0; i < dataObj._qMan._queueCollection.size(); i++)
  {
    QueueItem q = dataObj._qMan._queueCollection[i];
    if (q.qID == errorQueueID)
      return true;
  }
  return false;
}

void FirebaseESP32::buildFirebaseRequest(FirebaseData &dataObj, const std::string &host, uint8_t method, const std::string &path, const std::string &auth, int payloadLength, std::string &request)
{

  char *contentLength = new char[20];
  memset(contentLength, 0, 20);

  char *num = new char[20];

  request.clear();
  if (method == FirebaseMethod::STREAM)
  {
    request = ESP32_FIREBASE_STR_22;
    dataObj._isStream = true;
  }
  else
  {
    if (method == FirebaseMethod::PUT || method == FirebaseMethod::PUT_SILENT || method == FirebaseMethod::SET_RULES)
      request = ESP32_FIREBASE_STR_23;
    else if (method == FirebaseMethod::POST)
      request = ESP32_FIREBASE_STR_24;
    else if (method == FirebaseMethod::GET || method == FirebaseMethod::BACKUP || method == FirebaseMethod::GET_RULES)
      request = ESP32_FIREBASE_STR_25;
    else if (method == FirebaseMethod::PATCH || method == FirebaseMethod::PATCH_SILENT || method == FirebaseMethod::RESTORE)
      request = ESP32_FIREBASE_STR_26;
    else if (method == FirebaseMethod::DELETE)
      request = ESP32_FIREBASE_STR_27;
    request += ESP32_FIREBASE_STR_6;
    dataObj._isStream = false;
  }

  if (path.length() > 0)
    if (path[0] != '/')
      request += ESP32_FIREBASE_STR_1;

  request += path;

  if (method == FirebaseMethod::PATCH || method == FirebaseMethod::PATCH_SILENT)
    request += ESP32_FIREBASE_STR_1;

  request += ESP32_FIREBASE_STR_2;
  request += auth;

  if (method == FirebaseMethod::GET && dataObj.queryFilter._orderBy != "")
  {
    request += ESP32_FIREBASE_STR_96;
    request += dataObj.queryFilter._orderBy;

    if (method == FirebaseMethod::GET && dataObj.queryFilter._limitToFirst != "")
    {
      request += ESP32_FIREBASE_STR_97;
      request += dataObj.queryFilter._limitToFirst;
    }

    if (method == FirebaseMethod::GET && dataObj.queryFilter._limitToLast != "")
    {
      request += ESP32_FIREBASE_STR_98;
      request += dataObj.queryFilter._limitToLast;
    }

    if (method == FirebaseMethod::GET && dataObj.queryFilter._startAt != "")
    {
      request += ESP32_FIREBASE_STR_99;
      request += dataObj.queryFilter._startAt;
    }

    if (method == FirebaseMethod::GET && dataObj.queryFilter._endAt != "")
    {
      request += ESP32_FIREBASE_STR_100;
      request += dataObj.queryFilter._endAt;
    }

    if (method == FirebaseMethod::GET && dataObj.queryFilter._equalTo != "")
    {
      request += ESP32_FIREBASE_STR_101;
      request += dataObj.queryFilter._equalTo;
    }
  }

  if (method == FirebaseMethod::BACKUP)
  {
    request += ESP32_FIREBASE_STR_28;
    std::string filename = "";

    for (int i = 0; i < dataObj._backupNodePath.length(); i++)
    {
      if (dataObj._backupNodePath.c_str()[i] == '/')
        filename += ESP32_FIREBASE_STR_4;
      else
        filename += dataObj._backupNodePath.c_str()[i];
    }

    request += filename;
    std::string().swap(filename);
  }

  if (method == FirebaseMethod::GET && dataObj._fileName != "")
  {
    request += ESP32_FIREBASE_STR_28;
    request += dataObj._fileName;
  }

  if (method == FirebaseMethod::RESTORE || method == FirebaseMethod::PUT_SILENT || method == FirebaseMethod::PATCH_SILENT)
    request += ESP32_FIREBASE_STR_29;

  request += ESP32_FIREBASE_STR_30;
  request += ESP32_FIREBASE_STR_31;
  request += host;
  request += ESP32_FIREBASE_STR_21;
  request += ESP32_FIREBASE_STR_32;
  request += ESP32_FIREBASE_STR_33;

  if (method == FirebaseMethod::STREAM)
  {
    request += ESP32_FIREBASE_STR_34;
    request += ESP32_FIREBASE_STR_35;
  }
  else if (method == FirebaseMethod::BACKUP || method == FirebaseMethod::RESTORE)
  {
    request += ESP32_FIREBASE_STR_34;
  }
  else
  {
    request += ESP32_FIREBASE_STR_36;
    request += ESP32_FIREBASE_STR_37;
  }

  if (method != FirebaseMethod::BACKUP && method != FirebaseMethod::RESTORE)
    request += ESP32_FIREBASE_STR_38;

  if (method == FirebaseMethod::PUT || method == FirebaseMethod::PUT_SILENT || method == FirebaseMethod::POST || method == FirebaseMethod::PATCH || method == FirebaseMethod::PATCH_SILENT || method == FirebaseMethod::RESTORE || method == FirebaseMethod::SET_RULES)
  {
    request += ESP32_FIREBASE_STR_12;
    itoa(payloadLength, contentLength, 10);
    request += contentLength;
  }
  request += ESP32_FIREBASE_STR_21;
  request += ESP32_FIREBASE_STR_21;

  delete[] contentLength;
  delete[] num;
}

bool FirebaseESP32::cancelCurrentResponse(FirebaseData &dataObj)
{
  forceEndHTTP(dataObj);
  dataObj._isStream = false;
  dataObj._streamDataChanged = false;
  dataObj._dataMillis = millis();
  dataObj._data2.clear();
  dataObj._path2.clear();
  dataObj._dataAvailable = false;
  dataObj._isStreamTimeout = false;
  dataObj._httpCode = HTTPC_ERROR_CONNECTION_REFUSED;
  return false;
}

void FirebaseESP32::setDataType(FirebaseData &dataObj, const std::string &data)
{

  if (data.length() > 0)
  {
    if (data[0] == '{')
      dataObj._dataType = FirebaseDataType::JSON;
    else if (data == ESP32_FIREBASE_STR_106 || data == ESP32_FIREBASE_STR_107)
      dataObj._dataType = FirebaseDataType::BOOLEAN;
    else if (data.substr(0, strlen(ESP32_FIREBASE_STR_92)) == ESP32_FIREBASE_STR_92)
      dataObj._dataType = FirebaseDataType::BLOB;
    else if (data.substr(0, strlen(ESP32_FIREBASE_STR_93)) == ESP32_FIREBASE_STR_93)
      dataObj._dataType = FirebaseDataType::FILE;
    else if (data[0] == '"')
      dataObj._dataType = FirebaseDataType::STRING;
    else if (data.find(ESP32_FIREBASE_STR_4) != std::string::npos)
    {

      if (data.length() <= 7)
        dataObj._dataType = FirebaseDataType::FLOAT;
      else
        dataObj._dataType = FirebaseDataType::DOUBLE;
    }
    else if (data.find(ESP32_FIREBASE_STR_19) != std::string::npos)
      dataObj._dataType = FirebaseDataType::NULL_;
    else
      dataObj._dataType = FirebaseDataType::INTEGER;

    if (data == ESP32_FIREBASE_STR_19 && dataObj.queryFilter._orderBy == "")
      dataObj._data.clear();
    else if (data == ESP32_FIREBASE_STR_19 && dataObj.queryFilter._orderBy != "")
    {
      dataObj._dataType = FirebaseDataType::JSON;
    }
  }
  else
  {
    dataObj._dataType = FirebaseDataType::NULL_;
  }

  dataObj._dataTypeNum = dataObj._dataType;
}

bool FirebaseESP32::commError(FirebaseData &dataObj)
{
  return dataObj._httpCode == HTTPC_ERROR_CONNECTION_REFUSED || dataObj._httpCode == HTTPC_ERROR_CONNECTION_LOST ||
         dataObj._httpCode == HTTPC_ERROR_SEND_PAYLOAD_FAILED || dataObj._httpCode == HTTPC_ERROR_SEND_HEADER_FAILED ||
         dataObj._httpCode == HTTPC_ERROR_NOT_CONNECTED || dataObj._httpCode == HTTPC_ERROR_READ_TIMEOUT;
}

void FirebaseESP32::resetFirebasedataFlag(FirebaseData &dataObj)
{
  dataObj._streamDataChanged = false;
  dataObj._streamPathChanged = false;
  dataObj._dataAvailable = false;
  dataObj._pushName.clear();
}
bool FirebaseESP32::handleNetNotConnected(FirebaseData &dataObj)
{
  if (!dataObj.http.http_connected())
  {
    dataObj._contentLength = -1;
    dataObj._dataType = FirebaseDataType::NULL_;
    dataObj._httpCode = HTTPC_ERROR_NOT_CONNECTED;
    dataObj._data.clear();
    dataObj._path.clear();
    resetFirebasedataFlag(dataObj);
    return false;
  }
  return true;
}

bool FirebaseESP32::sdBegin(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss)
{
  _sck = sck;
  _miso = miso;
  _mosi = mosi;
  _ss = ss;
  _sdConfigSet = true;
  SPI.begin(_sck, _miso, _mosi, _ss);
  return SD.begin(_ss, SPI);
}

bool FirebaseESP32::sdBegin(void)
{
  _sdConfigSet = false;
  return SD.begin();
}

void FirebaseESP32::errorToString(int httpCode, std::string &buf)
{
  buf.clear();
  switch (httpCode)
  {
  case HTTPC_ERROR_CONNECTION_REFUSED:
    buf = ESP32_FIREBASE_STR_39;
    return;
  case HTTPC_ERROR_SEND_HEADER_FAILED:
    buf = ESP32_FIREBASE_STR_40;
    return;
  case HTTPC_ERROR_SEND_PAYLOAD_FAILED:
    buf = ESP32_FIREBASE_STR_41;
    return;
  case HTTPC_ERROR_NOT_CONNECTED:
    buf = ESP32_FIREBASE_STR_42;
    return;
  case HTTPC_ERROR_CONNECTION_LOST:
    buf = ESP32_FIREBASE_STR_43;
    return;
  case HTTPC_ERROR_NO_HTTP_SERVER:
    buf = ESP32_FIREBASE_STR_44;
    return;
  case HTTP_CODE_BAD_REQUEST:
    buf = ESP32_FIREBASE_STR_45;
    return;
  case HTTP_CODE_NON_AUTHORITATIVE_INFORMATION:
    buf = ESP32_FIREBASE_STR_46;
    return;
  case HTTP_CODE_NO_CONTENT:
    buf = ESP32_FIREBASE_STR_47;
    return;
  case HTTP_CODE_MOVED_PERMANENTLY:
    buf = ESP32_FIREBASE_STR_48;
    return;
  case HTTP_CODE_USE_PROXY:
    buf = ESP32_FIREBASE_STR_49;
    return;
  case HTTP_CODE_TEMPORARY_REDIRECT:
    buf = ESP32_FIREBASE_STR_50;
    return;
  case HTTP_CODE_PERMANENT_REDIRECT:
    buf = ESP32_FIREBASE_STR_51;
    return;
  case HTTP_CODE_UNAUTHORIZED:
    buf = ESP32_FIREBASE_STR_52;
    return;
  case HTTP_CODE_FORBIDDEN:
    buf = ESP32_FIREBASE_STR_53;
    return;
  case HTTP_CODE_NOT_FOUND:
    buf = ESP32_FIREBASE_STR_54;
    return;
  case HTTP_CODE_METHOD_NOT_ALLOWED:
    buf = ESP32_FIREBASE_STR_55;
    return;
  case HTTP_CODE_NOT_ACCEPTABLE:
    buf = ESP32_FIREBASE_STR_56;
    return;
  case HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED:
    buf = ESP32_FIREBASE_STR_57;
    return;
  case HTTP_CODE_REQUEST_TIMEOUT:
    buf = ESP32_FIREBASE_STR_58;
    return;
  case HTTP_CODE_LENGTH_REQUIRED:
    buf = ESP32_FIREBASE_STR_59;
    return;
  case HTTP_CODE_TOO_MANY_REQUESTS:
    buf = ESP32_FIREBASE_STR_60;
    return;
  case HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE:
    buf = ESP32_FIREBASE_STR_61;
    return;
  case HTTP_CODE_INTERNAL_SERVER_ERROR:
    buf = ESP32_FIREBASE_STR_62;
    return;
  case HTTP_CODE_BAD_GATEWAY:
    buf = ESP32_FIREBASE_STR_63;
    return;
  case HTTP_CODE_SERVICE_UNAVAILABLE:
    buf = ESP32_FIREBASE_STR_64;
    return;
  case HTTP_CODE_GATEWAY_TIMEOUT:
    buf = ESP32_FIREBASE_STR_65;
    return;
  case HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED:
    buf = ESP32_FIREBASE_STR_66;
    return;
  case HTTP_CODE_NETWORK_AUTHENTICATION_REQUIRED:
    buf = ESP32_FIREBASE_STR_67;
    return;
  case HTTPC_ERROR_READ_TIMEOUT:
    buf = ESP32_FIREBASE_STR_69;
    return;
  case FIREBASE_ERROR_DATA_TYPE_MISMATCH:
    buf = ESP32_FIREBASE_STR_70;
    return;
  case FIREBASE_ERROR_PATH_NOT_EXIST:
    buf = ESP32_FIREBASE_STR_71;
    return;
  case HTTPC_ERROR_CONNECTION_INUSED:
    buf = ESP32_FIREBASE_STR_94;
    return;
  default:
    return;
  }
}

void FirebaseESP32::setStreamCallback(FirebaseData &dataObj, StreamEventCallback dataAvailablecallback, StreamTimeoutCallback timeoutCallback)
{
  int index = dataObj._index;

  std::string taskName = ESP32_FIREBASE_STR_72;
  size_t idxSize = 20;
  char *idx = new char[idxSize];
  memset(idx, 0, idxSize);

  bool hasHandle = false;

  if (dataObj._handle)
  {
    hasHandle = true;
    vTaskDelete(dataObj._handle);
    dataObj._handle = NULL;
  }

  if (dataObj._q_handle)
  {
    hasHandle = true;
  }

  memset(idx, 0, idxSize);

  if (index == -1)
  {
    index = dataObjectIndex;
    dataObjectIndex++;
  }

  itoa(index, idx, 10);
  taskName += ESP32_FIREBASE_STR_113;
  taskName += idx;

  dataObj._index = index;
  streamIndex = index;
  dataObj._dataAvailableCallback = dataAvailablecallback;
  dataObj._timeoutCallback = timeoutCallback;

  //object created
  if (hasHandle)
    firebaseDataObject[index] = dataObj;
  else
    firebaseDataObject.push_back(dataObj);

  delete[] idx;

  TaskFunction_t taskCode = [](void *param) {
    int id = streamIndex;

    for (;;)
    {
      if ((firebaseDataObject[id].get()._dataAvailableCallback || firebaseDataObject[id].get()._timeoutCallback))
      {

        Firebase.readStream(firebaseDataObject[id].get());

        if (firebaseDataObject[id].get().streamTimeout() && firebaseDataObject[id].get()._timeoutCallback)
          firebaseDataObject[id].get()._timeoutCallback(true);

        if (firebaseDataObject[id].get().streamAvailable() && firebaseDataObject[id].get()._dataAvailableCallback)
        {

          StreamData s;
          s._streamPath = firebaseDataObject[id].get()._streamPath;
          s._data = firebaseDataObject[id].get()._data;
          s._path = firebaseDataObject[id].get()._path;

          s._dataType = firebaseDataObject[id].get()._dataType;
          s._dataTypeStr = firebaseDataObject[id].get().getDataType(s._dataType);
          s._eventTypeStr = firebaseDataObject[id].get()._eventType;

          if (s._dataType == FirebaseESP32::FirebaseDataType::BLOB)
          {
            s._blob = firebaseDataObject[id].get()._blob;
            //Free ram in case of callback data was used
            firebaseDataObject[id].get()._blob.clear();
          }

          firebaseDataObject[id].get()._dataAvailableCallback(s);
          s.empty();
        }
      }

      yield();
      vTaskDelay(30);
    }

    vTaskDelete(NULL);
    firebaseDataObject[id].get()._handle = NULL;
  };

  xTaskCreatePinnedToCore(taskCode, taskName.c_str(), 10000, NULL, 3, &dataObj._handle, 1);
}

void FirebaseESP32::removeStreamCallback(FirebaseData &dataObj)
{
  int index = dataObj._index;

  if (index != -1)
  {
    bool hasOherHandle = false;

    if (dataObj._q_handle)
    {
      hasOherHandle = true;
    }

    if (!hasOherHandle)
      dataObj._index = -1;

    if (dataObj._handle)
      vTaskDelete(dataObj._handle);

    dataObj._handle = NULL;

    dataObj._dataAvailableCallback = NULL;
    dataObj._timeoutCallback = NULL;

    if (!hasOherHandle)
      firebaseDataObject.erase(firebaseDataObject.begin() + index);
  }
}

void FirebaseESP32::beginAutoRunErrorQueue(FirebaseData &dataObj, QueueInfoCallback callback)
{

  int index = dataObj._index;

  std::string taskName = ESP32_FIREBASE_STR_72;
  size_t idxSize = 20;
  char *idx = new char[idxSize];
  memset(idx, 0, idxSize);

  bool hasHandle = false;

  if (dataObj._handle || dataObj._q_handle)
  {
    hasHandle = true;
  }

  memset(idx, 0, idxSize);

  if (index == -1)
  {
    index = dataObjectIndex;
    dataObjectIndex++;
  }

  itoa(index, idx, 10);
  taskName += ESP32_FIREBASE_STR_114;
  taskName += idx;

  if (callback)
    dataObj._queueInfoCallback = callback;
  else
    dataObj._queueInfoCallback = NULL;

  dataObj._index = index;
  errorQueueIndex = index;

  //object created
  if (hasHandle)
    firebaseDataObject[index] = dataObj;
  else
    firebaseDataObject.push_back(dataObj);

  delete[] idx;

  TaskFunction_t taskCode = [](void *param) {
    int id = errorQueueIndex;

    for (;;)
    {

      if (firebaseDataObject[id].get()._queueInfoCallback)
        Firebase.processErrorQueue(firebaseDataObject[id].get(), firebaseDataObject[id].get()._queueInfoCallback);
      else
        Firebase.processErrorQueue(firebaseDataObject[id].get(), NULL);

      yield();
      vTaskDelay(200);
    }

    vTaskDelete(NULL);
    firebaseDataObject[id].get()._q_handle = NULL;
  };

  xTaskCreatePinnedToCore(taskCode, taskName.c_str(), 10000, NULL, 3, &dataObj._q_handle, 1);
}

void FirebaseESP32::endAutoRunErrorQueue(FirebaseData &dataObj)
{
  int index = dataObj._index;

  if (index != -1)
  {
    bool hasOherHandle = false;

    if (dataObj._handle)
    {
      hasOherHandle = true;
    }

    if (!hasOherHandle)
      dataObj._index = -1;

    if (dataObj._q_handle)
      vTaskDelete(dataObj._q_handle);

    dataObj._q_handle = NULL;
    dataObj._queueInfoCallback = NULL;

    if (!hasOherHandle)
      firebaseDataObject.erase(firebaseDataObject.begin() + index);
  }
}
void FirebaseESP32::clearErrorQueue(FirebaseData &dataObj)
{
  for (uint8_t i = 0; i < dataObj._qMan._queueCollection.size(); i++)
  {
    QueueItem item = dataObj._qMan._queueCollection[i];
    dataObj.clearQueueItem(item);
  }
}

bool FirebaseESP32::backup(FirebaseData &dataObj, const String &nodePath, const String &dirPath)
{
  dataObj._backupDir = dirPath.c_str();
  dataObj._backupNodePath = nodePath.c_str();
  dataObj._backupFilename.clear();
  dataObj._fileName.clear();
  bool flag = sendRequest(dataObj, nodePath.c_str(), FirebaseMethod::BACKUP, FirebaseDataType::JSON, "");
  return flag;
}

bool FirebaseESP32::restore(FirebaseData &dataObj, const String &nodePath, const String &dirPath)
{
  dataObj._backupDir = dirPath.c_str();
  dataObj._backupNodePath = nodePath.c_str();
  dataObj._backupFilename.clear();
  dataObj._fileName.clear();
  bool flag = sendRequest(dataObj, nodePath.c_str(), FirebaseMethod::RESTORE, FirebaseDataType::JSON, "");
  return flag;
}

bool FirebaseESP32::sdTest()
{

  if (_sdConfigSet)
    sdBegin(_sck, _miso, _mosi, _ss);
  else
    sdBegin();

  File file = SD.open(ESP32_FIREBASE_STR_73, FILE_WRITE);
  if (!file)
    return false;

  if (!file.write(32))
    return false;
  file.close();

  file = SD.open(ESP32_FIREBASE_STR_73);
  if (!file)
    return false;

  while (file.available())
  {
    if (file.read() != 32)
      return false;
  }
  file.close();

  SD.remove(ESP32_FIREBASE_STR_73);

  return true;
}

void FirebaseESP32::setMaxRetry(FirebaseData &dataObj, uint8_t num)
{
  dataObj._maxRetry = num;
}

void FirebaseESP32::setMaxErrorQueue(FirebaseData &dataObj, uint8_t num)
{
  dataObj._qMan._maxQueue = num;

  if (dataObj._qMan._queueCollection.size() > num)
  {
    for (uint8_t i = dataObj._qMan._queueCollection.size() - 1; i >= num; i--)
    {
      QueueItem item = dataObj._qMan._queueCollection[i];
      dataObj.clearQueueItem(item);
    }
  }
}

bool FirebaseESP32::saveErrorQueue(FirebaseData &dataObj, const String &filename, uint8_t storageType)
{
  File file;

  if (storageType == QueueStorageType::SD)
  {

    if (!sdTest())
      return false;

    file = SD.open(filename.c_str(), FILE_WRITE);
  }
  else
  {

    SPIFFS.begin(true);
    file = SPIFFS.open(filename.c_str(), "w");
  }

  uint8_t idx = 0;
  std::string buf = "";

  size_t nbufSize = 10;
  char *nbuf = new char[nbufSize];

  for (uint8_t i = 0; i < dataObj._qMan._queueCollection.size(); i++)
  {
    QueueItem item = dataObj._qMan._queueCollection[i];

    if (item.firebaseMethod != FirebaseESP32::FirebaseMethod::GET)
    {
      if (idx > 0)
        buf.append("\r");

      memset(nbuf, 0, nbufSize);
      itoa(item.firebaseDataType, nbuf, 10);
      buf.append(nbuf);
      buf.append("~");

      memset(nbuf, 0, nbufSize);
      itoa(item.firebaseMethod, nbuf, 10);
      buf.append(nbuf);
      buf.append("~");

      buf += item.path.c_str();
      buf.append("~");

      buf += item.payload.c_str();
      buf.append("~");

      for (int j = 0; j < item.blob.size(); j++)
      {
        memset(nbuf, 0, nbufSize);
        itoa(item.blob[j], nbuf, 10);
      }
      buf.append("~");

      buf += item.filename.c_str();

      idx++;
    }
  }

  file.print(buf.c_str());
  file.close();

  delete[] nbuf;
  std::string().swap(buf);

  return true;
}

bool FirebaseESP32::restoreErrorQueue(FirebaseData &dataObj, const String &filename, uint8_t storageType)
{
  return openErrorQueue(dataObj, filename, storageType, 1) != 0;
}

uint8_t FirebaseESP32::errorQueueCount(FirebaseData &dataObj, const String &filename, uint8_t storageType)
{
  return openErrorQueue(dataObj, filename, storageType, 0);
}

bool FirebaseESP32::deleteStorageFile(const String &filename, uint8_t storageType)
{

  if (storageType == QueueStorageType::SD)
  {
    if (!sdTest())
      return false;
    return SD.remove(filename.c_str());
  }
  else
  {
    SPIFFS.begin(true);
    return SPIFFS.remove(filename.c_str());
  }
}

uint8_t FirebaseESP32::openErrorQueue(FirebaseData &dataObj, const String &filename, uint8_t storageType, uint8_t mode)
{

  uint8_t count = 0;

  File file;

  if (storageType == QueueStorageType::SD)
  {

    if (!sdTest())
      return 0;

    file = SD.open(filename.c_str(), FILE_READ);
  }
  else
  {

    SPIFFS.begin(true);

    file = SPIFFS.open(filename.c_str(), "r");
  }

  std::string t = "";

  uint8_t c = 0;

  while (file.available())
  {
    c = file.read();
    t += (char)c;
  }

  file.close();

  std::vector<std::string> p = splitString(dataObj._maxBlobSize, t.c_str(), '\r');

  for (int i = 0; i < p.size(); i++)
  {

    std::vector<std::string> q = splitString(dataObj._maxBlobSize, p[i].c_str(), '~');

    if (q.size() == 6)
    {
      count++;

      if (mode == 1)
      {

        //Restore Firebase Error Queues
        QueueItem item;

        item.firebaseDataType = atoi(q[0].c_str());
        item.firebaseMethod = atoi(q[1].c_str());
        item.path.append(q[2].c_str());
        item.payload.append(q[3].c_str());

        for (int j = 0; j < q[4].length(); j++)
          item.blob.push_back(atoi(q[4].c_str()));

        item.filename.append(q[5].c_str());

        dataObj._qMan._queueCollection.push_back(item);
      }
    }
  }
  std::string().swap(t);

  return count;
}

bool FirebaseESP32::isErrorQueueFull(FirebaseData &dataObj)
{
  if (dataObj._qMan._maxQueue > 0)
    return dataObj._qMan._queueCollection.size() >= dataObj._qMan._maxQueue;
  return false;
}

uint8_t FirebaseESP32::errorQueueCount(FirebaseData &dataObj)
{
  return dataObj._qMan._queueCollection.size();
}

std::vector<std::string> FirebaseESP32::splitString(int size, const char *str, const char delim)
{
  uint16_t index = 0;
  uint16_t len = strlen(str);
  int buffSize = (int)(size * 1.4f);
  char *buf = new char[buffSize];
  std::vector<std::string> out;

  for (uint16_t i = 0; i < len; i++)
  {
    if (str[i] == delim)
    {
      memset(buf, 0, buffSize);
      strncpy(buf, (char *)str + index, i - index);
      buf[i - index] = '\0';
      index = i + 1;
      out.push_back(buf);
    }
  }
  if (index < len + 1)
  {
    memset(buf, 0, buffSize);
    strncpy(buf, (char *)str + index, len - index);
    buf[len - index] = '\0';
    out.push_back(buf);
  }

  delete[] buf;

  return out;
}

void FirebaseESP32::createDirs(std::string dirs)
{
  std::string dir = "";
  int count = 0;
  for (int i = 0; i < dirs.length(); i++)
  {
    dir.append(1, dirs[i]);
    count++;
    if (dirs[i] == '/')
    {
      if (dir.length() > 0)
        SD.mkdir(dir.substr(0, dir.length() - 1).c_str());
      count = 0;
    }
  }
  if (count > 0)
    SD.mkdir(dir.c_str());
  std::string().swap(dir);
}

bool FirebaseESP32::replace(std::string &str, const std::string &from, const std::string &to)
{
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos)
    return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

std::string FirebaseESP32::base64_encode_string(const unsigned char *src, size_t len)
{
  unsigned char *out, *pos;
  const unsigned char *end, *in;

  size_t olen;

  olen = 4 * ((len + 2) / 3); /* 3-byte blocks to 4-byte */

  if (olen < len)
    return std::string();

  std::string outStr;
  outStr.resize(olen);
  out = (unsigned char *)&outStr[0];

  end = src + len;
  in = src;
  pos = out;

  while (end - in >= 3)
  {
    *pos++ = ESP32_FIREBASE_base64_table[in[0] >> 2];
    *pos++ = ESP32_FIREBASE_base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
    *pos++ = ESP32_FIREBASE_base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
    *pos++ = ESP32_FIREBASE_base64_table[in[2] & 0x3f];
    in += 3;
    yield();
  }

  if (end - in)
  {
    *pos++ = ESP32_FIREBASE_base64_table[in[0] >> 2];
    if (end - in == 1)
    {
      *pos++ = ESP32_FIREBASE_base64_table[(in[0] & 0x03) << 4];
      *pos++ = '=';
    }
    else
    {
      *pos++ = ESP32_FIREBASE_base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
      *pos++ = ESP32_FIREBASE_base64_table[(in[1] & 0x0f) << 2];
    }
    *pos++ = '=';
  }

  return outStr;
}

void FirebaseESP32::send_base64_encode_file(WiFiClient *netClient, const std::string &filePath)
{

  File file = SD.open(filePath.c_str(), FILE_READ);

  if (!file)
    return;

  size_t chunkSize = 512;
  size_t fbufSize = 3;
  size_t byteAdd = 0;
  size_t byteSent = 0;

  unsigned char *buf = new unsigned char[chunkSize];
  memset(buf, 0, chunkSize);

  size_t len = file.size();
  size_t fbufIndex = 0;
  unsigned char *fbuf = new unsigned char[3];

  while (file.available())
  {
    memset(fbuf, 0, fbufSize);
    if (len - fbufIndex >= 3)
    {
      file.read(fbuf, 3);

      buf[byteAdd++] = ESP32_FIREBASE_base64_table[fbuf[0] >> 2];
      buf[byteAdd++] = ESP32_FIREBASE_base64_table[((fbuf[0] & 0x03) << 4) | (fbuf[1] >> 4)];
      buf[byteAdd++] = ESP32_FIREBASE_base64_table[((fbuf[1] & 0x0f) << 2) | (fbuf[2] >> 6)];
      buf[byteAdd++] = ESP32_FIREBASE_base64_table[fbuf[2] & 0x3f];

      if (len > chunkSize)
      {
        if (byteAdd >= chunkSize)
        {
          byteSent += byteAdd;
          netClient->write(buf, byteAdd);
          memset(buf, 0, chunkSize);
          byteAdd = 0;
        }
      }
      fbufIndex += 3;

      yield();
    }
    else
    {

      if (len - fbufIndex == 1)
      {
        fbuf[0] = file.read();
      }
      else if (len - fbufIndex == 2)
      {
        fbuf[0] = file.read();
        fbuf[1] = file.read();
      }

      break;
    }
  }

  file.close();

  if (byteAdd > 0)
    netClient->write(buf, byteAdd);

  if (len - fbufIndex > 0)
  {

    memset(buf, 0, chunkSize);
    byteAdd = 0;

    buf[byteAdd++] = ESP32_FIREBASE_base64_table[fbuf[0] >> 2];
    if (len - fbufIndex == 1)
    {
      buf[byteAdd++] = ESP32_FIREBASE_base64_table[(fbuf[0] & 0x03) << 4];
      buf[byteAdd++] = '=';
    }
    else
    {
      buf[byteAdd++] = ESP32_FIREBASE_base64_table[((fbuf[0] & 0x03) << 4) | (fbuf[1] >> 4)];
      buf[byteAdd++] = ESP32_FIREBASE_base64_table[(fbuf[1] & 0x0f) << 2];
    }
    buf[byteAdd++] = '=';

    netClient->write(buf, byteAdd);
  }

  delete[] buf;
  delete[] fbuf;
}

bool FirebaseESP32::base64_decode_string(const std::string src, std::vector<uint8_t> &out)
{
  unsigned char *dtable = new unsigned char[256];
  memset(dtable, 0x80, 256);
  for (size_t i = 0; i < sizeof(ESP32_FIREBASE_base64_table) - 1; i++)
    dtable[ESP32_FIREBASE_base64_table[i]] = (unsigned char)i;
  dtable['='] = 0;

  unsigned char *block = new unsigned char[4];
  unsigned char tmp;
  size_t i, count;
  int pad = 0;
  size_t extra_pad;
  size_t len = src.length();

  count = 0;

  for (i = 0; i < len; i++)
  {
    if ((uint8_t)dtable[(uint8_t)src[i]] != 0x80)
      count++;
  }

  if (count == 0)
    return false;

  extra_pad = (4 - count % 4) % 4;

  count = 0;
  for (i = 0; i < len + extra_pad; i++)
  {
    unsigned char val;

    if (i >= len)
      val = '=';
    else
      val = src[i];

    tmp = dtable[val];

    if (tmp == 0x80)
      continue;

    if (val == '=')
      pad++;

    block[count] = tmp;
    count++;
    if (count == 4)
    {
      out.push_back((block[0] << 2) | (block[1] >> 4));
      count = 0;
      if (pad)
      {
        if (pad == 1)
          out.push_back((block[1] << 4) | (block[2] >> 2));
        else if (pad > 2)
          return false;

        break;
      }
      else
      {
        out.push_back((block[1] << 4) | (block[2] >> 2));
        out.push_back((block[2] << 6) | block[3]);
      }
    }
  }

  delete[] block;
  delete[] dtable;

  return true;
}

bool FirebaseESP32::base64_decode_file(File &file, const char *src, size_t len)
{
  unsigned char *dtable = new unsigned char[256];
  memset(dtable, 0x80, 256);
  for (size_t i = 0; i < sizeof(ESP32_FIREBASE_base64_table) - 1; i++)
    dtable[ESP32_FIREBASE_base64_table[i]] = (unsigned char)i;
  dtable['='] = 0;

  unsigned char *block = new unsigned char[4];
  unsigned char tmp;
  size_t i, count;
  int pad = 0;
  size_t extra_pad;

  count = 0;

  for (i = 0; i < len; i++)
  {
    if ((uint8_t)dtable[(uint8_t)src[i]] != 0x80)
      count++;
  }

  if (count == 0)
    return false;

  extra_pad = (4 - count % 4) % 4;

  count = 0;
  for (i = 0; i < len + extra_pad; i++)
  {

    unsigned char val;

    if (i >= len)
      val = '=';
    else
      val = src[i];
    tmp = dtable[val];
    if (tmp == 0x80)
      continue;

    if (val == '=')
      pad++;

    block[count] = tmp;
    count++;
    if (count == 4)
    {
      file.write((block[0] << 2) | (block[1] >> 4));
      count = 0;
      if (pad)
      {
        if (pad == 1)
          file.write((block[1] << 4) | (block[2] >> 2));
        else if (pad > 2)
          return false;
        break;
      }
      else
      {
        file.write((block[1] << 4) | (block[2] >> 2));
        file.write((block[2] << 6) | block[3]);
      }
    }
  }

  delete[] block;
  delete[] dtable;

  return true;
}

FirebaseData::FirebaseData()
{
  _index = -1;
}

FirebaseData ::~FirebaseData()
{
  clear();
}

void FirebaseData::clear()
{
  std::string().swap(_path);
  std::string().swap(_path2);
  std::string().swap(_data);
  std::string().swap(_data2);
  std::string().swap(_streamPath);
  std::string().swap(_pushName);
  std::string().swap(_backupNodePath);
  std::string().swap(_backupDir);
  std::string().swap(_backupFilename);
  std::string().swap(_file_transfer_error);
  std::string().swap(_fileName);
  std::string().swap(_redirectURL);
  std::string().swap(_firebaseError);
  std::string().swap(_eventType);

  for (uint8_t i = 0; i < _qMan._queueCollection.size(); i++)
  {
    QueueItem item = _qMan._queueCollection[i];
    clearQueueItem(item);
  }

  _dataAvailableCallback = NULL;
  _timeoutCallback = NULL;
  _queueInfoCallback = NULL;
  _handle = NULL;
  _q_handle = NULL;
}

void FirebaseData::addQueue(uint8_t FirebaseMethod,
                            uint8_t FirebaseDataType,
                            const std::string path,
                            const std::string filename,
                            const std::string payload,
                            bool isQuery,
                            int *intTarget,
                            float *floatTarget,
                            double *doubleTarget,
                            bool *boolTarget,
                            String *stringTarget,
                            std::vector<uint8_t> *blobTarget)
{
  if (_qMan._queueCollection.size() < _qMan._maxQueue && payload.length() <= _maxBlobSize)
  {
    QueueItem item;
    item.firebaseMethod = FirebaseMethod;
    item.firebaseDataType = FirebaseDataType;
    item.path = path;
    item.filename = filename;
    item.payload = payload;

    if (isQuery)
      item.queryFilter = queryFilter;
    else
      item.queryFilter.clear();

    item.stringPtr = stringTarget;
    item.intPtr = intTarget;
    item.floatPtr = floatTarget;
    item.doublePtr = doubleTarget;
    item.boolPtr = boolTarget;
    item.blobPtr = blobTarget;
    item.qID = random(100000, 200000);
    if (_qMan.add(item))
      _qID = item.qID;
    else
      _qID = 0;
  }
}

void FirebaseData::clearQueueItem(QueueItem &item)
{
  std::string().swap(item.path);
  std::string().swap(item.filename);
  std::string().swap(item.payload);

  item.stringPtr = nullptr;
  item.intPtr = nullptr;
  item.floatPtr = nullptr;
  item.doublePtr = nullptr;
  item.boolPtr = nullptr;
  item.blobPtr = nullptr;
  item.queryFilter.clear();
}

void FirebaseData::setQuery(QueryFilter &query)
{
  queryFilter._orderBy = query._orderBy;
  queryFilter._limitToFirst = query._limitToFirst;
  queryFilter._limitToLast = query._limitToLast;
  queryFilter._startAt = query._startAt;
  queryFilter._endAt = query._endAt;
  queryFilter._equalTo = query._equalTo;
}

bool FirebaseData::pauseFirebase(bool pause)
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  if (http.http_connected() && pause != _pause)
  {

    WiFiClient *netClient = http.http_getStreamPtr();

    if (netClient->available() > 0)
    {
      netClient->flush();
      delay(50);
    }
    netClient->stop();
    delay(50);
    if (!http.http_connected())
    {
      _pause = pause;
      return true;
    }
    return false;
  }
  else
  {
    _pause = pause;
    return true;
  }
}

String FirebaseData::dataType()
{
  std::string res = getDataType(_dataType);
  return res.c_str();
}

String FirebaseData::eventType()
{
  std::string res = _eventType;
  return res.c_str();
}

std::string FirebaseData::getDataType(uint8_t type)
{
  if (type == FirebaseESP32::FirebaseDataType::JSON)
    return ESP32_FIREBASE_STR_74;

  if (type == FirebaseESP32::FirebaseDataType::STRING)
    return ESP32_FIREBASE_STR_75;

  if (type == FirebaseESP32::FirebaseDataType::FLOAT)
    return ESP32_FIREBASE_STR_76;

  if (type == FirebaseESP32::FirebaseDataType::DOUBLE)
    return ESP32_FIREBASE_STR_108;

  if (type == FirebaseESP32::FirebaseDataType::BOOLEAN)
    return ESP32_FIREBASE_STR_105;

  if (type == FirebaseESP32::FirebaseDataType::INTEGER)
    return ESP32_FIREBASE_STR_77;

  if (type == FirebaseESP32::FirebaseDataType::BLOB)
    return ESP32_FIREBASE_STR_91;

  if (type == FirebaseESP32::FirebaseDataType::NULL_)
    return ESP32_FIREBASE_STR_78;

  return std::string();
}

std::string FirebaseData::getMethod(uint8_t method)
{
  if (method == FirebaseESP32::FirebaseMethod::GET)
    return ESP32_FIREBASE_STR_115;

  if (method == FirebaseESP32::FirebaseMethod::PUT || method == FirebaseESP32::FirebaseMethod::PATCH_SILENT)
    return ESP32_FIREBASE_STR_116;

  if (method == FirebaseESP32::FirebaseMethod::POST)
    return ESP32_FIREBASE_STR_117;

  if (method == FirebaseESP32::FirebaseMethod::PATCH || method == FirebaseESP32::FirebaseMethod::PATCH_SILENT)
    return ESP32_FIREBASE_STR_118;

  if (method == FirebaseESP32::FirebaseMethod::DELETE)
    return ESP32_FIREBASE_STR_119;

  return std::string();
}

String FirebaseData::streamPath()
{
  return _streamPath.c_str();
}

String FirebaseData::dataPath()
{
  return _path.c_str();
}

int FirebaseData::intData()
{
  if ((_data.length() > 0 && (_dataType == FirebaseESP32::FirebaseDataType::INTEGER || _dataType == FirebaseESP32::FirebaseDataType::FLOAT || _dataType == FirebaseESP32::FirebaseDataType::DOUBLE)))
    return atoi(_data.c_str());
  else
    return 0;
}

float FirebaseData::floatData()
{
  if (_data.length() > 0 && (_dataType == FirebaseESP32::FirebaseDataType::INTEGER || _dataType == FirebaseESP32::FirebaseDataType::FLOAT || _dataType == FirebaseESP32::FirebaseDataType::DOUBLE))
    return atof(_data.c_str());
  else
    return 0.0;
}

double FirebaseData::doubleData()
{
  if (_data.length() > 0 && (_dataType == FirebaseESP32::FirebaseDataType::INTEGER || _dataType == FirebaseESP32::FirebaseDataType::FLOAT || _dataType == FirebaseESP32::FirebaseDataType::DOUBLE))
    return atof(_data.c_str());
  else
    return 0.0;
}

bool FirebaseData::boolData()
{
  bool res = false;

  if (_data.length() > 0 && _dataType == FirebaseESP32::FirebaseDataType::BOOLEAN)
    res = _data == ESP32_FIREBASE_STR_107;

  return res;
}

String FirebaseData::stringData()
{
  if (_data.length() > 0 && _dataType == FirebaseESP32::FirebaseDataType::STRING)
    return _data.substr(1, _data.length() - 2).c_str();
  else
    return std::string().c_str();
}

String FirebaseData::jsonData()
{
  if (_data.length() > 0 && _dataType == FirebaseESP32::FirebaseDataType::JSON)
    return _data.c_str();
  else
    return std::string().c_str();
}

std::vector<uint8_t> FirebaseData::blobData()
{
  if (_blob.size() > 0 && _dataType == FirebaseESP32::FirebaseDataType::BLOB)
  {
    return _blob;
  }
  else
    return std::vector<uint8_t>();
}

String FirebaseData::pushName()
{
  if (_pushName.length() > 0)
    return _pushName.c_str();
  else
    return std::string().c_str();
}

bool FirebaseData::isStream()
{
  return _isStream;
}

bool FirebaseData::httpConnected()
{
  return _httpConnected;
}

bool FirebaseData::streamTimeout()
{
  return _isStreamTimeout;
}

bool FirebaseData::dataAvailable()
{
  return _dataAvailable;
}

bool FirebaseData::streamAvailable()
{
  return !_httpConnected && _dataAvailable && _streamDataChanged;
}

bool FirebaseData::mismatchDataType()
{
  return _mismatchDataType;
}

size_t FirebaseData::getBackupFileSize()
{
  return _backupzFileSize;
}

String FirebaseData::getBackupFilename()
{
  return _backupFilename.c_str();
}

String FirebaseData::fileTransferError()
{
  return _file_transfer_error.c_str();
}

String FirebaseData::payload()
{
  return _data.c_str();
}

String FirebaseData::errorReason()
{
  std::string buf = "";

  if (_httpCode == HTTP_CODE_OK)
  {
    if (_mismatchDataType)
      _httpCode = FIREBASE_ERROR_DATA_TYPE_MISMATCH;
    else if (_pathNotExist)
      _httpCode = FIREBASE_ERROR_PATH_NOT_EXIST;
  }

  Firebase.errorToString(_httpCode, buf);

  if (_firebaseError != "")
    buf += ", " + _firebaseError;

  return buf.c_str();
}

int FirebaseData::httpCode()
{
  return _httpCode;
}

StreamData::StreamData()
{
}
StreamData::~StreamData()
{
  empty();
}

String StreamData::dataPath()
{
  return _path.c_str();
}

String StreamData::streamPath()
{
  return _streamPath.c_str();
}

int StreamData::intData()
{
  if (_data.length() > 0 && (_dataType == FirebaseESP32::FirebaseDataType::INTEGER || _dataType == FirebaseESP32::FirebaseDataType::FLOAT || _dataType == FirebaseESP32::FirebaseDataType::DOUBLE))
    return atoi(_data.c_str());
  else
    return 0;
}

float StreamData::floatData()
{
  if (_data.length() > 0 && (_dataType == FirebaseESP32::FirebaseDataType::INTEGER || _dataType == FirebaseESP32::FirebaseDataType::FLOAT || _dataType == FirebaseESP32::FirebaseDataType::DOUBLE))
    return atof(_data.c_str());
  else
    return 0;
}

double StreamData::doubleData()
{

  if (_data.length() > 0 && (_dataType == FirebaseESP32::FirebaseDataType::INTEGER || _dataType == FirebaseESP32::FirebaseDataType::FLOAT || _dataType == FirebaseESP32::FirebaseDataType::DOUBLE))
    return atof(_data.c_str());
  else
    return 0.0;
}

bool StreamData::boolData()
{
  bool res = false;

  if (_data.length() > 0 && _dataType == FirebaseESP32::FirebaseDataType::BOOLEAN)
    res = _data == ESP32_FIREBASE_STR_107;

  return res;
}

String StreamData::stringData()
{
  if (_dataType == FirebaseESP32::FirebaseDataType::STRING)
    return _data.c_str();
  else
    return std::string().c_str();
}

String StreamData::jsonData()
{
  if (_dataType == FirebaseESP32::FirebaseDataType::JSON)
    return _data.c_str();
  else
    return std::string().c_str();
}

std::vector<uint8_t> StreamData::blobData()
{
  if (_blob.size() > 0 && _dataType == FirebaseESP32::FirebaseDataType::BLOB)
  {
    return _blob;
  }
  else
    return std::vector<uint8_t>();
}

String StreamData::dataType()
{
  return _dataTypeStr.c_str();
}

String StreamData::eventType()
{
  return _eventTypeStr.c_str();
}

void StreamData::empty()
{
  std::string().swap(_streamPath);
  std::string().swap(_path);
  std::string().swap(_data);
  std::string().swap(_dataTypeStr);
  std::string().swap(_eventTypeStr);
  std::vector<uint8_t>().swap(_blob);
}

QueryFilter::QueryFilter()
{
}

QueryFilter::~QueryFilter()
{
  clear();
}

void QueryFilter::clear()
{

  std::string().swap(_orderBy);
  std::string().swap(_limitToFirst);
  std::string().swap(_limitToLast);
  std::string().swap(_startAt);
  std::string().swap(_endAt);
  std::string().swap(_equalTo);
}

void QueryFilter::orderBy(const String &val)
{
  _orderBy = ESP32_FIREBASE_STR_3;
  _orderBy += val.c_str();
  _orderBy += ESP32_FIREBASE_STR_3;
}
void QueryFilter::limitToFirst(int val)
{
  char *num = new char[20];
  _limitToFirst = itoa(val, num, 10);
  delete[] num;
}

void QueryFilter::limitToLast(int val)
{
  char *num = new char[20];
  _limitToLast = itoa(val, num, 10);
  delete[] num;
}

void QueryFilter::startAt(int val)
{
  char *num = new char[20];
  _startAt = itoa(val, num, 10);
  delete[] num;
}

void QueryFilter::endAt(int val)
{
  char *num = new char[20];
  _endAt = itoa(val, num, 10);
  delete[] num;
}

void QueryFilter::startAt(const String &val)
{
  _startAt = ESP32_FIREBASE_STR_3;
  _startAt += val.c_str();
  _startAt += ESP32_FIREBASE_STR_3;
}

void QueryFilter::endAt(const String &val)
{
  _endAt = ESP32_FIREBASE_STR_3;
  _endAt += val.c_str();
  _endAt += ESP32_FIREBASE_STR_3;
}

void QueryFilter::equalTo(int val)
{
  char *num = new char[20];
  _equalTo = itoa(val, num, 10);
  delete[] num;
}

void QueryFilter::equalTo(const String &val)
{
  _equalTo = ESP32_FIREBASE_STR_3;
  _equalTo += val.c_str();
  _equalTo += ESP32_FIREBASE_STR_3;
}

QueueManager::QueueManager()
{
}
QueueManager::~QueueManager()
{
  clear();
}

void QueueManager::clear()
{
  for (uint8_t i = 0; i < _queueCollection.size(); i++)
  {
    QueueItem item = _queueCollection[i];

    std::string().swap(item.path);
    std::string().swap(item.filename);
    std::string().swap(item.payload);

    item.stringPtr = nullptr;
    item.intPtr = nullptr;
    item.floatPtr = nullptr;
    item.doublePtr = nullptr;
    item.boolPtr = nullptr;
    item.blobPtr = nullptr;
    item.queryFilter.clear();
  }
}

bool QueueManager::add(QueueItem q)
{

  if (_queueCollection.size() < _maxQueue)
  {
    _queueCollection.push_back(q);
    return true;
  }
  return false;
}

void QueueManager::remove(uint8_t index)
{
  _queueCollection.erase(_queueCollection.begin() + index);
}

QueueInfo::QueueInfo()
{
}

QueueInfo::~QueueInfo()
{
  clear();
}

uint8_t QueueInfo::totalQueues()
{
  return _totalQueue;
}

uint32_t QueueInfo::currentQueueID()
{
  return _currentQueueID;
}

bool QueueInfo::isQueueFull()
{
  return _isQueueFull;
}

String QueueInfo::dataType()
{
  return _dataType.c_str();
}

String QueueInfo::firebaseMethod()
{
  return _method.c_str();
}

String QueueInfo::dataPath()
{
  return _path.c_str();
}

void QueueInfo::clear()
{
  std::string().swap(_dataType);
  std::string().swap(_method);
  std::string().swap(_path);
}

FirebaseESP32 Firebase = FirebaseESP32();

#endif
