/* This class is a high level implementation of DHCP protocol
 * It's intented for easy use
 * Written by Hugo Woesthuis
 */ 

using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using deamon.External;

namespace deamon.Handlers
{
    class TcpClientHandler
    {
        private TcpServer tcpsrv = new TcpServer();

        /// <summary>
        /// Opens the socket and makes the server available
        /// </summary>
        /// <param name="_port">The port to listen to.</param>
        public void Open(int _port)
        {
            // Check if the current socket is already open
            if (tcpsrv.IsOpen)
            {
                tcpsrv.Close();
            }
            tcpsrv.Port = _port;
            tcpsrv.Open();
        }

        public struct Client {
            int id;
            IPAddress ipaddr;
        }
    }
}
