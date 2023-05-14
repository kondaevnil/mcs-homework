using System.Net.Sockets;

namespace tmpClient;

public static class Program
{
    private enum RequestType
    {
        NextGeneration,
        ResizeField,
        SetColor,
        RandomFill,
    }

    private class Cell
    {
        public enum Color
        {
            Dead, Alive,
        }

        public Color CurrentColor;
        public ulong Longevity;

        public Cell(Color color, ulong longevity)
        {
            Longevity = longevity;
            CurrentColor = color;
        }
    }

    private static async Task Main()
    {
        using var tcpClient = new TcpClient();
        await tcpClient.ConnectAsync("127.0.0.1", 8888);
        var stream = tcpClient.GetStream();
        var bw = new BinaryWriter(stream);
        bw.Write((int)RequestType.ResizeField);
        bw.Write(400);
        bw.Write(400);
        
        var br = new BinaryReader(stream);
        ReadField(br);
    }

    private static void ReadField(BinaryReader br)
    {
        var w = br.ReadInt32();
        var h = br.ReadInt32();
        
        var aliveNeighbour = br.ReadInt32();
        var alive = new SortedSet<int>();
        for (var i = 0; i < aliveNeighbour; i++)
            alive.Add(br.ReadInt32());
        
        var deadNeighbour = br.ReadInt32();
        var dead = new SortedSet<int>();
        for (var i = 0; i < deadNeighbour; i++)
            dead.Add(br.ReadInt32());

        var field = new List<Cell>(w * h);
        for (var i = 0; i < w * h; i++)
        {
            var color = (Cell.Color)br.ReadInt32();
            var longevity = br.ReadUInt64();
            field.Add(new Cell(color, longevity));
            Console.WriteLine(i);
        }

        Console.WriteLine("Ok!");
    }
}