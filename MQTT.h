//---------------------------------------------------------------------------

#ifndef MQTTH
#define MQTTH

//---------------------------------------------------------------------------
#define TOPIC_NODE          "/NODE" //Publish node data
#define TOPIC_GW            "/GW"   //Publish GW info
#define TOPIC_CMD           "/CMD"  //Subscribe to command
#define TOPIC_CFG           "/CFG"  //Subscribe to GW info request

//---------------------------------------------------------------------------
enum {mdServer, mdSimulator, mdNone};
enum {tpNone, tpNode, tpGW, tpCmd, tpCfg};

extern void ReadOpts(int mode);
extern bool OpenMQTT(int mode);
extern bool UnSubscribe(int mode);
extern bool Subscribe(int mode);
extern void CloseMQTT(void);
extern void PublishMQTT(const char *topic, char *data);
extern bool isConnectMQTT(void);
extern int isModeMQTT(void);
extern bool isReceivedMQTT(void);
extern int getLastTopic(void);
extern char *getLastCmd(void);
extern char *getLastNode(void);
extern char *getLastGW(void);


#endif
