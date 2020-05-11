using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Reflection;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Net.Sockets;
using System.Linq;

using Discord;
using Discord.Commands;
using System.Globalization;

namespace BirthdayBot.Core.Commands
{
    public class sendMSG : ModuleBase<SocketCommandContext>
    {
        // HttpClient is intended to be instantiated once per application, rather than per-use. See Remarks.
        private static readonly HttpClient client = new HttpClient();
        string s = DisCS.Program.s;
        string toIP = DisCS.Program.toIP;

        [Command("send"), Alias("msg", "message"), Summary("Send Command to Arduino Command")]
        public async Task sendMSGCommand([Remainder]string Input)
        {
            // Call asynchronous network methods in a try/catch block to handle exceptions.
            try
            {
                string host = toIP;
                int port = 80;
                string result = SocketSendReceive(host, port, Input);
                Console.WriteLine(result);
                if (result != null) { await Context.Channel.SendMessageAsync(Context.User.Mention + " Message Delivered"); }
                else { await Context.Channel.SendMessageAsync(Context.User.Mention + " Message Delivery Failed"); }
            }
            catch (HttpRequestException e)
            {
                await Context.Channel.SendMessageAsync(Context.User.Mention + " Message Delivery Failed");
                Console.WriteLine("\nException Caught!");
                Console.WriteLine("Message :{0} ", e.Message);
            }
        }

        private static Socket ConnectSocket(string server, int port)
        {
            Socket s = null;
            //IPHostEntry hostEntry = null;

            // Get host related information.
            //hostEntry = Dns.GetHostEntry(server);

            // Loop through the AddressList to obtain the supported AddressFamily. This is to avoid
            // an exception that occurs when the host IP Address is not compatible with the address family
            // (typical in the IPv6 case).
            IPAddress ipAddress = IPAddress.Parse(DisCS.Program.toIP);
            IPEndPoint ipe = new IPEndPoint(ipAddress, 80);
            Socket tempSocket =
                new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            tempSocket.Connect(ipe);

            if (tempSocket.Connected)
            {
                s = tempSocket;
            }
            return s;
        }

        // This method requests the home page content for the specified server.
        private static string SocketSendReceive(string server, int port, string msg)
        {
            string request = "GET / HTTP/1.1\r\nHost: " + server + "\r\n" + msg + "\r\nConnection: Close\r\n\r\n";
            Byte[] bytesSent = Encoding.ASCII.GetBytes(request);
            Byte[] bytesReceived = new Byte[256];
            string page = "";

            // Create a socket connection with the specified server and port.
            using (Socket s = ConnectSocket(server, port))
            {

                if (s == null)
                    return ("Connection failed");

                // Send request to the server.
                s.Send(bytesSent, bytesSent.Length, 0);

                // Receive the server home page content.
                int bytes = 0;
                page = "Default HTML page on " + server + ":\r\n";

                // The following will block until the page is transmitted.
                do
                {
                    bytes = s.Receive(bytesReceived, bytesReceived.Length, 0);
                    page = page + Encoding.ASCII.GetString(bytesReceived, 0, bytes);
                }
                while (bytes > 0);
            }

            return page;
        }
    }
}