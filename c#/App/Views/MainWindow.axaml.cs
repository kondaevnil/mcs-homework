using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Threading.Tasks;
using App.Models;
using App.ViewModels;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Primitives;
using Avalonia.Controls.Shapes;
using Avalonia.Input;
using Avalonia.Interactivity;
using Avalonia.Media;
using MessageBox.Avalonia.Enums;
using SkiaSharp;

namespace App.Views;

public partial class MainWindow : Window
{
    private const int CanvasWidth = 700;
    private const int CanvasHeight = 700;
    private float _zoom = 1;
    private float _shiftX = 0;
    private float _shiftY = 0;

    private Canvas _canvasControl;
    private Label _cursorLabel;
    private StackPanel _checkBoxes;
    private List<CheckBox> _checkBoxesList;

    public MainWindow()
    {
        InitializeComponent();

        AttachControls();
        Closing += OnClosing;
        MainWindowViewModel.MyFunctionTriggered += RenderFieldEventHandler;
        KeyDown += KeyHandler;
    }
    
    private void AttachControls()
    {
        // Label settings
        _cursorLabel = this.FindControl<Label>("CursorLabel");
        _cursorLabel.IsVisible = false;

        // Canvas settings
        _canvasControl = this.FindControl<Canvas>("Canvas");
        _canvasControl.Width = CanvasWidth;
        _canvasControl.Height = CanvasHeight;
        // _canvasControl.PointerMoved += CanvasMouseMoveHandler;
        PointerMoved += CanvasMouseMoveHandler;
        _canvasControl.PointerPressed += CanvasMouseClickHandler;
        _canvasControl.PointerEnter += (sender, args) =>
        {
            _cursorLabel.IsVisible = true;
        };
        _canvasControl.PointerLeave += (sender, args) =>
        {
            _cursorLabel.IsVisible = false;
        };
        _canvasControl.PointerWheelChanged += (sender, args) =>
        {
            if (args.Delta.Y >= 0)
            {
                _zoom *= (float)1.1;
            }
            else
            {
                _zoom /= (float)1.1;
            }
            RenderField();
        };
        
        // CheckBoxes settings
        _checkBoxesList = new List<CheckBox>();
        _checkBoxes = this.FindControl<StackPanel>("CheckBoxes");
        var toggle = this.FindControl<ToggleButton>("ShowCb");
        _checkBoxes.IsVisible = (bool)toggle.IsChecked!;
        toggle.Click += (sender, args) =>
        {
            _checkBoxes.IsVisible = !_checkBoxes.IsVisible;
            var i = 0;
            foreach (var item in _checkBoxesList)
            {
                if (i % 2 == 0)
                {
                    item.IsChecked = MainWindowViewModel.MyField.Result.NeighborsForAlive.Contains(i / 2);
                }
                else
                {
                    item.IsChecked = MainWindowViewModel.MyField.Result.NeighborsForDead.Contains(i / 2);
                }
                i++;
            }
        };
        
        for (var i = 0; i < 9; i++)
        {
            var cha = this.FindControl<CheckBox>($"A{i}");
            _checkBoxesList.Add(cha);
            cha.Content = $"{i}";
            if (!MainWindowViewModel.MyField.IsFaulted && MainWindowViewModel.MyField.IsCompleted)
                cha.IsChecked = MainWindowViewModel.MyField.Result.NeighborsForAlive.Contains(i);
            var i1 = i;
            cha.Click += (sender, args) =>
            {
                (DataContext as MainWindowViewModel)!.ChangeAliveRules(Field.Cell.Color.Alive, i1, !(bool)((CheckBox)sender!).IsChecked!);
            };

            var chd = this.FindControl<CheckBox>($"D{i}");
            _checkBoxesList.Add(chd);
            chd.Content = $"{i}";
            chd.Click += (sender, args) =>
            {
                (DataContext as MainWindowViewModel)!.ChangeAliveRules(Field.Cell.Color.Dead, i1, !(bool)((CheckBox)sender!).IsChecked!);
            };
            if (!MainWindowViewModel.MyField.IsFaulted && MainWindowViewModel.MyField.IsCompleted)
                chd.IsChecked = MainWindowViewModel.MyField.Result.NeighborsForAlive.Contains(i);
        }
    }

    private void KeyHandler(object? sender, KeyEventArgs e)
    {
        switch (e.Key)
        {
            case Key.Left:
                _shiftX += 10;
                RenderField();
                break;
            case Key.Right:
                _shiftX -= 10;
                RenderField();
                break;
            case Key.Up:
                _shiftY += 10;
                RenderField();
                break;
            case Key.Down:
                _shiftY -= 10;
                RenderField();
                break;
        }
    }

    private async void OnClosing(object? sender, System.ComponentModel.CancelEventArgs e)
    {
        e.Cancel = true; // Prevent the window from closing immediately
        var mb = MessageBox.Avalonia.MessageBoxManager.
            GetMessageBoxStandardWindow("Warning", "Do you want to save last game", ButtonEnum.YesNoCancel);
        var res = await mb.Show(this);

        switch (res)
        {
            case ButtonResult.Yes:
                // TODO handle exception
                MainWindowViewModel.MyField.Result.CacheLastGame();
                e.Cancel = false;
                base.OnClosing(e);
                Environment.Exit(0);
                break;
            case ButtonResult.No:
                e.Cancel = false;
                base.OnClosing(e);
                Environment.Exit(0);
                break;
        }
    }

    private void RenderFieldEventHandler(object? sender, EventArgs e)
    {
        RenderField();
    }

    private void CanvasMouseMoveHandler(object? sender, PointerEventArgs e)
    {
        if (MainWindowViewModel.MyField.Status != TaskStatus.RanToCompletion)
            return;
            
        var cursorPosition = e.GetPosition(_canvasControl);
        Canvas.SetLeft(_cursorLabel, cursorPosition.X + 5);
        Canvas.SetTop(_cursorLabel, cursorPosition.Y + 5);
        
        var (x, y) = GetIndexFromPos(cursorPosition);
        if (x >= 0 && x < MainWindowViewModel.MyField.Result.Width && y >= 0 && y < MainWindowViewModel.MyField.Result.Height)
            _cursorLabel.Content = MainWindowViewModel.MyField.Result.GetCell(x, y).Longevity;
    }

    private void CanvasMouseClickHandler(object? sender, PointerEventArgs e)
    {
        if (MainWindowViewModel.MyField.Status != TaskStatus.RanToCompletion)
            return;
        
        var (x, y) = GetIndexFromPos(e.GetPosition(_canvasControl));
        if (x >= 0 && x < MainWindowViewModel.MyField.Result.Width && y >= 0 &&
            y < MainWindowViewModel.MyField.Result.Height)
        {
            MainWindowViewModel.MyField.Result.ChangeCellColor(x, y);
            RenderField();
        }
    }

    private (int, int) GetIndexFromPos(Point position)
    {
        var x = (int)(position.X * MainWindowViewModel.MyField.Result.Width / CanvasWidth / _zoom) - _shiftX;
        var y = (int)(position.Y * MainWindowViewModel.MyField.Result.Height / CanvasHeight / _zoom) - _shiftY;
        return ((int, int))(x, y);
    }

    private void RenderField()
    {
        var imageInfo = new SKImageInfo(CanvasWidth, CanvasHeight);
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
            var diffX = CanvasWidth / (float)w * _zoom; // TODO
            var diffY = CanvasHeight / (float)h * _zoom; // TODO
            
            for (var y = 0; y < h; y++)
            {
                for (var x = 0; x < w; x++)
                {
                    if (MainWindowViewModel.MyField.Result.Cells[w * y + x].CurrentColor == Field.Cell.Color.Alive)
                        canvas.DrawRect((x + _shiftX) * diffX, (y + _shiftY) * diffY, diffX, diffY, new SKPaint
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
            var imageBrush = new ImageBrush(new Avalonia.Media.Imaging.Bitmap(mStream));
            var rect = new Rectangle
            {
                Width = CanvasWidth,
                Height = CanvasHeight,
                Fill = imageBrush,
                Stroke = Brushes.Black,
                StrokeThickness = 2
            };
            _canvasControl.Children.Clear();
            _canvasControl.Children.Add(rect);
            _canvasControl.Children.Add(_cursorLabel);
        }
    }

    private void Spinner_OnSpin(object? sender, SpinEventArgs e)
    {
        if (e.Direction == SpinDirection.Increase)
        {
            _zoom *= (float)1.5;
        }
        else
        {
            _zoom /= (float)1.5;
        }
        RenderField();
    }

    private async void OnSaveButtonClick(object sender, RoutedEventArgs e)
    {
        var dialog = new SaveFileDialog();
        dialog.Filters!.Add(new FileDialogFilter() { Name = "Files", Extensions = { "lf" } });

        var result = await dialog.ShowAsync(this);

        if (string.IsNullOrEmpty(result)) return;

        MainWindowViewModel.MyField.Result.SaveFieldStateLocally(result, "");
    }

    private async void OnLoadButtonClick(object sender, RoutedEventArgs e)
    {
        var dialog = new OpenFileDialog();
        dialog.AllowMultiple = false;
        dialog.Filters?.Add(new FileDialogFilter() { Name = "Files", Extensions = { "lf" } });

        var result = await dialog.ShowAsync(this);

        if (result != null && (result.Length <= 0 || string.IsNullOrEmpty(result[0]))) return;
        
        var filePath = result![0];
        MainWindowViewModel.MyField.Result.LoadFieldStateLocally(filePath, "");
        RenderField();
    }
}