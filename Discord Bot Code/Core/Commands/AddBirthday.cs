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
    public class AddBirthday : ModuleBase<SocketCommandContext>
    {
        string db = (Path.GetDirectoryName(Assembly.GetEntryAssembly().Location)).Replace(@"bin/Debug/netcoreapp2.0", @"Data/Database");
        [Command("aefafa"), Alias("aefafa", "referf", "feafasf"), Summary("Hello Command")]
        public async Task cakeMe([Remainder]string Input = null)
        {
            string db = (Path.GetDirectoryName(Assembly.GetEntryAssembly().Location)).Replace(@"bin/Debug/netcoreapp2.0", @"Data/Database");
            string permissionValue = System.IO.File.ReadAllText(db + "/Servers/" + Context.Guild.Id + "/permissions/cmdBirthday.txt");
            string logFile = db + "/log.txt";
            if (permissionValue == "true")
            {
                if (Context.Message.ToString().ToLower() == "/cake me daddy")
                {
                    await Context.Channel.SendMessageAsync(Context.User.Mention + " ew, no you kinky so-and-so.");
                    File.AppendAllText(logFile, "[S] " + DateTime.UtcNow + ": " + Context.User + " | AddBirthday.cs | Kinky Birthday response sent.\n");
                    return;
                }
                Console.WriteLine("ADD BIRTHDAY COMMAND RECEIVED");
                try
                {
                    if (Input == null)
                    {

                        if (System.IO.File.Exists(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/day.txt"))
                        {
                            string bDay = null;
                            string utcRead = System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/utc.txt");
                            if (Convert.ToInt64(utcRead) >= 0)
                            {
                                bDay = System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/mth.txt") + " " + System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/day.txt") + " UTC+" + utcRead;
                            }
                            else
                            {
                                bDay = System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/mth.txt") + " " + System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/day.txt") + " UTC" + utcRead;
                            }
                            EmbedBuilder Embed = new EmbedBuilder();
                            Embed.WithAuthor(Convert.ToString(Context.User), Context.User.GetAvatarUrl());
                            Embed.WithColor(Color.Blue);
                            Embed.WithFooter("~~~~~~~~~~~~~~~~~", Context.Guild.Owner.GetAvatarUrl());
                            Embed.WithDescription(bDay);
                            await Context.Channel.SendMessageAsync("", false, Embed.Build());
                        }
                        else
                        {
                            await Context.Channel.SendMessageAsync("Hey, " + Context.User.Mention + " - When is your birthday? Use the command '/cake me [DayNumber] [MonthNumber]', and I'll make a note of it <3");
                            EmbedBuilder Embed = new EmbedBuilder();
                            Embed.WithAuthor(Convert.ToString(Context.User), Context.User.GetAvatarUrl());
                            Embed.WithColor(Color.Blue);
                            Embed.WithFooter("~~~~~~~~~~~~~~~~~", Context.Guild.Owner.GetAvatarUrl());
                            Embed.WithDescription("Birthday Unknown...");
                            await Context.Channel.SendMessageAsync("", false, Embed.Build());
                        }
                    }
                    else
                    {

                        int i = 0;
                        try
                        {
                            if (!System.IO.Directory.Exists(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User)))
                            {
                                Console.WriteLine("[REDACTED] - 12/11/19");
                            }
                            if (true)
                            {
                                System.IO.Directory.CreateDirectory(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User));
                                string[] args = Input.Split(" ");
                                System.IO.File.WriteAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/day.txt", Convert.ToString(Convert.ToInt32(args[0])));
                                string mth = DateTimeFormatInfo.CurrentInfo.GetMonthName(Convert.ToInt16(args[1]));
                                System.IO.File.WriteAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/mth.txt", mth);
                                string utc = Convert.ToString(args[2]);
                                int utcCheck = Convert.ToInt32(utc) + 4;
                                Console.WriteLine(utcCheck);
                                try
                                {
                                    System.IO.File.WriteAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/utc.txt", utc);
                                    string[] lines = System.IO.File.ReadAllLines(db + "/" + Convert.ToString(Context.Guild.Id) + "/BB_QRF.txt");
                                    bool okay = true;
                                    if (lines.Contains(Convert.ToString(Context.User)))
                                    {
                                        List<string> newBB = new List<string>();
                                        foreach (string x in lines)
                                        {
                                            if (i < (lines.Count() - 1))
                                            {
                                                if (i > 0)
                                                {
                                                    if (okay)
                                                    {
                                                        if (!x.Equals(Convert.ToString(Context.User)))
                                                        {
                                                            newBB.Add(lines[i - 1]);
                                                            i++;
                                                        }
                                                        else
                                                        {
                                                            i++;
                                                            okay = false;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        i++;
                                                        okay = true;
                                                    }
                                                }
                                                else
                                                {
                                                    i++;
                                                }
                                            }
                                            else
                                            {
                                                if (!x.Equals(Convert.ToString(Context.User)))
                                                {
                                                    newBB.Add(lines[i - 1]);
                                                    newBB.Add(lines[i]);
                                                    i++;
                                                }
                                            }
                                        }
                                        System.IO.File.Delete(db + "/" + Convert.ToString(Context.Guild.Id) + "/BB_QRF.txt");
                                        System.IO.File.WriteAllLines(db + "/" + Convert.ToString(Context.Guild.Id) + "/BB_QRF.txt", newBB);
                                        System.IO.File.AppendAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/BB_QRF.txt", "\n" + Convert.ToString(Convert.ToInt32(args[0])) + " " + mth + " " + utc);
                                        System.IO.File.AppendAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/BB_QRF.txt", "\n" + Convert.ToString(Context.User));
                                        Console.WriteLine("Birthday Updated");
                                    }
                                    else
                                    {
                                        System.IO.File.AppendAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/BB_QRF.txt", "\n" + Convert.ToString(Convert.ToInt32(args[0])) + " " + mth + " " + utc);
                                        System.IO.File.AppendAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/BB_QRF.txt", "\n" + Convert.ToString(Context.User));
                                        Console.WriteLine("Birthday Added");
                                    }
                                    EmbedBuilder Embed = new EmbedBuilder();
                                    Embed.WithAuthor(Convert.ToString(Context.User), Context.User.GetAvatarUrl());
                                    Embed.WithColor(Color.Blue);
                                    Embed.WithFooter("~~~~~~~~~~~~~~~~~", Context.Guild.Owner.GetAvatarUrl());
                                    string bDay = null;
                                    string utcRead = System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/utc.txt");
                                    if (Convert.ToInt64(utcRead) >= 0)
                                    {
                                        bDay = System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/mth.txt") + " " + System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/day.txt") + " UTC+" + utcRead;
                                    }
                                    else
                                    {
                                        bDay = System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/mth.txt") + " " + System.IO.File.ReadAllText(db + "/" + Convert.ToString(Context.Guild.Id) + "/" + Convert.ToString(Context.User) + "/day.txt") + " UTC" + utcRead;
                                    }
                                    Embed.WithDescription(bDay);
                                    await Context.Channel.SendMessageAsync("", false, Embed.Build());
                                    File.AppendAllText(logFile, "[S] " + DateTime.UtcNow + ": " + Context.User + " | AddBirthday.cs | New birthday recorded successfully.\n");
                                    await Context.Channel.SendMessageAsync("All sorted, " + Context.User.Mention + "! If there's a problem, please contact Simmotipo");
                                }
                                catch (Exception err)
                                {
                                    File.AppendAllText(logFile, "[D] " + DateTime.UtcNow + ": " + Context.User + " | AddBirthday.cs | Error occurred. Likely file-saving error, potentially syntax or NaN inputs.\n");
                                    await Context.Channel.SendMessageAsync("I'm sorry, " + Context.User.Mention + " - an error occurred trying to record your birthday. Please ensure you used the correct syntax: '/cake me [DayNumber] [MonthNumber] [UTC Difference]'.");
                                    Console.BackgroundColor = ConsoleColor.DarkRed;
                                    Console.WriteLine(err);
                                    Console.WriteLine(i);
                                    Console.BackgroundColor = ConsoleColor.Black;
                                }
                            }
                            else
                            {
                                await Context.Channel.SendMessageAsync(Context.User.Mention + " - I've already got your birthday, silly!");
                            }
                        }
                        catch (Exception err)
                        {
                            File.AppendAllText(logFile, "[F] " + DateTime.UtcNow + ": " + Context.User + " | AddBirthday.cs | Error occurred. Likely UTC recording error.\n");
                            await Context.Channel.SendMessageAsync("I'm sorry, " + Context.User.Mention + " - an error occurred trying to record your birthday. Please ensure you used the correct syntax: '/cake me [DayNumber] [MonthNumber] [UTC Difference]'.");
                            Console.BackgroundColor = ConsoleColor.DarkRed;
                            Console.WriteLine(err);
                            Console.WriteLine(i);
                            Console.BackgroundColor = ConsoleColor.Black;
                        }

                    }
                }
                catch (Exception e)
                {
                    File.AppendAllText(logFile, "[F] " + DateTime.UtcNow + ": " + Context.User + " | AddBirthday.cs | Error occurred. Likely syntax or NaN input.\n");
                    await Context.Channel.SendMessageAsync("I'm sorry, " + Context.User.Mention + " - an error occurred executing this command. Make sure you're using a public channel (like a 'free-for-all channel). If errors persist, please contact simmotipo.");
                    Console.BackgroundColor = ConsoleColor.DarkRed;
                    Console.WriteLine(e);
                    Console.BackgroundColor = ConsoleColor.Black;
                }
            }
            else
            {
                File.AppendAllText(logFile, "[D] " + DateTime.UtcNow + ": " + Context.User + " | AddBirthday.cs | Permssion denied.\n");
            }
        }

    }
}
