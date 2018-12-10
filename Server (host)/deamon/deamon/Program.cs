/* Code written by Mike Krop
 * Licenced under GPLv3
 */


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using deamon.Handlers;

namespace ServerTest
{
    class Program
    {
        public static int[] ports = new int[] { 2121, 8383 };
        public static DHCPHandler tcp;
        static void Main(string[] args)
        {
            Console.WriteLine("Daemon Running...");
            CompileArgs(args);
            tcp = new DHCPHandler(ports[0], ports[1]);
            tcp.StartService();
            Console.CancelKeyPress += new ConsoleCancelEventHandler(WantsToExit);
            
        }
        static void WantsToExit(object sender, ConsoleCancelEventArgs args)
        {
            Console.WriteLine(string.Format("Key {0} was pressed, exiting...", args.SpecialKey));
            tcp.Dispose();

        }
        static void CompileArgs(string[] args)
        {
            int remport = 2121;
            int locport = 8383;
            for(int i = 0; i < args.Length; i++)
            {
                switch (args[i])
                {
                    case "--localport":
                        string lp = args[i + 1];
                        locport = Convert.ToInt32(lp);
                        break;
                    case "--remoteport":
                        string rp = args[i + 1];
                        remport = Convert.ToInt32(rp);
                        break;
                }
            }
            ports = new int[] { remport, locport };
        }
        
    }
}
