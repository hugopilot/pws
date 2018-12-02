﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Net;      //required
using System.Net.Sockets;    //required

namespace ServerTest
{
    class Program
    {
        static void Main(string[] args)
        {
            TcpListener server = new TcpListener(IPAddress.Any, 3001);
            // we set our IP address as server's address, and we also set the port: 9999

            server.Start();  // this will start the server
            Console.WriteLine("Why are you running?!");
            while (true)   //we wait for a connection
            {
                TcpClient client = server.AcceptTcpClient();  //if a connection exists, the server will accept it

                NetworkStream ns = client.GetStream(); //networkstream is used to send/receive messages

                byte[] hello = new byte[16];   //any message must be serialized (converted to byte array)
                hello = Encoding.ASCII.GetBytes("FLREBOOT\0\0\0\0\0\0\0");  //conversion string => byte array

                ns.Write(hello, 0, hello.Length);     //sending the message
                Console.WriteLine("REBOOT MOTHERFUCKER!");
                while (client.Connected)  //while the client is connected, we look for incoming messages
                {
                    byte[] msg = new byte[1024];     //the messages arrive as byte array
                    ns.Read(msg, 0, msg.Length);   //the same networkstream reads the message sent by the client
                    //Console.WriteLine(encoder.GetString(msg).Trim('')); //now , we write the message as string
                }
            }

        }
    }
}
