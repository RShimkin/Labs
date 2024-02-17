using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Client
{
    class Program
    {
        static void Client(string ip, int tcpPort, int udpPort, string fname, int timeout)
        {
            IPEndPoint remoteTcpEP = new(IPAddress.Parse(ip), tcpPort);
            Socket tcpSocket = new(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            byte[] buf = new byte[256];

            tcpSocket.Connect(remoteTcpEP);
            tcpSocket.Receive(buf);
            Console.WriteLine(buf[0]);

            var fnbytes = Encoding.UTF8.GetBytes(fname);
            tcpSocket.Send(fnbytes);

            IPEndPoint udpEndPoint = new(IPAddress.Parse(ip), udpPort);
            Socket udpSocket = new(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            udpSocket.Connect(udpEndPoint);
            var testwords = new string[]
            {
                "First",
                "Second",
                "third"
            };
            foreach (var word in testwords)
            {
                udpSocket.Send(Encoding.UTF8.GetBytes(word));
            }
            Console.WriteLine("Finished");
            /*
            string mes = Console.ReadLine();
            byte[] data = Encoding.UTF8.GetBytes(mes);

            byte[] buffer = new byte[256];
            int size = 0;
            var answer = new StringBuilder();

            do
            {
                size = tcpSocket.Receive(buffer);
                answer.Append(Encoding.UTF8.GetString(buffer, 0, size));
            }
            while (tcpSocket.Available > 0);

            Console.WriteLine(answer);
            */

            tcpSocket.Shutdown(SocketShutdown.Both);
            tcpSocket.Close();

            Console.ReadLine();
        }
        static void Main(string[] args)
        {
            Client(args[0], int.Parse(args[1]), int.Parse(args[2]), args[3], int.Parse(args[4]));
        }
    }
}
