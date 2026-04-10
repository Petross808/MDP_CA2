/*
* Written in class, editedby:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <thread>
#include <cstdint>
#include <map>
#include <memory>

class GameServer
{
public:
	typedef std::shared_ptr<GameServer> Ptr;

public:
	static const unsigned short kServerPort = 50000;

public:
	explicit GameServer();
	~GameServer();

private:
	struct RemotePeer
	{
		RemotePeer();
		sf::TcpSocket m_socket;
		sf::Time m_last_packet_time;
		bool m_ready;
		bool m_timed_out;
	};

	typedef std::unique_ptr<RemotePeer> PeerPtr;

private:
	void SetListening(bool enable);
	void ExecutionThread();
	void Tick();
	sf::Time Now() const;

	void HandleIncomingPackets();
	void ResolvePacket(sf::Packet& packet, RemotePeer& receiving_peer, bool& detected_timeout);

	void HandleIncomingConnections();
	void HandleDisconnections();

	void InformWorldState(sf::TcpSocket& socket);
	void SendToAll(sf::Packet& packet);

private:
	std::thread m_thread;
	sf::Clock m_clock;
	sf::TcpListener m_listener_socket;
	bool m_listening_state;
	sf::Time m_client_timeout;

	std::size_t m_max_connected_players;
	std::size_t m_connected_players;

	std::vector<PeerPtr> m_peers;
	bool m_waiting_thread_end;
};

