using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace TcpUdpTwo
{
    class Program
    {
        static void TcpClient()
        {
            const string ip = "127.0.0.1";
            const int port = 7777;

            IPEndPoint tcpEndPoint = new(IPAddress.Parse(ip), port);
            Socket tcpSocket = new(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            Console.WriteLine("Введите сообщение:");
            string mes = Console.ReadLine();
            byte[] data = Encoding.UTF8.GetBytes(mes);

            tcpSocket.Connect(tcpEndPoint);
            tcpSocket.Send(data);

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

            tcpSocket.Shutdown(SocketShutdown.Both);
            tcpSocket.Close();

            Console.ReadLine();
        }

        static void UdpClient()
        {
            const string ip = "127.0.0.1";
            const int port = 9999;

            var udpEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            var udpSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            udpSocket.Bind(udpEndPoint);

            while (true)
            {
                Console.WriteLine("Ваше сообщение: ");
                var mes = Console.ReadLine();
                udpSocket.Send(Encoding.UTF8.GetBytes(mes));


            }
        }
        static void Main(string[] args)
        {
            TcpClient();
        }
    }
}
