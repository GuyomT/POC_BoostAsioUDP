/*
** EPITECH PROJECT, 2022
** RTYPE
** File description:
** IServer
*/

#pragma once
#include <array>
#include <utility>
#include <vector>

/**
 * IServer interface is to facilitate the creation of the server class
 */
namespace network
{
    /**
     * Standard pair class that contains a string and the ID of the client that
     * sent it
     */
    typedef std::pair<std::array<char, 10>, unsigned int> ClientMessage;
    typedef std::pair<std::array<char, 10>, std::vector<unsigned int>> ServerMessage;

    class IServer {
      public:
        virtual ~IServer(){};
        /**
         * This function allows us to check if the server has received messages
         *@return true if there are messages
         */
        virtual bool hasMessages() = 0;
        /**
         * This function sends a message to a client determined by his ID
         *@param message the message to send
         *@param clientID the ID of the client
         */
        virtual void sendToClient(const std::array<char, 10> &message, uint32_t clientID) = 0;
        /**
         * Get the amount of clients that are connected
         *@return Amount of connected clients
         */
        virtual size_t getClientCount() = 0;
        /**
         * Get the ID of client from the clients array
         *@param n the index for the array
         *@return client ID of client n
         */
        virtual uint32_t getClientIdByIndex(size_t n) = 0;
    };
} // namespace network