using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace TcpUdpOne
{
    class Program
    {
        static void TcpServer()
        {
            const string ip = "127.0.0.1";
            const int port = 7777;

            IPEndPoint tcpEndPoint = new(IPAddress.Parse(ip), port);
            Socket tcpSocket = new(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            tcpSocket.Bind(tcpEndPoint);
            tcpSocket.Listen(5);

            while (true)
            {
                Socket listener = tcpSocket.Accept();
                byte[] buffer = new byte[4096];
                int size = 0, sum = 0, k = 0;
                var data = new StringBuilder();

                //Console.Read();
                Console.WriteLine(listener.Connected ? "Connected" : "Disconnected");
                var local = (listener.LocalEndPoint as IPEndPoint);
                var remote = (listener.RemoteEndPoint as IPEndPoint);
                Console.WriteLine($"local: {local.Address} {local.Port}");
                Console.WriteLine($"remote: {remote.Address} {remote.Port}");

                do
                {
                    size = listener.Receive(buffer);
                    sum += size;
                    k++;
                    data.Append(Encoding.UTF8.GetString(buffer, 0, size));
                }
                while (listener.Available > 0);

                Console.WriteLine($"{data} -> {sum} {k}");

                listener.Send(Encoding.UTF8.GetBytes("Получил".ToUpper()));
                listener.Shutdown(SocketShutdown.Both);
                listener.Close();
            }
        }

        static void UdpServer()
        {
            const string ip = "127.0.0.1";
            const int port = 8888;

            var udpEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            var udpSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            udpSocket.Bind(udpEndPoint);

            while (true)
            {
                var buf = new byte[256];
                var size = 0;
                var data = new StringBuilder();
                EndPoint senderEndPoint = new IPEndPoint(IPAddress.Any, 0);

                do
                {
                    size = udpSocket.ReceiveFrom(buf, ref senderEndPoint);
                    data.Append(Encoding.UTF8.GetString(buf));
                }
                while (udpSocket.Available > 0);

                udpSocket.SendTo(Encoding.UTF8.GetBytes("Сообщение получено"), senderEndPoint);
                udpSocket.Shutdown(SocketShutdown.Both);
                Console.WriteLine(data);
            }
        }

        static void Main(string[] args)
        {
            TcpServer();
        }
    }
}
