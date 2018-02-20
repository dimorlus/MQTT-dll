//---------------------------------------------------------------------------
#pragma warn -8060

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


#include <vcl.h>
#include <IniFiles.hpp>
#pragma hdrstop

#include "sys_utils.h"
#include "StrUtils.h"

#include <math.h>

#include "MQTTClient.h"
#include "MQTTClientPersistence.h"

#include "strnum.h"
#include "router_srv.h"
#include "datastr.h"

#pragma hdrstop
#include "tcp_srv.h"
#include "MQTT.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma comment(lib,"paho-mqtt3c.lib")
//---------------------------------------------------------------------------

static volatile int toStop = 0;
static bool Connected = false;
static int Mode = mdNone;
static bool MQTTData = false;
static int lastTopic = tpNone;
static char lastCMD[64];
static char lastNode[64];
static char lastGW[64];


static const MQTTClient_connectOptions MQTTClient_ConnectOptions_Initializer =
 MQTTClient_connectOptions_initializer;
static const MQTTClient_SSLOptions MQTTClient_SSLOptions_Initializer =
 MQTTClient_SSLOptions_initializer;

//---------------------------------------------------------------------------

static MQTTClient client;

static char url[128];
static char username[128];
static char password[128];
static char dev_id[128];
static char topic_base[128];
static char sbsGW[128];

//---------------------------------------------------------------------------
bool isTopic(const char *rtopic, const char *topic)
{
 int ll = strlen(rtopic);
 int lr = strlen(topic);
 return (ll>=lr)&&(strcmp(&rtopic[ll-lr], topic)==0);
}

//---------------------------------------------------------------------------

static int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* m)
{
  char str[256];
  unsigned char dat[48];

  int i = sprintf(str, "%s: ", topicName);
  memcpy(&str[i], m->payload, m->payloadlen);
  str[i+m->payloadlen] = '\0';

  if (isTopic(topicName, TOPIC_NODE))
   {
    memset(dat, 0, sizeof(dat));
    b642data((char *)m->payload, dat);
    PutData((tnode *)dat, dat[32]);
    //strcpy(lastNode, str);
    memcpy(lastNode, m->payload, m->payloadlen);
    lastNode[m->payloadlen] = '\0';
    lastTopic = tpNode;
    MQTTData = true;
   }
  else
  if (isTopic(topicName, TOPIC_GW))
   {
    memset(&GW, 0, sizeof(GW));
    b642data((char *)m->payload, (char *)&GW);
    memcpy(lastGW, m->payload, m->payloadlen);
    lastGW[m->payloadlen] = '\0';
    lastTopic = tpGW;
    MQTTData = true;
   }
  else
  if (isTopic(topicName, TOPIC_CMD))
   {
    lastTopic = tpCmd;
    //strcpy(lastCMD, str);
    memcpy(lastCMD, m->payload, m->payloadlen);
    lastCMD[m->payloadlen] = '\0';
    MQTTData = true;
   }
  else
  if (isTopic(topicName, TOPIC_CFG))
   {
    lastTopic = tpCfg;
    MQTTData = true;
   }
  MQTTClient_freeMessage(&m);
  MQTTClient_free(topicName);
  return 1;
}
//---------------------------------------------------------------------------

static void connectionLost(void* context, char* cause)
{
 Connected = false;
}
//---------------------------------------------------------------------------

static void deliveryComplete(void* context, MQTTClient_deliveryToken dt)
{
}
//---------------------------------------------------------------------------

static void cfinish(int sig)
{
  signal(SIGINT, NULL);
  signal(SIGTERM, NULL);
  toStop = 1;
  Connected = false;
}
//---------------------------------------------------------------------------

bool CheckServer(const char *url, int Timeout_ms, void* owner, tprint print_fn);
static MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;;
static MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
//---------------------------------------------------------------------------

bool OpenMQTT(int mode)
{
  int rc = 0;
  Mode = mode;
  Connected = false;

  if (!CheckServer(url, 5000, NULL, NULL)) return false;


  rc = MQTTClient_create(&client, url, dev_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
  if (rc != 0) return false;

  signal(SIGINT, cfinish);
  signal(SIGTERM, cfinish);

  //rc = MQTTClient_setCallbacks(client, NULL, connectionLost, messageArrived, deliveryComplete);
  rc = MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);

  if (rc != 0) return false;

  conn_opts.keepAliveInterval = 10;
  conn_opts.reliable = 0;
  conn_opts.cleansession = 1;
  conn_opts.username = username;
  conn_opts.password = password;
  ssl_opts.enableServerCertAuth = 0;
  conn_opts.ssl = &ssl_opts;

  rc = MQTTClient_connect(client, &conn_opts);
  if (rc != 0) return false;

  Connected = (rc == 0);
  Subscribe(mode);
  MQTTData = Connected;
  return Connected;
}
//---------------------------------------------------------------------------
bool UnSubscribe(int mode)
{
 int rc;
 if (Connected)
  {
   char subscribe[128];
   switch(mode)
    {
     case mdServer:
      sprintf(subscribe, "%s%s", topic_base, TOPIC_NODE);
      rc = MQTTClient_unsubscribe(client, subscribe);
      sprintf(subscribe, "%s%s", topic_base, TOPIC_GW);
      rc += MQTTClient_unsubscribe(client, subscribe);
     break;
     case mdSimulator:
      sprintf(subscribe, "%s%s", topic_base, TOPIC_CMD);
      rc = MQTTClient_unsubscribe(client, subscribe);
      sprintf(subscribe, "%s%s", topic_base, TOPIC_CFG);
      rc+=MQTTClient_unsubscribe(client, subscribe);
     break;
    }
   return (rc==0);
  }
 else return false;
}
//---------------------------------------------------------------------------
bool Subscribe(int mode)
{
 int rc;
 if (Connected)
  {
   char subscribe[128];
   switch(mode)
    {
     case mdServer:
      sprintf(subscribe, "%s%s", topic_base, TOPIC_NODE);
      rc = MQTTClient_subscribe(client, subscribe, 2);
      sprintf(subscribe, "%s%s", topic_base, TOPIC_GW);
      rc += MQTTClient_subscribe(client, subscribe, 2);
     break;
     case mdSimulator:
      sprintf(subscribe, "%s%s", topic_base, TOPIC_CMD);
      rc = MQTTClient_subscribe(client, subscribe, 2);
      sprintf(subscribe, "%s%s", topic_base, TOPIC_CFG);
      rc += MQTTClient_subscribe(client, subscribe, 2);
     break;
    }
   return (rc==0);
  }
 else return false; 
}
//---------------------------------------------------------------------------

void CloseMQTT(void)
{
  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);
  Connected = false;
}

//---------------------------------------------------------------------------
void PublishMQTT(const char *topic, char *data)
{
 if (Connected&&MQTTClient_isConnected(client))
 //if (Connected)
  {
   char fulltopic[128];
   sprintf(fulltopic, "%s%s", topic_base, topic);
   int rc = MQTTClient_publish(client, fulltopic, strlen(data), data, 0, 0, NULL);
   Connected = (rc==0);
   MQTTData = Connected;
  }
}
//---------------------------------------------------------------------------
bool isConnectMQTT(void)
{
 return Connected;
}
//---------------------------------------------------------------------------
int isModeMQTT(void)
{
 return Mode;
}
//---------------------------------------------------------------------------
int getLastTopic(void)
{
 int topic = lastTopic;
 lastTopic = tpNone;
 return topic;
}
//---------------------------------------------------------------------------
char *getLastCmd(void)
{
 return lastCMD;
}
//---------------------------------------------------------------------------
char *getLastNode(void)
{
 return lastNode;
}
//---------------------------------------------------------------------------
char *getLastGW(void)
{
 return lastGW;
}
//---------------------------------------------------------------------------
void ReadOpts(int mode)
{
 AnsiString asStr;
 Mode = mode;

 TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));

 asStr = ini->ReadString("MQTT", "URL", "127.0.0.1:1883");
 strcpy(url, asStr.c_str());

 asStr = ini->ReadString("MQTT", "user", "guest");
 strcpy(username, asStr.c_str());

 asStr = ini->ReadString("MQTT", "passwd", "guest");
 strcpy(password, asStr.c_str());

 asStr = ini->ReadString("MQTT", "topic_base", "");
 strcpy(topic_base, asStr.c_str());

 asStr = ini->ReadString("MQTT", "dev_id", "");
 strcpy(dev_id, asStr.c_str());

 delete ini;
}
//---------------------------------------------------------------------------

bool CheckServer(const char *url, int Timeout_ms, void* owner, tprint print_fn)
{
 char addr[128];
 char port[128];
 int error;
 SOCKET test;
 int l = strlen(url);
 if (l--)
  {
   while(l && (url[l] != ':')) l--;
   int i, j;
   for(i=0, j=0; i<l; i++) addr[i] = url[i];
   addr[i++] = '\0';
   while(port[j++] = url[i]) i++;
   test = Connect_to(addr, port, Timeout_ms, error, owner, print_fn);
   if (test == INVALID_SOCKET) return false;
   return Disconnect(test, error, owner, print_fn);
  }
 else return false;
}
//---------------------------------------------------------------------------
bool isReceivedMQTT(void)
{
 if (MQTTData)
  {
   MQTTData = false;
   return true;
  }
 else return false;
}
//---------------------------------------------------------------------------
