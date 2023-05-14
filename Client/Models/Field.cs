using System;
using System.Collections.Generic;
using System.IO;
using System.Net.NetworkInformation;
using System.Net.Sockets;

namespace Client.Models;

public class Field
{
    public class Cell
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

    private static readonly TcpClient TcpClient = new();
    private readonly BinaryReader _binaryReader;
    private readonly BinaryWriter _binaryWriter;
    private readonly Stream _stream;
    
    public List<Cell> Cells { get; set; } = null!;
    public int Width { get; set; }
    public int Height { get; set; }
    public SortedSet<int> NeighborsForAlive { get; set; } = null!;
    public SortedSet<int> NeighborsForDead { get; set; } = null!;

    public Field()
    {
        try
        {
            TcpClient.ConnectAsync("127.0.0.1", 8888);
            _stream = TcpClient.GetStream();
            _binaryReader = new BinaryReader(_stream);
            _binaryWriter = new BinaryWriter(_stream);
            _binaryWriter.Write((int)RequestType.RandomFill); // tmp
            ReadField();
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
            throw;
        }
    }

    public void CalculateNextGeneration()
    {
        _binaryWriter.Write((int)RequestType.NextGeneration);
        SendField();

        try
        {
            ReadField();
        }
        catch (IOException e)
        {
            Console.WriteLine(e);
        }
    }

    public void GenerateRandom()
    {
        _binaryWriter.Write((int)RequestType.RandomFill);
        try
        {
            ReadCells(_binaryReader, Width, Height);
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
        }
    }
    
    private enum RequestType
    {
        NextGeneration,
        ResizeField,
        RandomFill,
        UploadField,
        DownloadField,
        ChangeCellColor,
    }
    
    private void SendSize(BinaryWriter bw, int w, int h)
    {
        bw.Write(Width);
        bw.Write(Height);
    }

    private void SendNeighbors(BinaryWriter bw)
    {
        bw.Write(NeighborsForAlive.Count);
        foreach (var i in NeighborsForAlive)
            bw.Write(i);
        
        bw.Write(NeighborsForDead.Count);
        foreach (var i in NeighborsForDead)
            bw.Write(i);
    }

    private void SendCells(BinaryWriter bw)
    {
        foreach (var cell in Cells)
        {
            bw.Write((int)cell.CurrentColor);
            bw.Write((long)cell.Longevity);
        }
    }

    private void SendField()
    {
        SendSize(_binaryWriter, Width, Height);
        SendNeighbors(_binaryWriter);
        SendCells(_binaryWriter);
        _binaryWriter.Flush();
    }
    
    public void ResizeField(int w, int h)
    {
        _binaryWriter.Write((int)RequestType.ResizeField);
        SendSize(_binaryWriter, w, h);
        ReadCells(_binaryReader, w, h);
        Width = w;
        Height = h;
    }
    
    private void ReadField()
    {
        var w = _binaryReader.ReadInt32();
        var h = _binaryReader.ReadInt32();
        
        var aliveNeighbour = _binaryReader.ReadInt32();
        var alive = new SortedSet<int>();
        if (alive == null) throw new ArgumentNullException(nameof(alive));
        
        for (var i = 0; i < aliveNeighbour; i++)
            alive.Add(_binaryReader.ReadInt32());
        
        var deadNeighbour = _binaryReader.ReadInt32();
        var dead = new SortedSet<int>();
        if (dead == null) throw new ArgumentNullException(nameof(dead));
        
        for (var i = 0; i < deadNeighbour; i++)
            dead.Add(_binaryReader.ReadInt32());

        var cells = new List<Cell>(w * h);
        if (cells == null) throw new ArgumentNullException(nameof(cells));
        for (var i = 0; i < w * h; i++)
        {
            var color = (Cell.Color)_binaryReader.ReadInt32();
            var longevity = _binaryReader.ReadUInt64();
            cells.Add(new Cell(color, longevity));
            Console.WriteLine(i);
        }

        Width = w;
        Height = h;
        Cells = cells;
        NeighborsForAlive = alive;
        NeighborsForDead = dead;
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
    
    public void SaveFieldStateLocally(string path, string filename)
    {
        var exactPath = Path.Combine(Path.GetFullPath(path), filename);

        using var bw = new BinaryWriter(File.Open(exactPath, FileMode.Create));
        bw.Write(Width);
        bw.Write(Height);
        
        bw.Write(NeighborsForAlive.Count);
        foreach (var n in NeighborsForAlive)
            bw.Write(n);
        
        bw.Write(NeighborsForDead.Count);
        foreach (var n in NeighborsForDead)
            bw.Write(n);

        for (var i = 0; i < Width * Height; i++)
        {
            bw.Write((int)Cells[i].CurrentColor);
            bw.Write(Cells[i].Longevity);
        }
    }

    public void LoadFieldStateLocally(string path, string filename)
    {
        var exactPath = Path.Combine(Path.GetFullPath(path), filename);

        if (!File.Exists(exactPath))
            throw new FileNotFoundException($"File {filename} doesn't exist");

        using var br = new BinaryReader(File.Open(exactPath, FileMode.Open));
        
        
        // need to handle IO exceptions
        var width = br.ReadInt32();
        var height = br.ReadInt32();
        var cells = new List<Cell>(width * height);
        
        var aliveCount = br.ReadInt32();
        var neighborsForAlive = new SortedSet<int>();

        for (var i = 0; i < aliveCount; i++)
            neighborsForAlive.Add(br.ReadInt32());
        
        var deadCount = br.ReadInt32();
        var neighborsForDead = new SortedSet<int>();

        for (var i = 0; i < deadCount; i++)
            neighborsForDead.Add(br.ReadInt32());

        for (var i = 0; i < width * height; i++)
        {
            var color = (Cell.Color)br.ReadInt32();
            var longevity = br.ReadUInt64();
            
            cells.Add(new Cell(color, longevity));
        }

        Width = width;
        Height = height;
        NeighborsForAlive = neighborsForAlive;
        NeighborsForDead = neighborsForDead;
        Cells = cells;
    }
    
    public void CacheLastGame()
    {
        var sep = Path.DirectorySeparatorChar;
        var path = $".{sep}Cache{sep}";
        const string lastGameFileName = "LastGame.lf";

        if (!Directory.Exists(path))
            Directory.CreateDirectory(path);
        SaveFieldStateLocally(path, lastGameFileName);
    }

    public bool LoadLastGame()
    {
        var sep = Path.DirectorySeparatorChar;
        var path = $".{sep}Cache{sep}";
        const string lastGameFileName = "LastGame.lf";

        if (!Directory.Exists(path)) return false;
        LoadFieldStateLocally(path, lastGameFileName);
        SendField();
        return true;
    }

    public void ChangeCellColor(int x, int y, Cell.Color newColor)
    {
        _binaryWriter.Write((int)RequestType.ChangeCellColor);
        _binaryWriter.Write(x);
        _binaryWriter.Write(y);
        _binaryWriter.Write((int)newColor);
        // TODO CHECK IF RESULT OK
    }
}