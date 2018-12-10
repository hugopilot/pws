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

        
        // DHCP server classes
        private volatile List<DHCPClient> dHCPClients;  // Make this volatile: should be able to be manipulated by other threads
        private TcpListener remsrv;
        private TcpListener locsrv;
        private DHCPCollection pcol;

        // Threads
        private Thread acceptingThread;
        private Thread connectionManager;
        private Thread cmdService;
        private Thread garbageService;

        // Actual stuff
        /// <summary>
        /// Constructor for DHCPServer. This class fully implements the DHCP protocol
        /// </summary>
        /// <param name="remport">Port for the server that handles remote connections (clients)</param>
        /// <param name="locport">Port for the local server for other programs to interact with</param>
        public DHCPHandler(int remport, int locport)
        {
            remsrv = new TcpListener(IPAddress.Any, remport);
            locsrv = new TcpListener(IPAddress.Parse("127.0.0.1"), locport);
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

        // This is used to force garbage collection. It won't clean up by it self
        private void GarbargeCollection()
        {
            while (true)
            {
                // Collect once per minute or we will have a bad time
                Thread.Sleep(60000);
                GC.Collect();
            }
        }

        private void BusyClient(DHCPClient c)
        {
            //Tiny buffer needed
            byte[] buff = new byte[4];

            //Set a really long timout
            c._socket.ReceiveTimeout = 999999;
            c._socket.Receive(buff);

            //if OK is recieved, set busy property to false.
            string recv = Encoding.ASCII.GetString(buff);
            if (recv == "OK\0\0")
                c._isBusy = false;
        }
        /* NOTG(<posis>) (Use databases?)
         * TESTALLPS(devicename)
         * FLREBOOT(devicename)
         * DEVICELIST()
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
                        bool used = false; // This states if the current command has been sent to a client.
                        do
                        {
                            Thread.Sleep(10); // Wait 10 ms because of refreshing.

                            foreach (DHCPClient c in dHCPClients) // Loop though all clients
                            {
                                if (c._isBusy) { continue; } // If it's busy, skip to the next one
                                c._socket.Send(Encoding.ASCII.GetBytes("NOTG\0"));  // Send the NOTG command.
                                DEBUG("NOTG sended"); 
                                byte[] tB = new byte[8]; // Tiny buffer
                                c._socket.ReceiveTimeout = 3000; // Give the client 3s to react
                                try { c._socket.Receive(tB); } catch (SocketException) { /*Nothing*/ } 
                                if (Encoding.ASCII.GetString(tB) == "ERRBSY") { DEBUG("Client is busy"); c._isBusy = true; continue; } // If the clients states it's busy, update and skip
                                c._socket.Send(Encoding.ASCII.GetBytes(Rcmd[1] + "\0")); // Send positions to client
                                c._isBusy = true; // Update busy status
                                DEBUG(string.Format("Command to {0} was sent", c.ip.Address.ToString()));
                                var ho = new Thread(() => BusyClient(c)); // Assign a new thread that keeps track of the status of the client
                                ho.Start();
                                used = true; // Command has been used
                                break; // Break the loop
                            }
                        } while (!used);
                        break;
                    case "TESTALLPS":
                        try
                        {
                            DHCPClient c = dHCPClients[Convert.ToInt32(Rcmd[1])];
                            c._socket.Send(Encoding.ASCII.GetBytes("TESTALLPS\0"));
                            DEBUG("Testing all PS on device " + Rcmd[1]);
                        }
                        catch (Exception e) { DEBUG(string.Format("Failed to send command to device {0}: {1}", Rcmd[1], e.Message)); }
                        break;
                    case "FLREBOOT":
                        try
                        {
                            DHCPClient c = dHCPClients[Convert.ToInt32(Rcmd[1])];
                            c._socket.Send(Encoding.ASCII.GetBytes("FLREBOOT\0"));
                            DEBUG("Rebooting device " + Rcmd[1]);
                        }
                        catch (Exception e) { DEBUG(string.Format("Failed to send command to device {0}: {1}", Rcmd[1], e.Message)); }
                        break;
                    case "DEVICELIST":
                        try
                        {
                            DEBUG("DeviceList: \n");
                            Console.WriteLine("DEVICE  | IPADDRESS | PORT");
                            for(int i = 0; i < dHCPClients.Count; i++)
                            {
                                DHCPClient c = dHCPClients[i];
                                Console.WriteLine(string.Format("{0}    |{1}    |{2}", i, c.ip.Address, c.ip.Port));
                            }
                        }catch(Exception e) { DEBUG("Error while listing: " + e.Message); }
                        break;
                }
            }
            

        }

        /// <summary>
        /// Starts the DHCP-daemon service
        /// </summary>
        public void StartService()
        {
            // Start the service and clear the clients list.
            dHCPClients = new List<DHCPClient>();
            remsrv.Start();
            locsrv.Start();
            acceptingThread = new Thread(() => AcceptConnections());
            connectionManager = new Thread(() => ConnectionManager());
            cmdService = new Thread(() => CommandService());
            garbageService = new Thread(() => GarbargeCollection());
            acceptingThread.Start();
            connectionManager.Start();
            cmdService.Start();
            garbageService.Start();
            DEBUG("Services started!");
        }

        /// <summary>
        /// Disposes all objects
        /// </summary>
        public void Dispose()
        {
            try
            {
                DEBUG("Disposing the threads...");
                // Dispose the Threads
                acceptingThread.Abort();
                connectionManager.Abort();
                cmdService.Abort();
                garbageService.Abort();

                DEBUG("Stopping TCP Servers...");
                // Stop the TCP servers
                remsrv.Stop();
                locsrv.Stop();
            }catch(Exception e) { DEBUG("ERROR while disposing: " + e.Message); Thread.Sleep(2000); Environment.Exit(1); }
            

            DEBUG("Disposing successful!");
        }

    }
}
