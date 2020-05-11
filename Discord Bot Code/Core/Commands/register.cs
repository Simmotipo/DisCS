using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Reflection;
using System.Linq;

using Discord;
using Discord.Commands;
using System.Globalization;

namespace BirthdayBot.Core.Commands
{
    public class register : ModuleBase<SocketCommandContext>
    {
        [Command("register"), Alias("connect"), Summary("Check Status Command")]
        public async Task registerCommand([Remainder]string Input)
        {
            DisCS.Program.toIP = Convert.ToString(Input);
            Console.WriteLine("Registered connection to IP " + Input);
            await Context.Channel.SendMessageAsync(Context.User.Mention + " registered as receiver.");
        }
    }
}
