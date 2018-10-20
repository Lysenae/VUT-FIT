// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package gui.dialogs;

import models.DatabaseMultimedial;
import oracle.ord.im.OrdImage;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.filechooser.FileNameExtensionFilter;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
/**
 * @author xstane33
 */
public class PhotoEditDialog extends JPanel {

    private BufferedImage bufferedImage;
    private JLabel imgLabel;

    /**
     * Creates window with editing panel and the photo itself
     * @param reservationID ID of the reservation
     */
    public void createPhotoWinow(int reservationID) {
        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                JFrame frame = new JFrame("Reservation Photo");
                frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                frame.setMinimumSize(new Dimension(600, 250));
                frame.setLocationRelativeTo(null);
                frame.setLocationByPlatform(true);

                try {
                    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
                } catch (Exception e) {
                    e.printStackTrace();
                }
                GridBagConstraints gbc = new GridBagConstraints();
                frame.setLayout(new GridBagLayout());

                final int resID = reservationID; //=reservationID
                JPanel leftPanel = new JPanel();
                JPanel buttonPanel = new JPanel(new GridLayout(1, 2));
                JPanel rotatePanel = new JPanel(new GridLayout(4, 1));
                JPanel scalePanel = new JPanel(new GridLayout(2, 1));

                Border rotateBorder = BorderFactory.createTitledBorder("Rotation Options: ");
                rotatePanel.setBorder(rotateBorder);
                JCheckBox checkRotateRight = new JCheckBox("Clockwise");
                rotatePanel.add(checkRotateRight);
                JCheckBox checkRotateYAxis = new JCheckBox("Y Axis");
                rotatePanel.add(checkRotateYAxis);
                JCheckBox checkRotateXAxis = new JCheckBox("X Axis");
                rotatePanel.add(checkRotateXAxis);
                JButton rotateButton = new JButton("Rotate");
                rotatePanel.add(rotateButton);


                Border scaleBorder = BorderFactory.createTitledBorder("Scale Percentage: ");
                scalePanel.setBorder(scaleBorder);
                JSpinner scaleSpinner = new JSpinner(new SpinnerNumberModel(100, 0,300, 1));
                scalePanel.add(scaleSpinner);
                JButton scaleButton = new JButton("Rescale");
                scalePanel.add(scaleButton);


                JButton loadButton = new JButton("Load");
                JButton exportButton = new JButton("Export");

                JButton deleteButton = new JButton("Delete");
                JButton similarButton = new JButton("Most Similar Photo");

                leftPanel.setLayout(new BoxLayout(leftPanel, BoxLayout.Y_AXIS));

                JPanel deletePanel = new JPanel(new GridLayout(2, 1));
                deletePanel.add(similarButton);
                deletePanel.add(deleteButton);

                InputStream inputStream;
                bufferedImage = null;

                try{
                    inputStream = DatabaseMultimedial.getPhoto(resID).getDataInStream();
                    bufferedImage = ImageIO.read(inputStream);

                } catch (Exception ex) {
                    System.err.format("nemuzu loadnout obr: %s\n", ex.getMessage());
                    try{
                        //Tady to obcas udela bordel
                        bufferedImage = ImageIO.read(this.getClass().getResource("/resources/images/NP.png"));

                    } catch (Exception ex2) {
                        System.err.println("mock image failed");
                    }
                }


                loadButton.addActionListener(new java.awt.event.ActionListener() {
                    public void actionPerformed(java.awt.event.ActionEvent evt) {
                        JFileChooser fileChooser = new JFileChooser();
                        FileNameExtensionFilter extensionFilter = new FileNameExtensionFilter(
                                "jpg, png", "jpg", "png");
                        fileChooser.setFileFilter(extensionFilter);
                        int fileChooserRes = fileChooser.showOpenDialog(null);
                        if (fileChooserRes == JFileChooser.APPROVE_OPTION) {
                            try {
                                //System.out.println(photoId);
                                //System.out.println(houseId);
                                DatabaseMultimedial.insertPhoto(fileChooser.getSelectedFile().getPath(), resID);
                            } catch (Exception ex) {
                                System.out.println("Inserting failed" + ex.getMessage());
                            }

                        }
                        try {
                            OrdImage ordImage = DatabaseMultimedial.getPhoto(resID);
                            InputStream stream = ordImage.getDataInStream();
                            bufferedImage = ImageIO.read(stream);
                        } catch (Exception ex) {
                            System.out.println("Inserting failed" + ex.getMessage());
                        }
                        imgLabel.setIcon(new ImageIcon(bufferedImage));
                        imgLabel.revalidate();
                        imgLabel.repaint();
                    }
                });

                exportButton.addActionListener(new java.awt.event.ActionListener() {
                    public void actionPerformed(java.awt.event.ActionEvent evt) {
                        try {
                            OrdImage ordImage = DatabaseMultimedial.getPhoto(resID);
                            InputStream stream = ordImage.getDataInStream();
                            bufferedImage = ImageIO.read(stream);
                        } catch (Exception ex) {
                            System.out.println("Fetching photo failed " + ex.getMessage());
                            JOptionPane.showMessageDialog(frame, "Error: This reservation has no photo.", "Error", JOptionPane.PLAIN_MESSAGE);
                            return;
                        }
                        JFileChooser fileChooser = new JFileChooser();
                        fileChooser.setFileFilter(new FileNameExtensionFilter("JPG and JPEG Images", "jpg", "jpeg"));
                        int result = fileChooser.showSaveDialog(null);
                        if (result == JFileChooser.APPROVE_OPTION) {

                            try {
                                String filename = fileChooser.getSelectedFile().toString();
                                if (!filename .endsWith(".jpg"))
                                    filename += ".jpg";
                                File outputfile = new File(filename);
                                ImageIO.write(bufferedImage, "jpg", outputfile);
                            }catch (IOException ex) {
                                System.out.println("Export failed " + ex.getMessage());
                            }
                        }
                    }
                });

                deleteButton.addActionListener(new java.awt.event.ActionListener() {
                    public void actionPerformed(java.awt.event.ActionEvent evt) {
                        try {
                            //System.out.println(photoId);
                            //System.out.println(houseId);
                            DatabaseMultimedial.deletePhoto(resID);
                            bufferedImage = ImageIO.read(this.getClass().getResource("/resources/images/NP.png"));
                            imgLabel.setIcon(new ImageIcon(bufferedImage));
                            imgLabel.revalidate();
                            imgLabel.repaint();
                        } catch (Exception ex) {
                            System.out.println("Inserting failed" + ex.getMessage());
                        }
                    }
                });


                rotateButton.addActionListener(new java.awt.event.ActionListener() {
                    public void actionPerformed(java.awt.event.ActionEvent evt) {
                        try {
                            int rotate = 0;
                            //System.out.println(photoId);
                            //System.out.println(houseId);
                            if (!checkRotateRight.isSelected() && !checkRotateXAxis.isSelected() && !checkRotateYAxis.isSelected())
                                return;
                            if (checkRotateRight.isSelected())
                                rotate = 90;

                            DatabaseMultimedial.rotatePhoto(resID, rotate, checkRotateXAxis.isSelected(), checkRotateYAxis.isSelected());
                        } catch (Exception ex) {
                            System.out.println("Saving failed" + ex.getMessage());
                        }
                        try {
                            OrdImage ordImage = DatabaseMultimedial.getPhoto(resID);
                            InputStream stream = ordImage.getDataInStream();
                            bufferedImage = ImageIO.read(stream);
                        } catch (Exception ex) {
                            System.out.println("Inserting failed" + ex.getMessage());
                        }
                        imgLabel.setIcon(new ImageIcon(bufferedImage));
                        imgLabel.revalidate();
                        imgLabel.repaint();
                    }
                });

                scaleButton.addActionListener(new java.awt.event.ActionListener() {
                    public void actionPerformed(java.awt.event.ActionEvent evt) {
                        try {
                            int rotate = 0;
                            //System.out.println(photoId);
                            //System.out.println(houseId);
                            if ((Integer)scaleSpinner.getValue() != 100){
                                double scaleFactor = ((Integer)scaleSpinner.getValue())/100.0;
                                DatabaseMultimedial.scalePhoto(resID, scaleFactor);
                                scaleSpinner.setValue(100);
                            }
                        } catch (Exception ex) {
                            System.out.println("Saving failed" + ex.getMessage());
                        }
                        try {
                            OrdImage ordImage = DatabaseMultimedial.getPhoto(resID);
                            InputStream stream = ordImage.getDataInStream();
                            bufferedImage = ImageIO.read(stream);
                        } catch (Exception ex) {
                            System.out.println("Inserting failed" + ex.getMessage());
                        }
                        imgLabel.setIcon(new ImageIcon(bufferedImage));
                        imgLabel.revalidate();
                        imgLabel.repaint();
                    }
                });

                similarButton.addActionListener(new java.awt.event.ActionListener() {
                    public void actionPerformed(java.awt.event.ActionEvent evt) {

                        try {
                            SimilarPhotoWindow spw = new SimilarPhotoWindow();
                            OrdImage ordImage = null;
                            if ((ordImage = DatabaseMultimedial.getMostSimilarImage(resID)) == null){
                                JOptionPane.showMessageDialog(null, "Not enough photos in database.", "Info", JOptionPane.INFORMATION_MESSAGE);
                                return;
                            }
                            InputStream stream = ordImage.getDataInStream();
                            bufferedImage = ImageIO.read(stream);
                            spw.creteSimilarPhotoWindow(new ImageIcon(bufferedImage));

                        } catch (Exception ex) {
                            System.out.println("Getting most similar failed: " + ex.getMessage());
                            return;
                        }
                    }
                });





                imgLabel = new JLabel(new ImageIcon(bufferedImage));
                JScrollPane scrollPane = new JScrollPane(imgLabel);

                buttonPanel.add(loadButton);
                buttonPanel.add(exportButton);

                leftPanel.add(buttonPanel);
                leftPanel.add(rotatePanel);
                leftPanel.add(scalePanel);
                leftPanel.add(deletePanel);



                //leftPanel.setBackground(Color.yellow);
                //frame.setBackground(Color.gray);


                gbc.fill    = GridBagConstraints.HORIZONTAL;
                gbc.anchor  = GridBagConstraints.NORTH;
                gbc.gridx   = 0;
                gbc.gridy   = 0;
                gbc.weightx = 1;
                gbc.weighty = 1;
                frame.add(leftPanel, gbc);
                gbc.fill    = GridBagConstraints.BOTH;
                //gbc.anchor  = GridBagConstraints.NORTH;


                scrollPane.setHorizontalScrollBarPolicy(
                        JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
                scrollPane.setVerticalScrollBarPolicy(
                        JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
              scrollPane.setViewportView(imgLabel);

                gbc.anchor  = GridBagConstraints.NORTH;
                gbc.fill    = GridBagConstraints.BOTH;
                gbc.gridx   = 1;
                gbc.gridy   = 0;
                gbc.weightx = 12;
                gbc.weighty = 1;

                frame.add(scrollPane, gbc);


               // frame.getContentPane().add(BorderLayout.WEST, buttonPanel);
                frame.pack();
                frame.setVisible(true);
                frame.setResizable(true);

            }
        });
    }
}
