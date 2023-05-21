using System;
using System.Threading.Tasks;
using App.Models;
using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Threading;

namespace App.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
    public static readonly Task<Field> MyField = Field.GetInstanse();

    public MainWindowViewModel()
    {
        Console.WriteLine("Create model");
    }
    
    // TODO
    // queue in parallel thread that contains queries to server
    
    public static event EventHandler? MyFunctionTriggered;

    public async void NextGeneration()
    {
        await Task.Run(() =>
        {
            MyField.Result.CalculateNextGenerationAsync();
        }).ContinueWith(task =>
        {
            Dispatcher.UIThread.Post(() =>
            {
                MyFunctionTriggered?.Invoke(this, EventArgs.Empty);
            });
        });
        
        // TODO multiply calls doesn't work (server fails)
    }

    public async void GenerateField()
    {
        await Task.Run(() =>
        {
            MyField.Result.GenerateRandom();
        }).ContinueWith(task =>
        {
            Dispatcher.UIThread.Post(() =>
            {
                MyFunctionTriggered?.Invoke(this, EventArgs.Empty);
            });
        });
    }

    public async void ChangeAliveRules(Field.Cell.Color color, int count, bool toDelete)
    {
        Console.WriteLine($"{count}:{toDelete}");
        await Task.Run(() =>
        {
            MyField.Result.ChangeLiveRules(color, count, toDelete);
        });
    }

    public async void UploadField()
    {
        
    }
}