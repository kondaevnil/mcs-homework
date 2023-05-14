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
        LoadField,
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

    public void LoadField()
    {
        
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
}