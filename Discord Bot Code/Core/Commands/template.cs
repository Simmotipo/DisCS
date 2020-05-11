using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Reflection;

using Discord;
using Discord.Commands;

namespace BirthdayBot.Core.Commands
{
    public class template : ModuleBase<SocketCommandContext>
    {
        [Command("hello"), Alias("hello!", "hi", "hi!", "hey", "hey!"), Summary("Hello Command")]
        public async Task helloCommand()
        {
            string db = (Path.GetDirectoryName(Assembly.GetEntryAssembly().Location)).Replace(@"bin/Debug/netcoreapp2.0", @"Data/Database");
            string logFile = db + "/log.txt";
            if (true)
            {
                File.AppendAllText(logFile, "[S] " + DateTime.UtcNow + ": " + Context.User + " | HelloWorld.cs | User greeted.\n");
                Random rnd = new Random();
                int num = rnd.Next(0, 6);
                if (num == 0) await Context.Channel.SendMessageAsync("Hello, " + Context.User.Mention + "!");
                if (num == 1) await Context.Channel.SendMessageAsync("Hi, " + Context.User.Mention + "!");
                if (num == 2) await Context.Channel.SendMessageAsync("Hey there, " + Context.User.Mention + "!");
                if (num == 3) await Context.Channel.SendMessageAsync("Good day, " + Context.User.Mention + "!");
                if (num == 4) await Context.Channel.SendMessageAsync("It's nice to see you, " + Context.User.Mention + "!");
                if (num == 5) await Context.Channel.SendMessageAsync("Hiya, " + Context.User.Mention + "!");
            }
            else
            {
                File.AppendAllText(logFile, "[D] " + DateTime.UtcNow + ": " + Context.User + " | HelloWorld.cs | Permission Denied.\n");
            }
        }
    }
}
