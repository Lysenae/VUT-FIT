// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package gui.dialogs;

import javax.swing.*;
import java.awt.*;

/**
 *
 * @author xstane33
 */
public class SimilarPhotoWindow extends JPanel {
    private JLabel imgLabel;

    /**
     * Shows window wtith the most similar picture.
     * @param photo icon to show in the window
     */
    public void creteSimilarPhotoWindow(ImageIcon photo) {
        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                JFrame frame = new JFrame("The Most Similar Photo");
                frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                frame.setPreferredSize(new Dimension(photo.getIconWidth(), photo.getIconHeight()));
                frame.setLocationByPlatform(true);
                frame.setLocationRelativeTo(null);
                try {
                    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
                } catch (Exception e) {
                    e.printStackTrace();
                }
                GridBagConstraints gbc = new GridBagConstraints();
                frame.setLayout(new GridBagLayout());
                imgLabel = new JLabel(photo);
                JScrollPane scrollPane = new JScrollPane(imgLabel);

                frame.add(scrollPane);

                frame.pack();
                frame.setVisible(true);
                frame.setResizable(true);
            }
        });
    }
    /**
     * @param photo icon to show in the window
     */
    public void setIcon(ImageIcon photo){
        imgLabel.setIcon(photo);
        imgLabel.revalidate();
        imgLabel.repaint();
    }
}