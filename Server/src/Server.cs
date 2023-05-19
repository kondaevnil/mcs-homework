using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using static Server.Field;

using System.Net.NetworkInformation;

namespace Server;

public class Server
{
    private static Server? _instance;
    private TcpListener _tcpListener;
    private bool _isRunning;
    private Field? _field;

    private Server()
    {
        _tcpListener = new TcpListener(IPAddress.Any, 8888); 
        _isRunning = false;
        // connect to database
        
    }
 
    public static Server GetInstance()
    {
        return _instance ??= new Server();
    }

    public async Task Start()
    {
        try
        {
            _tcpListener.Start();
            // Is it really should be here?
            _field = new Field();
            _isRunning = true;

            using var tcpClient = await _tcpListener.AcceptTcpClientAsync();
            Console.WriteLine("Connected");
            var stream = tcpClient.GetStream();
            var br = new BinaryReader(stream);
            var bw = new BinaryWriter(stream);

            while (_isRunning)
            {
                var request = (RequestType)br.ReadInt32();
                Console.WriteLine($"Got request {request}");

                switch (request)
                {
                    case RequestType.NextGeneration:
                        _field.NextGeneration();
                        Console.WriteLine("Calculated");
                        SendField(bw);
                        // should send all field
                        break;
                    case RequestType.ResizeField:
                        try
                        {
                            HandleResizeField(br);
                            // send ok status
                            
                            // should send only new field size
                            SendField(bw);
                        }
                        catch (IOException e)
                        {
                            // send error status
                            // TODO 
                        }
                        break;
                    case RequestType.RandomFill:
                        // should send all field
                        _field.GenerateRandom();
                        SendField(bw);
                        break;
                    case RequestType.UploadField:
                        try
                        {
                            HandleReadField(br);
                        }
                        catch (IOException e)
                        {
                            // Send error
                            // TODO
                        }
                        break;
                    case RequestType.ChangeCellColor:
                        HandleCellColor(br);
                        break;
                    case RequestType.DownloadField:
                        // TODO
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }
        finally
        {
            _tcpListener.Stop();
        }
    }

    // Client request
    private enum RequestType
    {
        NextGeneration,
        ResizeField,
        RandomFill,
        UploadField,
        DownloadField,
        ChangeCellColor,
    }

    private void SendField(BinaryWriter bw)
    {
        SendSize(bw);
        Console.WriteLine("Send Size"); // TODO
        SendNeighbors(bw);
        Console.WriteLine("Send Neighbors"); // TODO
        SendCells(bw);
        Console.WriteLine("Send Cells"); // TODO
        bw.Flush();
        Console.WriteLine("Has sent!");
    }

    private void SendSize(BinaryWriter bw)
    {
        bw.Write(_field!.Width);
        bw.Write(_field!.Height);
    }

    private void SendNeighbors(BinaryWriter bw)
    {
        bw.Write(_field!.NeighborsForAlive.Count);
        foreach (var i in _field.NeighborsForAlive)
            bw.Write(i);
        
        bw.Write(_field.NeighborsForDead.Count);
        foreach (var i in _field.NeighborsForDead)
            bw.Write(i);
    }

    private void SendCells(BinaryWriter bw)
    {
        foreach (var cell in _field!.Cells)
        {
            bw.Write((int)cell.CurrentColor);
            bw.Write((long)cell.Longevity);
        }
    }
    private void HandleResizeField(BinaryReader br)
    {
        var width = br.ReadInt32();
        var height = br.ReadInt32();
        _field!.Resize(width, height);
    }
    
    private void HandleReadField(BinaryReader br)
    {
        var (w, h) = ReadSize(br);
        var (alive, dead) = ReadNeighbors(br);
        var cells = ReadCells(br, w, h);

        _field!.Width = w;
        _field!.Height = h;
        _field!.Cells = cells;
        _field!.NeighborsForAlive = alive;
        _field!.NeighborsForDead = dead;
    }

    private void HandleCellColor(BinaryReader br)
    {
        var x = br.ReadInt32();
        var y = br.ReadInt32();
        var color = (Cell.Color)br.ReadInt32();

        if (x >= 0 && x <= _field!.Width && y >= 0 && y <= _field.Height)
            _field.ChangeCellColor(x, y, color);
    }

    private static (int, int) ReadSize(BinaryReader br)
    {
        var w = br.ReadInt32();
        var h = br.ReadInt32();

        return (w, h);
    }

    private static (SortedSet<int>, SortedSet<int>) ReadNeighbors(BinaryReader br)
    {
        var aliveNeighbour = br.ReadInt32();
        var alive = new SortedSet<int>();
        if (alive == null) throw new ArgumentNullException(nameof(alive));
        
        for (var i = 0; i < aliveNeighbour; i++)
            alive.Add(br.ReadInt32());
        
        var deadNeighbour = br.ReadInt32();
        var dead = new SortedSet<int>();
        if (dead == null) throw new ArgumentNullException(nameof(dead));
        
        for (var i = 0; i < deadNeighbour; i++)
            dead.Add(br.ReadInt32());

        return (alive, dead);
    }

    private static List<Cell> ReadCells(BinaryReader br, int w, int h)
    {
        var cells = new List<Cell>(w * h);
        if (cells == null) throw new ArgumentNullException(nameof(cells));
        for (var i = 0; i < w * h; i++)
        {
            var color = (Cell.Color)br.ReadInt32();
            var longevity = br.ReadUInt64();
            cells.Add(new Cell(color, longevity));
            Console.WriteLine(i);
        }

        return cells;
    }
}