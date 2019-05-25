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

#ifndef FirebaseESP32_H
#define FirebaseESP32_H

#include <Arduino.h>
#include <WiFi.h>
#include "FS.h"
#include "SPIFFS.h"
#include <HTTPClientESP32Ex.h>
#include <functional>
#include <SD.h>
#include <vector>

#define FIEBASE_PORT 443

#define KEEP_ALIVE_TIMEOUT 30000

#define FIREBASE_ERROR_DATA_TYPE_MISMATCH -14
#define FIREBASE_ERROR_PATH_NOT_EXIST -15
#define HTTPC_ERROR_CONNECTION_INUSED -16

static const char ESP32_FIREBASE_STR_1[] PROGMEM = "/";
static const char ESP32_FIREBASE_STR_2[] PROGMEM = ".json?auth=";
static const char ESP32_FIREBASE_STR_3[] PROGMEM = "\"";
static const char ESP32_FIREBASE_STR_4[] PROGMEM = ".";
static const char ESP32_FIREBASE_STR_5[] PROGMEM = "HTTP/1.1 ";
static const char ESP32_FIREBASE_STR_6[] PROGMEM = " ";
static const char ESP32_FIREBASE_STR_7[] PROGMEM = ":";
static const char ESP32_FIREBASE_STR_8[] PROGMEM = "Content-Type: ";
static const char ESP32_FIREBASE_STR_9[] PROGMEM = "text/event-stream";
static const char ESP32_FIREBASE_STR_10[] PROGMEM = "Connection: ";
static const char ESP32_FIREBASE_STR_11[] PROGMEM = "keep-alive";
static const char ESP32_FIREBASE_STR_12[] PROGMEM = "Content-Length: ";
static const char ESP32_FIREBASE_STR_13[] PROGMEM = "event: ";
static const char ESP32_FIREBASE_STR_14[] PROGMEM = "data: ";
static const char ESP32_FIREBASE_STR_15[] PROGMEM = "put";
static const char ESP32_FIREBASE_STR_16[] PROGMEM = "patch";
static const char ESP32_FIREBASE_STR_17[] PROGMEM = "\"path\":\"";
static const char ESP32_FIREBASE_STR_18[] PROGMEM = "\"data\":";
static const char ESP32_FIREBASE_STR_19[] PROGMEM = "null";
static const char ESP32_FIREBASE_STR_20[] PROGMEM = "{\"name\":\"";
static const char ESP32_FIREBASE_STR_21[] PROGMEM = "\r\n";
static const char ESP32_FIREBASE_STR_22[] PROGMEM = "GET ";
static const char ESP32_FIREBASE_STR_23[] PROGMEM = "PUT";
static const char ESP32_FIREBASE_STR_24[] PROGMEM = "POST";
static const char ESP32_FIREBASE_STR_25[] PROGMEM = "GET";
static const char ESP32_FIREBASE_STR_26[] PROGMEM = "PATCH";
static const char ESP32_FIREBASE_STR_27[] PROGMEM = "DELETE";
static const char ESP32_FIREBASE_STR_28[] PROGMEM = "&download=";
static const char ESP32_FIREBASE_STR_29[] PROGMEM = "&print=silent";
static const char ESP32_FIREBASE_STR_30[] PROGMEM = " HTTP/1.1\r\n";
static const char ESP32_FIREBASE_STR_31[] PROGMEM = "Host: ";
static const char ESP32_FIREBASE_STR_32[] PROGMEM = "User-Agent: ESP32\r\n";
static const char ESP32_FIREBASE_STR_33[] PROGMEM = "X-Firebase-Decoding: 1\r\n";
static const char ESP32_FIREBASE_STR_34[] PROGMEM = "Connection: close\r\n";
static const char ESP32_FIREBASE_STR_35[] PROGMEM = "Accept: text/event-stream\r\n";
static const char ESP32_FIREBASE_STR_36[] PROGMEM = "Connection: keep-alive\r\n";
static const char ESP32_FIREBASE_STR_37[] PROGMEM = "Keep-Alive: timeout=30, max=100\r\n";
static const char ESP32_FIREBASE_STR_38[] PROGMEM = "Accept-Encoding: identity;q=1,chunked;q=0.1,*;q=0\r\n";
static const char ESP32_FIREBASE_STR_39[] PROGMEM = "connection refused";
static const char ESP32_FIREBASE_STR_40[] PROGMEM = "send header failed";
static const char ESP32_FIREBASE_STR_41[] PROGMEM = "send payload failed";
static const char ESP32_FIREBASE_STR_42[] PROGMEM = "not connected";
static const char ESP32_FIREBASE_STR_43[] PROGMEM = "connection lost";
static const char ESP32_FIREBASE_STR_44[] PROGMEM = "no HTTP server";
static const char ESP32_FIREBASE_STR_45[] PROGMEM = "bad request";
static const char ESP32_FIREBASE_STR_46[] PROGMEM = "non-authoriative information";
static const char ESP32_FIREBASE_STR_47[] PROGMEM = "no content";
static const char ESP32_FIREBASE_STR_48[] PROGMEM = "moved permanently";
static const char ESP32_FIREBASE_STR_49[] PROGMEM = "use proxy";
static const char ESP32_FIREBASE_STR_50[] PROGMEM = "temporary redirect";
static const char ESP32_FIREBASE_STR_51[] PROGMEM = "permanent redirect";
static const char ESP32_FIREBASE_STR_52[] PROGMEM = "unauthorized";
static const char ESP32_FIREBASE_STR_53[] PROGMEM = "forbidden";
static const char ESP32_FIREBASE_STR_54[] PROGMEM = "not found";
static const char ESP32_FIREBASE_STR_55[] PROGMEM = "method not allow";
static const char ESP32_FIREBASE_STR_56[] PROGMEM = "not acceptable";
static const char ESP32_FIREBASE_STR_57[] PROGMEM = "proxy authentication required";
static const char ESP32_FIREBASE_STR_58[] PROGMEM = "request timeout";
static const char ESP32_FIREBASE_STR_59[] PROGMEM = "length required";
static const char ESP32_FIREBASE_STR_60[] PROGMEM = "too many requests";
static const char ESP32_FIREBASE_STR_61[] PROGMEM = "request header fields too larg";
static const char ESP32_FIREBASE_STR_62[] PROGMEM = "internal server error";
static const char ESP32_FIREBASE_STR_63[] PROGMEM = "bad gateway";
static const char ESP32_FIREBASE_STR_64[] PROGMEM = "service unavailable";
static const char ESP32_FIREBASE_STR_65[] PROGMEM = "gateway timeout";
static const char ESP32_FIREBASE_STR_66[] PROGMEM = "http version not support";
static const char ESP32_FIREBASE_STR_67[] PROGMEM = "network authentication required";
static const char ESP32_FIREBASE_STR_68[] PROGMEM = "data buffer overflow";
static const char ESP32_FIREBASE_STR_69[] PROGMEM = "read Timeout";
static const char ESP32_FIREBASE_STR_70[] PROGMEM = "data type mismatch";
static const char ESP32_FIREBASE_STR_71[] PROGMEM = "path not exist";
static const char ESP32_FIREBASE_STR_72[] PROGMEM = "task";
static const char ESP32_FIREBASE_STR_73[] PROGMEM = "/esp.32";
static const char ESP32_FIREBASE_STR_74[] PROGMEM = "json";
static const char ESP32_FIREBASE_STR_75[] PROGMEM = "string";
static const char ESP32_FIREBASE_STR_76[] PROGMEM = "float";
static const char ESP32_FIREBASE_STR_77[] PROGMEM = "int";
static const char ESP32_FIREBASE_STR_78[] PROGMEM = "null";
static const char ESP32_FIREBASE_STR_79[] PROGMEM = ";";
static const char ESP32_FIREBASE_STR_80[] PROGMEM = "Content-Disposition: ";
static const char ESP32_FIREBASE_STR_81[] PROGMEM = "application/octet-stream";
static const char ESP32_FIREBASE_STR_82[] PROGMEM = "attachment";
static const char ESP32_FIREBASE_STR_83[] PROGMEM = "Backup file was not exist";
static const char ESP32_FIREBASE_STR_84[] PROGMEM = "SD card is in used";
static const char ESP32_FIREBASE_STR_85[] PROGMEM = "SD card was not available";
static const char ESP32_FIREBASE_STR_86[] PROGMEM = "Could not read/write backup file";
static const char ESP32_FIREBASE_STR_87[] PROGMEM = "Transmission error, ";
static const char ESP32_FIREBASE_STR_88[] PROGMEM = "Node path is not exist";
static const char ESP32_FIREBASE_STR_89[] PROGMEM = ".json";
static const char ESP32_FIREBASE_STR_90[] PROGMEM = "/root.json";
static const char ESP32_FIREBASE_STR_91[] PROGMEM = "blob";
static const char ESP32_FIREBASE_STR_92[] PROGMEM = "\"blob,base64,";
static const char ESP32_FIREBASE_STR_93[] PROGMEM = "\"file,base64,";
static const char ESP32_FIREBASE_STR_94[] PROGMEM = "http connection was used by other processes";
static const char ESP32_FIREBASE_STR_95[] PROGMEM = "Location: ";
static const char ESP32_FIREBASE_STR_96[] PROGMEM = "&orderBy=";
static const char ESP32_FIREBASE_STR_97[] PROGMEM = "&limitToFirst=";
static const char ESP32_FIREBASE_STR_98[] PROGMEM = "&limitToLast=";
static const char ESP32_FIREBASE_STR_99[] PROGMEM = "&startAt=";
static const char ESP32_FIREBASE_STR_100[] PROGMEM = "&endAt=";
static const char ESP32_FIREBASE_STR_101[] PROGMEM = "&equalTo=";
static const char ESP32_FIREBASE_STR_102[] PROGMEM = "\"error\" : ";
static const char ESP32_FIREBASE_STR_103[] PROGMEM = "/.settings/rules";
static const char ESP32_FIREBASE_STR_104[] PROGMEM = "{\"status\":\"ok\"}";
static const char ESP32_FIREBASE_STR_105[] PROGMEM = "boolean";
static const char ESP32_FIREBASE_STR_106[] PROGMEM = "false";
static const char ESP32_FIREBASE_STR_107[] PROGMEM = "true";
static const char ESP32_FIREBASE_STR_108[] PROGMEM = "double";
static const char ESP32_FIREBASE_STR_109[] PROGMEM = "cancel";
static const char ESP32_FIREBASE_STR_110[] PROGMEM = "auth_revoked";
static const char ESP32_FIREBASE_STR_111[] PROGMEM = "http://";
static const char ESP32_FIREBASE_STR_112[] PROGMEM = "https://";
static const char ESP32_FIREBASE_STR_113[] PROGMEM = "_stream";
static const char ESP32_FIREBASE_STR_114[] PROGMEM = "_error_queue";
static const char ESP32_FIREBASE_STR_115[] PROGMEM = "get";
static const char ESP32_FIREBASE_STR_116[] PROGMEM = "set";
static const char ESP32_FIREBASE_STR_117[] PROGMEM = "push";
static const char ESP32_FIREBASE_STR_118[] PROGMEM = "update";
static const char ESP32_FIREBASE_STR_119[] PROGMEM = "delete";

static const unsigned char ESP32_FIREBASE_base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class FirebaseData;
class StreamData;
class QueueInfo;
class FirebaseESP32;

struct QueueStorageType
{
  static const uint8_t SPIFFS = 0;
  static const uint8_t SD = 1;
};

typedef void (*StreamEventCallback)(StreamData);
typedef void (*StreamTimeoutCallback)(bool);
typedef void (*QueueInfoCallback)(QueueInfo);

static std::vector<std::reference_wrapper<FirebaseData>> firebaseDataObject;
static uint8_t dataObjectIndex = 0;
static uint8_t streamIndex = 0;
static uint8_t errorQueueIndex = 0;

class QueryFilter
{
public:
  QueryFilter();
  ~QueryFilter();
  void orderBy(const String &);
  void limitToFirst(int);
  void limitToLast(int);
  void startAt(int);
  void endAt(int);
  void startAt(const String &);
  void endAt(const String &);
  void equalTo(int);
  void equalTo(const String &);
  void clear();
  friend FirebaseESP32;
  friend FirebaseData;

protected:
  std::string _orderBy = "";
  std::string _limitToFirst = "";
  std::string _limitToLast = "";
  std::string _startAt = "";
  std::string _endAt = "";
  std::string _equalTo = "";
};

class QueueInfo
{
public:
  QueueInfo();
  ~QueueInfo();
  uint8_t totalQueues();
  uint32_t currentQueueID();
  bool isQueueFull();
  String dataType();
  String firebaseMethod();
  String dataPath();

  friend FirebaseESP32;

protected:
  void clear();
  uint8_t _totalQueue = 0;
  uint32_t _currentQueueID = 0;
  bool _isQueueFull = false;
  bool _isQueue = false;
  std::string _dataType = "";
  std::string _method = "";
  std::string _path = "";
};

struct QueueItem
{
  uint8_t firebaseDataType = 0;
  uint8_t firebaseMethod = 0;
  uint32_t qID = 0;
  uint32_t timestamp = 0;
  uint8_t runCount = 0;
  uint8_t runIndex = 0;

  std::string path = "";
  std::string payload = "";
  std::vector<uint8_t> blob = std::vector<uint8_t>();
  std::string filename = "";
  QueryFilter queryFilter;
  int *intPtr = nullptr;
  float *floatPtr = nullptr;
  double *doublePtr = nullptr;
  bool *boolPtr = nullptr;
  String *stringPtr = nullptr;
  std::vector<uint8_t> *blobPtr = nullptr;
};

class QueueManager
{
public:
  QueueManager();
  ~QueueManager();

  bool add(QueueItem q);
  void remove(uint8_t index);

  friend FirebaseESP32;
  friend FirebaseData;

private:
  void clear();
  std::vector<QueueItem> _queueCollection = std::vector<QueueItem>();
  uint8_t _maxQueue = 10;
};

class FirebaseESP32
{
public:
  struct FirebaseDataType;
  struct FirebaseMethod;

  FirebaseESP32();
  ~FirebaseESP32();

  /*
    Store Firebase's authentication credentials.
    
    @param host - Your Firebase database project host without http:// or https:// protocol e.g. Your_ProjectID.firebaseio.com.
    @param auth - Your database secret.

   */
  void begin(const String &host, const String &auth);

  /*
    Store Firebase's authentication credentials.
    
    @param host - Your Firebase database project host without http:// or https:// protocol e.g. Your_ProjectID.firebaseio.com.
    @param auth - Your database secret.
    @param rootCA - Base64 encoded root certificate string.

   */
  void begin(const String &host, const String &auth, const char *rootCA);

  /*
    Stop Firebase and release all resources.
    
    @param dataObj - Firebase Data Object to hold data and instances.

   */
  void end(FirebaseData &dataObj);

  /*
    Reconnect WiFi if lost connection.
    
    @param reconnect - The boolean to set/unset WiFi AP reconnection.

   */
  void reconnectWiFi(bool reconnect);

  /*
    Read the database rules.
    
    @param dataObj - Firebase Data Object to hold data and instances.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].jsonData will return the JSON string value of
    database rules returned from server.

   */
  bool getRules(FirebaseData &dataObj);

  /*
    Write the database rules.
    
    @param dataObj - Firebase Data Object to hold data and instances.
    @param rules - Database rules in jSON String format.

    @return - Boolean type status indicates the success of operation.

   */
  bool setRules(FirebaseData &dataObj, const String &rules);

  /*
    Determine whether defined database path is existed or not.
    
    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path to be checked.

    @return - Boolean type result indicates whether the defined database
    path was existed or not.

   */
  bool pathExist(FirebaseData &dataObj, const String &path);

  /*
    Append new integer value to the defined database path. 
    
    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which integer value will be appended.
    @param intValue - The appended value.
    
    @return - Boolean type status indicates the success of operation.
    
    The new appended node's key will be stored in Firebase Data object, 
    which its value can be accessed via function [FirebaseData object].pushName().

   */
  bool pushInt(FirebaseData &dataObj, const String &path, int intValue);

  /*
    Append new float value to the defined database path.
    
    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which float value will be appended.
    @param floatValue - The appended value.
    
    @return - Boolean type status indicates the success of operation.
    
    The new appended node's key will be stored in Firebase Data object, 
    which its value can be accessed via function [FirebaseData object].pushName().

   */
  bool pushFloat(FirebaseData &dataObj, const String &path, float floatValue);

  /*
    Append new double value (8 bytes) to the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which float value will be appended.
    @param doubleValue - The appended value.

    @return - Boolean type status indicates the success of operation.

    The new appended node's key will be stored in Firebase Data object,
    which its value can be accessed via function [FirebaseData object].pushName().

  */
  bool pushDouble(FirebaseData &dataObj, const String &path, double doubleValue);

  /*
    Append new Boolean value to the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which Boolean value will be appended.
    @param boolValue - The appended value.

    @return - Boolean type status indicates the success of operation.

    The new appended node's key will be stored in Firebase Data object,
    which its value can be accessed via function [FirebaseData object].pushName().

  */
  bool pushBool(FirebaseData &dataObj, const String &path, bool boolValue);

  /*
    Append new string (text) to the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which string will be appended.
    @param StringValue - The appended value.

    @return - Boolean type status indicates the success of operation.
    
    The new appended node's key stored in Firebase Data object, 
    which can be accessed via function [FirebaseData object].pushName().

   */
  bool pushString(FirebaseData &dataObj, const String &path, const String &stringValue);

  /*
    Append new child nodes's key and value (using JSON data) to the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which key and value in JSON data will be appended.
    @param jsonString - The appended JSON string (should be valid JSON data).

    @return - Boolean type status indicates the success of operation.
    
    The new appended node's key will be stored in Firebase Data object, 
    which its value can be accessed via function [FirebaseData object].pushName().

   */
  bool pushJSON(FirebaseData &dataObj, const String &path, const String &jsonString);

  /*
    Append new blob (binary data) to the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which binary data will be appended.
    @param blob - Byte array of data.
    @param size - Size of byte array.

    @return - Boolean type status indicates the success of operation.
    
    The new appended node's key will be stored in Firebase Data object, 
    which its value can be accessed via function [FirebaseData object].pushName().

   */
  bool pushBlob(FirebaseData &dataObj, const String &path, uint8_t *blob, size_t size);

  /*
    Append new binary data from file store on SD card to the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which binary data from file will be appended.
    @param fileName - File name (full file path) in SD card.

    @return - Boolean type status indicates the success of operation.
    
    The new appended node's key will be stored in Firebase Data object, 
    which its value can be accessed via function [FirebaseData object].pushName().

   */
  bool pushFile(FirebaseData &dataObj, const String &path, const String &fileName);

  /*
    Set integer data at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which integer data will be set.
    @param intValue - Integer value to set.

    @return - Boolean type status indicates the success of operation.
    
    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].intData will return the integer value of
    payload returned from server.

   */
  bool setInt(FirebaseData &dataObj, const String &path, int intValue);

  /*
    Set float data at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which float data will be set.
    @param floatValue - Float value to set.

    @return - Boolean type status indicates the success of operation.
    
    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].floatData will return the float value of
    payload returned from server.

   */
  bool setFloat(FirebaseData &dataObj, const String &path, float floatValue);

  /*
    Set double data at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which float data will be set.
    @param doubleValue - Double value to set.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database.

    Call [FirebaseData object].doubleData will return the double value of
    payload returned from server.

  */
  bool setDouble(FirebaseData &dataObj, const String &path, double doubleValue);

  /*
    Set Boolean data at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which Boolean data will be set.
    @param boolValue - Boolean value to set.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database.

    Call [FirebaseData object].boolData will return the Boolean value of
    payload returned from server.

  */
  bool setBool(FirebaseData &dataObj, const String &path, bool boolValue);

  /*
    Set string (text) at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which string data will be set.
    @param stringValue - String or text to set.

    @return - Boolean type status indicates the success of operation.
    
    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].stringData will return the string value of
    payload returned from server.

   */
  bool setString(FirebaseData &dataObj, const String &path, const String &stringValue);

  /*

    Set child nodes's key and value (using JSON data) to the defined database path.

    This will replace any child nodes inside the defined path with node' s key
    and value defined in JSON data.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which key and value in JSON data will be replaced or set.
    @param jsonString - The JSON string to set (should be valid JSON data).

    @return - Boolean type status indicates the success of operation.
    
    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].jsonData will return the JSON string value of
    payload returned from server.

   */
  bool setJSON(FirebaseData &dataObj, const String &path, const String &jsonString);

  /*
    Set blob (binary data) at the defined database path.

    This will replace any child nodes inside the defined path with blob or binary data.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which binary data will be set.
    @param blob - Byte array of data.
    @param size - Size of byte array.

    @return - Boolean type status indicates the success of operation.
    
    No payload returned from server.

   */
  bool setBlob(FirebaseData &dataObj, const String &path, uint8_t *blob, size_t size);

  /*
    Set binary data from file store on SD card to the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which binary data from file will be set.
    @param fileName - File name (full file path) in SD card.

    @return - Boolean type status indicates the success of operation.
    
    No payload returned from server.

   */
  bool setFile(FirebaseData &dataObj, const String &path, const String &fileName);

  /*
    Update child nodes's key or exising key's value (using JSON data) under the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which key and value in JSON data will be update.
    @param jsonString - The JSON string use for update.

    @return - Boolean type status indicates the success of operation.
    
    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].jsonData will return the json string value of
    payload returned from server.

    To reduce the network data usage, use updateNodeSilent instead.

   */
  bool updateNode(FirebaseData &dataObj, const String &path, const String &jsonString);

  /*
    Update child nodes's key or exising key's value (using JSON data) under the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Target database path which key and value in JSON data will be update.
    @param jsonString - The JSON string use for update.

    @return - Boolean type status indicates the success of operation.
    
    Owing to the objective of this function to reduce the netwok data usage, 
    no payload will be returned from server.

   */
  bool updateNodeSilent(FirebaseData &dataObj, const String &path, const String &jsonString);

  /*
    Read the integer value at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the integer value is being read.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].intData will return the integer value of
    payload returned from server.

    If the payload returned from server is not integer or float type, 
    the function [FirebaseData object].intData will return zero (0).

    If the payload returned from server is float type, 
    the function [FirebaseData object].intData will return rounded integer value.

   */
  bool getInt(FirebaseData &dataObj, const String &path);

  /*
    Read the integer value at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the float value is being read.
    @param target - The integer type variable to store value.

    @return - Boolean type status indicates the success of operation.

    If the type of payload returned from server is not integer, float and double, 
    the target variable's value will be zero (0).

  */
  bool getInt(FirebaseData &dataObj, const String &path, int &target);

  /*
    Read the float value at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the float value is being read.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].floatData will return the float value of
    payload returned from server.

    If the payload returned from server is not integer or float type, 
    the function [FirebaseData object].intData will return zero (0).

   */
  bool getFloat(FirebaseData &dataObj, const String &path);

  /*
    Read the float value at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the float value is being read.
    @param target - The float type variable to store value.

    @return - Boolean type status indicates the success of operation.


    If the type of payload returned from server is not integer, float and double, 
    the target variable's value will be zero (0).

   */
  bool getFloat(FirebaseData &dataObj, const String &path, float &target);

  /*
    Read the double value at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the float value is being read.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].doubleData will return the double value of
    payload returned from server.

    If the payload returned from server is not integer, float and double, 
    the function [FirebaseData object].doubleData will return zero (0).

   */
  bool getDouble(FirebaseData &dataObj, const String &path);

  /*
    Read the float value at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the float value is being read.
    @param target - The double type variable to store value.

    @return - Boolean type status indicates the success of operation.


    If the type of payload returned from server is not integer, float and double, 
    the target variable's value will be zero (0).

   */
  bool getDouble(FirebaseData &dataObj, const String &path, double &target);

  /*
    Read the Boolean value at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the Boolean value is being read.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].boolData will return the Boolean value of
    payload returned from server.

    If the payload returned from server is not Boolean type, 
    the function [FirebaseData object].boolData will return false.

   */
  bool getBool(FirebaseData &dataObj, const String &path);

  /*
    Read the Boolean value at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the Boolean value is being read.
    @param target - The boolean type variable to store value.

    @return - Boolean type status indicates the success of operation.


    If the type of payload returned from server is not Boolean, 
    the target variable's value will be false.

   */
  bool getBool(FirebaseData &dataObj, const String &path, bool &target);

  /*
    Read the string or text at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the string value is being read.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].stringData will return the string value of
    payload returned from server.

    If the payload returned from server is not string type, 
    the function [FirebaseData object].stringData will return empty string (String object).

   */
  bool getString(FirebaseData &dataObj, const String &path);

  /*
    Read the string or text at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the string value is being read.
    @param target - The String object to store value.

    @return - Boolean type status indicates the success of operation.

    If the type of payload returned from server is not string,
    the target String object's value will be empty.

  */
  bool getString(FirebaseData &dataObj, const String &path, String &target);

  /*
    Read the JSON string at the defined database path.
    The returned payload JSON string represents the child nodes and their value.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the JSON string value is being read.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].jsonData will return the JSON string value of
    payload returned from server.

    If the payload returned from server is not json type, 
    the function [FirebaseData object].jsonData will return empty string (String object).

   */
  bool getJSON(FirebaseData &dataObj, const String &path);

  /*
    Read the JSON string at the defined database path.
    The returned payload JSON string represents the child nodes and their value.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the JSON string value is being read.
    @param target - The String object to store JSON string.

    @return - Boolean type status indicates the success of operation.

    If the type of payload returned from server is not string,
    the target String object's value will be empty.

  */
  bool getJSON(FirebaseData &dataObj, const String &path, String &target);

  /*
    Read the JSON string at the defined database path.
    The returned payload JSON string represents the child nodes and their value.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the JSON string value is being read.
    @param query - QueryFilter class to set query parameters to filter data.

    @return - Boolean type status indicates the success of operation.

    Available query parameters for filtering the data are the following.

    QueryFilter.orderBy -       Required parameter to specify which data used for data filtering included child key, key and value.
                                Use "$key" for filtering data by keys of all nodes at the defined database path.
                                Use "$value" for filtering data by value of all nodes at the defined database path.
                                Use "$priority" for filtering data by "virtual child" named .priority of all nodes.
                                Use  any child key to filter by that key.


    QueryFilter.limitToFirst -  The total children (number) to filter from the first child.
    QueryFilter.limitToLast -   The total last children (number) to filter. 
    QueryFilter.startAt -       Starting value of range (number or string) of query upon orderBy param.
    QueryFilter.endAt -         Ending value of range (number or string) of query upon orderBy param.
    QueryFilter.equalTo -       Value (number or string) matches the orderBy param

    
    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].jsonData will return the JSON string value of
    payload returned from server.

    If the payload returned from server is not json type, 
    the function [FirebaseData object].jsonData will return empty string (String object).

    [FirebaseData object].jsonData will return null when the filtered data is empty.

   */
  bool getJSON(FirebaseData &dataObj, const String &path, QueryFilter &query);

  /*
     Read the JSON string at the defined database path as above

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the JSON string value is being read.
    @param target - The String object to store JSON string.

    @return - Boolean type status indicates the success of operation.

    If the type of payload returned from server is not json,
    the target String object's value will be empty.

  */
  bool getJSON(FirebaseData &dataObj, const String &path, QueryFilter &query, String &target);

  /*
    Read the blob (binary data) at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the binary data is being read.

    @return - Boolean type status indicates the success of operation.

    Call [FirebaseData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [FirebaseData object].blobData will return the dynamic array of unsigned 8-bit data (i.e. std::vector<uint8_t>) of
    payload returned from server.

    If the payload returned from server is not blob type, 
    the function [FirebaseData object].blobData will return empty array.

   */
  bool getBlob(FirebaseData &dataObj, const String &path);

  /*
    Read the blob (binary data) at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path which the binary data is being read.
    @param target - Dynamic array of unsigned 8-bit data (i.e. std::vector<uint8_t>) to store value.

    @return - Boolean type status indicates the success of operation.


    If the type of payload returned from server is not blob,
    the target variable value will be empty array.

  */
  bool getBlob(FirebaseData &dataObj, const String &path, std::vector<uint8_t> &target);

  /*
    Download file data in database at defined database path and save to SD card.

    The downloaded data will be decoded to binary and save to SD card, then
    please make sure that data at the defined database path is file type.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param nodePath - Database path that file data will be downloaded.
    @param fileName - File name (full path) to save in SD card.

    @return Boolean type status indicates the success of operation.

   */
  bool getFile(FirebaseData &dataObj, const String &nodePath, const String &fileName);

  /*
    Delete all child nodes at the defined database path.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path to be deleted.

    @return - Boolean type status indicates the success of operation.

   */
  bool deleteNode(FirebaseData &dataObj, const String &path);

  /*
    Start monitoring the value changes at the defined path and its children.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param path - Database path being monitor.

    @return - Boolean type status indicates the success of operation.

   */
  bool beginStream(FirebaseData &dataObj, const String &path);

  /*
    Read the stream event data at defined database path. 

    Once beginStream was called e.g. in setup(), the readStream function
    should call inside the loop function.

    @param dataObj - Firebase Data Object to hold data and instances.

    @return - Boolean type status indicates the success of operation.

    Using the same Firebase Data object for stream read/monitoring associated 
    with getXXX, setXXX, pushXXX, updateNode and deleteNode will break or quit 
    the current stream connection. 
    
    The stream will be resumed or reconnected automatically when calling readStream.

   */
  bool readStream(FirebaseData &dataObj);

  /*
    End the stream connection at defined path.
    
    Can be restart again by calling beginStream.

    @param dataObj - Firebase Data Object to hold data and instances.

    @return - Boolean type status indicates the success of operation.

   */
  bool endStream(FirebaseData &dataObj);

  /*
    Set the stream callback functions.

    setStreamCallback should be called before Firebase.beginStream.
    
    @param dataObj - Firebase Data Object to hold data and instances.
    @param dataAvailablecallback - Callback function that accepts streamData parameter.
    @param timeoutCallback - Callback function will be called when stream connection was timeout (optional).

    dataAvailablecallback will be called When data in the defined path changed or the stream path changed or stream connection
    was resumed from getXXX, setXXX, pushXXX, updateNode, deleteNode.

    The payload returned from server will be one of these integer, float, string, json and blob types.

    Call [streamData object].dataType to determine what type of data that successfully
    stores in database. 
    
    Call [streamData object].xxxData will return the appropiated data type of
    payload returned from server.

   */
  void setStreamCallback(FirebaseData &dataObj, StreamEventCallback dataAvailablecallback, StreamTimeoutCallback timeoutCallback = NULL);

  /*
    Remove stream callback functions.

    @param dataObj - Firebase Data Object to hold data and instances.
   */
  void removeStreamCallback(FirebaseData &dataObj);

  /*
    Start the Firbase Error Queues Auto Run Process.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param callback - Callback function that accepts QueueInfo Object as parameter, optional.

    The following functions are available from QueueInfo Object accepted by callback.

    queueInfo.totalQueues(), get the total Error Queues in Error Queue Collection.

    queueInfo.currentQueueID(), get current Error Queue ID that being process.

    queueInfo.isQueueFull(), determine whether Error Queue Collection is full or not.

    queueInfo.dataType(), get string of the Frebase call data type that being process of current Error Queue.

    queueInfo.method(), get string of the Firebase call method that being process of current Error Queue.

    queueInfo.path(), get string of the Firebase call path that being process of current Error Queue.

   */
  void beginAutoRunErrorQueue(FirebaseData &dataObj, QueueInfoCallback callback = NULL);

  /*
    Stop the Firbase Error Queues Auto Run Process.

    @param dataObj - Firebase Data Object to hold data and instances.

  */
  void endAutoRunErrorQueue(FirebaseData &dataObj);

  /*
    Clear all Firbase Error Queues in Error Queue collection.

    @param dataObj - Firebase Data Object to hold data and instances.

  */
  void clearErrorQueue(FirebaseData &dataObj);

  /*
    Backup (download) database at defined database path to SD card.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param nodePath - Database path to be backuped.
    @param dirPath - Folder in SD card to save the downloaed file.

    @return Boolean type status indicates the success of operation.

    The backup .json filename is constructed from the database path by replace slash (/) with dot (.).

   */
  bool backup(FirebaseData &dataObj, const String &nodePath, const String &dirPath);

  /*
    Restore database at defined path usin backup file saved on SD card.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param nodePath - Database path to  be restored.
    @param dirPath - Path/Folder in SD card that the backup file was saved. 


    @return Boolean type status indicates the success of operation.
    
   */
  bool restore(FirebaseData &dataObj, const String &nodePath, const String &dirPath);

  /*
  
    Init SD card with GPIO pins.
  
    @param sck -  SPI Clock pin.
    @param miso - SPI MISO pin.
    @param mosi - SPI MOSI pin.
    @param ss -   SPI Chip/Slave Select pin.

    @return Boolean type status indicates the success of operation.

   */
  bool sdBegin(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss);

  /*
  
    Init SD card with default GPIO pins.

    @return Boolean type status indicates the success of operation.
  
   */
  bool sdBegin(void);

  /*

    Set maximum Firebase read/store retry operation (0 - 255) in case of network problems and buffer overflow.
    @param dataObj - Firebase Data Object to hold data and instances.
    @param num - The maximum retry.

  */
  void setMaxRetry(FirebaseData &dataObj, uint8_t num);

  /*
   
   Set the maximum Firebase Error Queues in collection (0 - 255).

   Firebase read/store operation causes by network problems and buffer overflow will be added to Firebase Error Queues collection.

   @param dataObj - Firebase Data Object to hold data and instances.
   @param num - The maximum Firebase Error Queues.

  */
  void setMaxErrorQueue(FirebaseData &dataObj, uint8_t num);

  /*
   
   Save Firebase Error Queues as SPIFFS file (save only database store queues).

   Firebase read (get) operation will not be saved.

   @param dataObj - Firebase Data Object to hold data and instances.
   @param filename - File name to be saved.
   @param storageType - Type of storage to save file, QueueStorageType::SPIFS or QueueStorageType::SD.
    
  */
  bool saveErrorQueue(FirebaseData &dataObj, const String &filename, uint8_t storageType);

  /*
   
   Delete file in Flash (SPIFFS) or SD card.

   @param filename - File name to delete.
   @param storageType - Type of storage to save file, QueueStorageType::SPIFS or QueueStorageType::SD.
    
  */
  bool deleteStorageFile(const String &filename, uint8_t storageType);

  /*
   
   Restore Firebase Error Queues from SPIFFS file.

   @param dataObj - Firebase Data Object to hold data and instances.
   @param filename - File name to be read and restore queues.
   @param storageType - Type of storage to read file, QueueStorageType::SPIFS or QueueStorageType::SD.
    
  */
  bool restoreErrorQueue(FirebaseData &dataObj, const String &filename, uint8_t storageType);

  /*
    Determine number of Firebase Error Queues stored in defined SPIFFS file.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param filename - File name to be read and count for queues.
    @param storageType - Type of storage to read file, QueueStorageType::SPIFS or QueueStorageType::SD.

    @return Number (0-255) of queues store in defined SPIFFS file.

  */
  uint8_t errorQueueCount(FirebaseData &dataObj, const String &filename, uint8_t storageType);

  /*
    Determine number of queues in Firebase Data object Firebase Error Queues collection.

    @param dataObj - Firebase Data Object to hold data and instances.
    @return Number (0-255) of queues in Firebase Data object queue collection.

  */
  uint8_t errorQueueCount(FirebaseData &dataObj);

  /*
    Determine whether the Firebase Error Queues collection was full or not.

    @param dataObj - Firebase Data Object to hold data and instances.

    @return Boolean type status indicates whether the  Firebase Error Queues collection was full or not.

  */
  bool isErrorQueueFull(FirebaseData &dataObj);

  /*
    Pocess all failed Firebase operation queue items when network is available.

    @param dataObj - Firebase Data Object to hold data and instances.

    @param callback - Callback function that accepts QueueInfo parameter.
  
  */
  void processErrorQueue(FirebaseData &dataObj, QueueInfoCallback callback = NULL);

  /*
    Return Firebase Error Queue ID of last Firebase Error.

    Return 0 if there is no Firebase Error from last operation.

    @param dataObj - Firebase Data Object to hold data and instances.
    
    @return Number of Queue ID.

   */
  uint32_t getErrorQueueID(FirebaseData &dataObj);

  /*
    Determine whether Firebase Error Queue is currently exsted is Error Queue collection or not.

    @param dataObj - Firebase Data Object to hold data and instances.
    @param errorQueueID - The Firebase Error Queue ID get from getErrorQueueID.
    
   @return - Boolean type status indicates the queue existence.

   */
  bool isErrorQueueExisted(FirebaseData &dataObj, uint32_t errorQueueID);

  void errorToString(int httpCode, std::string &buf);

  friend FirebaseData;

protected:
  bool buildRequest(FirebaseData &dataObj, uint8_t firebaseMethod, uint8_t firebaseDataType, const String &path, const char *buf, bool queue);
  bool buildRequestFile(FirebaseData &dataObj, uint8_t firebaseMethod, const String &path, const String &fileName, bool queue);
  bool sendRequest(FirebaseData &dataObj, const std::string &path, const uint8_t _method, uint8_t dataType, const std::string &payload);
  bool firebaseConnectStream(FirebaseData &dataObj, const std::string &path);
  bool getServerStreamResponse(FirebaseData &dataObj);
  bool getServerResponse(FirebaseData &dataObj);
  bool getDownloadResponse(FirebaseData &dataObj);
  bool getUploadResponse(FirebaseData &dataObj);
  void endFileTransfer(FirebaseData &dataObj);

  void buildFirebaseRequest(FirebaseData &dataObj, const std::string &host, uint8_t _method, const std::string &path, const std::string &auth, int payloadLength, std::string &request);
  void resetFirebasedataFlag(FirebaseData &dataObj);
  bool handleNetNotConnected(FirebaseData &dataObj);
  void forceEndHTTP(FirebaseData &dataObj);
  bool apConnected(FirebaseData &dataObj);
  int firebaseConnect(FirebaseData &dataObj, const std::string &path, const uint8_t _method, uint8_t dataType, const std::string &payload);
  bool cancelCurrentResponse(FirebaseData &dataObj);
  void setDataType(FirebaseData &dataObj, const std::string &data);
  bool commError(FirebaseData &dataObj);
  uint8_t openErrorQueue(FirebaseData &dataObj, const String &filename, uint8_t storageType, uint8_t mode);
  std::vector<std::string> splitString(int size, const char *str, const char delim);

  bool sdTest();
  void createDirs(std::string dirs);
  bool replace(std::string &str, const std::string &from, const std::string &to);
  std::string base64_encode_string(const unsigned char *src, size_t len);
  void send_base64_encode_file(WiFiClient *tcp, const std::string &filePath);
  bool base64_decode_string(const std::string src, std::vector<uint8_t> &out);
  bool base64_decode_file(File &file, const char *src, size_t len);

  std::string _host = "";
  std::string _auth = "";
  std::vector<const char *> _rootCA = std::vector<const char *>();

  uint16_t _port = 443;
  bool _reconnectWiFi = false;
  bool _sdOk = false;
  bool _sdInUse = false;
  bool _sdConfigSet = false;
  uint8_t _sck, _miso, _mosi, _ss;
  File file;
};

class FirebaseData
{

public:
  FirebaseData();
  ~FirebaseData();

  /*
    Pause/Unpause WiFiClient from all Firebase operations.
    
    @param pause The boolean to set/unset pause operation.

    @return - Boolean type status indicates the success of operation.
   */
  bool pauseFirebase(bool pause);

  /*
    Determine the data type of payload returned from server.

    @return The one of these data type e.g. integer, float, string, json and blob.

   */
  String dataType();

  /*
    Determine the event type of stream.

    @return The one of these event type e.g. put, patch, cancel, and auth_revoked.

    The event type "put" indicated that data at event path relative to stream path was completely changed. Event path can be determined from dataPath().

    The event type "patch" indicated that data at event path relative to stream path was updated. Event path can be determined from dataPath().

    The event type "cancel" indeicated something wrong and cancel by server.

    The event type "auth_revoked" indicated the provided Firebase Authentication Data (Database secret) is no longer valid.

   */
  String eventType();

  /*
    Determine the current stream path.

    @return The database streaming path.

   */
  String streamPath();

  /*
    Determine the current data path.

    @return The database path which belong to server's returned payload.

    The database path returned from this function in case of stream, also changed up on the child or parent's stream
    value changes.

   */
  String dataPath();

  /*
    Determine the error reason String from process.

    @return The error description string (String object).

   */
  String errorReason();

  /*
    Return the ineger data of server returned payload.

    @return Integer value.

   */
  int intData();

  /*
    Return the float data of server returned payload.

    @return Float value.

   */
  float floatData();

  /*

    Return the double data of server returned payload.

    @return double value.

  */
  double doubleData();

  /*

    Return the Boolean data of server returned payload.

    @return Boolean value.

  */
  bool boolData();

  /*
    Return the String data of server returned payload.

    @return String (String object).

   */
  String stringData();

  /*
    Return the JSON String data of server returned payload.

    @return String (String object).

   */
  String jsonData();

  /*
    Return the blob data (uint8_t) array of server returned payload.

    @return Dynamic array of 8-bit unsigned integer i.e. std::vector<uint8_t>.

   */
  std::vector<uint8_t> blobData();

  /*
    Return the new appended node's name or key of server returned payload when calling pushXXX function.

    @return String (String object).

   */
  String pushName();

  /*
    Determine the stream connection status.

    @return Boolean type status indicates whether the Firebase Data object is working with stream or not.

   */
  bool isStream();

  /*
    Determine the server connection status.

    @return Boolean type status indicates whether the Firebase Data object is connected to server or not.

   */
  bool httpConnected();

  /*
    Determine the timeout event of server's stream (30 sec is default).

    Nothing to do when stream connection timeout, the stream connection will be automatic resumed.

    @return Boolean type status indicates whether the stream was timeout or not.

   */
  bool streamTimeout();

  /*
    Determine the availability of data or paylaod returned from server.

    @return Boolean type status indicates whether the server return back the new payload or not.

   */
  bool dataAvailable();

  /*
    Determine the availability of stream event-data paylaod returned from server.

    @return Boolean type status indicates whether the server return back the stream event-data 
    payload or not.

   */
  bool streamAvailable();

  /*
    Determine the matching between data type that intend to get from/store to database and the server's return payload data type.

    @return Boolean type status indicates whether the type of data being get from/store to database 
    and server's returned payload are matched or not.

   */
  bool mismatchDataType();

  /*
    Determine the http status code return from server.

    @return integer number of HTTP status.

   */
  int httpCode();

  /*
    Determine the name (full path) of backup file in SD card.
    
    @return String (String object) of file name that store on SD card after backup operation.

   */
  String getBackupFilename();

  /*
    Determine the size of backup file.
    
    @return Size of backup file in byte after backup operation.

   */
  size_t getBackupFileSize();

  /*
    Clear or empty data in Firebase Data object.

   */
  void clear();

  /*
    Determine the error description for file transfering (pushFile, setFile, backup and restore).
    
    @return Error description string (String object).

   */
  String fileTransferError();

  /*
    Return the server's payload data.
    
    @return Payload string (String object).

   */
  String payload();

  HTTPClientESP32Ex http;

  QueryFilter queryFilter;

  friend FirebaseESP32;
  friend QueueManager;

protected:
  StreamEventCallback _dataAvailableCallback = NULL;
  StreamTimeoutCallback _timeoutCallback = NULL;
  QueueInfoCallback _queueInfoCallback = NULL;
  TaskHandle_t _handle = NULL;
  TaskHandle_t _q_handle = NULL;
  int _index = -1;
  uint8_t _dataTypeNum = 0;

  bool _firebaseCall = false;
  bool _streamCall = false;
  bool _isStreamTimeout = false;
  bool _isStream = false;
  bool _streamStop = false;
  bool _isSilentResponse = false;

  bool _streamDataChanged = false;
  bool _streamPathChanged = false;
  bool _dataAvailable = false;
  bool _keepAlive = false;
  bool _httpConnected = false;
  bool _interruptRequest = false;
  bool _mismatchDataType = false;
  bool _pathNotExist = false;
  bool _pause = false;
  bool _file_transfering = false;
  uint8_t _dataType = 0;
  uint8_t _dataType2 = 0;
  uint8_t _connectionStatus = 0;
  uint32_t _qID = 0;
  QueueManager _qMan;
  uint8_t _maxRetry = 0;

  uint8_t _r_method = 0;
  uint8_t _r_dataType = 0;

  std::string _path = "";
  std::string _path2 = "";
  std::string _data = "";
  std::string _data2 = "";
  std::string _streamPath = "";
  std::string _pushName = "";
  std::string _file_transfer_error = "";
  std::string _fileName = "";
  std::string _redirectURL = "";
  std::string _firebaseError = "";
  std::string _eventType = "";
  uint16_t _maxBlobSize = 1024;

  std::vector<uint8_t> _blob = std::vector<uint8_t>();

  int _httpCode = -1000;
  int _contentLength = 0;

  unsigned long _dataMillis = 0;
  unsigned long _streamMillis = 0;
  unsigned long _streamResetMillis = 0;
  std::string _backupNodePath = "";
  std::string _backupDir = "";
  std::string _backupFilename = "";
  size_t _backupzFileSize = 0;

  bool getServerStreamResponse();
  bool getServerResponse();
  std::string getDataType(uint8_t type);
  std::string getMethod(uint8_t method);

  void addQueue(uint8_t firebaseMethod,
                uint8_t firebaseDataType,
                const std::string path,
                const std::string filename,
                const std::string payload,
                bool isQuery,
                int *intTarget,
                float *floatTarget,
                double *doubleTarget,
                bool *boolTarget,
                String *stringTarget,
                std::vector<uint8_t> *blobTarget);

  void clearQueueItem(QueueItem &item);

  void setQuery(QueryFilter &query);

  friend FirebaseESP32;
};

class StreamData
{
public:
  StreamData();
  ~StreamData();
  String dataPath();
  String streamPath();
  int intData();
  float floatData();
  double doubleData();
  bool boolData();
  String stringData();
  String jsonData();
  std::vector<uint8_t> blobData();
  String dataType();
  String eventType();
  void empty();
  friend FirebaseESP32;

protected:
  std::string _streamPath = "";
  std::string _path = "";
  std::string _data = "";
  std::vector<uint8_t> _blob = std::vector<uint8_t>();
  std::string _dataTypeStr = "";
  std::string _eventTypeStr = "";
  uint8_t _dataType = 0;
};

extern FirebaseESP32 Firebase;

#endif
