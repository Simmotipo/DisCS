using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Reflection;
using System.Threading.Tasks;
using System.Net.Http;

using Discord;
using Discord.Commands;
using Discord.WebSocket;

namespace DisCS
{
    public class Program
    {
        private static readonly HttpClient client = new HttpClient();
        private DiscordSocketClient Client;
        private CommandService Commands;
        private static string _toIP = "";
        private static string _s = "/";
        public static string logFile = (Path.GetDirectoryName(Assembly.GetEntryAssembly().Location)).Replace(@"bin/Debug/netcoreapp2.0", @"") + "log.txt";
        public static string msgFile = (Path.GetDirectoryName(Assembly.GetEntryAssembly().Location)).Replace(@"bin/Debug/netcoreapp2.0", @"") + "msg.txt";
        public static List<long[]> activeComms = new List<long[]>();
        public static List<long[]> awaitingHelp = new List<long[]>();
        public static string s
        {
            get
            {
                return _s;
            }
            set
            {
                _s = value;
            }
        }
        public static string toIP
        {
            get
            {
                return _toIP;
            }
            set
            {
                _toIP = value;
            }
        }

        static void Main(string[] args)
            => new Program().MainAsync().GetAwaiter().GetResult();
        private async Task MainAsync()
        {
            Client = new DiscordSocketClient(new DiscordSocketConfig
            {
                LogLevel = LogSeverity.Debug,
                AlwaysDownloadUsers = true,
            });

            Commands = new CommandService(new CommandServiceConfig
            {
                CaseSensitiveCommands = false,
                DefaultRunMode = RunMode.Async,
                LogLevel = LogSeverity.Debug
            });
            Client.MessageReceived += Client_MessageReceived;
            await Commands.AddModulesAsync(Assembly.GetEntryAssembly(),null);

            Client.Ready += Client_Ready;
            Client.Log += Client_Log;

            int q = 0;
            string Token = "";
            try
            {
                Token = "NjkyNzYwOTA4MjIwNTk2MzI0.XnzQyw.a8sEumuYOHMo_QUA2nQyQKtOLsc";
                /*Token = System.IO.File.ReadAllText((Path.GetDirectoryName(Assembly.GetEntryAssembly().Location)).Replace(@"bin" + s + "Debug" + s + "netcoreapp2.0", @"Data" + s + "Tkn.txt"));
                q = q + 1;
                File.AppendAllText(logFile, "[S] " + DateTime.UtcNow + ": " + "[SYSTEM]" + " | program.cs | Token fetched.\n");
                Console.WriteLine("Token fetched from file");*/
            }
            catch (Exception e)
            {
                File.AppendAllText(logFile, "[F] " + DateTime.UtcNow + ": " + "[SYSTEM]" + " | program.cs | Unable to retreive token\n");
            }
            await Client.LoginAsync(TokenType.Bot, Token);
            await Client.StartAsync();
            await Task.Delay(-1);
        }

        private async Task Client_Log(LogMessage message)
        {
            Console.WriteLine($"{DateTime.Now} at {message.Source}] {message.Message}");
        }

        private async Task Client_Ready()
        {
            await Client.SetGameAsync("something, probably.", "", ActivityType.Playing);
            string loc = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location).Replace(@"bin" + s + "Debug" + s + "netcoreapp2.0", @"Data" + s + "Database" + s + "Servers" + s);
            var guildList = Client.Guilds;
        }

        private async Task Client_MessageReceived(SocketMessage command)
        {
            try
            {
                //Initiate Task
                var Message = command as SocketUserMessage;
                var Context = new SocketCommandContext(Client, Message);

                //Proceed with Command Execution
                if (Context.User.IsBot && Context.User.Username != "Alan Arduino") return;
                int ArgPos = 0;
                if (Message.HasStringPrefix("/disc ", ref ArgPos) || Message.HasStringPrefix("!disc ", ref ArgPos) || Message.HasMentionPrefix(Client.CurrentUser, ref ArgPos))
                {
                    Console.WriteLine("Received command: " + command);
                    var Result = await Commands.ExecuteAsync(Context, ArgPos, null);
                    if (!Result.IsSuccess)
                    {
                        Console.WriteLine($"{DateTime.Now} at Commands] Something went wrong executing a command. Text: {Context.Message.Content} | Error: {Result.ErrorReason}");
                    }
                }
                else { Console.WriteLine("Received message: " + command); }
            }
            catch(Exception e)
            {
                Console.BackgroundColor = ConsoleColor.DarkRed;
                Console.WriteLine(e);
                Console.BackgroundColor = ConsoleColor.Black;
            }
        }
    }
}
