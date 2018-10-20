using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace MUL
{
    public partial class MainWindow : Window
    {
        [DllImport("gdi32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool DeleteObject(IntPtr value);

        private List<CheckBox> CheckBoxes
        {
            get
            {
                return new List<CheckBox>
                {
                    ChbAlbum, ChbAlbumArtist, ChbArtist, ChbComposers, ChbConductor, ChbCopyright, ChbDisc,
                    ChbGenres, ChbGrouping, ChbTrack, ChbYear
                };
            }
        }

        private List<AutoCompleteBox> TextBoxes
        {
            get
            {
                return new List<AutoCompleteBox>
                {
                    TbAlbum, TbAlbumArtist, TbArtist, TbComposers, TbConductor, TbCopyright, TbDisc, TbGenres,
                    TbGrouping, TbTitle, TbTrack, TbYear, TbTrackCount, TbDiscCount
                };
            }
        }

        private void DisableEditorControls()
        {
            foreach (CheckBox chb in CheckBoxes)
            {
                chb.IsChecked = false;
                chb.IsEnabled = false;
            }
            foreach (AutoCompleteBox tb in TextBoxes)
            {
                tb.Text = "";
                tb.IsEnabled = false;
            }
            TblockLyrics.IsEnabled = false;
        }

        private void SetEditorControls()
        {
            Debug.WriteLine("SetEditorControls: {0}", TrackList.SelectedCount);
            if (TrackList.SelectedCount > 0)
            {
                if (TrackList.SelectedCount == 1)
                {
                    foreach (CheckBox ch in CheckBoxes)
                    {
                        ch.IsChecked = false;
                        ch.IsEnabled = false;
                    }
                    foreach (AutoCompleteBox tb in TextBoxes)
                        tb.IsEnabled = true;
                    TblockLyrics.IsEnabled = true;
                }
                else
                {
                    foreach (CheckBox ch in CheckBoxes)
                    {
                        ch.IsChecked = false;
                        ch.IsEnabled = true;
                    }
                    foreach (AutoCompleteBox tb in TextBoxes)
                        tb.IsEnabled = false;
                    TblockLyrics.IsEnabled = false;
                }
                Models.SelectionData data = TrackList.GetSelectionData();

                TbTitle.Text = data.Title;
                TbTitle.ItemsSource = data.AllTitles;

                TbArtist.Text = data.Artists;
                TbArtist.ItemsSource = data.AllArtists;

                TbAlbum.Text = data.Album;
                TbAlbum.ItemsSource = data.AllAlbums;

                TbAlbumArtist.Text = data.AlbumArtists;
                TbAlbumArtist.ItemsSource = data.AllAlbumArtists;
                
                TbGenres.Text = data.Genres;
                TbGenres.ItemsSource = data.AllGenres;

                TbComposers.Text = data.Composers;
                TbComposers.ItemsSource = data.AllComposers;

                TbConductor.Text = data.Conductor;
                TbConductor.ItemsSource = data.AllConductors;

                TbCopyright.Text = data.Copyright;
                TbCopyright.ItemsSource = data.AllCopyrights;

                TbGrouping.Text = data.Grouping;
                TbGrouping.ItemsSource = data.AllGroupings;

                TblockLyrics.Text = data.Lyrics;
                TbYear.Text = data.Year.ToString();
                TbDisc.Text = data.Disc.ToString();
                TbTrack.Text = data.Track.ToString();
                TbTrackCount.Text = data.TrackCount.ToString();
                TbDiscCount.Text = data.DiscCount.ToString();
                if (data.Image != null)
                {
                    // Taken from https://stackoverflow.com/questions/10077498/show-drawing-image-in-wpf
                    var bitmap = new Bitmap(data.Image);
                    IntPtr bmpPt = bitmap.GetHbitmap();
                    BitmapSource bitmapSource =
                    System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
                           bmpPt,
                           IntPtr.Zero,
                           Int32Rect.Empty,
                           BitmapSizeOptions.FromEmptyOptions()
                    );
                    bitmapSource.Freeze();
                    DeleteObject(bmpPt);
                    ImgCover.Source =  bitmapSource;
                }
                else
                {
                    ImgCover.Source = null;
                }
            }
            else
            {
                ImgCover.Source = null;
                DisableEditorControls();
            }
        }
    }
}
