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
using System.Linq;

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
            // Server that can interact with any incoming connection
            remsrv = new TcpListener(IPAddress.Any, 2121);
            // Local server
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
                client.ReceiveTimeout = 429496729;
                // Connection is new. Check if the client sended a handshake command
                if (sb.ToString() == "INITHDSK")
                {
                    //TODO: Authentication
                    client.Send(pcol.HANDSHAKE_REPLY(true));
                    DEBUG(string.Format("Client {0} has been authenticated!", ipaddr.Address.ToString()));
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
                            //cl._socket.Disconnect(false);
                            cl._socket.Dispose();
                            dHCPClients.RemoveAt(it);
                        }
                    }
                    catch (NullReferenceException) { };
                }
            }
        }
        /* NOTG(<posis>) (Use databases?)
         * TESTALLPS(devicename)
         * FLREBOOT(devicename)
         */

        private void CommandService()
        {
            while (true)
            {
                // One socket is accepted per command
                Socket localclient = locsrv.AcceptSocket();
                localclient.ReceiveTimeout = 10000;
                DEBUG("A local client has connected!");
                // Wait for a command to come in
                StringBuilder SB = new StringBuilder();
                byte[] cmdBuffer = new byte[1024];
                try { localclient.Receive(cmdBuffer); } catch (SocketException) { DEBUG("Exception thrown: Local client probaby timed out"); }
                SB.Append(Encoding.ASCII.GetChars(cmdBuffer));

                // Split the command first into sections
                string[] Rcmd = SB.ToString().Split('(', ')');
                string Scmd = Rcmd[0];
                switch (Scmd)
                {
                    case "NOTG":
                        bool used = false;
                        do
                        {
                            foreach (DHCPClient c in dHCPClients)
                            {
                                if (c._isBusy) { continue; }
                                c._socket.Send(Encoding.ASCII.GetBytes("NOTG\0"));
                                DEBUG("NOTG sended");
                                byte[] tB = new byte[8];
                                c._socket.ReceiveTimeout = 3000;
                                try { c._socket.Receive(tB); } catch (SocketException) { /*continue;*/ }
                                if (Encoding.ASCII.GetString(tB) == "ERRBSY") { DEBUG("Client is busy"); c._isBusy = true; continue; }
                                c._socket.Send(Encoding.ASCII.GetBytes(Rcmd[1] + "\0"));
                                DEBUG(string.Format("Command to {0} was sent", c.ip.Address.ToString()));
                                used = true;
                                break;
                            }
                        } while (!used);
                        break;
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
            var cmdService = new Thread(() => CommandService());
            acceptingThread.Start();
            connectionManager.Start();
            cmdService.Start();
            DEBUG("TCP services are started!");
        }
        public void Dispose()
        {
            remsrv.Stop();
            locsrv.Stop();
        }

    }
}
