using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace MUL
{
    public partial class MainWindow : Window
    {
        private DispatcherTimer Timer { get; set; }

        private void TimerTicked(object sender, EventArgs e)
        {
            UpdatePosition();
        }
    }
}
