using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace MUL
{
    public partial class MainWindow : Window
    {
        private void VolumeChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            float volume = (float)(SldVolume.Value / 100);
            Debug.WriteLine("Volume: {0}", volume);
            if (TrackList.HasCurrent)
                TrackList.CurrentTrack.SetVolume(volume);
        }
    }
}
