using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace App.Views;

public partial class Field : UserControl
{
    public Field()
    {
        InitializeComponent();
    }

    private void InitializeComponent()
    {
        AvaloniaXamlLoader.Load(this);
    }
}