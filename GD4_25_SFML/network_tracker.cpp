#include "network_tracker.h"
#include <iostream>
#include "constants.hpp"

void NetworkTracker::LogSentPacket(sf::Packet& packet)
{
	m_packets_sent++;
	m_sent_size += (int)packet.getDataSize();
}

void NetworkTracker::LogReceivedPacket(sf::Packet& packet)
{
	m_packets_received++;
	m_received_size += (int)packet.getDataSize();
}

void NetworkTracker::Update(sf::Time dt)
{
	m_update_timer += dt;
	if (m_update_timer > sf::seconds(1.))
	{
		int sent = m_packets_sent - m_last_sent;
		int sent_size = m_sent_size - m_last_sent_size;
		int received = m_packets_received - m_last_received;
		int received_size = m_received_size - m_last_received_size;
		m_last_sent = m_packets_sent;
		m_last_sent_size = m_sent_size;
		m_last_received = m_packets_received;
		m_last_received_size = m_received_size;

		int bandwidth = kHeaderSize * (sent + received) + sent_size + received_size;

		m_update_timer = sf::Time::Zero;

		if (kLogBandwidth)
		{
			std::cout << "Sent packets: " << sent << " Size: " << sent_size << std::endl;
			std::cout << "Received packets: " << received << " Size: " << received_size << std::endl;
			std::cout << "Total sent packets: " << m_packets_sent << " Total size: " << m_sent_size << std::endl;
			std::cout << "Total received packets: " << m_packets_received << " Total size: " << m_received_size << std::endl;
			std::cout << "Bandwidth: " << bandwidth << " Bytes / s" << std::endl << std::endl;
		}
	}
}
