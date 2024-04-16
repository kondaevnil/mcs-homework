using System.Net;
using System.Net.Sockets;

namespace Server;

public static class Program
{
    static async Task Main()
    {
        var server = Server.GetInstance();
        await server.Start();
    }
}