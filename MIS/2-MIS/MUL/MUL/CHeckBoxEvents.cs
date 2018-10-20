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
        private void ChbAlbum_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbAlbum.IsEnabled = true;
        }

        private void ChbAlbum_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbAlbum.IsEnabled = false;
        }

        private void ChbArtist_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbArtist.IsEnabled = true;
        }

        private void ChbArtist_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbArtist.IsEnabled = false;
        }

        private void ChbAlbumArtist_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbAlbumArtist.IsEnabled = true;
        }

        private void ChbAlbumArtist_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbAlbumArtist.IsEnabled = false;
        }

        private void ChbYear_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbYear.IsEnabled = true;
        }

        private void ChbYear_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbYear.IsEnabled = false;
        }

        private void ChbTrack_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
            {
                TbTrack.IsEnabled = true;
                TbTrackCount.IsEnabled = true;
            }
        }

        private void ChbTrack_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
            {
                TbTrack.IsEnabled = false;
                TbTrackCount.IsEnabled = false;
            }
        }

        private void ChbDisc_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
            {
                TbDisc.IsEnabled = true;
                TbDiscCount.IsEnabled = true;
            }
        }

        private void ChbDisc_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
            {
                TbDisc.IsEnabled = false;
                TbDiscCount.IsEnabled = false;
            }
        }

        private void ChbGenres_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbGenres.IsEnabled = true;
        }

        private void ChbGenres_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbGenres.IsEnabled = false;
        }

        private void ChbComposers_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbComposers.IsEnabled = true;
        }

        private void ChbComposers_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbComposers.IsEnabled = false;
        }

        private void ChbConductor_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbConductor.IsEnabled = true;
        }

        private void ChbConductor_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbConductor.IsEnabled = false;
        }

        private void ChbCopyright_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbCopyright.IsEnabled = true;
        }

        private void ChbCopyright_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbCopyright.IsEnabled = false;
        }

        private void ChbGrouping_Checked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbGrouping.IsEnabled = true;
        }

        private void ChbGrouping_Unchecked(object sender, RoutedEventArgs e)
        {
            if (TrackList.SelectedCount > 1)
                TbGrouping.IsEnabled = false;
        }
    }
}
