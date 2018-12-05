/* This class is a high level implementation of DHCP protocol
 * It's intented for easy use
 * Written by Hugo Woesthuis
 */ 

using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using deamon.Collections;

namespace deamon.Handlers
{
    class DHCPHandler : IDisposable
    {
        private void DEBUG(string message)
        {
            Console.WriteLine(string.Format("[{0}] {1}", string.Format("{0} {1}", DateTime.Now.ToShortDateString(), DateTime.Now.ToLongTimeString()), message));
        }
        private volatile List<DHCPClient> dHCPClients;
        private TcpListener remsrv;
        private TcpListener locsrv;
        //private List<Client> clients;
        private DHCPCollection pcol;
        /// <summary>
        /// Constructor for DHCPServer. This class fully implements the DHCP protocol
        /// </summary>
        /// <param name="remport">Port for the server that handles remote connections (clients)</param>
        /// <param name="locport">Port for the local server for other programs to interact with</param>
        public DHCPHandler(int remport, int locport)
        {
            remsrv = new TcpListener(IPAddress.Any, remport);
            locsrv = new TcpListener(IPAddress.Parse("localhost"), locport);
        }
        /// <summary>
        /// Constructor for DHCPServer. This class fully implements the DHCP protocol, uses the default settings
        /// </summary>
        public DHCPHandler()
        {
            remsrv = new TcpListener(IPAddress.Any, 2121);
            locsrv = new TcpListener(IPAddress.Parse("127.0.0.1"), 8383);
        }
        private void AcceptConnections()
        {
            while (true)
            {
                Socket client = remsrv.AcceptSocket();
                IPEndPoint ipaddr = client.RemoteEndPoint as IPEndPoint;
                DEBUG("Accepted an connection: " + ipaddr.Address + " on port: " + ipaddr.Port);
                // Preform a handshake
                byte[] data = new byte[8];
                // Add a 3 second time out. Keeps the Thread from freezing in case something goes wrong
                client.ReceiveTimeout = 3000;
                try
                {
                    int sz = client.Receive(data);
                }
                catch (SocketException) { client.Disconnect(true); }
                StringBuilder sb = new StringBuilder();

                sb.Append(Encoding.ASCII.GetChars(data));

                // Connection is new. Check if the client sended a handshake command
                if (sb.ToString() == "INITHDSK")
                {
                    //TODO: Authentication
                    client.Send(pcol.HANDSHAKE_REPLY(true));
                }
                DHCPClient dHCPClient = new DHCPClient(client);
                dHCPClient.ip = ipaddr;
                dHCPClients.Add(dHCPClient);
            }
        }
        private void ConnectionManager()
        {
            while (true)
            {
                for (int it = 0; it < dHCPClients.Count; it++)
                {
                    DHCPClient cl = dHCPClients[it];
                    try
                    {
                        if (cl._socket.Poll(1000, SelectMode.SelectRead) && cl._socket.Available == 0)
                        {
                            DEBUG(string.Format("{0} has disconnected", cl.ip.Address.ToString()));
                            cl._socket.Disconnect(false);
                            cl._socket.Dispose();
                            dHCPClients.RemoveAt(it);
                        }
                    }
                    catch (NullReferenceException) { };
                }
            }
        }
        public void StartService()
        {
            // Start the service and clear the clients list.
            dHCPClients = new List<DHCPClient>();
            remsrv.Start();
            locsrv.Start();
            var acceptingThread = new Thread(() => AcceptConnections());
            var connectionManager = new Thread(() => ConnectionManager());
            acceptingThread.Start();
            connectionManager.Start();
        }
        public void Dispose()
        {
            remsrv.Stop();
            locsrv.Stop();
        }

    }
}
