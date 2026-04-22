/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Time.hpp>

class NetworkTracker
{
public:
	void LogSentPacket(sf::Packet& packet);
	void LogReceivedPacket(sf::Packet& packet);
	void Update(sf::Time dt);
private:
	const int kHeaderSize = 20;

	int m_packets_sent;
	int m_packets_received;
	int m_sent_size;
	int m_received_size;

	int m_last_sent;
	int m_last_sent_size;
	int m_last_received;
	int m_last_received_size;

	sf::Time m_update_timer;
};

