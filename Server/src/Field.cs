

namespace Server;

internal class Field
{
    internal class Cell
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

    public Field()
    {
        Width = 0;
        Height = 0;
        Cells = new List<Cell>();
        Resize(1000, 1000);
        NeighborsForAlive = new SortedSet<int> {2, 3};
        NeighborsForDead = new SortedSet<int> {3};
    }
    
    public List<Cell> Cells { get; set; }
    public int Width { get; set; }
    public int Height { get; set; }
    public SortedSet<int> NeighborsForAlive { get; set; }
    public SortedSet<int> NeighborsForDead { get; set; }

    public void GenerateRandom()
    {
        var rnd = new Random();

        for (var i = 0; i < Height * Width; i++)
        {
            var color = rnd.Next() % 2 == 0 ? Cell.Color.Dead : Cell.Color.Alive;
            Cells[i] = new Cell(color, 0);
        }
    }
    
    public void Resize(int width, int height)
    {
        var tmpList = new List<Cell>(width * height);

        for (var i = 0; i < height; i++)
        {
            for (var j = 0; j < width; j++)
            {
                if (i < Height && j < Width)
                    tmpList.Add(Cells[i * width + j]);
                else
                    tmpList.Add(new Cell(Cell.Color.Dead, 0));
            }
        }

        Cells = tmpList;
        Width = width;
        Height = height;
    }
    
    public void ChangeCellColor(int posX, int posY, Cell.Color color)
    {
        Cells[posY * Width + posX].CurrentColor = color;
    }
    
    private void CellFate(int posX, int posY)
    {
        var livingCount = 0;
        var currentCell = Cells[Width * posY + posX];

        // now only realization for 2 colors
        for (var i = -1; i < 2; i++)
        {
            for (var j = -1; j < 2; j++)
                livingCount += Cells[Width * (i + Height + posY) % Height + (posX + j + Width) % Width].CurrentColor == Cell.Color.Alive ? 1 : 0;
        }

        if (currentCell.CurrentColor != Cell.Color.Dead)
        {
            livingCount--;

            if (!NeighborsForAlive.Contains(livingCount))
            {
                Cells[Width * posY + posX].CurrentColor = Cell.Color.Dead;
                Cells[Width * posY + posX].Longevity = 0;
            }
            else
                Cells[Width * posY + posX].Longevity++;
        }
        else if (NeighborsForDead.Contains(livingCount))
        {
            Cells[Width * posY + posX].CurrentColor = Cell.Color.Alive;
            Cells[Width * posY + posX].Longevity = 0;
        }
        else
            Cells[Width * posY + posX].Longevity++;
    }
    
    public void NextGeneration()
    {
        for (var j = 0; j < Height; j++)
            for (var i = 0; i < Width; i++)
                CellFate(i, j);
    }
}