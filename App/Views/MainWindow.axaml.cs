using System;
using System.IO;
using App.Models;
using App.ViewModels;
using Avalonia.Controls;
using Avalonia.Controls.Shapes;
using Avalonia.Media;
using Avalonia.Utilities;
using SkiaSharp;

namespace App.Views;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
        MainWindowViewModel.MyFunctionTriggered += RenderFieldEventHandler;
    }
    
    private void RenderFieldEventHandler(object? sender, EventArgs e)
    {
        RenderField();
    }
    
    public void RenderField()
    {
        const int canvasWidth = 500;
        const int canvasHeight = 500;
        
        var imageInfo = new SKImageInfo(canvasWidth, canvasHeight);
        using var surface = SKSurface.Create(imageInfo);
        var canvas = surface.Canvas;
        canvas.DrawColor(SKColors.Black);
        canvas.Clear(SKColors.Black);
        using (var paint = new SKPaint())
        {
            paint.Color = SKColors.Blue;
            paint.IsAntialias = true;
            paint.StrokeWidth = 15;
            paint.Style = SKPaintStyle.Stroke;
            var w = MainWindowViewModel.MyField.Result.Width;
            var h = MainWindowViewModel.MyField.Result.Height;
            var diffX = canvasWidth / (float)w; // TODO
            var diffY = canvasHeight / (float)h; // TODO
            
            for (var y = 0; y < h; y++)
            {
                for (var x = 0; x < w; x++)
                {
                    if (MainWindowViewModel.MyField.Result.Cells[w * y + x].CurrentColor == Field.Cell.Color.Alive)
                        canvas.DrawRect(x * diffX, y * diffY, diffX, diffY, new SKPaint
                        {
                            Color = SKColors.Blue,
                            Style = SKPaintStyle.Fill,
                        });
                }
            }
        }
        using (var image = surface.Snapshot())
        using (var data = image.Encode(SKEncodedImageFormat.Png, 100))
        using (var mStream = new MemoryStream(data.ToArray()))
        {
            var b = new Avalonia.Media.Imaging.Bitmap(mStream);
            var cv = this.FindControl<Canvas>("Canvas");
            cv.Width = canvasWidth;
            cv.Height = canvasHeight;
            var x = new ImageBrush(b);
            var rect = new Rectangle
            {
                Width = cv.Width,
                Height = cv.Height,
                Fill = x,
                Stroke = Brushes.Black,
                StrokeThickness = 2
            };
            cv.Children.Add(rect);
        }
    }
}