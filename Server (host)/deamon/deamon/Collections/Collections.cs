/* Code written by Hugo Woesthuis
 * Licenced under GPLv3
 */

using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Net;

namespace deamon.Collections
{
    public struct Client
    {
        public int Id;
        public System.Net.IPAddress ipAddr;
        public Socket socket;
    }
    public struct DHCPCollection
    {
        public byte[] HANDSHAKE_REPLY(bool IsOK = false)
        {
            if (IsOK)
                return Encoding.ASCII.GetBytes("INITHSOK\0");
            else if (!IsOK)
                return Encoding.ASCII.GetBytes("HDSERROR\0");
            return Encoding.ASCII.GetBytes("HDSERROR\0");

        }
    }
    public class DHCPClient{
        public volatile Socket _socket;
        public bool _isBusy;
        public IPEndPoint ip;

        public DHCPClient(Socket socket)
        {
            _socket = socket;
            _isBusy = false;
        }

        public void SendCommand(string command)
        {
            //TODO: Make this happen
        }

        public bool GetBusyState()
        {
            return _isBusy;
        }
    }
}
