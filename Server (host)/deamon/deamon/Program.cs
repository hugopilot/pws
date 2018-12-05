using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using deamon.Handlers;

namespace ServerTest
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Deamon Running...");
            DHCPHandler tcp = new DHCPHandler();
            tcp.StartService();

        }
    }
}
