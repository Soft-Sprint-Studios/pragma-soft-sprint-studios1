#include "stdafx_client.h"
#include "pragma/networking/local_client.hpp"

extern DLLENGINE Engine *engine;

#pragma optimize("",off)
std::string pragma::networking::LocalClient::GetIdentifier() const {return "localhost";}
bool pragma::networking::LocalClient::Connect(const std::string &ip,Port port,Error &outErr)
{
	if(ip != "127.0.0.1")
		return false;
	auto result = engine->ConnectLocalHostPlayerClient();
	if(result == false)
		return result;
	OnConnected();
	return result;
}
bool pragma::networking::LocalClient::Disconnect(Error &outErr)
{
	OnDisconnected();
	return true;
}
bool pragma::networking::LocalClient::SendPacket(Protocol protocol,NetPacket &packet,Error &outErr)
{
	packet.SetTimeActivated(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
	packet->SetOffset(0);
	engine->HandleLocalHostPlayerServerPacket(packet);
	OnPacketSent(protocol,packet);
	return true;
}
bool pragma::networking::LocalClient::IsRunning() const {return true;}
bool pragma::networking::LocalClient::IsDisconnected() const {return false;}
bool pragma::networking::LocalClient::PollEvents(pragma::networking::Error &outErr) {return true;}
uint16_t pragma::networking::LocalClient::GetLatency() const {return 0;}
void pragma::networking::LocalClient::SetTimeoutDuration(float duration) {}
std::optional<std::string> pragma::networking::LocalClient::GetIP() const {return {};}
std::optional<pragma::networking::Port> pragma::networking::LocalClient::GetLocalTCPPort() const {return {};}
std::optional<pragma::networking::Port> pragma::networking::LocalClient::GetLocalUDPPort() const {return {};}
#pragma optimize("",on)
