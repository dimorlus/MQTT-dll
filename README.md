# MQTT-dll
Windows MQTT client for Borland C++ Builder

1. Create paho-mqtt3c.dll using Visual Studio from the eclipse-paho-mqtt-c-src-1.2.0\Windows Build\Paho C MQTT APIs.sln

2. Make paho-mqtt3c.lib from paho-mqtt3c.dll using follow script
	implib -c -f -a paho-mqtt3c.lib paho-mqtt3c.dll
	tdump paho-mqtt3c.lib > paho-mqtt3c.lib.txt
	tdump paho-mqtt3c.dll > paho-mqtt3c.dll.txt

3. Modify MQTTClient.h as follow
	...

	#if defined(__cplusplus)
	
		 extern "C" {
	
	#endif
	
	#if !defined(MQTTCLIENT_H)
	
	#define MQTTCLIENT_H 
	
	#if defined(__BORLANDC__)
	
	#pragma option push
	
	#pragma option -VC
	
	#define _CALL_ __stdcall
	
	#else
	
	#define _CALL_
	
	#endif 
	...
	
	DLLExport int _CALL_ MQTTClient_setCallbacks(MQTTClient handle, void* context, MQTTClient_connectionLost* cl,
	MQTTClient_messageArrived* ma, MQTTClient_deliveryComplete* dc);
	...
	 
	
	#if defined(__BORLANDC__)
	
	#pragma option pop
	
	#endif
	
	#endif
	
	#ifdef __cplusplus
	
	}
	
	#endif 

