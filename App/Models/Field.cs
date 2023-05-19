using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Threading.Tasks;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Overby.Extensions.AsyncBinaryReaderWriter;

namespace App.Models;

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
    private readonly AsyncBinaryReader _asyncBinaryReader;
    private readonly AsyncBinaryWriter _asyncBinaryWriter;
    private readonly Stream _stream;
    
    public List<Cell> Cells { get; set; } = null!;
    private List<Cell> _tmpCells;
    public int Width { get; set; }
    public int Height { get; set; }
    public SortedSet<int> NeighborsForAlive { get; set; } = null!;
    public SortedSet<int> NeighborsForDead { get; set; } = null!;

    private static Field? _instanse = null;

    public static async Task<Field> GetInstanse()
    {
        return _instanse ??= await BuildFieldAsync();
    }
    
    private static async Task<Field> BuildFieldAsync()
    {
        Console.WriteLine("Here!");
        await TcpClient.ConnectAsync("127.0.0.1", 8888);
        Console.WriteLine($"connected: {TcpClient.Connected}");
        var field = new Field();
        Console.WriteLine($"Ready");
        return field;
    }

    private Field()
    {
        _stream = TcpClient.GetStream();
        _binaryReader = new BinaryReader(_stream);
        _binaryWriter = new BinaryWriter(_stream);
        _asyncBinaryReader = new AsyncBinaryReader(_stream);
        _asyncBinaryWriter = new AsyncBinaryWriter(_stream);
        _tmpCells = new List<Cell>();
        _binaryWriter.Write((int)RequestType.RandomFill); // tmp
        ReadField();
    }

    public void CalculateNextGeneration()
    {
        _binaryWriter.Write((int)RequestType.NextGeneration);
        //SendField();

        try
        {
            ReadField();
        }
        catch (IOException e)
        {
            Console.WriteLine(e);
        }
    }
    
    public async void CalculateNextGenerationAsync()
    {
        await _asyncBinaryWriter.WriteAsync((int)RequestType.NextGeneration);
        //SendField();

        try
        {
            ReadFieldAsync();
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
        var i = 0;
        
        foreach (var cell in Cells)
        {
            if (i == 20000)
                bw.Flush();
            bw.Write((int)cell.CurrentColor);
            bw.Write((long)cell.Longevity);
            i++;
            if (i % 1000 == 0)
                Console.WriteLine(i);
        }
    }
    
    private async void SendSizeAsync(AsyncBinaryWriter bw, int w, int h)
    {
        await bw.WriteAsync(Width);
        await bw.WriteAsync(Height);
    }

    private async void SendNeighborsAsync(AsyncBinaryWriter bw)
    {
        await bw.WriteAsync(NeighborsForAlive.Count);
        foreach (var i in NeighborsForAlive)
            await bw.WriteAsync(i);
        
        await bw.WriteAsync(NeighborsForDead.Count);
        foreach (var i in NeighborsForDead)
            await bw.WriteAsync(i);
    }

    private async void SendCellsAsync(AsyncBinaryWriter bw)
    {
        var i = 0;
        
        foreach (var cell in Cells)
        {
            await bw.WriteAsync((int)cell.CurrentColor);
            await bw.WriteAsync((long)cell.Longevity);
            i++;
            if (i % 1000 == 0)
                Console.WriteLine(i);
        }
    }

    private void SendField()
    {
        SendSize(_binaryWriter, Width, Height);
        SendNeighbors(_binaryWriter);
        SendCells(_binaryWriter);
        _binaryWriter.Flush();
    }
    
    private async void SendFieldAsync()
    {
        SendSizeAsync(_asyncBinaryWriter, Width, Height);
        SendNeighborsAsync(_asyncBinaryWriter);
        SendCellsAsync(_asyncBinaryWriter);
    }
    
    public void ResizeField(int w, int h)
    {
        _binaryWriter.Write((int)RequestType.ResizeField);
        SendSize(_binaryWriter, w, h);
        ReadCells(_binaryReader, w, h);
        Width = w;
        Height = h;
        Cells = _tmpCells;
        _tmpCells.EnsureCapacity(w * h);
    }
    
    public void ResizeFieldAsync(int w, int h)
    {
        _asyncBinaryWriter.WriteAsync((int)RequestType.ResizeField);
        SendSizeAsync(_asyncBinaryWriter, w, h);
        ReadCellsAsync(_asyncBinaryReader, w, h);
        Width = w;
        Height = h;
        Cells = _tmpCells;
        _tmpCells.EnsureCapacity(w * h);
    }
    
    private void ReadField()
    {
        Console.WriteLine("Start Reading"); // TODO
        var (w, h) = ReadSize(_binaryReader);

        if (w * h > _tmpCells.Capacity)
            _tmpCells.EnsureCapacity(w * h);

        var (alive, dead) = ReadNeighbors(_binaryReader);

        Console.WriteLine("Reading cells"); // TODO
        ReadCells(_binaryReader, w, h);

        Console.WriteLine("writing results"); // TODO
        Width = w;
        Height = h;
        Cells = _tmpCells;
        NeighborsForAlive = alive;
        NeighborsForDead = dead;
    }
    
    private async void ReadFieldAsync()
    {
        Console.WriteLine("Start Reading"); // TODO
        var (w, h) = await ReadSizeAsync(_asyncBinaryReader);

        if (w * h > _tmpCells.Capacity)
            _tmpCells.EnsureCapacity(w * h);

        var (alive, dead) = await ReadNeighborsAsync(_asyncBinaryReader);

        Console.WriteLine("Reading cells"); // TODO
        ReadCellsAsync(_asyncBinaryReader, w, h);

        Console.WriteLine("writing results"); // TODO
        Width = w;
        Height = h;
        Cells = _tmpCells;
        NeighborsForAlive = alive;
        NeighborsForDead = dead;
    }
    
    private static (int, int) ReadSize(BinaryReader br)
    {
        var w = br.ReadInt32();
        var h = br.ReadInt32();

        return (w, h);
    }

    private static (SortedSet<int> alive, SortedSet<int> dead) ReadNeighbors(BinaryReader br)
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

    private void ReadCells(BinaryReader br, int w, int h)
    {
        for (var i = 0; i < w * h; i++)
        {
            var color = (Cell.Color)br.ReadInt32();
            var longevity = br.ReadUInt64();
            if (i >= _tmpCells.Count)
                _tmpCells.Add(new Cell(color, longevity));
            else
            {
                _tmpCells[i].CurrentColor = color;
                _tmpCells[i].Longevity = longevity;
            }
        }
    }
    
    private static async Task<(int, int)> ReadSizeAsync(AsyncBinaryReader br)
    {
        var w = await br.ReadInt32Async();
        var h = await br.ReadInt32Async();

        return (w, h);
    }

    private static async Task<(SortedSet<int> alive, SortedSet<int> dead)> ReadNeighborsAsync(AsyncBinaryReader br)
    {
        var aliveNeighbour = await br.ReadInt32Async();
        var alive = new SortedSet<int>();
        if (alive == null) throw new ArgumentNullException(nameof(alive));
        
        for (var i = 0; i < aliveNeighbour; i++)
            alive.Add(await br.ReadInt32Async());
        
        var deadNeighbour = await br.ReadInt32Async();
        var dead = new SortedSet<int>();
        if (dead == null) throw new ArgumentNullException(nameof(dead));
        
        for (var i = 0; i < deadNeighbour; i++)
            dead.Add(await br.ReadInt32Async());

        return (alive, dead);
    }

    private async void ReadCellsAsync(AsyncBinaryReader br, int w, int h)
    {
        for (var i = 0; i < w * h; i++)
        {
            var color = (Cell.Color)await br.ReadInt32Async();
            var longevity = await br.ReadUInt64Async();
            if (i >= _tmpCells.Count)
                _tmpCells.Add(new Cell(color, longevity));
            else
            {
                _tmpCells[i].CurrentColor = color;
                _tmpCells[i].Longevity = longevity;
            }
        }
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